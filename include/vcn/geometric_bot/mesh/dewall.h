#ifndef __VCN_GEOMETRIC_BOT_MESH_DEWALL_H__
#define __VCN_GEOMETRIC_BOT_MESH_DEWALL_H__

#include "vcn/geometric_bot/mesh/mesh2D.h"

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief Create a Delaunay triangulation from a given set of vertices.
	 * @param[in,out] Mesh structure to store Delaunay triangulation.
	 * @param[in] N_vertices Number of vertices.
	 * @param[in] vertices Concatenated coordinates of the vertices.
	 */
	void vcn_dewall_get_delaunay(vcn_mesh_t *mesh, uint32_t N_vertices,
				     const double *const vertices);
#ifdef __cplusplus
}
#endif

#endif
