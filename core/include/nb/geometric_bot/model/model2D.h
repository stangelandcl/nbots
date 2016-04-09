#ifndef __NB_GEOMETRIC_BOT_MODEL_MODEL2D_H__
#define __NB_GEOMETRIC_BOT_MODEL_MODEL2D_H__

#include <stdbool.h>
#include <stdint.h>

#include "nb/geometric_bot/mesh/elements2D/triangles_struct.h"
#include "nb/graph_bot.h"

/**
 * @brief Geometry model defining the domain.
 */
typedef struct vcn_model_s vcn_model_t;

uint16_t vcn_model_get_memsize(void);
void vcn_model_init(void *model_ptr);
void vcn_model_copy(void *model_ptr, const void *src_model_ptr);
void vcn_model_finish(void *model_ptr);
void* vcn_model_create(void);
void* vcn_model_clone(const void *model_ptr);
void vcn_model_destroy(void *model_ptr);
void vcn_model_clear(void *model_ptr);
  
/**
 * @brief Load geometry from a plain text file.
 * @param[in] filename Name of the file storing the model.
 * @return The model if success, NULL if something goes wrong.
 */
vcn_model_t* vcn_model_load(const char* filename);

void vcn_model_load_from_arrays(vcn_model_t *model,
				double vertices[], uint32_t N_vertices,
				uint32_t segments[], uint32_t N_segments,
				double holes[], uint32_t N_holes);

void vcn_model_load_from_farrays(vcn_model_t *model,
				 float vertices[], uint32_t N_vertices,
				 uint32_t segments[], uint32_t N_segments,
				 float holes[], uint32_t N_holes);

/**
 * @brief Creates a geometry model of a rectangle.
 * @param[in] x_min X coordinate of the inferior left corner.
 * @param[in] y_min Y coordinate of the inferior left corner.
 * @param[in] x_min X coordinate of the superior right corner.
 * @param[in] y_min Y coordinate of the superior right corner.
 * @return Model of the rectangle.
 */
vcn_model_t* vcn_model_create_rectangle(double x_min,
					double y_min,
					double x_max,
					double y_max);

/**
 * @brief Creates a geometry model of a regular polygon.
 * @param[in] radius Radius of the polygon (distance from the center to the
 * vertices).
 * @param[in] x_center X coordinate of the center.
 * @param[in] y_center Y coordinate of the center.
 * @param[in] N_sides Number of sides of the polygon.
 * @return Model of the regular polygon.
 */
vcn_model_t* vcn_model_create_polygon(double radius,
				      double x_center,
				      double y_center,
				      uint32_t N_sides);

/**
 * @brief Creates a geometry model of a discretized circle, which is a 
 * regular polygon where the number of sides is defined by the side_length
 * parameter. The polygon approximating the circle will have at least 10
 * sides.
 * @param[in] radius Radius of the circle
 * @param[in] x_center X coordinate of the center.
 * @param[in] y_center Y coordinate of the center.
 * @param[in] side_length Expected size of the sides
 * @return Model of the circle.
 */
vcn_model_t* vcn_model_create_circle(double radius,
				     double x_center,
				     double y_center,
				     double side_length);

/**
 * @brief Build a geometry model from a triangulation.
 * @param[in] msh3trg Input triangulation.
 * @return Model if success, NULL if something goes wrong.
 */
vcn_model_t* vcn_model_create_from_msh3trg
(const vcn_msh3trg_t *const msh3trg);

void vcn_model_generate_from_msh3trg
(vcn_model_t *model, const vcn_msh3trg_t *const msh3trg);

/**
 * @brief Build a geometry model from a triangulation with disabled
 * triangles.
 * @param[in] msh3trg Input triangulation.
 * @param[in] trg_enabled Array of booleans indicating wich triangles are
 * enabled. NULL to enable all.
 * @param[out] N_real_vtx_boundaries Stores the number of vertices forming
 * the boundary that appears in the original input.
 * @param[out] real_vtx_boundaries Stores an array of the vertices ids in
 * the new model corresponding to original input vertices.
 * @return Model if success, NULL if something goes wrong.
 */
vcn_model_t* vcn_model_create_from_msh3trg_with_disabled_trg
(const vcn_msh3trg_t *const msh3trg,
 const bool *const trg_enabled,
 uint32_t *N_real_vtx_boundaries,
 uint32_t **real_vtx_boundaries);

/**
 * @brief Save model into a plain text file.
 * @param[in] model Model to be saved.
 * @param[in] filename File name.
 */
uint8_t vcn_model_save(const vcn_model_t *const model, const char* filename);

vcn_graph_t* vcn_model_get_vtx_graph(const vcn_model_t *const model);


/**
 * @brief Mark as holes all the enveloped areas in the model.
 * The enveloped areas are internal bounded areas.
 * @param[in] model to be processed.
 */
void vcn_model_set_enveloped_areas_as_holes(vcn_model_t* model);

/**
 * @brief Check if the vertex lies inside the model.
 * @n<b>WARNING:</b> A mesh is built internally, making this function
 * expensive, if you are going to test several vertices for the same
 * model please build a mesh using VCN_DENSITY_CDT and use the function
 * vcn_mesh_is_vtx_inside().
 * @param[in] model Geometry of the domain.
 * @param[in] vtx Vertex which is checked to be inside the domain.
 * @return <b>true</b> if the vertes lies inside the geometry
 * (<b>false</b> if the vertex lies outside the geometry).
 */
bool vcn_model_is_vtx_inside(const vcn_model_t *const model,
			     const double *const vtx);

/**
 * @brief Get enveloping box of the model.
 * @param[in] model Geometric model which will be inside the enveloping box.
 * @param[out] box Array (of doubles) storing the coordinates of the lower
 * left and the upper right corners of the minimum enveloping box of the
 * model.
 */  
void vcn_model_get_enveloping_box(const vcn_model_t *const model,
				  double box[4]);
  
double* vcn_model_get_holes(const vcn_model_t *const model, 
			    uint32_t* N_holes /* Output */);

double* vcn_model_get_vertices(const vcn_model_t *const model, 
			       uint32_t* N_vertices /* Output */);

uint32_t vcn_model_get_vertex_id(const vcn_model_t *const model, double* vtx);

double* vcn_model_get_vertex_coordinate
(const vcn_model_t *const model, uint32_t id);
  
uint32_t vcn_model_get_edge_id
(const vcn_model_t *const model, double* edge_vertices);
double* vcn_model_get_edge_coordinates
(const vcn_model_t *const model, uint32_t id);

uint32_t vcn_model_get_number_of_vertices(const vcn_model_t *const model);
  
uint32_t vcn_model_get_N_edges(const vcn_model_t *const model);
  
uint32_t vcn_model_get_number_of_hole_seeds(const vcn_model_t *const model);

double vcn_model_get_length_of_ith_edge(const vcn_model_t* model, uint32_t i);
  
double vcn_model_get_area(const vcn_model_t *const model);
  
double vcn_model_get_sum_of_sgm_length(const vcn_model_t *const model);

#endif