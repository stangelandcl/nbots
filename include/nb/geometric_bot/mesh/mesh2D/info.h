#ifndef __NB_GEOMETRIC_BOT_MESH_MESH2D_INFO_H__
#define __NB_GEOMETRIC_BOT_MESH_MESH2D_INFO_H__

typedef enum {
	NB_ELEMENT,
	NB_NODE
} nb_mesh2D_entity;

typedef enum {
	NB_FIELD,
	NB_CLASS
} nb_mesh2D_array_type;

typedef enum {
	NB_NODES_LINKED_BY_EDGES, NB_ELEMS_LINKED_BY_EDGES,
	NB_NODES_LINKED_BY_ELEMS, NB_ELEMS_LINKED_BY_NODES,
	NB_ELEMS_CONNECTED_TO_NODES
} nb_mesh2D_graph_type;

#endif