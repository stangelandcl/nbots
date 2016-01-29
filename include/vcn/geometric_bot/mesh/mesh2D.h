#ifndef __VCN_MESH_ENGINE2D_H__
#define __VCN_MESH_ENGINE2D_H__

#include <stdbool.h>
#include <stdint.h>

#include "vcn/geometric_bot/model/model2D.h"
#include "vcn/graph_bot.h"

  /**
   * @brief Set as max_angle param in vcn_mesh_refine() for unconstrained
   * minimum angle.
   */
#define VCN_ANGLE_UNCONSTRAIN (0.0)

  /**
   * @brief Set as max_angle param in vcn_mesh_refine() in order to generate
   * a refined mesh with the maximum quality predicted by theory.
   * Minimum angle bound equivalent to 26.45 degrees.
   */
#define VCN_ANGLE_MAX (0.46163958715250017309)


#ifdef __cplusplus
extern "C" {
#endif
	/**
	 * @brief Write-only mesh structure used to create and modify meshes.
	 * This mesh is based on a Delaunay triangulation.
	 * In order to read the mesh, it must be "converted" into a read-only 
	 * structure, such as vcn_msh3trg_t.
	 */
	typedef struct vcn_mesh_s vcn_mesh_t;  /* Triangular Based Mesh */

	vcn_mesh_t* vcn_mesh_create(void);
	void vcn_mesh_set_do_after_trg(vcn_mesh_t *mesh,
				       void (*do_after_trg)
				       		(const vcn_mesh_t *const));
	void vcn_mesh_set_max_vtx(vcn_mesh_t* mesh, uint32_t N);
	void vcn_mesh_set_max_trg(vcn_mesh_t* mesh, uint32_t N);
	void vcn_mesh_set_min_angle(vcn_mesh_t* mesh, double angle);
	void vcn_mesh_set_density_CDT(vcn_mesh_t* mesh);
	void vcn_mesh_set_density_EDGE(vcn_mesh_t* mesh, double edge_max_size,
				       double subsgm_max_size);
	void vcn_mesh_set_density_IMG(vcn_mesh_t* mesh, void *img_data);
	void vcn_mesh_set_density_function(vcn_mesh_t* mesh,
					   double (*density)(double*));
	bool vcn_mesh_is_empty(const vcn_mesh_t *const mesh);

	/**
	 * @brief Create an identical copy of the mesh.
	 * @param[in] mesh Mesh to be cloned.
	 * @return Cloned mesh if success, NULL if something goes wrong.
	 */
	vcn_mesh_t* vcn_mesh_clone(const vcn_mesh_t* const mesh);
  
	/**
	 * @brief Creates a mesh from a Planar Straight Line Graph (PSLG, encoded
	 * in the model) as exposed by Schewchuck:
	 *    + Calculates the Constrained Delauany Triangulation from the PSLG.
	 *    + Remove triangles from concavities and from holes.
	 *    + Delaunay Refinement (Circumcenter insertion of poor-quaility
	 *      triangles). The internal routine is mesh_refine(), which
	 *      is size optimal for a given minimum angle (please read the description
	 *      of this routine).
	 *
	 * @see J. R. Shewchuk.
	 * <b> Reprint of: Delaunay refinement algorithms for triangular mesh
	 * generation.</b> Computational Geometry 47 (2014), pages 741-778.
	 * @see G. L. Miller, S. E. Pav and N. J. Walkington.
	 * <b> When and why Ruppert's algorithm works.</b> 2003
	 *
	 * @param[in] model Structure which contains the PSLG to be triangulated;
	 * the model also contains the location of the points inside the holes,
	 * at least one point for each hole.
	 *
	 * @param[in] max_vtx Maximum number of vertices, which should be 
	 * superior to the number of input vertices. Set zero for an undefined
	 * maximum number of vertices (keeping guarantees).
	 * The use of a max number of vertices could truncates the Delaunay 
	 * refinment process if it runs-out of vertices. 
	 * Use this feature only if it is strictly necessary.
	 *
	 * @param[in] max_trg Maximum number of triangles. Set zero for an undefined
	 * maximum number of triangles (keeping guarantees).
	 * The use of a max number of triangles could truncates the Delaunay 
	 * refinment process if it runs-out of triangles.
	 * Use this feature only if it is strictly necessary.
	 *
	 * @param[in] min_angle Minimum angle allowed in the triangulation (in 
	 * radians).
	 * This angle must be in the range of 0 and <b>VCN_ANGLE_MAX</b>,
	 * which  corresponds to 26.45 degrees (0.4616 radians approx).
	 *
	 * @param[in] density Function to control the density of the triangulation,
	 * which must be greater than zero for all points.
	 * (See mesh_refine() documentation).
	 *
	 * @param[in] density_data Data used to estimate the density. It is given to
	 * the density function without alterations (See mesh_refine() documentation).
	 *
	 * @return The mesh created from the model geometry.
	 */
	void vcn_mesh_generate_from_model(vcn_mesh_t *mesh,
					  const vcn_model_t *const model);


	/**
	 * @brief Destroy mesh.
	 * @param[in] mesh Mesh to be destroyed.
	 */
	void vcn_mesh_destroy(vcn_mesh_t* mesh);

	/**
	 * @brief Check if the vertex lies inside the mesh.
	 * @param[in] mesh Discretization of the domain.
	 * @param[in] vtx Vertex which is checked to be inside the domain.
	 * @return <b>true</b> if the vertes lies inside the mesh 
	 * (<b>false</b> if the vertex lies outside the mesh).
	 */
	bool vcn_mesh_is_vtx_inside(const vcn_mesh_t *const mesh,
				    const double *const vtx);

	/**
	 * @brief Produces a refined Delaunay triangulation. The algorithm have
	 * the folowing properties:
	 * + The minimal angle in the output is greater or equal to 26.45
	 * degrees if the minimum input angle is greater than 36.53 degrees,
	 * otherwise it produces the least quantity of poor-quality triangles.
	 * + Termination is proven.
	 * + Size optimality is proven.
	 * + Good grading guarantees.
	 *
	 * @see J. Ruppert.
	 * <b> A Delaunay Refinement Algorithm for Quality 2-Dimensional
	 * Mesh Generation.</b> Journal of Algorithms 18 (1995), pages 548-585.
	 * @see J. R. Shewchuk.
	 * <b> Reprint of: Delaunay refinement algorithms for triangular mesh
	 * generation.</b> Computational Geometry 47 (2014), pages 741-778.
	 * @see G. L. Miller, S. E. Pav and N. J. Walkington.
	 * <b> When and why Ruppert's algorithm works.</b> 2003
	 *
	 * @param[in] mesh The triangulation which is going to be refined, it
	 * is assumed to be a Constrained Delaunay Triangulation.
	 *
	 * @param[in] max_vtx Maximum number of vertices, which should be 
	 * superior to the number of input vertices. Set zero for an undefined
	 * maximum number of vertices (keeping guarantees).
	 * The use of a max number of vertices could truncates the Delaunay 
	 * refinment process if it runs-out of vertices. 
	 * Use this feature only if it is strictly necessary.
	 *
	 * @param[in] max_trg Maximum number of triangles. Set zero for an undefined
	 * maximum number of triangles (keeping guarantees).
	 * The use of a max number of triangles could truncates the Delaunay 
	 * refinment process if it runs-out of triangles.
	 * Use this feature only if it is strictly necessary.
	 *
	 * @param[in] min_angle Minimum angle allowed in the triangulation (in 
	 * radians).
	 * This angle must be in the range of 0 and <b>VCN_ANGLE_MAX</b>,
	 * which corresponds to 26.45 degrees (0.4616 radians approx).
	 *
	 * @param[in] density Function to control the density of the triangulation,
	 * which must be greater than zero for all points.
	 * @n
	 * Small <b>local features</b> could produce finer meshes than those 
	 * indicated by the density function (because the minimum angle constraint). 
	 * @n
	 * The expected size of an edge is the inverse of the density.
	 * @n
	 * The following values trigger a default behaviour:
	 * + <b>NULL (or 0)</b>: No density defined, build the mesh with the minimum 
	 *   number of triangles for a given minimum angle.
	 *   <b>density_data</b> must be <b>NULL</b>.
	 * + <b>VCN_DENSITY_CDT</b>: Used to build the Constrained Delaunay
	 *   Triangulation (CDT) inside the domain. The CDT is the triangulation which
	 *   maximizes the minimum angle of all the triangles, using only the input
	 *   vertices.
	 * + <b>VCN_DENSITY_MAX</b>: Used to define maximum size of edges and input
	 *   subsegments.
	 *   <b>density_data</b> must be an array <b>'double max[2]'</b>, which
	 *   defines the maximum edge length into max[0] and the maximum subsegment
	 *   length into max[1]. To have any effect, the value max[1] must be smaller
	 *   than max[0] because an input segment is also an edge (a greater value has
	 *   not effect).
	 *   If some max value equals zero then it is not considered, hence you can
	 *   constrain only the subsegments size.
	 * + <b>VCN_DENSITY_IMG</b>: The density is given by an image.
	 *   <b>density_data</b> must be a pointer to <b>vcn_density_img_t</b>.
	 *
	 * @param[in] density_data Data used to estimate the density. It is given
	 * to the density function without alterations.
	 * Set NULL if not required.
	 */
	void vcn_mesh_refine(vcn_mesh_t *const mesh,
			     uint32_t max_vtx,
			     uint32_t max_trg,
			     double min_angle,
			     double (*density)(const void *const data,
					       const double const x[2]),
			     const void *const density_data);
  
	/**
	 * @brief Insert a new vertex inside the mesh while maintaining
	 * the Delaunay condition.
	 * @param[in] mesh Mesh where the vertex is going to be inserted.
	 * @param[in] vertex Vertex to insert.
	 * @return Status of the operation:
	 * + <b>0</b> Successful insertion.
	 * + <b>1</b> The point lies outside the mesh.
	 */
	int vcn_mesh_insert_vertex(vcn_mesh_t *const mesh, const double *const vertex);

  
	void vcn_mesh_get_vertices(vcn_mesh_t* mesh, double* vertices);

	uint32_t vcn_mesh_get_N_vtx(const vcn_mesh_t *const mesh);

	uint32_t vcn_mesh_get_N_trg(const vcn_mesh_t *const mesh);

	uint32_t vcn_mesh_get_N_edg(const vcn_mesh_t *const mesh);

	double vcn_mesh_get_area(const vcn_mesh_t *const mesh);

#ifdef __cplusplus
}
#endif

#endif
