#ifndef __NB_PDE_BOT_MATERIAL_H__
#define __NB_PDE_BOT_MATERIAL_H__

typedef struct nb_material_s nb_material_t;

nb_material_t* nb_material_create(void);
nb_material_t* nb_material_clone(nb_material_t* mat);
void nb_material_destroy(nb_material_t* mat);

void nb_material_set_poisson_module(nb_material_t* mat, double pmodule);
void nb_material_set_elasticity_module(nb_material_t* mat, double emodule);
void nb_material_set_density(nb_material_t* mat, double density);
void nb_material_set_fracture_energy(nb_material_t* mat, double frac_energy);
void nb_material_set_traction_limit_stress(nb_material_t* mat,
					   double max_stress);
void nb_material_set_compression_limit_stress(nb_material_t* mat,
					      double max_stress);
void nb_material_set_energy_release_rate(nb_material_t *mat, double energy_rr);
void nb_material_set_damage_length_scale(nb_material_t *mat,
					 double length_scale);

double nb_material_get_poisson_module(const nb_material_t *const mat);
double nb_material_get_elasticity_module(const nb_material_t *const mat);
double nb_material_get_density(const nb_material_t *const mat);
double nb_material_get_fracture_energy(const nb_material_t *const mat);
double nb_material_get_traction_limit_stress(const nb_material_t *const mat);
double nb_material_get_compression_limit_stress(const nb_material_t *const mat);
double nb_material_get_energy_release_rate(const nb_material_t *const mat);
double nb_material_get_damage_length_scale(const nb_material_t *const mat);
int nb_material_verify(nb_material_t* material);

#endif
