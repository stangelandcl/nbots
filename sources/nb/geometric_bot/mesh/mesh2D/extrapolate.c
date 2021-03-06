#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "nb/math_bot.h"
#include "nb/memory_bot.h"
#include "nb/geometric_bot/mesh/mesh2D.h"

#include "mesh2D_struct.h"
#include "mesh2D_private.h"

#define POW2(a) ((a)*(a))

static void assemble_system(const nb_mesh2D_t *mesh,
			    const double *elem_values,
			    uint8_t N_comp, double *M, double *F);
static void assemble_elem(const nb_mesh2D_t *mesh,
			  const double *elem_values,
			  uint32_t elem_id, uint8_t N_comp,
			  double *M, double *F);
static void eval_shape_funcs(const nb_mesh2D_t *mesh, uint32_t elem_id,
			     double *f);
static void solve_system(const nb_mesh2D_t *mesh, uint8_t N_comp, 
			 double *M, double *F, double *nodal_values);
static double distort_using_nodal_field(nb_mesh2D_t *mesh, double *disp,
					double max_disp);
static void elem_get_interpolation(const nb_mesh2D_t *mesh, uint32_t elem_id,
				   const double *value, uint8_t N_comp,
				   double *out);
static double get_max_displacement(uint32_t N, double *disp);
static double distort_using_elem_field(nb_mesh2D_t *mesh, double *disp,
				       double max_disp);

void nb_mesh2D_extrapolate_elems_to_nodes(const nb_mesh2D_t *mesh,
					   uint8_t N_comp,
					   const double *elem_values,
					   double *nodal_values)
{
	uint32_t N_nodes = nb_mesh2D_get_N_nodes(mesh);

	char *memblock = nb_allocate_mem((1 + N_comp) * N_nodes * sizeof(double));

	double *M = (void*) memblock;
	double *F = (void*) (memblock + N_nodes * sizeof(double));
	memset(M, 0, N_nodes * sizeof(*M));
	memset(F, 0, N_nodes * N_comp * sizeof(*F));
	
	assemble_system(mesh, elem_values, N_comp, M, F);

	solve_system(mesh, N_comp, M, F, nodal_values);

	nb_free_mem(memblock);
}

static void assemble_system(const nb_mesh2D_t *mesh,
			    const double *elem_values,
			    uint8_t N_comp, double *M, double *F)
{
	uint32_t N_elems = nb_mesh2D_get_N_elems(mesh);
	for (uint32_t i = 0; i < N_elems; i++)
		assemble_elem(mesh, elem_values, i, N_comp, M, F);
}

static void assemble_elem(const nb_mesh2D_t *mesh, const double *elem_values,
			  uint32_t elem_id, uint8_t N_comp,
			  double *M, double *F)
{
	uint16_t N_adj = nb_mesh2D_elem_get_N_adj(mesh, elem_id);
	uint16_t f_memsize = N_adj * sizeof(double);
	double *f = nb_soft_allocate_mem(f_memsize);

	eval_shape_funcs(mesh, elem_id, f);

	double area = nb_mesh2D_elem_get_area(mesh, elem_id);

	for (uint32_t i = 0; i < N_adj; i++) {
		uint32_t node_id = nb_mesh2D_elem_get_adj(mesh, elem_id, i);
		/* Assemble mass matrix */
		for (uint32_t j = 0; j < N_adj; j++) {
			M[node_id] += area * f[i] * f[j];
		}
		/* Assemble independent vector for each component */
		for (uint8_t j = 0; j < N_comp; j++) {
			uint32_t nid = node_id * N_comp + j;
			uint32_t eid = elem_id * N_comp + j;
			F[nid] += area * elem_values[eid] * f[i];
		}
	}

	nb_soft_free_mem(f_memsize, f);
}

static void eval_shape_funcs(const nb_mesh2D_t *mesh, uint32_t elem_id,
			     double *f)
{
	uint16_t N = nb_mesh2D_elem_get_N_adj(mesh, elem_id);

	double x[2];
	x[0] = nb_mesh2D_elem_get_x(mesh, elem_id);
	x[1] = nb_mesh2D_elem_get_y(mesh, elem_id);

	uint32_t memsize = 2 * N * sizeof(double);
	double *ni = nb_soft_allocate_mem(memsize);
	for (uint16_t i = 0; i < N; i++) {
		double node_id = nb_mesh2D_elem_get_adj(mesh, elem_id, i);
		ni[i * 2] = nb_mesh2D_node_get_x(mesh, node_id);
		ni[i*2+1] = nb_mesh2D_node_get_y(mesh, node_id);
	}
	
	nb_nonpolynomial_eval(N, 2, ni, NULL, x, f);

	nb_soft_free_mem(memsize, ni);
}

static void solve_system(const nb_mesh2D_t *mesh, uint8_t N_comp, 
			 double *M, double *F, double *nodal_values)
{
	uint32_t N_nodes = nb_mesh2D_get_N_nodes(mesh);
	for (uint32_t i = 0; i < N_nodes; i++) {
		for (uint8_t j = 0; j < N_comp; j++) {
			uint32_t id = i * N_comp + j;
			nodal_values[id] = F[id] / M[i];
		}
	}
}


double nb_mesh2D_distort_with_field(nb_mesh2D_t *mesh,
				     nb_mesh2D_entity field_entity,
				     double *disp,
				     double max_disp)
{
	double scale = 1.0;
	if (NB_NODE == field_entity)
		scale = distort_using_nodal_field(mesh, disp, max_disp);
	else if (NB_ELEMENT == field_entity)
		scale = distort_using_elem_field(mesh, disp, max_disp);
	return scale;
}

static double distort_using_nodal_field(nb_mesh2D_t *mesh, double *disp,
					double max_disp)
{
	nb_mesh2D_private_i priv;
	nb_mesh2D_init_private_interface(&priv, mesh);

	uint32_t N_nodes = nb_mesh2D_get_N_nodes(mesh);
	double max_field_disp = get_max_displacement(N_nodes, disp);
	double scale = max_disp / max_field_disp;
	
	for (uint32_t i = 0; i < N_nodes; i++) {
		priv.node_move_x(mesh->msh, i, disp[i * 2] * scale);
		priv.node_move_y(mesh->msh, i, disp[i*2+1] * scale);
	}

	uint32_t N_elems = nb_mesh2D_get_N_elems(mesh);
	for (uint32_t i = 0; i < N_elems; i++) {
		double d[2];
		elem_get_interpolation(mesh, i, disp, 2, d);
		priv.elem_move_x(mesh->msh, i, d[0] * scale);
		priv.elem_move_y(mesh->msh, i, d[1] * scale);
	}

	return scale;
}

static void elem_get_interpolation(const nb_mesh2D_t *mesh, uint32_t elem_id,
				   const double *value, uint8_t N_comp,
				   double *out)
{
	memset(out, 0, N_comp * sizeof(*out));
	
	uint32_t N_adj = nb_mesh2D_elem_get_N_adj(mesh, elem_id);
	double *f = nb_allocate_on_stack(N_adj * sizeof(*f));
	eval_shape_funcs(mesh, elem_id, f);

	for (uint32_t j = 0; j < N_adj; j++) {
		uint32_t id = nb_mesh2D_elem_get_adj(mesh, elem_id, j);
		for (uint8_t c = 0; c < N_comp; c++)
			out[c] += value[id * N_comp + c] * f[j];
	}
}

static double get_max_displacement(uint32_t N, double *disp)
{
	double max = 0;
	for (uint32_t i = 0; i < N; i++) {
		double disp2 = POW2(disp[i * 2]) + POW2(disp[i*2+1]);
		if (disp2 > max)
			max = disp2;
	}
	return sqrt(max);
}

static double distort_using_elem_field(nb_mesh2D_t *mesh, double *disp,
				       double max_disp)
{
	nb_mesh2D_private_i priv;
	nb_mesh2D_init_private_interface(&priv, mesh);

	uint32_t N_nodes = nb_mesh2D_get_N_nodes(mesh);
	uint32_t N_elems = nb_mesh2D_get_N_elems(mesh);

	double max_field_disp = get_max_displacement(N_elems, disp);
	double scale = max_disp / max_field_disp;

	for (uint32_t i = 0; i < N_elems; i++) {
		priv.elem_move_x(mesh->msh, i, disp[i * 2] * scale);
		priv.elem_move_y(mesh->msh, i, disp[i*2+1] * scale);
	}

	uint32_t memsize = 2 * N_nodes * sizeof(double);
	double *nodal_disp = nb_soft_allocate_mem(memsize);
	nb_mesh2D_extrapolate_elems_to_nodes(mesh, 2, disp, nodal_disp);
	
	for (uint32_t i = 0; i < N_nodes; i++) {
		priv.node_move_x(mesh->msh, i, nodal_disp[i * 2] * scale);
		priv.node_move_y(mesh->msh, i, nodal_disp[i*2+1] * scale);
	}

	nb_soft_free_mem(memsize, nodal_disp);
	return scale;
}
