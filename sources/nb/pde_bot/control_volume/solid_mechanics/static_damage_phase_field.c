#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "nb/math_bot.h"
#include "nb/memory_bot.h"
#include "nb/container_bot.h"
#include "nb/solver_bot.h"
#include "nb/geometric_bot.h"
#include "nb/graph_bot.h"
#include "nb/pde_bot.h"

#include "../calculation_points.h"
#include "../integration_mesh.h"

#include "elasticity2D.h"
#include "set_bconditions.h"

#define MAX_ITER 100
#define SMOOTH 2

#define POW2(a) ((a)*(a))

typedef struct {
	int smooth;
	const nb_mesh2D_t *intmsh;
	const double *disp;
	const double *xc;
	const nb_material_t *material;
	nb_analysis2D_t analysis2D;
} eval_damage_data_t;

static uint32_t get_cvfa_memsize(uint32_t N_elems, uint32_t N_faces);
static void distribute_cvfa_memory(char *memblock, uint32_t N_elems,
				   uint32_t N_faces, double **xc, double **F,
				   double **delta_disp, double **residual,
				   double **nodal_damage, double **rhs_damage,
				   nb_mesh2D_t **intmsh, nb_graph_t **trg_x_vol,
				   face_t ***faces, nb_glquadrature_t *glq);
static void init_eval_dmg(nb_cvfa_eval_damage_t * eval_dmg, int smooth,
			  const nb_mesh2D_t *intmsh,
			  const double *displacement,
			  const double *xc,
			  const nb_material_t *material,
			  nb_analysis2D_t analysis2D);
static double get_damage(const face_t *face, uint16_t subface_id,
			 uint8_t gp, const nb_glquadrature_t *glq,
			 const void *data);
static int solve_damage_equation(const nb_mesh2D_t *mesh,
				 const nb_material_t *material,
				 double *nodal_damage, /* Output */
				 const nb_mesh2D_t *intmsh,
				 const double *xc,
				 face_t **faces,
				 double *H, nb_sparse_t *D,
				 nb_glquadrature_t *glq);
static void compute_damage(double *damage, face_t **faces,
			   const nb_mesh2D_t *const mesh,
			   const double *elem_damage,
			   const nb_glquadrature_t *glq,
			   const nb_cvfa_eval_damage_t *eval_dmg);
static void get_face_damage(double *damage,
			    face_t **faces, uint32_t face_id,
			    const nb_mesh2D_t *mesh,
			    const double *elem_damage,
			    const nb_glquadrature_t *glq,
			    const nb_cvfa_eval_damage_t *eval_dmg);
static void get_internal_face_damage(double *damage,
				     face_t **faces, uint32_t face_id,
				     const nb_glquadrature_t *glq,
				     const nb_cvfa_eval_damage_t *eval_dmg);
static void get_boundary_face_damage(double *damage, face_t **faces,
				     uint32_t face_id);
static void finish_eval_dmg(nb_cvfa_eval_damage_t * eval_dmg);

int nb_cvfa_compute_2D_damage_phase_field
			(const nb_mesh2D_t *const mesh,
			 const nb_material_t *const material,
			 const nb_bcond_t *const bcond,
			 bool enable_self_weight, double gravity[2],
			 nb_analysis2D_t analysis2D,
			 nb_analysis2D_params *params2D,
			 double *displacement, /* Output */
			 double *strain,       /* Output */
			 double *damage,       /* Output */
			 char *boundary_mask   /* Output */)
{
	uint32_t N_elems = nb_mesh2D_get_N_elems(mesh);
	uint32_t N_faces = nb_mesh2D_get_N_edges(mesh);
	uint32_t memsize = get_cvfa_memsize(N_elems, N_faces);
	char *memblock = nb_soft_allocate_mem(memsize);
	double *xc;
	double *F;
	double *delta_disp;
	double *residual;
	double *rhs_damage;
	double *elem_damage;
	nb_mesh2D_t *intmsh;
	nb_graph_t *trg_x_vol;
	face_t **faces;
	nb_glquadrature_t glq;
	distribute_cvfa_memory(memblock, N_elems, N_faces, &xc, &F,
			       &delta_disp, &residual, &elem_damage, 
			       &rhs_damage, &intmsh, &trg_x_vol,
			       &faces, &glq);

	nb_glquadrature_load(&glq, SMOOTH + 1);

  	nb_cvfa_set_calculation_points(mesh, xc);
	nb_cvfa_init_integration_mesh(intmsh);
	nb_cvfa_load_integration_mesh(intmsh, N_elems, xc);

	nb_graph_init(trg_x_vol);
	nb_cvfa_correlate_mesh_and_integration_mesh(mesh, intmsh,
						    trg_x_vol);
  	nb_sparse_t *A;
	nb_cvfa_init_global_matrix(&A, trg_x_vol, intmsh);

	nb_cvfa_load_faces(mesh, intmsh, trg_x_vol, faces);

	nb_cvfa_eval_damage_t eval_dmg;
	init_eval_dmg(&eval_dmg, SMOOTH, intmsh, displacement, xc,
		      material, analysis2D);

	int iter = 0;
	double rnorm = 1;
	int status = 0;
	memset(displacement, 0, 2 * N_elems * sizeof(*displacement));
	while (iter < MAX_ITER) {
		nb_cvfa_assemble_global_forces(F, mesh, material,
					       enable_self_weight,
					       gravity);
		nb_cvfa_assemble_global_stiffness(A, mesh, SMOOTH, intmsh, xc,
						  faces, material, analysis2D,
						  params2D, &glq, &eval_dmg);

		nb_cvfa_set_bconditions(mesh, material, analysis2D, 
					A, F, bcond, 1.0);
		
		nb_sparse_multiply_vector(A, displacement, residual, 1);
		nb_vector_substract_to(2 * N_elems, residual, F);
		rnorm = nb_vector_get_norm(residual, 2 * N_elems);
		if (rnorm < 1e-6)
			break;

		status = nb_sparse_relabel_and_solve_using_LU(A, residual,
							      delta_disp, 1);
		if (status != 0)
			goto CLEAN_AND_EXIT;

		nb_vector_sum(2 * N_elems, displacement, delta_disp);

		status = solve_damage_equation(mesh, material, elem_damage,
					       intmsh, xc, faces,
					       rhs_damage, A, &glq);
		if (status != 0)
			goto CLEAN_AND_EXIT;
		
		iter ++;
		printf(" ----> DAMAGE ITER: %i (%e)\n", iter, rnorm);/* TEMP */
	}
	nb_cvfa_compute_strain(strain, boundary_mask, faces, mesh, SMOOTH,
			       intmsh, xc, bcond, displacement, &glq);
	compute_damage(damage, faces, mesh, elem_damage, &glq, &eval_dmg);

CLEAN_AND_EXIT:
	printf(" >>>>> [%i] DAMAGE ITER: %i (%e)\n",
	       status, iter, rnorm);/* TEMPORAL */
	nb_cvfa_finish_faces(N_faces, faces);
	nb_sparse_destroy(A);
	nb_graph_finish(trg_x_vol);
	nb_mesh2D_finish(intmsh);
	nb_soft_free_mem(memsize, memblock);
	finish_eval_dmg(&eval_dmg);
	return status;
}

static uint32_t get_cvfa_memsize(uint32_t N_elems, uint32_t N_faces)
{
	uint32_t system_size = 10 * N_elems * sizeof(double);
	uint32_t intmsh_size = nb_cvfa_get_integration_mesh_memsize();
	uint32_t graph_size = nb_graph_get_memsize();
	uint16_t Nq = SMOOTH + 1;
	uint32_t glq_size = 2 * Nq * sizeof(double);
	uint32_t faces_size = N_faces * (sizeof(void*) + sizeof(face_t));
	return graph_size + system_size + intmsh_size + faces_size + glq_size;
}

static void distribute_cvfa_memory(char *memblock, uint32_t N_elems,
				   uint32_t N_faces, double **xc, double **F,
				   double **delta_disp, double **residual,
				   double **elem_damage, double **rhs_damage,
				   nb_mesh2D_t **intmsh, nb_graph_t **trg_x_vol,
				   face_t ***faces, nb_glquadrature_t *glq)
{
	uint32_t elem_size = N_elems * sizeof(double);
	uint32_t system_size = 2 * elem_size;
	uint32_t intmsh_size = nb_cvfa_get_integration_mesh_memsize();
	uint32_t graph_size = nb_graph_get_memsize();
	uint16_t Nq = SMOOTH + 1;
	uint32_t glq_size = 2 * Nq * sizeof(double);
	*F = (void*) memblock;
	*xc = (void*) (memblock + system_size);
	*delta_disp = (void*) (memblock + 2 * system_size);
	*residual = (void*) (memblock + 3 * system_size);
	*elem_damage = (void*) (memblock + 4 * system_size);
	*rhs_damage = (void*) (memblock + 4 * system_size + elem_size);
	*intmsh = (void*) (memblock + 4 * system_size + 2 * elem_size);
	*trg_x_vol = (void*) (memblock + 4 * system_size +
			      2 * elem_size + intmsh_size);
	glq->x = (void*) (memblock + 4 * system_size + 2 * elem_size +
			  intmsh_size + graph_size);
	glq->w = (void*) (memblock + 4 * system_size + 2 * elem_size +
			  intmsh_size + graph_size + Nq * sizeof(double));
	*faces = (void*) (memblock + 4 * system_size + 2 * elem_size +
			  intmsh_size + graph_size + glq_size);
	memblock +=  4 * system_size + 2 * elem_size + intmsh_size +
		graph_size + glq_size + N_faces * sizeof(void*);
	for (uint32_t i = 0; i < N_faces; i++) {
		(*faces)[i] = (void*) (memblock + i * sizeof(face_t));
		memset((*faces)[i], 0, sizeof(face_t));
	}
}

static void init_eval_dmg(nb_cvfa_eval_damage_t * eval_dmg, int smooth,
			  const nb_mesh2D_t *intmsh,
			  const double *displacement,
			  const double *xc,
			  const nb_material_t *material,
			  nb_analysis2D_t analysis2D)
{
	eval_damage_data_t *data = nb_allocate_mem(sizeof(*data));
	data->smooth = smooth;
	data->intmsh = intmsh;
	data->disp = displacement;
	data->xc = xc;
	data->material = material;
	data->analysis2D = analysis2D;
	eval_dmg->data = data;
	eval_dmg->get_damage = get_damage;
}

static double get_damage(const face_t *face, uint16_t subface_id,
			 uint8_t gp, const nb_glquadrature_t *glq,
			 const void *data)
{
	const eval_damage_data_t* dmg_data = data;

	double strain[3];
	memset(strain, 0, 3 * sizeof(*strain));
	nb_cvfa_subface_sum_strain(dmg_data->smooth,
				   dmg_data->intmsh,
				   face, face->subfaces[subface_id],
				   dmg_data->xc,
				   dmg_data->disp,
				   glq, gp, strain);

	double lame[2];
	nb_pde_get_lame_params(lame, dmg_data->material,
			       dmg_data->analysis2D);
	double tr = strain[0] + strain[1];
	double norm2 = POW2(strain[0]) +
		2 * POW2(0.5 * strain[2]) + POW2(strain[1]);
	double energy = lame[0] * norm2 + 0.5 * lame[1] * POW2(tr);

	double h = nb_material_get_damage_length_scale(dmg_data->material);
	double G = nb_material_get_fracture_energy(dmg_data->material);
	return h * energy / G;
}

static int solve_damage_equation(const nb_mesh2D_t *mesh,
				 const nb_material_t *material,
				 double *elem_damage, /* Output */
				 const nb_mesh2D_t *intmsh,
				 const double *xc,
				 face_t **faces,
				 double *H, nb_sparse_t *D,
				 nb_glquadrature_t *glq)
{
	return 0;/* TEMPORAL */
}

static void compute_damage(double *damage, face_t **faces,
			   const nb_mesh2D_t *const mesh,
			   const double *elem_damage,
			   const nb_glquadrature_t *glq,
			   const nb_cvfa_eval_damage_t *eval_dmg)
{
	uint32_t N_faces = nb_mesh2D_get_N_edges(mesh);

 	for (uint32_t i = 0; i < N_faces; i++)
		get_face_damage(damage, faces, i, mesh,
				elem_damage, glq, eval_dmg);
}

static void get_face_damage(double *damage,
			    face_t **faces, uint32_t face_id,
			    const nb_mesh2D_t *mesh,
			    const double *elem_damage,
			    const nb_glquadrature_t *glq,
			    const nb_cvfa_eval_damage_t *eval_dmg)
{
	uint32_t N_elems = nb_mesh2D_get_N_elems(mesh);
	if (faces[face_id]->elems[1] < N_elems)
		get_internal_face_damage(damage, faces, face_id,
					 glq, eval_dmg);
	else
		get_boundary_face_damage(damage, faces, face_id);
}

static void get_internal_face_damage(double *damage,
				     face_t **faces, uint32_t face_id,
				     const nb_glquadrature_t *glq,
				     const nb_cvfa_eval_damage_t *eval_dmg)
{
	damage[face_id] = 0.0;
	face_t *face = faces[face_id];
	for (uint16_t i = 0; i < face->N_sf; i++) {
		for (uint8_t q = 0; q < glq->N; q++)
			damage[face_id] += eval_dmg->get_damage(face, i, q, glq,
							       eval_dmg->data);
	}
	double length = nb_utils2D_get_dist(face->x1, face->x2);
	damage[face_id] /= length;
}

static void get_boundary_face_damage(double *damage, face_t **faces,
				     uint32_t face_id)
{
	damage[face_id] = 0.0;
}

static void finish_eval_dmg(nb_cvfa_eval_damage_t * eval_dmg)
{
	nb_free_mem(eval_dmg->data);
}
