#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "nb/eigen_bot.h"
#include "nb/pde_bot/material.h"
#include "nb/pde_bot/common_solid_mechanics/analysis2D.h"
#include "nb/pde_bot/boundary_conditions/bcond.h"
#include "nb/pde_bot/boundary_conditions/bcond_iter.h"
#include "nb/pde_bot/finite_element/element.h"
#include "nb/pde_bot/finite_element/gaussp_to_nodes.h"
#include "nb/pde_bot/finite_element/solid_mechanics/static_damage2D.h"
  
#include "../utils.h"
#include "pipeline.h"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define POW2(a) ((a)*(a))

struct vcn_fem_implicit_s{
	uint32_t N_steps;
	uint32_t N_max_iter;
	uint32_t N_max_iter_without_enhance;
	double residual_tolerance;
};

static double tension_damage_r0(const nb_material_t *const mat);
static double tension_damage(const nb_material_t *const mat, 
			     double *strain,
			     double *r_damage_prev,
			     double *r_damage,
			     double characteristic_length_of_fractured_domain,
			     nb_analysis2D_t analysis2D);
/*
SECOND OPTION
static double tension_truncated_damage_r0(const nb_material_t *const mat);
static double tension_truncated_damage
			(const nb_material_t *const mat, 
			 double *strain,
			 double *r_damage_prev,
			 double* r_damage,
			 double characteristic_length_of_fractured_domain,
			 nb_analysis2D_t analysis2D);
*/
static void DMG_pipeline_assemble_system
		(vcn_sparse_t* K, double* M, double *F,
		 const vcn_msh3trg_t *const mesh,
		 const vcn_fem_elem_t *const elem,
		 const nb_material_t *const material,
		 bool enable_self_weight,
		 double gravity[2],
		 nb_analysis2D_t analysis2D,
		 nb_analysis2D_params *params2D,
		 bool enable_computing_damage,
		 double* damage_elem,
		 bool* elements_enabled /* NULL to enable all */);

static void DMG_pipeline_compute_strain
			(double *strain,
			 const vcn_msh3trg_t *const mesh,
			 double *displacement,
			 const vcn_fem_elem_t *const elem,
			 bool enable_computing_damage,
			 nb_analysis2D_t analysis2D,
			 const nb_material_t *const material,
			 double *damage,
			 double *r_dmg_prev,
			 double *r_dmg);

vcn_fem_implicit_t* vcn_fem_implicit_create(void)
{
	return (vcn_fem_implicit_t*)
		calloc(1, sizeof(vcn_fem_implicit_t));
}

void vcn_fem_implicit_destroy(vcn_fem_implicit_t* isparams){
	free(isparams);
}

void vcn_fem_implicit_set_N_steps(vcn_fem_implicit_t* isparams,
				  uint32_t N_steps){
	isparams->N_steps = N_steps;
}

void vcn_fem_implicit_set_N_max_iter(vcn_fem_implicit_t* isparams,
				     uint32_t N_max_iter){
	isparams->N_max_iter = N_max_iter;
}

void vcn_fem_implicit_set_N_max_iter_without_enhance
(vcn_fem_implicit_t* isparams, uint32_t N_max_iter){
	isparams->N_max_iter_without_enhance = N_max_iter;
}

void vcn_fem_implicit_set_residual_tolerance
(vcn_fem_implicit_t* isparams, double penergy_tol)
{
	isparams->residual_tolerance = penergy_tol;
}

uint32_t vcn_fem_implicit_get_N_steps(vcn_fem_implicit_t* isparams){
	return isparams->N_steps;
}

uint32_t vcn_fem_implicit_get_N_max_iter(vcn_fem_implicit_t* isparams){
	return isparams->N_max_iter;
}

uint32_t vcn_fem_implicit_get_N_max_iter_without_enhance
(vcn_fem_implicit_t* isparams){
	return isparams->N_max_iter_without_enhance;
}

double vcn_fem_implicit_get_residual_tolerance
(vcn_fem_implicit_t* isparams)
{
	return isparams->residual_tolerance;
}

static inline double tension_damage_r0(const nb_material_t *const mat)
{
	return nb_material_get_traction_limit_stress(mat) /
		sqrt(nb_material_get_elasticity_module(mat));
}

static double tension_damage(const nb_material_t *const mat, 
			     double *strain,
			     double *r_damage_prev,
			     double *r_damage,
			     double characteristic_length_of_fractured_domain,
			     nb_analysis2D_t analysis2D)
{
	double D[4];
	nb_pde_get_constitutive_matrix(D, mat, analysis2D);
	double E = nb_material_get_elasticity_module(mat);
	double Gf = nb_material_get_fracture_energy(mat);
	double ft = nb_material_get_traction_limit_stress(mat);

	/* Compute effective stress */
	double effective_stress[3];
	effective_stress[0] = D[0] * strain[0] + D[1] * strain[1];
	effective_stress[1] = D[1] * strain[0] + D[2] * strain[1];
	effective_stress[2] = D[3] * strain[2];

	/* Compute principal stress using Mohr's circle */
	double sigma_avg = 0.5 * (effective_stress[0] + effective_stress[1]);
	double R = sqrt(0.25 * 
			POW2(effective_stress[0] - effective_stress[1]) +
			POW2(effective_stress[2]));
	double positive_stress1 = MAX(0, sigma_avg + R);
	double positive_stress2 = MAX(0, sigma_avg - R);

	/* Compute inverse of D */
	double detD2x2 = D[0]*D[2] - D[1]*D[1];
	double id11 =  D[2]/detD2x2;
	double id12 = -D[1]/detD2x2;
	double id22 =  D[0]/detD2x2;
	/* Compute stress ^T strain */
	double sTs =
		id11 * POW2(positive_stress1) + 
		2 * id12 * positive_stress1 * positive_stress2 +
		id22 * POW2(positive_stress2);

	/* Compute tau */
	double tau = sqrt(sTs);

	/* Compute and return damage */
	double r0 = tension_damage_r0(mat);
	r_damage[0] = MAX(r_damage_prev[0], tau);
	double div =  
		(Gf/characteristic_length_of_fractured_domain)*(E/POW2(ft));
	double A = 1.0 / (div - 0.5);
	double G = 1.0 - (r0/r_damage[0])*exp(A*(1.0-(r_damage[0]/r0)));
	return G;
}

/*
SECOND OPTION
static inline double tension_truncated_damage_r0
			(const nb_material_t *const mat)
{
	return nb_material_get_traction_limit_stress(mat);
}

static double tension_truncated_damage
			(const nb_material_t *const mat, 
			 double *strain,
			 double *r_damage_prev,
			 double* r_damage,
			 double characteristic_length_of_fractured_domain,
			 nb_analysis2D_t analysis2D)
{
	double D[4];
	nb_pde_get_constitutive_matrix(D, mat, analysis2D);
	double E = nb_material_get_elasticity_module(mat);
	double Gf = nb_material_get_fracture_energy(mat);
	double ft = nb_material_get_traction_limit_stress(mat);

// Compute effective stress
	double effective_stress[3];
	effective_stress[0] = D[0] * strain[0] + D[1] * strain[1];
	effective_stress[1] = D[1] * strain[0] + D[2] * strain[1];
	effective_stress[2] = D[3] * strain[2] * 0.5;

// Compute principal stress using Mohr's circle
	double sigma_avg = 0.5 * (effective_stress[0] + effective_stress[1]);
	double R = sqrt(0.25 * 
			POW2(effective_stress[0] - effective_stress[1]) +
			POW2(effective_stress[2]));
	double main_stress1 = sigma_avg + R;
	double main_stress2 = sigma_avg - R;

// Compute tau
	double tau = MAX(0, main_stress1) + 
		MAX(0, main_stress2);

// Compute and return damage
	double r0 = tension_damage_r0(mat);
	double beta = 2.0;
	r_damage[0] = MAX(r_damage_prev[0], tau);
	double pi = (beta * characteristic_length_of_fractured_domain *
		     POW2(ft))
		/(2*E*Gf);
	if(pi > 1.0) pi = 1.0;
	double Hs = pi/(1.0 - pi);
	double G = 1.0 - (r0/r_damage[0])*exp(2.0*Hs*(1.0-r_damage[0]/r0));
	return G;
}
*/

void vcn_fem_compute_2D_Non_Linear_Solid_Mechanics
			(const vcn_msh3trg_t *const mesh,
			 const vcn_fem_elem_t *const elem,
			 const nb_material_t *const material,
			 const nb_bcond_t *const bcond,
			 bool enable_self_weight,
			 double gravity[2],
			 bool enable_Cholesky_solver,
			 nb_analysis2D_t analysis2D,
			 nb_analysis2D_params *params2D,
			 vcn_fem_implicit_t* params,
			 bool restore_computation,
			 const char* logfile)
/* Quasistatic formulation */
{
	uint32_t N_vertices = mesh->N_vertices;
	uint32_t N_elements = mesh->N_triangles;
	double* vertices = mesh->vertices;
	uint32_t* connectivity_mtx = mesh->vertices_forming_triangles;

	uint32_t omp_parallel_threads = 1;

	FILE *log = fopen(logfile, "a");
	fprintf(log, "FEM: Damage Model\n");
	fclose(log);
  
	uint32_t N_system_size = N_vertices * 2;

	uint8_t N_gp = vcn_fem_elem_get_N_gpoints(elem);

	/*******************************************************************/
	/*********************** > ?????? **********************************/
	/*******************************************************************/
	double* displacement = (double*)calloc(2 * N_vertices, sizeof(double));
	double* strain = (double*)calloc(3 * N_elements * N_gp, sizeof(double));
	double* damage = (double*)calloc(N_elements * N_gp, sizeof(double));
	double* r_dmg = (double*)malloc(N_gp * N_elements * sizeof(double));

	/* Initialize r parameter used for damage calculation */
	for (uint32_t i = 0; i < N_gp * N_elements; i++)
		r_dmg[i] = tension_damage_r0(material);

	/*******************************************************************/
	/****************** > Allocate system ******************************/
	/*******************************************************************/
	/* Allocate global Stiffness Matrices */
	vcn_graph_t *graph = vcn_msh3trg_create_vtx_graph(mesh);
	vcn_sparse_t* K = vcn_sparse_create(graph, NULL, 2);
	vcn_sparse_t *L = NULL;
	vcn_sparse_t *U = NULL;
	/* Allocate the triangular matrices L and U using symbolic Cholesky */
	vcn_sparse_alloc_LU(K, &L, &U);

  
	/* Allocate force vectors and displacement increment */
	double* F = (double*)calloc(N_system_size, sizeof(double));
	double* P = (double*)calloc(N_system_size, sizeof(double));
	double* residual = (double*)calloc(N_system_size, sizeof(double));
	double* du = (double*)calloc(N_system_size, sizeof(double));

	/* Allocate damage parameter 'r' */
	double* r_dmg_prev = (double*)malloc(N_gp * N_elements * sizeof(double));
  
	/*******************************************************************/
	/******************* > Start simulation of N steps *****************/
	/*******************************************************************/
	for (uint32_t n = 0; n < vcn_fem_implicit_get_N_steps(params); n++) {
		log = fopen(logfile, "a");
		fprintf(log, "  [ Load step %i]\n", n + 1);
		fclose(log);
		memcpy(r_dmg_prev, r_dmg, N_gp * N_elements * sizeof(double));

		/***********************************************************/
		/*************** > Implicit integration ********************/
		/***********************************************************/
		/* Implicit integration scheme */
		if(!enable_Cholesky_solver)
			memset(du, 0, N_system_size * sizeof(double));

		double residual_norm = 1;
		uint32_t residual_iter = 0;
		uint32_t residual_iter_without_enhance = 0;
		double residual_best;
		while(1) {
			/**************************************************/
			/********* > Assemble system **********************/
			/**************************************************/
			DMG_pipeline_assemble_system(K, NULL, F,
						     mesh,
						     elem,
						     material,
						     enable_self_weight,
						     gravity,
						     analysis2D,
						     params2D,
						     true, /* Enable computing damage */
						     damage,
						     NULL);

			/*****************************************/
			/****** > Set boundary conditions ********/
			/*****************************************/
			double condition_factor =
				(n + 1.0)/(double) vcn_fem_implicit_get_N_steps(params);

			/* Set Boundary Conditions */
			nb_pde_smech_set_bconditions(mesh, K, F, bcond,
						     condition_factor);

			/*******************************************/
			/******* > Verify residual *****************/
			/*******************************************/
			/* Compute P increment */
			vcn_sparse_multiply_vector(K, displacement, P, omp_parallel_threads);

			/* Compute residual norm */
			residual_norm = 0;
			for(uint32_t i=0; i < N_system_size; i++){
				residual[i] = F[i] - P[i];
				residual_norm += POW2(residual[i]);
			}
      
			residual_norm = sqrt(residual_norm);

			/* Check if residual is minimized */
			if(residual_iter == 0){
				residual_best = residual_norm;
			}else{
				if(residual_norm < residual_best){
					residual_best = residual_norm;
					residual_iter_without_enhance = 0;
				}else
					residual_iter_without_enhance += 1;
			}

			/* Check stop criteria */
			if((residual_norm < params->residual_tolerance
			    && residual_iter > 0) || residual_iter >= params->N_max_iter ||
			   residual_iter_without_enhance >= params->N_max_iter_without_enhance){
				/* Write logfile */
				log = fopen(logfile, "a");
				fprintf(log, "    [%i: Residual: %e ]\n",
					residual_iter, residual_norm);
				fclose(log);
				break;
			}

			/***********************************************/
			/** > Solve system (to compute displacements) **/
			/***********************************************/
			char solver_status = 0; /* Default initialization (Must be initialized) */
			if(enable_Cholesky_solver){
				/* Decompose matrix */
				solver_status = 
					vcn_sparse_decompose_Cholesky(K, L, U, omp_parallel_threads);

				if(solver_status != 0)
					vcn_sparse_decompose_LU(K, L, U, omp_parallel_threads);
  
				/* Solve system */
				vcn_sparse_solve_LU(L, U, residual, du);
			}
			if(!enable_Cholesky_solver || solver_status != 0){
				if(solver_status != 0){
					log = fopen(logfile, "a");
					fprintf(log, "    [Cholesky Fails (The stiffness matrix is not positive definite).]\n");
					fprintf(log, "    [Solving with Conjugate Gradient (Tolerance: 1e-8)...]\n");
					fclose(log);
				}
				double error = 0.0; /* Default initialization (Must be initialized) */
				uint32_t iters = 0;     /* Default initialization (Must be initialized) */
				solver_status = 
					vcn_sparse_solve_CG_precond_Jacobi(K, residual, du,
									   vcn_sparse_get_size(K),
									   1e-8,
									   &iters, &error,
									   omp_parallel_threads);
				if(solver_status != 0){
					solver_status = vcn_sparse_solve_Gauss_Seidel(K, residual, du,
										      60 * vcn_sparse_get_size(K),
										      1e-8,
										      &iters, &error,
										      omp_parallel_threads);
					if(solver_status != 0){
						log = fopen(logfile, "a");
						fprintf(log, "    [Conjugate Gradient (p. Jacobi) and Gauss-Seidel fails.]\n");
						fprintf(log, "      [Gauss-Seidel iterations: %u]\n", iters);
						fprintf(log, "      [Gauss-Seidel error: %e ]\n", error);
						fclose(log);
					}
				}
			}
			/* Write logfile */
			log = fopen(logfile, "a");
			fprintf(log, "    [%i: Residual: %e]\n", residual_iter, residual_norm);
			fclose(log);

			/* Update displacements, velocities and accelerations */
			for(uint32_t i=0; i < N_system_size; i++)
				displacement[i] += du[i];
            

			/**********************************************/
			/***** > Compute Strain and Damage      *******/
			/**********************************************/
			DMG_pipeline_compute_strain(strain, mesh, displacement, elem, true,
						    analysis2D,
						    material, damage,
						    r_dmg_prev, r_dmg);

			/* Increase iterator */
			residual_iter ++;
		}
	}
	/*****************************************************************/
	/******************** > Free memory ******************************/
	/*****************************************************************/
	free(displacement);
	free(strain);
	free(damage);

	vcn_sparse_destroy(K);
	if(enable_Cholesky_solver){
		vcn_sparse_destroy(L);
		vcn_sparse_destroy(U);
	}

	free(F);
	free(du);
	free(residual);
	free(P); 
  
	free(r_dmg_prev);
	free(r_dmg);

	if(restore_computation){
		free(vertices);
		free(connectivity_mtx);
	}
}

static void DMG_pipeline_assemble_system
		(vcn_sparse_t* K, double* M, double *F,
		 const vcn_msh3trg_t *const mesh,
		 const vcn_fem_elem_t *const elem,
		 const nb_material_t *const material,
		 bool enable_self_weight,
		 double gravity[2],
		 nb_analysis2D_t analysis2D,
		 nb_analysis2D_params *params2D,
		 bool enable_computing_damage,
		 double* damage_elem,
		 bool* elements_enabled /* NULL to enable all */)
{
	uint32_t N_elements = mesh->N_triangles;

	vcn_sparse_reset(K);
	if (NULL != M)
		memset(M, 0, vcn_sparse_get_size(K) * sizeof(double));
	memset(F, 0, vcn_sparse_get_size(K) * sizeof(double));

	/* Allocate elemental Stiffness Matrix and Force Vector */
	uint8_t N_nodes = vcn_fem_elem_get_N_nodes(elem);
	double* Ke = (double*)malloc(4 * POW2(N_nodes) * sizeof(double));
	double* Me = NULL;
	if(M != NULL)
		Me = (double*)malloc(2 * N_nodes * sizeof(double));
	double* Fe = (double*)
		malloc(2 * N_nodes * sizeof(double));

	/* Assembly global system */
	uint32_t N_negative_jacobians = 0;
	for (uint32_t k = 0; k < N_elements; k++) {
		double D[4] = {1e-6, 1e-6, 1e-6, 1e-6};
		double density = nb_material_get_density(material);
		if (pipeline_elem_is_enabled(elements_enabled, k)) {
			nb_pde_get_constitutive_matrix(D, material,
						       analysis2D);
			density = 1e-6;
		}

		/* Allocate Cartesian derivatives for each Gauss Point */
		double* dNi_dx = malloc(N_nodes * sizeof(double));
		double* dNi_dy = malloc(N_nodes * sizeof(double));

		/* Compute constitutive matrix */
		double fx = 0.0;
		double fy = 0.0;
		if(enable_self_weight){
			fx = gravity[0] * density;
			fy = gravity[1] * density;
		}
    
		/* Integrate Ke and Fe using Gauss quadrature */
		memset(Ke, 0, 4 * POW2(N_nodes) * sizeof(double));
		if(M != NULL) 
			memset(Me, 0, 2 * N_nodes * sizeof(double));
		memset(Fe, 0, 2 * N_nodes * sizeof(double));

		uint8_t N_gp = vcn_fem_elem_get_N_gpoints(elem);
		for (uint32_t j = 0; j < N_gp; j++) {
			/* Get constitutive model */
			double Dr[4];
			memcpy(Dr, D, 4 * sizeof(double));
			if (enable_computing_damage) {
				Dr[0] *= (1.0 - damage_elem[k * N_gp + j]);
				Dr[1] *= (1.0 - damage_elem[k * N_gp + j]);
				Dr[2] *= (1.0 - damage_elem[k * N_gp + j]);
				Dr[3] *= (1.0 - damage_elem[k * N_gp + j]);
			}

			/* Compute Jacobian derivatives */
			double Jinv[4];
			double detJ = nb_fem_get_jacobian(elem, k, mesh, j, Jinv);

			if (0 > detJ)
				goto EXIT;

			nb_fem_get_derivatives(elem, j, Jinv, dNi_dx, dNi_dy);

			double thickness = params2D->thickness;
			pipeline_sum_gauss_point(elem, j, Dr, density, thickness,
						 detJ, dNi_dx, dNi_dy, fx, fy,
						 Ke, Me, Fe);
		}

		pipeline_add_to_global_system(elem, k, mesh, Ke, Me, Fe,
					      K, M, F);

		free(dNi_dx);
		free(dNi_dy);
	}
 EXIT:
	/* Free elemental stiffness matrix and force vector */
	free(Ke);
	if (M != NULL) 
		free(Me);
	free(Fe);
}

static void DMG_pipeline_compute_strain
			(double *strain,
			 const vcn_msh3trg_t *const mesh,
			 double *displacement,
			 const vcn_fem_elem_t *const elem,
			 bool enable_computing_damage,
			 nb_analysis2D_t analysis2D,
			 const nb_material_t *const material,
			 double *damage,
			 double *r_dmg_prev,
			 double *r_dmg)
{
	double *vertices = mesh->vertices;
	uint32_t N_elements = mesh->N_triangles;
	uint32_t *conn_mtx = mesh->vertices_forming_triangles;

	/* Initialize strains */
	uint8_t N_gp = vcn_fem_elem_get_N_gpoints(elem);
	memset(strain, 0, 3 * N_gp * N_elements * sizeof(double));

	/* Iterate over elements to compute strain, stress and damage at nodes */
	for (uint32_t k = 0 ; k < N_elements; k++) {
		uint8_t N_nodes = vcn_fem_elem_get_N_nodes(elem);
		double* dNi_dx = malloc(N_nodes * sizeof(double));
		double* dNi_dy = malloc(N_nodes * sizeof(double));

		/* Integrate domain */
		uint8_t N_gp = vcn_fem_elem_get_N_gpoints(elem);
		for (uint32_t j = 0; j < N_gp; j++) {
			double Jinv[4];
			double detJ = nb_fem_get_jacobian(elem, k, mesh, j, Jinv);      
			if (nb_fem_elem_is_distorted(detJ))
				goto EXIT;

			nb_fem_get_derivatives(elem, j, Jinv, dNi_dx, dNi_dy);

			uint32_t idx = k * N_gp + j;
			/* Compute Strain at Gauss Point */
			for (uint32_t i = 0; i < N_nodes; i++) {
				uint32_t inode = conn_mtx[k * N_nodes + i];
				strain[idx * 3] += dNi_dx[i] * displacement[inode * 2];
				strain[idx*3+1] += dNi_dy[i] * displacement[inode*2+1];
				strain[idx*3+2] += (dNi_dy[i] * displacement[inode * 2] +
						    dNi_dx[i] * displacement[inode*2+1]);
			}
			/* Compute damage */
			if(enable_computing_damage){
				uint32_t n1 = conn_mtx[k * 3];
				uint32_t n2 = conn_mtx[k*3+1];
				uint32_t n3 = conn_mtx[k*3+2];
				/* characteristic_length_of_fractured_domain */
				double clfd =
					sqrt(((vertices[n2 * 2] -
					       vertices[n1 * 2]) *
					      (vertices[n3*2+1] -
					       vertices[n1*2+1]) -
					      (vertices[n2*2+1] -
					       vertices[n1*2+1]) *
					      (vertices[n3 * 2] -
					       vertices[n1 * 2])));
      
				damage[idx] = 
					tension_damage(material, 
						       &(strain[idx*3]),
						       &(r_dmg_prev[idx]),
						       &(r_dmg[idx]),
						       clfd,
						       analysis2D);
			}
		}
		free(dNi_dx);
		free(dNi_dy);
	}
EXIT:
	return;
}
