#ifndef __VCN_MESH2D_GRAPH_GENERATOR_H__
#define __VCN_MESH2D_GRAPH_GENERATOR_H__

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief Build a graph from the triangular mesh. Each vertex of the mesh
	 * represent a node in the graph.
	 * @n<b>WARNING:</b> Assuming that vertices has an ID as attribute.
	 * @param[in] mesh Mesh representing the graph.
	 * @return The graph generated from the vertices of the mesh.
	 */
	vcn_graph_t* vcn_mesh_create_vtx_graph(const vcn_mesh_t *const mesh);

	/**
	 * @brief Build a graph from the triangular mesh. Each element of the mesh
	 * represent a node in the graph.
	 * @n<b>WARNING:</b> Assuming that triangles has an ID as attribute.
	 * @param[in] mesh Mesh representing the graph.
	 * @return The graph generated from the elements (triangles) of the mesh.
	 */
	vcn_graph_t* vcn_mesh_create_elem_graph(const vcn_mesh_t *const mesh);

#ifdef __cplusplus
}
#endif

#endif
