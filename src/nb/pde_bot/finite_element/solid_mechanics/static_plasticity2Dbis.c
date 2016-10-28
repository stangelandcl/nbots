#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "nb/memory_bot.h"
#include "nb/math_bot.h"
#include "nb/cfreader_cat.h"
#include "nb/solver_bot.h"
#include "nb/container_bot.h"
#include "nb/graph_bot.h"
#include "nb/geometric_bot.h"
#include "nb/pde_bot/material.h"
#include "nb/pde_bot/common_solid_mechanics/analysis2D.h"
#include "nb/pde_bot/common_solid_mechanics/formulas.h"
#include "nb/pde_bot/boundary_conditions/bcond.h"
#include "nb/pde_bot/boundary_conditions/bcond_iter.h"
#include "nb/pde_bot/finite_element/element.h"
#include "nb/pde_bot/finite_element/gaussp_to_nodes.h"
#include "nb/pde_bot/finite_element/solid_mechanics/static_elasticity2D.h"

#include "nb/pde_bot/finite_element/solid_mechanics/set_bconditions.h"
#include "pipeline.h"
#include "nb/pde_bot/common_solid_mechanics/analysis2D.h"
#include "nb/pde_bot/finite_element/solid_mechanics/static_plasticity2D.h"

#define POW2(a) ((a)*(a))

double stress_max_tolerance(double *plastic_stress, nb_plastified_analysis2D *elem_regime, uint32_t N_elem);

int fem_compute_plastic_2D_Solid_Mechanics_bis
			(const nb_mesh2D_t *const part,
			 const nb_fem_elem_t *const elemtype,
			 const nb_material_t *const material,
			 const nb_bcond_t *const bcond,
			 bool enable_self_weight,
			 double gravity[2],
			 nb_analysis2D_t analysis2D,
			 nb_analysis2D_params *params2D,
			 const bool *elements_enabled, /* NULL to enable all */
			 double *total_strain, /* Output*/
			 double *stress,
			 double *total_displacement, /* Output, just the last computed plastic displacement */
			 uint32_t N_force_steps,
			 double stress_tolerance)
{
    printf("Number of force steps: %i\n", N_force_steps);
    printf("Accepted tolerance for yield stress: %f\n", stress_tolerance);
	int status = 0;
	nb_graph_t *graph = malloc(nb_graph_get_memsize());
	nb_graph_init(graph);
	nb_mesh2D_load_graph(part, graph, NB_NODES_LINKED_BY_ELEMS);
	nb_sparse_t *K = nb_sparse_create(graph, NULL, 2);
	nb_graph_finish(graph);

	uint32_t N_nod = nb_mesh2D_get_N_nodes(part);
	printf("Number of nodes: %i\n", N_nod);
	uint32_t N_elem = nb_mesh2D_get_N_elems(part);
	printf("Number of elements: %i\n", N_elem);
	uint32_t F_memsize = 2 * N_nod * sizeof(double);
	uint32_t F_elemsize = 3*N_elem*sizeof(double);
	double *F = nb_soft_allocate_mem(F_memsize);
	memset(F, 0, F_memsize);
	double *strain = nb_allocate_mem(F_elemsize);
	double *displacement = nb_allocate_mem(F_memsize);
	double *elastic_strain = nb_allocate_mem(F_elemsize);
	double *elastic_force = nb_allocate_mem(F_memsize);
	memset(total_strain, 0, F_elemsize);

	int status_assemble =
	pipeline_assemble_system(K, NULL, F, part, elemtype, material,
					 enable_self_weight, gravity,
					 analysis2D, params2D,
					 elements_enabled);
	if (0 != status_assemble) {
		status = 1;
		goto CLEANUP_LINEAR_SYSTEM;
	}

	nb_fem_set_bconditions(part, K, F, bcond, 1.0);

    nb_plastified_analysis2D *elem_regime = nb_allocate_mem(N_elem*10*sizeof(char));

    for (int i = 0; i < N_elem; i++) {
        elem_regime[i] = NB_ELASTIC;
    }

    double *dF_basic = nb_soft_allocate_mem(F_memsize);
    memset(dF_basic, 0, F_memsize);
	for (int i = 0; i < 2*N_nod; i++) {
        dF_basic[i] = F[i] / N_force_steps;
    }
    double *dF = nb_soft_allocate_mem(F_memsize);
    memset(dF, 0, F_memsize);
    double *dFaux = nb_soft_allocate_mem(F_memsize);
    double *dF_increment = nb_allocate_mem(3*N_elem*sizeof(dF_increment));

    /********************************************************************************************************************************/
    /***************************************** Apply the force recursively step by step *********************************************/
    /********************************************************************************************************************************/
    for (int i = 0; i < N_force_steps; i++) {
        for (int j = 0; j < 2*N_nod; j++)
        {
            dF[j] += dF_basic[j];
        }
        int solver_status = plastic_solver(K, dF, displacement);

        if(0 != solver_status){
            status = 2;
            goto CLEANUP_LINEAR_SYSTEM;
        }

        pipeline_compute_strain(strain, part, displacement, elemtype);

        nb_fem_compute_plastic_stress_from_strain_bis(N_elem, elemtype, material, analysis2D,
                                                      strain, elements_enabled, stress, elem_regime, elastic_strain);

        double yield_stress = nb_material_get_yield_stress(material);
        uint32_t N_plastic_elem = 0;

        for(int j = 0; j < N_elem; j++) {
            double vm_stress;
            vm_stress = nb_pde_get_vm_stress(stress[3*j], stress[3*j +1], stress[3*j +2]);
            if(vm_stress > yield_stress && elem_regime[j] != NB_PLASTIC) {
                N_plastic_elem += 1;
            }
        }

        uint64_t n = 2;
        memset(dFaux, 0, F_memsize);
        memset(dF_increment, 0, F_memsize);
        if(N_plastic_elem != 0){
            for(int k = 0; k < 2*N_nod; k++){
                dF_increment[k] = dF_basic[k]/n;
                dFaux[k] = (dF[k]-dF_basic[k]) + dF_increment[k];
            }
        }
        printf("dF original 23: %lf\n", dF[23]);
        printf("dF auxiliar 23: %lf\n", dFaux[23]);
    /********************************************************************************************************************************/
    /************************************************* Loop over plastified elements ************************************************/
    /********************************************************************************************************************************/

        while(N_plastic_elem != 0) {
            double max_vm_stress = 0;
            uint32_t plastified_elem;
            for(int j = 0; j < N_elem; j++) {
                double vm_stress;
                vm_stress = nb_pde_get_vm_stress(stress[3*j], stress[3*j +1], stress[3*j +2]);
                if(vm_stress > yield_stress) {
                    if(vm_stress > max_vm_stress && elem_regime[j] != NB_PLASTIC) {
                        max_vm_stress = vm_stress;
                        plastified_elem = j;
                    }
                }
            }
            printf("Elements plastified: %d\n", N_plastic_elem);
            printf("Plastified element no %d\n", plastified_elem);
            printf("VM stress original: %lf\n", max_vm_stress); /* TEMPORAL */
            double limit_stress = abs(max_vm_stress - yield_stress);
            printf("Limit stress original: %lf\n", limit_stress); /* TEMPORAL */
    /********************************************************************************************************************************/
    /******************************************** Loop over maximum plastified element **********************************************/
    /********************************************************************************************************************************/
            uint16_t iter = 0;

            while(limit_stress > stress_tolerance){

            if(iter > 0 && max_vm_stress > yield_stress) {
                n *= 2;
                for(int k = 0; k < 2*N_nod; k++) {
                    dF_increment[k] = dF_basic[k]/n;
                    dFaux[k] -= dF_increment[k];
                }
            }
            if(iter > 0 && max_vm_stress < yield_stress) {
                n *= 2;
                for(int k = 0; k < 2*N_nod; k++) {
                    dF_increment[k] = dF_basic[k]/n;
                    dFaux[k] += dF_increment[k];
                }
            }
            printf("dF_increment 23: %lf\n", dF_increment[23]);
            printf("Force: %lf\n", dF[23]);
            printf("Aux. Force [%d]: %lf\n", iter, dFaux[23]);
            iter += 1;

                solver_status = plastic_solver(K, dFaux, displacement);

                if(0 != solver_status){
                    status = 3;
                    goto EXIT;
                }
                pipeline_compute_strain(strain, part, displacement, elemtype);

                nb_fem_compute_plastic_stress_from_strain_bis(N_elem, elemtype, material, analysis2D,
                                                              strain, elements_enabled, stress, elem_regime, elastic_strain);

                max_vm_stress = nb_pde_get_vm_stress(stress[3*plastified_elem], stress[3*plastified_elem + 1], stress[3*plastified_elem + 2]);
                printf("VM stress[%d]: %lf\n",iter, max_vm_stress); /* TEMPORAL */
                limit_stress = abs(max_vm_stress - yield_stress);
                printf("Limit stress[%d]: %lf\n",iter, limit_stress); /* TEMPORAL */
            }

            elastic_strain[3*plastified_elem] = strain[3*plastified_elem];
            elastic_strain[3*plastified_elem + 1] = strain[3*plastified_elem + 1];
            elastic_strain[3*plastified_elem + 2] = strain[3*plastified_elem + 2];
            elastic_force[2*plastified_elem] = dFaux[2*plastified_elem];
            elastic_force[2*plastified_elem + 1] = dFaux[2*plastified_elem];
            elem_regime[plastified_elem] = NB_PLASTIC;

            for(int k = 0; k < 2*N_nod; k++) {
                total_displacement[k] = displacement[k];
            }

            N_plastic_elem -= 1;

            EXIT:
            if(solver_status = 3)
                goto CLEANUP_LINEAR_SYSTEM;
        }
    }
    CLEANUP_LINEAR_SYSTEM:
    nb_sparse_destroy(K);
	nb_soft_free_mem(F_memsize, F);
	nb_soft_free_mem(F_memsize, dF_basic);
	nb_soft_free_mem(F_memsize, dF);
	nb_soft_free_mem(F_memsize, dFaux);
	nb_free_mem(dF_increment);
	nb_free_mem(displacement);
	nb_free_mem(strain);
	nb_free_mem(elem_regime);

	return status;
}

void nb_fem_compute_plastic_stress_from_strain_bis
			(uint32_t N_elements,
			 const nb_fem_elem_t *const elem,
			 const nb_material_t *const material,
			 nb_analysis2D_t analysis2D,
			 double* strain,
			 const bool* elements_enabled ,
			 double* stress ,
			 nb_plastified_analysis2D *elem_regime,
			 double *elastic_strain)
{

	//uint32_t omp_parallel_threads = 1;
    #pragma omp parallel for num_threads(omp_parallel_threads) schedule(guided)
    memset(stress, 0, 3*N_elements*sizeof(stress));
	for (uint32_t i = 0; i < N_elements; i++) {

		double De[4] = {1e-6, 1e-6, 1e-6, 1e-6};
		double Dp[4] = {1e-6, 1e-6, 1e-6, 1e-6};

        if (pipeline_elem_is_enabled(elements_enabled, i)) {
            switch(elem_regime[i]){
                case NB_PLASTIC:{
                    nb_pde_get_plastified_constitutive_matrix(Dp, material,
						       analysis2D, elem_regime[i]);
                    nb_pde_get_constitutive_matrix(De, material, analysis2D);
                    uint8_t N_gp = nb_fem_elem_get_N_gpoints(elem);
                    for (int j = 0; j < N_gp; j++) {
                        uint32_t id = i * N_gp + j;
                        stress[id * 3] = (strain[id * 3] - elastic_strain[id * 3]) * Dp[0] +
                            (strain[id*3 + 1] - elastic_strain[id*3 + 1]) * Dp[1] +
                            elastic_strain[id * 3]*De[0] + elastic_strain[id*3 + 1]*De[1];
                        stress[id*3 + 1] = (strain[id * 3] - elastic_strain[id * 3]) * Dp[1] +
                            (strain[id*3 + 1] - elastic_strain[id*3 + 1]) * Dp[2] +
                            elastic_strain[id*3 + 1]*De[1] + elastic_strain[id*3 + 1]*De[2];
                        stress[id*3 + 2] = (strain[id*3 + 2] - elastic_strain[id*3 + 2]) * Dp[3] +
                        elastic_strain[id*3 + 2]*De[3];
                    }
                break;
                }
                case NB_ELASTIC: {
                    nb_pde_get_constitutive_matrix(De, material, analysis2D);
                    uint8_t N_gp = nb_fem_elem_get_N_gpoints(elem);
                    for (int j = 0; j < N_gp; j++) {
                        uint32_t id = i * N_gp + j;
                        stress[id * 3] = strain[id * 3] * De[0] +
                            strain[id*3+1] * De[1];
                        stress[id*3+1] = strain[id * 3] * De[1] +
                            strain[id*3+1] * De[2];
                        stress[id*3+2] = strain[id*3+2] * De[3];
                    }
                break;
                }
            }
        }
	}
}
