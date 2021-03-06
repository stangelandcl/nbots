#ifndef __NB_PDE_BOT_CONTROL_VOLUME_SOLID_MECHANICS_STATIC_ELASTICITY2D_H__
#define __NB_PDE_BOT_CONTROL_VOLUME_SOLID_MECHANICS_STATIC_ELASTICITY2D_H__

#include <stdint.h>
#include <stdbool.h>
#include "nb/geometric_bot.h"

#include "nb/pde_bot/material.h"
#include "nb/pde_bot/common_solid_mechanics/analysis2D.h"
#include "nb/pde_bot/boundary_conditions/bcond.h"

int nb_cvfa_compute_2D_Solid_Mechanics
			(const nb_mesh2D_t *const part,
			 const nb_material_t *const material,
			 const nb_bcond_t *const bcond,
			 bool enable_self_weight, double gravity[2],
			 nb_analysis2D_t analysis2D,
			 nb_analysis2D_params *params2D,
			 double *displacement, /* Output */
			 double *strain,       /* Output */
			 char *boundary_mask   /* Output */);

void nb_cvfa_compute_stress_from_strain(const nb_mesh2D_t *part,
					const nb_material_t *const material,
					nb_analysis2D_t analysis2D,
					const double* strain,
					double* stress /* Output */);

#endif
