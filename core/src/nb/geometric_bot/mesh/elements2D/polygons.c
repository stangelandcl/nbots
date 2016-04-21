#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "nb/container_bot.h"
#include "nb/geometric_bot/point2D.h"
#include "nb/geometric_bot/utils2D.h"
#include "nb/geometric_bot/knn/bins2D.h"
#include "nb/geometric_bot/knn/bins2D_iterator.h"
#include "nb/geometric_bot/mesh/mesh2D.h"
#include "nb/geometric_bot/mesh/dewall.h"
#include "nb/geometric_bot/mesh/modules2D/graph_generator.h"
#include "nb/geometric_bot/mesh/elements2D/polygons.h"

#include "../mesh2D_structs.h"

#define POW2(a) ((a)*(a))

enum {
	VTX_END,
	VTX_INTERNAL_JOIN,
	VTX_EXTERNAL_JOIN
};

typedef struct {
	uint32_t N_subsgm;
	uint32_t N_subsgm_nod; /* Internal nodes forming a segment */
	uint32_t N_subsgm_cl; /* Circumcenters colineal to subsegments */
	uint32_t N_interior_nod; /* Nodes forming interior segments */
	uint32_t N_interior_end; /* Interior nodes ending interior segments */
} subsgm_data;

static void set_arrays_memory(nb_mshpoly_t *poly);
static void copy_nodes(nb_mshpoly_t* poly,
		       const nb_mshpoly_t *const src_poly);
static void copy_edges(nb_mshpoly_t* poly,
		       const nb_mshpoly_t *const src_poly);
static void copy_N_side(nb_mshpoly_t* poly,
			const nb_mshpoly_t *const src_poly);
static uint32_t get_size_of_adj_and_ngb(const nb_mshpoly_t *const poly);
static void set_mem_of_adj_and_ngb(nb_mshpoly_t *poly, uint32_t memsize);
static void copy_adj_and_ngb(nb_mshpoly_t* poly,
			     const nb_mshpoly_t *const src_poly);

static void copy_elem_vtx(nb_mshpoly_t* poly,
			  const nb_mshpoly_t *const src_poly);
static void copy_N_nod_x_sgm(nb_mshpoly_t* poly,
			     const nb_mshpoly_t *const src_poly);
static uint32_t get_size_of_nod_x_sgm(const nb_mshpoly_t *const poly);
static void set_mem_of_nod_x_sgm(nb_mshpoly_t *poly, uint32_t memsize);
static void copy_nod_x_sgm(nb_mshpoly_t* poly,
			   const nb_mshpoly_t *const src_poly);
static void* malloc_poly(void);
static void set_voronoi(nb_mshpoly_t *poly,
			const nb_graph_t *const graph,
			const nb_mesh_t *const mesh);
static uint32_t map_cocircularities(const nb_mesh_t *const mesh,
				    uint32_t *cc_map);
static void init_map_cocircularities(uint32_t *cc_map, uint32_t N);
static bool adj_is_cocircular(const msh_edge_t *const edg);
static void add_cc_to_map(uint32_t *cc_map, uint32_t N,
			  const msh_edge_t *const edg);
static void count_subsgm_data(const nb_mesh_t *const mesh, subsgm_data *data);
static bool edge_has_trg_with_cl_ccenter(const msh_edge_t *const edg);
static bool trg_is_cl_to_sgm(const msh_edge_t *edg, const msh_trg_t *trg);
static uint8_t sgm_get_N_internal_ends(const nb_mesh_t *const mesh,
				       uint32_t sgm_id, 
				       nb_container_t *joins);
static uint8_t vtx_is_end(nb_container_t *joins,
			  const nb_mesh_t *const mesh,
			  uint32_t sgm_id,
			  const msh_vtx_t *const vtx);
static int vtx_get_status(const nb_mesh_t *const mesh,
			  uint32_t sgm_id,
			  const msh_vtx_t *const vtx);
static void set_nodes(nb_mshpoly_t *poly,
		      const nb_mesh_t *const mesh,
		      const uint32_t *cc_map);
static void set_edges(nb_mshpoly_t *poly,
		      const nb_mesh_t *const mesh);
static uint32_t set_N_sides(nb_mshpoly_t *poly,
			   const nb_mesh_t *const mesh);
static void set_adj_and_ngb(nb_mshpoly_t *poly,
			    const nb_mesh_t *const mesh);
static void set_elem_vtx(nb_mshpoly_t *poly,
			 const nb_mesh_t *const mesh);
static uint32_t set_N_nod_x_sgm(nb_mshpoly_t *poly,
				const nb_mesh_t *const mesh);
static void set_nod_x_sgm(nb_mshpoly_t *poly,
			  const nb_mesh_t *const mesh);

uint32_t nb_mshpoly_get_memsize(void)
{
	return sizeof(nb_mshpoly_t);
}

void nb_mshpoly_init(void *mshpoly_ptr)
{
	memset(mshpoly_ptr, 0, nb_mshpoly_get_memsize());
}

void nb_mshpoly_copy(void *dest, const void *const src)
{
	memcpy(dest, src, nb_mshpoly_get_memsize());
	nb_mshpoly_t *poly = dest;
	const nb_mshpoly_t *const src_poly = src;
	
	set_arrays_memory(poly);

	copy_nodes(poly, src_poly);
	copy_edges(poly, src_poly);
	copy_N_side(poly, src_poly);

	uint32_t memsize = get_size_of_adj_and_ngb(poly);
	set_mem_of_adj_and_ngb(poly, memsize);
	copy_adj_and_ngb(poly, src_poly);

	copy_elem_vtx(poly, src_poly);
	copy_N_nod_x_sgm(poly, src_poly);

	memsize = get_size_of_nod_x_sgm(poly);
	set_mem_of_nod_x_sgm(poly, memsize);
	copy_nod_x_sgm(poly, src_poly);
}

static void set_arrays_memory(nb_mshpoly_t *poly)
{
	uint32_t nod_size = poly->N_nod * 2 * sizeof(*(poly->nod));
	uint32_t edg_size = poly->N_edg * 2 * sizeof(*(poly->edg));
	uint32_t N_side_size = poly->N_elems * sizeof(*(poly->N_side));
	uint32_t adj_size = poly->N_elems * sizeof(*(poly->adj));
	uint32_t ngb_size = poly->N_elems * sizeof(*(poly->ngb));
	uint32_t elem_vtx_size = poly->N_vtx * sizeof(*(poly->elem_vtx));
	uint32_t N_nod_x_sgm_size = poly->N_sgm * sizeof(*(poly->N_nod_x_sgm));
	uint32_t nod_x_sgm_size = poly->N_sgm * sizeof(*(poly->nod_x_sgm));

	uint32_t size = nod_size + edg_size + N_side_size +
		adj_size + ngb_size + elem_vtx_size +
		N_nod_x_sgm_size + nod_x_sgm_size;

	char *memblock = malloc(size);

	poly->nod = (void*) memblock;
	poly->edg = (void*) ((char*)(poly->nod) + nod_size);
	poly->N_side = (void*) ((char*)(poly->edg) + edg_size);
	poly->adj = (void*) ((char*)(poly->N_side) + N_side_size);
	poly->ngb = (void*) ((char*)(poly->adj) + adj_size);
	poly->elem_vtx = (void*) ((char*)(poly->ngb) + ngb_size);
	poly->N_nod_x_sgm = (void*) ((char*)(poly->elem_vtx) +
				     elem_vtx_size);
	poly->nod_x_sgm = (void*) ((char*)(poly->N_nod_x_sgm) +
				   N_nod_x_sgm_size);
}

static void copy_nodes(nb_mshpoly_t* poly, const nb_mshpoly_t *const src_poly)
{
	memcpy(poly->nod, src_poly->nod,
	       2 * poly->N_nod * sizeof(*(poly->nod)));
}

static void copy_edges(nb_mshpoly_t* poly, const nb_mshpoly_t *const src_poly)
{
	memcpy(poly->edg, src_poly->edg,
	       2 * poly->N_edg * sizeof(*(poly->edg)));
}

static void copy_N_side(nb_mshpoly_t* poly, const nb_mshpoly_t *const src_poly)
{
	memcpy(poly->N_side, src_poly->N_side,
	       poly->N_elems * sizeof(*(poly->N_side)));
}

static uint32_t get_size_of_adj_and_ngb(const nb_mshpoly_t *const poly)
{
	uint32_t size = 0;
	for (uint32_t i = 0; i < poly->N_sgm; i++)
		size += poly->N_side[i] * sizeof(**(poly->adj));
	return 2 * size;
}

static void set_mem_of_adj_and_ngb(nb_mshpoly_t *poly, uint32_t memsize)
{
	char *memblock1 = malloc(memsize);
	char *memblock2 = memblock1 + memsize / 2;
	for (uint32_t i = 0; i < poly->N_sgm; i++) {
		poly->adj[i] = (void*) memblock1;
		poly->ngb[i] = (void*) memblock2;
		memblock1 += poly->N_side[i] * sizeof(**(poly->adj));
		memblock2 += poly->N_side[i] * sizeof(**(poly->ngb));
	}
}

static void copy_adj_and_ngb(nb_mshpoly_t* poly,
			     const nb_mshpoly_t *const src_poly)
{
	for (int i = 0; i < poly->N_sgm; i++) {
		memcpy(&(poly->adj[i]), &(src_poly->adj[i]),
		       poly->N_side[i] * sizeof(**(poly->adj)));
		memcpy(&(poly->ngb[i]), &(src_poly->ngb[i]),
		       poly->N_side[i] * sizeof(**(poly->ngb)));
	}

}

static void copy_elem_vtx(nb_mshpoly_t* poly,
			  const nb_mshpoly_t *const src_poly)
{
	memcpy(poly->elem_vtx, src_poly->elem_vtx,
	       poly->N_vtx * sizeof(*(poly->elem_vtx)));
}

static void copy_N_nod_x_sgm(nb_mshpoly_t* poly,
			     const nb_mshpoly_t *const src_poly)
{
	memcpy(poly->N_nod_x_sgm, src_poly->N_nod_x_sgm,
	       poly->N_sgm * sizeof(*(poly->N_nod_x_sgm)));
}


static uint32_t get_size_of_nod_x_sgm(const nb_mshpoly_t *const poly)
{
	uint32_t size = 0;
	for (uint32_t i = 0; i < poly->N_sgm; i++)
		size += poly->N_nod_x_sgm[i] *
			sizeof(**(poly->nod_x_sgm));
	return size;
}

static void set_mem_of_nod_x_sgm(nb_mshpoly_t *poly, uint32_t memsize)
{
	char *memblock = malloc(memsize);
	for (uint32_t i = 0; i < poly->N_sgm; i++) {
		poly->nod_x_sgm[i] = (void*) memblock;
		memblock += poly->N_nod_x_sgm[i] *
			sizeof(**(poly->nod_x_sgm));
	}
}

static void copy_nod_x_sgm(nb_mshpoly_t* poly,
			   const nb_mshpoly_t *const src_poly)
{
	for (int i = 0; i < poly->N_sgm; i++) {
		memcpy(&(poly->nod_x_sgm[i]), &(src_poly->nod_x_sgm[i]),
		       poly->N_nod_x_sgm[i] * sizeof(**(poly->nod_x_sgm)));
	}
}

void nb_mshpoly_finish(void *mshpoly_ptr)
{
	nb_mshpoly_clear(mshpoly_ptr);
}

void* nb_mshpoly_create(void)
{
	nb_mshpoly_t *poly = malloc_poly();
	nb_mshpoly_init(poly);
	return poly;
}

static void* malloc_poly(void)
{
	uint32_t size = nb_mshpoly_get_memsize();
	nb_mshpoly_t *poly = malloc(size);
	return poly;
}

void* nb_mshpoly_clone(const void *const mshpoly_ptr)
{
	nb_mshpoly_t *poly = malloc_poly();
	nb_mshpoly_copy(poly, mshpoly_ptr);
	return poly;
}

void nb_mshpoly_destroy(void *mshpoly_ptr)
{
	nb_mshpoly_finish(mshpoly_ptr);
	free(mshpoly_ptr);
}

void nb_mshpoly_clear(void *mshpoly_ptr)
{
	nb_mshpoly_t *poly = mshpoly_ptr;
	if (NULL != poly->nod) {
		free(poly->adj[0]);
		free(poly->nod_x_sgm[0]);
		free(poly->nod);		
	}
	memset(mshpoly_ptr, 0, nb_mshpoly_get_memsize());
}

void nb_mshpoly_load_from_mesh(nb_mshpoly_t *mshpoly,
			       const nb_mesh_t *const mesh)
{
	mesh_alloc_vtx_ids((vcn_mesh_t*)mesh);
	mesh_alloc_trg_ids((vcn_mesh_t*)mesh);
	nb_graph_t *graph = vcn_mesh_create_vtx_graph(mesh);
	
	set_voronoi(mshpoly, graph, mesh);

	vcn_graph_destroy(graph);
	mesh_free_vtx_ids((vcn_mesh_t*)mesh);
	mesh_free_trg_ids((vcn_mesh_t*)mesh);
}

static void set_voronoi(nb_mshpoly_t *poly,
			const nb_graph_t *const graph,
			const nb_mesh_t *const mesh)
{
	uint32_t N_trg = vcn_mesh_get_N_trg(mesh);
	uint32_t N_edg = vcn_mesh_get_N_edg(mesh);

	uint32_t *cc_map = malloc(N_trg * sizeof(cc_map));
	uint32_t N_cc = map_cocircularities(mesh, cc_map);

	subsgm_data data;
	count_subsgm_data(mesh, &data);

	uint32_t N_correction = 3 * data.N_subsgm - N_cc - data.N_subsgm_cl;
	poly->N_nod = N_trg + N_correction;
	poly->N_edg = N_edg + N_correction -
		data.N_subsgm_nod + 2 * data.N_interior_end;
	poly->N_elems = vcn_mesh_get_N_vtx(mesh) +
		data.N_interior_nod + data.N_interior_end;
	poly->N_vtx = mesh->N_input_vtx;
	poly->N_sgm = mesh->N_input_sgm;

	set_arrays_memory(poly);

	set_nodes(poly, mesh, cc_map);
	set_edges(poly, mesh);

	uint32_t memsize = set_N_sides(poly, mesh);
	set_mem_of_adj_and_ngb(poly, memsize);
	set_adj_and_ngb(poly, mesh);

	set_elem_vtx(poly, mesh);

	memsize = set_N_nod_x_sgm(poly, mesh);
	set_mem_of_nod_x_sgm(poly, memsize);
	set_nod_x_sgm(poly, mesh);

	free(cc_map);
}

static uint32_t map_cocircularities(const nb_mesh_t *const mesh,
				    uint32_t *cc_map)
{
	uint32_t N_trg = vcn_mesh_get_N_trg(mesh);
	init_map_cocircularities(cc_map, N_trg);
	uint32_t N_cc = 0;

	uint16_t iter_size = nb_iterator_get_memsize();
	nb_iterator_t *iter = alloca(iter_size);
	nb_iterator_init(iter);
	nb_iterator_set_container(iter, mesh->ht_edge);
	while (nb_iterator_has_more(iter)) {
		msh_edge_t *edge = (msh_edge_t*) nb_iterator_get_next(iter);
		if (adj_is_cocircular(edge)) {
			add_cc_to_map(cc_map, N_trg, edge);
			N_cc += 1;
		}
	}
	nb_iterator_finish(iter);
	return N_cc;
}

static void init_map_cocircularities(uint32_t *cc_map, uint32_t N)
{
	for (uint32_t i = 0; i < N; i++)
		cc_map[i] = N;
}
				     

static bool adj_is_cocircular(const msh_edge_t *const edg)
{
	bool out;
	if (medge_is_boundary(edg)) {
		out = false;
	} else {
		msh_vtx_t *v4 = medge_get_opposite_vertex(edg->t2, edg);
		out = nb_utils2D_pnt_is_cocircular(edg->t1->v1->x,
						   edg->t1->v2->x,
						   edg->t1->v3->x,
						   v4->x);
	}
	return out;
}

static void add_cc_to_map(uint32_t *cc_map, uint32_t N,
			  const msh_edge_t *const edg)
{
	uint32_t id1 = *(uint32_t*)((void**)edge->t1->attr)[0];
	uint32_t id2 = *(uint32_t*)((void**)edge->t2->attr)[0];
	if (cc_map[id1] == N && cc_map[id2] == N) {
		cc_map[id1] = id1;
		cc_map[id2] = id1;
	} else if (cc_map[id1] != N && cc_map[id2] == N) {
		cc_map[id2] = cc_map[id1];
	} else if (cc_map[id1] == N && cc_map[id2] != N) {
		cc_map[id1] = cc_map[id2];
	} else {
		uint32_t root_id = cc_map[id1];
		uint32_t update_id = id2;
		while (cc_map[update_id] != update_id) {
			uint32_t next_id = cc_map[update_id];
			cc_map[update_id] = root_id;
			update_id = next_id;
		}
		cc_map[update_id] = root_id;
	}
}

static void count_subsgm_data(const nb_mesh_t *const mesh, subsgm_data *data)
{
	nb_container_t *joins = alloca(nb_container_get_memsize(NB_SORTED));
	nb_container_init(joins, NB_SORTED);

	memset(data, 0, sizeof(*data));
	for (uint32_t i = 0; i < mesh->N_input_sgm; i++) {
		uint32_t N_subsgm = 0;
		uint32_t N_interior = 0;
		msh_edge_t *sgm_iter = mesh->input_sgm[i];
		while (NULL != sgm_iter) {
			N_subsgm += 1;
			N_colineal += 
				edge_has_trg_with_cl_ccenter(sgm_iter)? 1 : 0;
			N_interior += medge_is_boundary(sgm_iter)? 0 : 1;
			sgm_iter = medge_subsgm_next(sgm_iter);
		}
		uint8_t N_end = 0;
		if (NULL != mesh->input_sgm[i])
			N_end += sgm_get_N_internal_ends(mesh, i, joins);

		data->N_subsgm += N_subsgm;
		data->N_subsgm_nod += N_subsgm - 1;
		data->N_subsgm_cl = N_colineal;
		data->N_interior_nod += N_interior + 1;
		data->N_interior_end += N_end;
	}
	data->N_interior_nod -= nb_container_get_length(joins);
	nb_container_finish(joins);
}

static bool edge_has_trg_with_cl_ccenter(const msh_edge_t *const edg)
/* Has a colinear circumcenter of the adjacent triangles? */
{
	bool is_cl;
	if (NULL != edg->t1)
		is_cl = trg_is_cl_to_sgm(edg->t1, edg);
	else
		is_cl = false;

	if (!is_cl) {
		if (NULL != edg->t2)
			is_cl = trg_is_cl_to_sgm(edg->t2, edg);
	}
	return is_cl;
}

static bool trg_is_cl_to_sgm(const msh_edge_t *edg, const msh_trg_t *trg)
{
	msh_vtx_t *v1, *v2, *v3;
	if (trg->s1 == edg) {
		v1 = trg->v2;
		v2 = trg->v3;
		v3 = trg->v1;
	} else if (trg->s2 == edg) {
		v1 = trg->v3;
		v2 = trg->v1;
		v3 = trg->v2;
	} else {
		v1 = trg->v1;
		v2 = trg->v2;
		v3 = trg->v3;
	}
	double angle = nb_utils2D_get_2vec_angle(v1->x, v2->x, v3->x);
	return (fabs(angle - NB_PI/2.0) < 1e-6);
}
static uint8_t sgm_get_N_internal_ends(const nb_mesh_t *const mesh,
				       uint32_t sgm_id,
				       nb_container_t *joins)
{
	uint8_t N_end;
	if (medge_is_boundary(mesh->input_sgm[sgm_id])) {
		N_end = 0;
	} else {
		msh_vtx_t *end_vtx[2];
		medge_subsgm_get_extreme_vtx(mesh->input_sgm[sgm_id],
					     end_vtx);
		N_end = vtx_is_end(joins, mesh, sgm_id, end_vtx[0]) + 
			vtx_is_end(joins, mesh, sgm_id, end_vtx[1]);
	}
	return N_end;
}

static uint8_t vtx_is_end(nb_container_t *joins,
			  const nb_mesh_t *const mesh,
			  uint32_t sgm_id,
			  const msh_vtx_t *const vtx)
{
	uint8_t is_end;
	if (container_exist(joins, vtx)) {
		is_end = 0;
	} else {
		int8_t vtx_status = vtx_get_end_status(mesh, sgm_id, vtx)
		if (VTX_INTERNAL_JOIN == vtx_status) {
			nb_container_insert(joins, vtx);
			is_end = 0;
		} else {
			if (VTX_END == vtx_status)
				is_end = 1;
			else
				is_end = 0;
		}
	}
	return is_end;
}

static int vtx_get_status(const nb_mesh_t *const mesh,
			  uint32_t sgm_id,
			  const msh_vtx_t *const vtx)
{
	int status = VTX_END;
	for (uint32_t i = 0; i < mesh->N_input_sgm; i++) {
		msh_edg_t *sgm = mesh->input_sgm[i];
		if (i != sgm_id && NULL != sgm) {
			msh_vtx_t *end_vtx[2];
			medge_subsgm_get_extreme_vtx(sgm, end_vtx);
			if (medge_is_boundary(sgm)) {
				if (vtx == end_vtx[0] || vtx == end_vtx[1]) {
					status = VTX_EXTERNAL_JOIN;
					break;
				}
			} else {
				if (vtx == end_vtx[0] || vtx == end_vtx[1]) {
					status = VTX_INTERNAL_JOIN;
					break;
				}		
			}
		}
	}
	return status;
}

static void set_nodes(nb_mshpoly_t *poly,
		      const nb_mesh_t *const mesh,
		      const uint32_t *cc_map)
{
	uint16_t iter_size = nb_iterator_get_memsize();
	nb_iterator_t *iter = alloca(iter_size);
	nb_iterator_init(iter);
	nb_iterator_set_container(iter, mesh->ht_trg);
	uint32_t elem_id = 0;
	while (nb_iterator_has_more(iter)) {
		msh_trg_t* trg = (msh_trg_t*) nb_iterator_get_next(iter);
	}
	nb_iterator_finish(iter);
}

static void set_edges(nb_mshpoly_t *poly,
		      const nb_mesh_t *const mesh);
static uint32_t set_N_sides(nb_mshpoly_t *poly,
			   const nb_mesh_t *const mesh);
static void set_adj_and_ngb(nb_mshpoly_t *poly,
			    const nb_mesh_t *const mesh);
static void set_elem_vtx(nb_mshpoly_t *poly,
			 const nb_mesh_t *const mesh);
static uint32_t set_N_nod_x_sgm(nb_mshpoly_t *poly,
				const nb_mesh_t *const mesh);
static void set_nod_x_sgm(nb_mshpoly_t *poly,
			  const nb_mesh_t *const mesh);

void nb_mshpoly_Lloyd_iteration(nb_mshpoly_t *mshpoly, uint32_t max_iter,
				double (*density)(const double[2],
						  const void *data),
				const void *density_data);

void nb_mshpoly_set_fem_graph(const nb_mshpoly_t *mshpoly,
				nb_graph_t *graph);
void nb_mshpoly_set_nodal_graph(const nb_mshpoly_t *mshpoly,
				nb_graph_t *graph);
void nb_mshpoly_set_elemental_graph(const nb_mshpoly_t *mshpoly,
				    nb_graph_t *graph);
