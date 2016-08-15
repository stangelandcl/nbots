#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "nb/memory_bot.h"
#include "nb/math_bot.h"
#include "nb/cfreader_cat.h"
#include "nb/eigen_bot.h"
#include "nb/container_bot.h"
#include "nb/graph_bot.h"
#include "nb/geometric_bot.h"
#include "nb/pde_bot/material.h"
#include "nb/pde_bot/common_solid_mechanics/analysis2D.h"
#include "nb/pde_bot/common_solid_mechanics/formulas.h"
#include "nb/pde_bot/common_solid_mechanics/boundary_conditions.h"
#include "nb/pde_bot/boundary_conditions/bcond.h"
#include "nb/pde_bot/boundary_conditions/bcond_iter.h"
#include "nb/pde_bot/finite_element/element.h"
#include "nb/pde_bot/finite_element/gaussp_to_nodes.h"
#include "nb/pde_bot/finite_element/solid_mechanics/static_elasticity2D.h"

#include "pipeline.h"

#define POW2(a) ((a)*(a))

static int solver(const vcn_sparse_t *const A,
		  const double *const b, double* x);

int vcn_fem_compute_2D_Solid_Mechanics
			(const nb_partition_t *const part,
			 const vcn_fem_elem_t *const elemtype,
			 const nb_material_t *const material,
			 const nb_bcond_t *const bcond,
			 bool enable_self_weight,
			 double gravity[2],
			 nb_analysis2D_t analysis2D,
			 nb_analysis2D_params *params2D,
			 const bool *elements_enabled, /* NULL to enable all */
			 double *displacement, /* Output */
			 double *strain        /* Output */)
{
	int status = 0;
	vcn_graph_t *graph = malloc(vcn_graph_get_memsize());
	vcn_graph_init(graph);
	nb_partition_load_elem_graph(part, graph);
	vcn_sparse_t *K = vcn_sparse_create(graph, NULL, 2);
	vcn_graph_finish(graph);

	uint32_t N_nod = nb_partition_get_N_nodes(part);
	uint32_t F_memsize = 2 * N_nod * sizeof(double);
	double* F = NB_SOFT_MALLOC(F_memsize);
	memset(F, 0, F_memsize);

	int status_assemble =
		pipeline_assemble_system(K, NULL, F, part, elemtype, material,
					 enable_self_weight, gravity,
					 analysis2D, params2D,
					 elements_enabled);
	if (0 != status_assemble) {
		status = 1;
		goto CLEANUP_LINEAR_SYSTEM;
	}

	nb_pde_smech_set_bconditions(part, K, F, bcond, 1.0);

  
	int solver_status = solver(K, F, displacement);
	if (0 != solver_status) {
		status = 2;
		goto CLEANUP_LINEAR_SYSTEM;
	}

	pipeline_compute_strain(strain, part, displacement, elemtype,
				analysis2D, material);
CLEANUP_LINEAR_SYSTEM:
	vcn_sparse_destroy(K);
	NB_SOFT_FREE(F_memsize, F);
	return status;
}

static int solver(const vcn_sparse_t *const A,
		  const double *const b, double* x)
{
	uint32_t N = vcn_sparse_get_size(A);
	memset(x, 0, N * sizeof(*x));
	int status = vcn_sparse_solve_CG_precond_Jacobi(A, b, x, N,
							1e-8, NULL,
							NULL, 1);
	int out;
	if (0 == status || 1 == status)
		out = 0;
	else
		out = 1; /* Tolerance not reached in CG Jacobi */
	return out;
}

void vcn_fem_compute_stress_from_strain
			(uint32_t N_elements,
			 const vcn_fem_elem_t *const elem,
			 const nb_material_t *const material,
			 nb_analysis2D_t analysis2D,
			 double* strain,
			 const bool* elements_enabled /* NULL to enable all */,
			 double* stress /* Output */)
{
	/* Compute stress from element strain */
	uint32_t omp_parallel_threads = 1;
#pragma omp parallel for num_threads(omp_parallel_threads) schedule(guided)
	for (uint32_t i = 0; i < N_elements; i++) {
		double D[4] = {1e-6, 1e-6, 1e-6, 1e-6};
		if (pipeline_elem_is_enabled(elements_enabled, i))
			nb_pde_get_constitutive_matrix(D, material,
						       analysis2D);

		uint8_t N_gp = vcn_fem_elem_get_N_gpoints(elem);
		for (int j = 0; j < N_gp; j++) {
			uint32_t id = i * N_gp + j;
			stress[id * 3] = strain[id * 3] * D[0] +
				strain[id*3+1] * D[1];
			stress[id*3+1] = strain[id * 3] * D[1] +
				strain[id*3+1] * D[2];
			stress[id*3+2] = strain[id*3+2] * D[3];
		}
	}
}

void vcn_fem_compute_von_mises(uint32_t N,
			       double *stress,
			       double *von_mises /* Output */)
{
	for (uint32_t i = 0; i < N; i++) {
		von_mises[i] = nb_pde_get_vm_stress(stress[i * 3],
						    stress[i*3+1],
						    stress[i*3+2]);
	}
}
