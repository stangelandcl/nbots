#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "nb/memory_bot.h"
#include "nb/container_bot.h"
#include "nb/geometric_bot/utils2D.h"
#include "nb/geometric_bot/knn/bins2D.h"
#include "nb/geometric_bot/knn/bins2D_iterator.h"
#include "nb/geometric_bot/mesh/modules2D/area_analizer.h"
#include "nb/geometric_bot/mesh/mesh2D/elements2D/msh3trg.h"
#include "nb/geometric_bot/mesh/mesh2D/elements2D/trg_exporter.h"

#include "msh3trg_struct.h"

#include "../../tessellator2D_structs.h"

#define POW2(a) ((a)*(a))

#define GET_MSH3TRG_FROM_STRUCT(exp_structure)	\
	(((void**)(exp_structure))[0])
#define GET_ISGM_FROM_STRUCT(exp_structure)	\
	(*((uint32_t*)((void**)(exp_structure))[1]))

static void set_msh3trg_exporter_interface(nb_trg_exporter_interface_t *exp);
static void null_statement(void *param){ ; }
static void msh3trg_set_N_vtx(void *exp_structure, uint32_t N);
static void msh3trg_allocate_vtx(void *exp_structure);
static void msh3trg_set_vtx(void *exp_structure, uint32_t i,
			    double x, double y);
static void msh3trg_set_N_edg(void *exp_structure, uint32_t N);
static void msh3trg_allocate_edg(void *exp_structure);
static void msh3trg_set_edg(void *exp_structure, uint32_t i,
			    uint32_t v1, uint32_t v2);
static void msh3trg_set_N_trg(void *exp_structure, uint32_t N);
static void msh3trg_allocate_trg(void *exp_structure, bool include_neighbours);
static void msh3trg_set_trg(void *exp_structure, uint32_t i,
			    uint32_t v1, uint32_t v2, uint32_t v3);
static void msh3trg_set_trg_neighbours(void *exp_structure,
				       uint32_t i, uint32_t t1,
				       uint32_t t2, uint32_t t3);
static void msh3trg_set_N_input_vtx(void *exp_structure, uint32_t N);
static void msh3trg_allocate_input_vtx(void *exp_structure);
static void msh3trg_set_input_vtx(void *exp_structure, uint32_t i,
				  uint32_t vtx_id);
static void msh3trg_set_N_input_sgm(void *exp_structure, uint32_t N);
static void msh3trg_allocate_input_sgm_table(void *exp_structure);
static void msh3trg_input_sgm_set_N_vtx(void *exp_structure, uint32_t i,
					uint32_t N);
static void msh3trg_input_sgm_allocate_vtx(void *exp_structure, uint32_t i);
static void msh3trg_input_sgm_start_access(void *exp_structure, uint32_t i);
static void msh3trg_input_sgm_set_vtx(void *exp_structure,
				      uint32_t ivtx, uint32_t vtx_id);
static void set_nodal_perm_to_nodes(nb_msh3trg_t *msh3trg, const uint32_t *perm);
static void set_nodal_perm_to_edges(nb_msh3trg_t *msh3trg, const uint32_t *perm);
static void set_nodal_perm_to_elems(nb_msh3trg_t *msh3trg, const uint32_t *perm);
static void set_nodal_perm_to_invtx(nb_msh3trg_t *msh3trg, const uint32_t *perm);
static void set_nodal_perm_to_insgm(nb_msh3trg_t *msh3trg, const uint32_t *perm);
static uint32_t itrg_get_right_triangle
                         (const nb_msh3trg_t *const delaunay, 
			  const bool *const enabled_elements,
			  uint32_t itrg, uint32_t ivtx);
static uint32_t itrg_get_left_triangle
                         (const nb_msh3trg_t *const delaunay,
			  const bool *const enabled_elements,
			  uint32_t itrg, uint32_t ivtx);
static uint32_t get_input_vtx(const void *msh, double *vertices,
			      uint32_t *vtx_index_relation);
static uint32_t get_input_sgm(const void *msh,
			      const uint32_t *vtx_index_relation,
			      uint32_t *segments);
static void set_holes_to_model(const void *msh, nb_model_t *model);
static uint32_t get_centroids_mask(const void *msh, uint32_t N_centroids,
				   double *centroids, char *mask_centroids);
static double *get_holes(uint32_t N_holes, uint32_t N_centroids,
			 const double *centroids, const char *mask_centroids);

uint32_t nb_msh3trg_get_memsize(void)
{
	return sizeof(nb_msh3trg_t);
}

void nb_msh3trg_init(void *msh3trg)
{
	uint32_t memsize = nb_msh3trg_get_memsize();
	memset(msh3trg, 0, memsize);
}

void nb_msh3trg_finish(void *msh3trg)
{
	nb_msh3trg_clear(msh3trg);
}

void nb_msh3trg_copy(void *msh3trg_ptr, const void *src_ptr)
{
	nb_msh3trg_t *msh3trg = msh3trg_ptr;
	const nb_msh3trg_t *src = src_ptr;

	msh3trg->N_nod = src->N_nod;
	msh3trg->nod = nb_allocate_mem(2 * msh3trg->N_nod * sizeof(*(msh3trg->nod)));
	memcpy(msh3trg->nod, src->nod, 
	       2 * msh3trg->N_nod * sizeof(*(msh3trg->nod)));

	msh3trg->N_elems = src->N_elems;
	if (msh3trg->N_elems > 0) {
		msh3trg->adj =
			nb_allocate_mem(3 * msh3trg->N_elems *
			       sizeof(*(msh3trg->adj)));
		memcpy(msh3trg->adj,
		       src->adj,
		       3 * msh3trg->N_elems * 
		       sizeof(*(msh3trg->adj)));
	}
	if (NULL != src->ngb) {
		msh3trg->ngb =
			nb_allocate_mem(3 * msh3trg->N_elems *
			       sizeof(*(msh3trg->ngb)));
		memcpy(msh3trg->ngb,
		       src->ngb,
		       3 * msh3trg->N_elems *
		       sizeof(*(msh3trg->ngb)));
	}
	msh3trg->N_vtx = src->N_vtx;
	msh3trg->vtx = nb_allocate_mem(msh3trg->N_vtx *
			      sizeof(*(msh3trg->vtx)));
	memcpy(msh3trg->vtx,
	       src->vtx,
	       msh3trg->N_vtx * sizeof(*(msh3trg->vtx)));

	msh3trg->N_sgm = src->N_sgm;
	if (msh3trg->N_sgm > 0) {
		msh3trg->N_nod_x_sgm = 
			nb_allocate_mem(msh3trg->N_sgm *
			       sizeof(*(msh3trg->N_nod_x_sgm)));
		memcpy(msh3trg->N_nod_x_sgm,
		       src->N_nod_x_sgm,
		       msh3trg->N_sgm *
		       sizeof(*(msh3trg->N_nod_x_sgm)));
		msh3trg->nod_x_sgm =
			nb_allocate_zero_mem(msh3trg->N_sgm *
			       sizeof(*(msh3trg->nod_x_sgm)));
		for (uint32_t i = 0; i < msh3trg->N_sgm; i++) {
			uint32_t N_vtx = msh3trg->N_nod_x_sgm[i];
			if (0 < N_vtx) {
				msh3trg->nod_x_sgm[i] =
					nb_allocate_mem(N_vtx *
					       sizeof(*(msh3trg->nod_x_sgm[i])));
				memcpy(msh3trg->nod_x_sgm[i],
				       src->nod_x_sgm[i],
				       N_vtx *
				       sizeof(*(msh3trg->nod_x_sgm[i])));
			}
		}
	}	
}

void* nb_msh3trg_create(void)
{
	uint32_t memsize = nb_msh3trg_get_memsize();
	nb_msh3trg_t* msh3trg = nb_allocate_mem(memsize);
	nb_msh3trg_init(msh3trg);
	return msh3trg;
}

void* nb_msh3trg_clone(void* msh3trg)
{
	uint32_t memsize = nb_msh3trg_get_memsize();
	nb_msh3trg_t* clone = nb_allocate_mem(memsize);
	nb_msh3trg_copy(clone, msh3trg);
	return clone;
}

void nb_msh3trg_clear(void* msh3trg_ptr)
{
	nb_msh3trg_t *msh3trg = msh3trg_ptr;

	if (msh3trg->N_nod > 0) 
		nb_free_mem(msh3trg->nod);
	if (msh3trg->N_edg > 0)
		nb_free_mem(msh3trg->edg);
	if (msh3trg->N_elems > 0) {
		nb_free_mem(msh3trg->adj);
		if (NULL != msh3trg->ngb)
			nb_free_mem(msh3trg->ngb);
	}
	if (msh3trg->N_vtx > 0)
		nb_free_mem(msh3trg->vtx);
	if (msh3trg->N_sgm > 0) {
		for (uint32_t i = 0; i < msh3trg->N_sgm; i++) {
			if (0 < msh3trg->N_nod_x_sgm[i])
				nb_free_mem(msh3trg->nod_x_sgm[i]);
		}
		nb_free_mem(msh3trg->N_nod_x_sgm);
		nb_free_mem(msh3trg->nod_x_sgm);
	}
	memset(msh3trg, 0, sizeof(*msh3trg));
}

void nb_msh3trg_destroy(void* msh3trg)
{
	nb_msh3trg_finish(msh3trg);
	nb_free_mem(msh3trg);
}

uint32_t nb_msh3trg_get_N_invtx(const void *msh)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->N_vtx;
}

uint32_t nb_msh3trg_get_N_insgm(const void *msh)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->N_sgm;
}

uint32_t nb_msh3trg_get_N_nodes(const void *msh)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->N_nod;
}

uint32_t nb_msh3trg_get_N_edges(const void *msh)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->N_edg;
}

uint32_t nb_msh3trg_get_N_elems(const void *msh)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->N_elems;
}

double nb_msh3trg_node_get_x(const void *msh, uint32_t id)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->nod[id * 2];
}

double nb_msh3trg_node_get_y(const void *msh, uint32_t id)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->nod[id*2+1];
}

uint32_t nb_msh3trg_edge_get_1n(const void *msh, uint32_t id)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->edg[id * 2];
}

uint32_t nb_msh3trg_edge_get_2n(const void *msh, uint32_t id)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->edg[id*2+1];
}

void nb_msh3trg_edge_get_midpoint(const void *msh,
				  uint32_t face_id, double w,
				  double midpoint[2])
{
	const nb_msh3trg_t *msh3trg = msh;
	uint32_t n1 =  msh3trg->edg[face_id * 2];
	uint32_t n2 =  msh3trg->edg[face_id*2+1];
	double *s1 = &(msh3trg->nod[n1 * 2]);
	double *s2 = &(msh3trg->nod[n2 * 2]);
	midpoint[0] = (1 - w) * s1[0] + w * s2[0];
	midpoint[1] = (1 - w) * s1[1] + w * s2[1];
}

double nb_msh3trg_edge_get_normal(const void *msh, uint32_t face_id,
				  double normal[2])
{

	const nb_msh3trg_t *msh3trg = msh;
	uint32_t n1 =  msh3trg->edg[face_id * 2];
	uint32_t n2 =  msh3trg->edg[face_id*2+1];
	double *s1 = &(msh3trg->nod[n1 * 2]);
	double *s2 = &(msh3trg->nod[n2 * 2]);
	double length = nb_utils2D_get_dist(s1, s2);
	normal[0] =  (s2[1] - s1[1]) / length;
	normal[1] = -(s2[0] - s1[0]) / length;
	return length;
}

double nb_msh3trg_elem_get_x(const void *msh, uint32_t id)
{
	uint32_t v1 = nb_msh3trg_elem_get_adj(msh, id, 0);
	uint32_t v2 = nb_msh3trg_elem_get_adj(msh, id, 1);
	uint32_t v3 = nb_msh3trg_elem_get_adj(msh, id, 2);
	double x_centroid =
		(nb_msh3trg_node_get_x(msh, v1) +
		 nb_msh3trg_node_get_x(msh, v2) +
		 nb_msh3trg_node_get_x(msh, v3)) / 3.0;
	return x_centroid;
}

double nb_msh3trg_elem_get_y(const void *msh, uint32_t id)
{
	uint32_t v1 = nb_msh3trg_elem_get_adj(msh, id, 0);
	uint32_t v2 = nb_msh3trg_elem_get_adj(msh, id, 1);
	uint32_t v3 = nb_msh3trg_elem_get_adj(msh, id, 2);
	double y_centroid =
		(nb_msh3trg_node_get_y(msh, v1) +
		 nb_msh3trg_node_get_y(msh, v2) +
		 nb_msh3trg_node_get_y(msh, v3)) / 3.0;
	return y_centroid;
}

double nb_msh3trg_elem_get_area(const void *msh, uint32_t id)
{
	const nb_msh3trg_t *msh3trg = msh;

	uint32_t v1 = nb_msh3trg_elem_get_adj(msh, id, 0);
	uint32_t v2 = nb_msh3trg_elem_get_adj(msh, id, 1);
	uint32_t v3 = nb_msh3trg_elem_get_adj(msh, id, 2);

	double *t1 = &(msh3trg->nod[v1 * 2]);
	double *t2 = &(msh3trg->nod[v2 * 2]);
	double *t3 = &(msh3trg->nod[v3 * 2]);

	return nb_utils2D_get_trg_area(t1, t2, t3);
}

double nb_msh3trg_elem_get_radius(const void *msh, uint32_t id)
{
	const nb_msh3trg_t *msh3trg = msh;
	double x[2];
	x[0] = nb_msh3trg_elem_get_x(msh, id);
	x[1] = nb_msh3trg_elem_get_y(msh, id);
	double max = 0;
	for (uint8_t i = 0; i < 3; i++) {
		uint32_t nid = nb_msh3trg_elem_get_adj(msh, id, i);
		double *ni = &(msh3trg->nod[nid * 2]);
		double dist2 = nb_utils2D_get_dist2(x, ni);
		if (dist2 > max)
			max = dist2;
	}
	return sqrt(max);	
}

double nb_msh3trg_elem_get_apotem(const void *msh, uint32_t id)
{
	const nb_msh3trg_t *msh3trg = msh;
	double x[2];
	x[0] = nb_msh3trg_elem_get_x(msh, id);
	x[1] = nb_msh3trg_elem_get_y(msh, id);
	double max = 0;
	for (uint16_t i = 0; i < 3; i++) {
		uint32_t id1 = nb_msh3trg_elem_get_adj(msh, id, i);
		uint32_t id2 = nb_msh3trg_elem_get_adj(msh, id, (i+1) % 3);
		double *n1 = &(msh3trg->nod[id1 * 2]);
		double *n2 = &(msh3trg->nod[id2 * 2]);
		double closest[2];
		nb_utils2D_get_closest_pnt_to_sgm(n1, n2, x, closest);
		double dist2 = nb_utils2D_get_dist2(x, closest);
		if (dist2 > max)
			max = dist2;
	}
	return sqrt(max);
}

uint32_t nb_msh3trg_elem_find_edge(const void *msh, uint32_t id,
				   uint16_t local_face_id)
{
	const nb_msh3trg_t *msh3trg = msh;
	uint16_t l1 = local_face_id;
	uint16_t l2 = (local_face_id + 1) % 3;

	uint32_t n1 = msh3trg->adj[id*3+l1];
	uint32_t n2 = msh3trg->adj[id*3+l2];

	uint32_t N_edges = msh3trg->N_edg;
	uint32_t edge_id = N_edges;
	for (uint32_t i = 0; i < N_edges; i++) {
		uint32_t v1 = msh3trg->edg[i * 2];
		uint32_t v2 = msh3trg->edg[i*2+1];
		if ((n1 == v1 && n2 == v2) || (n1 == v2 && n2 == v1)) {
			edge_id = i;
			break;
		}
	}
	return edge_id;
}

double nb_msh3trg_elem_face_get_length(const void *msh, 
				       uint32_t elem_id,
				       uint16_t face_id)
{
	const nb_msh3trg_t *msh3trg = msh;
	uint32_t n1 = nb_msh3trg_elem_get_adj(msh, elem_id, face_id);
	uint32_t n2 = nb_msh3trg_elem_get_adj(msh, elem_id, (face_id + 1)%3);
	double *s1 = &(msh3trg->nod[n1 * 2]);
	double *s2 = &(msh3trg->nod[n2 * 2]);
	return nb_utils2D_get_dist(s1, s2);
}

double nb_msh3trg_elem_face_get_normal(const void *msh, uint32_t elem_id,
				       uint16_t face_id, double normal[2])
{
	const nb_msh3trg_t *msh3trg = msh;
	uint32_t n1 = nb_msh3trg_elem_get_adj(msh, elem_id, face_id);
	uint32_t n2 = nb_msh3trg_elem_get_adj(msh, elem_id, (face_id + 1) % 3);
	double *s1 = &(msh3trg->nod[n1 * 2]);
	double *s2 = &(msh3trg->nod[n2 * 2]);
	double length = nb_utils2D_get_dist(s1, s2);
	normal[0] =  (s2[1] - s1[1]) / length;
	normal[1] = -(s2[0] - s1[0]) / length;
	return length;
}

double nb_msh3trg_elem_ngb_get_normal(const void *msh, uint32_t elem_id,
				      uint16_t ngb_id, double normal[2])
{
	uint32_t N_elems = nb_msh3trg_get_N_elems(msh);
	uint32_t nid = nb_msh3trg_elem_get_ngb(msh, elem_id, ngb_id);
	memset(normal, 0, 2 * sizeof(double));
	double dist = 0;
	if (nid < N_elems) {
		double id1[2];
		id1[0] = nb_msh3trg_elem_get_x(msh, elem_id);
		id1[1] = nb_msh3trg_elem_get_y(msh, elem_id);

		double id2[2];
		id2[0] = nb_msh3trg_elem_get_x(msh, nid);
		id2[1] = nb_msh3trg_elem_get_y(msh, nid);

		dist = nb_utils2D_get_dist(id1, id2);
		normal[0] = (id2[0] - id1[0]) / dist;
		normal[1] = (id2[1] - id1[1]) / dist;
	}
	return dist;
}

uint32_t nb_msh3trg_elem_get_N_adj(const void *msh, uint32_t id)
{
	return 3;
}

uint32_t nb_msh3trg_elem_get_adj(const void *msh,
				 uint32_t elem_id, uint8_t adj_id)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->adj[elem_id * 3 + adj_id];
}
uint32_t nb_msh3trg_elem_get_ngb(const void *msh,
				 uint32_t elem_id, uint8_t ngb_id)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->ngb[elem_id * 3 + ngb_id];
}

bool nb_msh3trg_elem_has_ngb(const void *msh, uint32_t elem_id,
			     uint16_t ngb_id)
{
	uint32_t N_elems = nb_msh3trg_get_N_elems(msh);
	uint32_t id = nb_msh3trg_elem_get_ngb(msh, elem_id, ngb_id);
	return id < N_elems;
}

bool nb_msh3trg_elem_is_boundary(const void *msh, uint32_t elem_id)
{
	bool out = false;
	if (!nb_msh3trg_elem_has_ngb(msh, elem_id, 0))
		out = true;
	if (!out) {
		if (!nb_msh3trg_elem_has_ngb(msh, elem_id, 1))
			out = true;
	}
	if (!out) {
		if (!nb_msh3trg_elem_has_ngb(msh, elem_id, 2))
			out = true;
	}
	return out;
}

uint32_t nb_msh3trg_get_invtx(const void *msh, uint32_t id)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->vtx[id];
}

uint32_t nb_msh3trg_insgm_get_N_nodes(const void *msh, uint32_t id)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->N_nod_x_sgm[id];
}

uint32_t nb_msh3trg_insgm_get_node(const void *msh, uint32_t sgm_id,
				     uint32_t node_id)
{
	const nb_msh3trg_t *msh3trg = msh;
	return msh3trg->nod_x_sgm[sgm_id][node_id];
}

bool nb_msh3trg_is_vtx_inside(const void *msh3trg_ptr, double x, double y)
{
	const nb_msh3trg_t *msh3trg = msh3trg_ptr;
	bool is_inside = false;
	for (uint32_t i = 0; i < msh3trg->N_elems; i++) {
		uint32_t id1 = msh3trg->adj[i * 3];
		uint32_t id2 = msh3trg->adj[i*3+1];
		uint32_t id3 = msh3trg->adj[i*3+2];
		double *v1 = &(msh3trg->nod[id1*2]);
		double *v2 = &(msh3trg->nod[id2*2]);
		double *v3 = &(msh3trg->nod[id3*2]);
		double p[2] = {x, y};
		if (nb_utils2D_pnt_lies_in_trg(v1, v2, v3, p)) {
			is_inside = true;
			break;
		}
	}
	return is_inside;
}

void nb_msh3trg_load_from_tessellator2D(void *msh3trg_ptr, nb_tessellator2D_t *mesh)
{
	nb_msh3trg_t *msh3trg = msh3trg_ptr;

	nb_trg_exporter_interface_t exp;

	uint32_t isgm;
	void *structure[2] = {msh3trg, &isgm};
	exp.structure = structure;

	set_msh3trg_exporter_interface(&exp);

	nb_tessellator2D_export(mesh, &exp);
}

static void set_msh3trg_exporter_interface(nb_trg_exporter_interface_t *exp)
{
	exp->set_N_vtx = msh3trg_set_N_vtx;
	exp->allocate_vtx = msh3trg_allocate_vtx;
	exp->start_vtx_access = null_statement;
	exp->set_vtx = msh3trg_set_vtx;
	exp->stop_vtx_access = null_statement;

	exp->set_N_edg = msh3trg_set_N_edg;
	exp->allocate_edg = msh3trg_allocate_edg;
	exp->start_edg_access = null_statement;
	exp->set_edg = msh3trg_set_edg;
	exp->stop_edg_access = null_statement;

	exp->set_N_trg = msh3trg_set_N_trg;
	exp->allocate_trg = msh3trg_allocate_trg;
	exp->start_trg_access = null_statement;
	exp->set_trg = msh3trg_set_trg;
	exp->stop_trg_access = null_statement;
	exp->start_trg_neighbours_access = null_statement;
	exp->set_trg_neighbours = msh3trg_set_trg_neighbours;
	exp->stop_trg_neighbours_access = null_statement;

	exp->set_N_input_vtx = msh3trg_set_N_input_vtx;
	exp->allocate_input_vtx = msh3trg_allocate_input_vtx;
	exp->start_input_vtx_access = null_statement;
	exp->set_input_vtx = msh3trg_set_input_vtx;
	exp->stop_input_vtx_access = null_statement;

	exp->set_N_input_sgm = msh3trg_set_N_input_sgm;
	exp->allocate_input_sgm_table = msh3trg_allocate_input_sgm_table;
	exp->start_input_sgm_table_access = null_statement;
	exp->input_sgm_set_N_vtx = msh3trg_input_sgm_set_N_vtx;
	exp->input_sgm_allocate_vtx = msh3trg_input_sgm_allocate_vtx;
	exp->input_sgm_start_access = msh3trg_input_sgm_start_access;
	exp->input_sgm_set_vtx = msh3trg_input_sgm_set_vtx;
	exp->input_sgm_stop_access = null_statement;
	exp->stop_input_sgm_table_access = null_statement;
}

static void msh3trg_set_N_vtx(void *exp_structure, uint32_t N)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->N_nod = N;
}

static void msh3trg_allocate_vtx(void *exp_structure)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);

	uint32_t memsize = 2 * msh3trg->N_nod * 
		sizeof(*(msh3trg->nod));
	msh3trg->nod = nb_allocate_mem(memsize);
}

static void msh3trg_set_vtx(void *exp_structure, uint32_t i,
			    double x, double y)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->nod[i * 2] = x;
	msh3trg->nod[i*2+1] = y;
}

static void msh3trg_set_N_edg(void *exp_structure, uint32_t N)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->N_edg = N;
}

static void msh3trg_allocate_edg(void *exp_structure)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	uint32_t memsize = 2 * msh3trg->N_edg * sizeof(*(msh3trg->edg));
	msh3trg->edg = nb_allocate_mem(memsize);
}

static void msh3trg_set_edg(void *exp_structure, uint32_t i,
			    uint32_t v1, uint32_t v2)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->edg[i * 2] = v1;
	msh3trg->edg[i*2+1] = v2;
}

static void msh3trg_set_N_trg(void *exp_structure, uint32_t N)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->N_elems = N;
}

static void msh3trg_allocate_trg(void *exp_structure, bool include_neighbours)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	uint32_t vtx_memsize = 3 * msh3trg->N_elems *
		sizeof(*(msh3trg->adj));
	msh3trg->adj = nb_allocate_mem(vtx_memsize);

	if (include_neighbours) {
		uint32_t nbg_memsize = 3 * msh3trg->N_elems *
			sizeof(*(msh3trg->ngb));
		msh3trg->ngb = nb_allocate_mem(nbg_memsize);
	}
}

static void msh3trg_set_trg(void *exp_structure, uint32_t i,
			    uint32_t v1, uint32_t v2, uint32_t v3)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->adj[i * 3] = v1;
	msh3trg->adj[i*3+1] = v2;
	msh3trg->adj[i*3+2] = v3;
}

static void msh3trg_set_trg_neighbours(void *exp_structure,
				       uint32_t i, uint32_t t1,
				       uint32_t t2, uint32_t t3)

{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->ngb[i * 3] = t1;
	msh3trg->ngb[i*3+1] = t2;
	msh3trg->ngb[i*3+2] = t3;
}

static void msh3trg_set_N_input_vtx(void *exp_structure, uint32_t N)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->N_vtx = N;
}

static void msh3trg_allocate_input_vtx(void *exp_structure)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	uint32_t memsize = msh3trg->N_vtx *
		sizeof(*(msh3trg->vtx));
	msh3trg->vtx = nb_allocate_mem(memsize);
}

static void msh3trg_set_input_vtx(void *exp_structure, uint32_t i,
				  uint32_t vtx_id)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->vtx[i] = vtx_id;
}

static void msh3trg_set_N_input_sgm(void *exp_structure, uint32_t N)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->N_sgm = N;
}

static void msh3trg_allocate_input_sgm_table(void *exp_structure)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	uint32_t sgm_memsize = msh3trg->N_sgm *
		sizeof(*(msh3trg->N_nod_x_sgm));
	msh3trg->N_nod_x_sgm = nb_allocate_mem(sgm_memsize);
	uint32_t vtx_memsize = msh3trg->N_sgm *
		sizeof(*(msh3trg->nod_x_sgm));
	msh3trg->nod_x_sgm = nb_allocate_mem(vtx_memsize);
}

static void msh3trg_input_sgm_set_N_vtx(void *exp_structure, uint32_t i,
					uint32_t N)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	msh3trg->N_nod_x_sgm[i] = N;
}

static void msh3trg_input_sgm_allocate_vtx(void *exp_structure, uint32_t i)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	uint32_t memsize = msh3trg->N_nod_x_sgm[i] *
		sizeof(**(msh3trg->nod_x_sgm));
	msh3trg->nod_x_sgm[i] = nb_allocate_mem(memsize);
}

static void msh3trg_input_sgm_start_access(void *exp_structure, uint32_t i)
{
	void **structure = exp_structure;
	uint32_t *isgm = structure[1];
	*isgm = i;
}

static void msh3trg_input_sgm_set_vtx(void *exp_structure,
				      uint32_t ivtx, uint32_t vtx_id)
{
	nb_msh3trg_t *msh3trg = GET_MSH3TRG_FROM_STRUCT(exp_structure);
	uint32_t isgm = GET_ISGM_FROM_STRUCT(exp_structure);
	msh3trg->nod_x_sgm[isgm][ivtx] = vtx_id;
}

void nb_msh3trg_set_nodal_permutation(void *msh, const uint32_t *perm)
{
	set_nodal_perm_to_nodes(msh, perm);
	set_nodal_perm_to_edges(msh, perm);
	set_nodal_perm_to_elems(msh, perm);
	set_nodal_perm_to_invtx(msh, perm);
	set_nodal_perm_to_insgm(msh, perm);
}

static void set_nodal_perm_to_nodes(nb_msh3trg_t *msh3trg, const uint32_t *perm)
{
	uint32_t N = msh3trg->N_nod;
	
	uint32_t memsize = N * 2 * sizeof(*(msh3trg->nod));
	double *nodes = nb_soft_allocate_mem(memsize);

	memcpy(nodes, msh3trg->nod, memsize);

	for (uint32_t i = 0; i < N; i++) {
		uint32_t id = perm[i];
		memcpy(&(msh3trg->nod[id*2]), &(nodes[i*2]), 2 * sizeof(*nodes));
	}

	nb_soft_free_mem(memsize, nodes);
}

static void set_nodal_perm_to_edges(nb_msh3trg_t *msh3trg, const uint32_t *perm)
{
	uint32_t N = msh3trg->N_edg;
	for (uint32_t i = 0; i < 2 * N; i++) {
		uint32_t id = msh3trg->edg[i];
		msh3trg->edg[i] = perm[id];
	}
}

static void set_nodal_perm_to_elems(nb_msh3trg_t *msh3trg, const uint32_t *perm)
{
	uint32_t N = msh3trg->N_elems;
	for (uint32_t i = 0; i < N; i++) {
		for (uint16_t j = 0; j < 3; j++) {
			uint32_t id = msh3trg->adj[i*3+j];
			msh3trg->adj[i*3+j] = perm[id];
		}
	}
}

static void set_nodal_perm_to_invtx(nb_msh3trg_t *msh3trg, const uint32_t *perm)
{
	uint32_t N = msh3trg->N_vtx;
	for (uint32_t i = 0; i < N; i++) {
		uint32_t id = msh3trg->vtx[i];
		msh3trg->vtx[i] = perm[id];
	}
}

static void set_nodal_perm_to_insgm(nb_msh3trg_t *msh3trg, const uint32_t *perm)
{
	uint32_t N = msh3trg->N_sgm;
	for (uint32_t i = 0; i < N; i++) {
		for (uint16_t j = 0; j < msh3trg->N_nod_x_sgm[i]; j++) {
			uint32_t id = msh3trg->nod_x_sgm[i][j];
			msh3trg->nod_x_sgm[i][j] = perm[id];
		}
	}
}

void nb_msh3trg_disable_single_point_connections(const void *msh3trg_ptr,
						 bool* enabled_elements)
{
	const nb_msh3trg_t *msh3trg = msh3trg_ptr;
	/* Allocate lists to store triangles per vertex */
	uint32_t *N_trg_x_vtx = nb_allocate_zero_mem(msh3trg->N_nod *
						     sizeof(*N_trg_x_vtx));
	uint32_t **trg_x_vtx = nb_allocate_mem(msh3trg->N_nod *
					       sizeof(*trg_x_vtx));
	for (uint32_t i = 0; i < msh3trg->N_nod; i++)
		trg_x_vtx[i] = nb_allocate_zero_mem(10 *
						    sizeof(*(trg_x_vtx[i])));
      
	/* Iterate over triangles to found relations */
	for (uint32_t i = 0; i < msh3trg->N_elems; i++) {
		if (!enabled_elements[i])
			continue;
		uint32_t v1 = msh3trg->adj[i * 3];
		uint32_t v2 = msh3trg->adj[i*3+1];
		uint32_t v3 = msh3trg->adj[i*3+2];
		trg_x_vtx[v1][N_trg_x_vtx[v1]++] = i;
		trg_x_vtx[v2][N_trg_x_vtx[v2]++] = i;
		trg_x_vtx[v3][N_trg_x_vtx[v3]++] = i;
	}

	/* Detect one point connections */
	for (uint32_t i = 0; i < msh3trg->N_nod; i++) {
		if(N_trg_x_vtx[i] < 2)
			continue;

		uint32_t itrg = trg_x_vtx[i][0];

		uint32_t itrg_twist = itrg;
		bool twist_around = false;

		while (itrg_get_right_triangle(msh3trg, 
					       enabled_elements,
					       itrg_twist, i) !=  msh3trg->N_elems) {
			itrg_twist = itrg_get_right_triangle(msh3trg, enabled_elements,
							     itrg_twist, i);
			if (itrg_twist == itrg) {
				twist_around = true;
				break;
			}
		}
		if (itrg_twist == itrg && twist_around)
			continue;

		uint32_t N_trg_fan = 0;
		uint32_t trg_fan[12];
		do {
			trg_fan[N_trg_fan++] = itrg_twist;
			itrg_twist = itrg_get_left_triangle(msh3trg, enabled_elements,
							    itrg_twist, i);
		} while (itrg_twist != msh3trg->N_elems);

		if (N_trg_fan == N_trg_x_vtx[i])
			continue;

		for (uint32_t j = 0; j < N_trg_x_vtx[i]; j++) {
			bool is_inside_the_fan = false;
			for (uint32_t k = 0; k < N_trg_fan; k++) {
				if (trg_x_vtx[i][j] == trg_fan[k]) {
					is_inside_the_fan = true;
					break;
				}
			}
			if (is_inside_the_fan)
				continue;
			enabled_elements[trg_x_vtx[i][j]] = false;
		}
	}
  
	/* Free memory */
	for (uint32_t i = 0; i < msh3trg->N_nod; i++)
		nb_free_mem(trg_x_vtx[i]);
	nb_free_mem(trg_x_vtx);
	nb_free_mem(N_trg_x_vtx);
}

static uint32_t itrg_get_right_triangle(const nb_msh3trg_t *const delaunay, 
					const bool *const enabled_elements,
					uint32_t itrg, uint32_t ivtx)
{
	if (delaunay->adj[itrg * 3] == ivtx) {
		uint32_t id = delaunay->ngb[itrg * 3];
		if (id < delaunay->N_elems)
			if (enabled_elements[id])
				return id;
		return delaunay->N_elems;
	}
	if (delaunay->adj[itrg*3+1] == ivtx) {
		uint32_t id = delaunay->ngb[itrg*3+1];
		if (id < delaunay->N_elems)
			if (enabled_elements[id])
				return id;
		return delaunay->N_elems;
	}
	if (delaunay->adj[itrg*3+2] == ivtx) {
		uint32_t id = delaunay->ngb[itrg*3+2];
		if (id < delaunay->N_elems)
			if (enabled_elements[id])
				return id;
		return delaunay->N_elems;
	}
	return delaunay->N_elems;
}

static uint32_t itrg_get_left_triangle(const nb_msh3trg_t *const delaunay,
				       const bool *const enabled_elements,
				       uint32_t itrg, uint32_t ivtx)
{
	if(delaunay->adj[itrg * 3] == ivtx) {
		uint32_t id = delaunay->ngb[itrg*3+2];
		if(id < delaunay->N_elems)
			if(enabled_elements[id])
				return id;
		return delaunay->N_elems;
	}
	if (delaunay->adj[itrg*3+1] == ivtx) {
		uint32_t id = delaunay->ngb[itrg * 3];
		if(id < delaunay->N_elems)
			if(enabled_elements[id])
				return id;
		return delaunay->N_elems;
	}
	if (delaunay->adj[itrg*3+2] == ivtx) {
		uint32_t id = delaunay->ngb[itrg*3+1];
		if(id < delaunay->N_elems)
			if(enabled_elements[id])
				return id;
		return delaunay->N_elems;
	}
	return delaunay->N_elems;
}

void nb_msh3trg_get_enveloping_box(const void *msh3trg_ptr,
				   double box[4])
{
	const nb_msh3trg_t *msh3trg = msh3trg_ptr;
	nb_utils2D_get_enveloping_box(msh3trg->N_nod, msh3trg->nod,
				       2 * sizeof(*(msh3trg->nod)),
				       nb_utils2D_get_x_from_darray,
				       nb_utils2D_get_y_from_darray,
				       box);
}

void nb_msh3trg_build_model(const void *msh3trg, nb_model_t *model)
{
	uint32_t N_vtx = nb_msh3trg_get_N_invtx(msh3trg);
	uint32_t N_sgm = nb_msh3trg_get_N_insgm(msh3trg);

	uint32_t sgm_memsize = 2 * N_sgm * sizeof(uint32_t);
	uint32_t* segments = nb_soft_allocate_mem(sgm_memsize);

	uint32_t vtx_memsize = 2 * N_vtx * sizeof(double);
	double* vertices = nb_soft_allocate_mem(vtx_memsize);

	uint32_t idx_memsize = N_vtx * sizeof(uint32_t);
	uint32_t* vtx_index_relation = nb_soft_allocate_mem(idx_memsize);

	uint32_t N_vertices = get_input_vtx(msh3trg, vertices,
					    vtx_index_relation);
	uint32_t N_segments = get_input_sgm(msh3trg, vtx_index_relation,
					    segments);

	nb_model_load_from_arrays(model, N_vertices, vertices,
				  N_segments, segments, 0 , NULL);

	nb_soft_free_mem(sgm_memsize, segments);
	nb_soft_free_mem(vtx_memsize, vertices);
	nb_soft_free_mem(idx_memsize, vtx_index_relation);

	set_holes_to_model(msh3trg, model);
}

static uint32_t get_input_vtx(const void *msh, double *vertices,
			      uint32_t *vtx_index_relation)
{
	uint32_t N_vtx = nb_msh3trg_get_N_invtx(msh);
	uint32_t N_nod = nb_msh3trg_get_N_nodes(msh);
	uint32_t N_vertices = 0;
	for (uint32_t i = 0; i < N_vtx; i++) {
		uint32_t id = nb_msh3trg_get_invtx(msh, i);
		if (id < N_nod) {
			vertices[N_vertices * 2] =
				nb_msh3trg_node_get_x(msh, id);
			vertices[N_vertices*2+1] =
				nb_msh3trg_node_get_y(msh, id);

			vtx_index_relation[i] = N_vertices;
			N_vertices += 1;
		}
	}
	return N_vertices;
}

static uint32_t get_input_sgm(const void *msh,
			      const uint32_t *vtx_index_relation,
			      uint32_t *segments)
{
	uint32_t N_vtx = nb_msh3trg_get_N_invtx(msh);
	uint32_t N_sgm = nb_msh3trg_get_N_insgm(msh);
	uint32_t N_segments = 0;
	for (uint32_t i = 0; i < N_sgm; i++) {
		uint32_t N_nod_x_sgm =
			nb_msh3trg_insgm_get_N_nodes(msh, i);
		if (0 < N_nod_x_sgm) {
			uint32_t v1 = nb_msh3trg_insgm_get_node(msh, i, 0);
			uint32_t last_idx = N_nod_x_sgm - 1;
			uint32_t v2 = nb_msh3trg_insgm_get_node(msh, i,
								last_idx);
			for (uint32_t j = 0; j < N_vtx; j++) {
				if (nb_msh3trg_get_invtx(msh, j) == v1) {
					v1 = j;
					break;
				}
			}
			for (uint32_t j = 0; j < N_vtx; j++) {
				if (nb_msh3trg_get_invtx(msh, j) == v2) {
					v2 = j;
					break;
				}
			}
			segments[N_segments * 2] = vtx_index_relation[v1];
			segments[N_segments*2+1] = vtx_index_relation[v2];
			N_segments += 1;
		}
	}
	return N_segments;
}

static void set_holes_to_model(const void *msh, nb_model_t *model)
{
	uint32_t N_centroids;
	double *centroids =
		nb_model_get_centroids_of_enveloped_areas(model, &N_centroids);

	uint32_t N_holes = 0;
	double *holes = NULL;

	if (0 < N_centroids) {
		char* mask_centroids = nb_soft_allocate_mem(N_centroids);
		memset(mask_centroids, 0, N_centroids);

		N_holes = get_centroids_mask(msh, N_centroids,
					     centroids, mask_centroids);
		holes = get_holes(N_holes, N_centroids, centroids,
				  mask_centroids);

		nb_soft_free_mem(N_centroids, mask_centroids);
		nb_free_mem(centroids);
	}
	
	model->H = N_holes;
	model->holes = holes;
}

static uint32_t get_centroids_mask(const void *msh, uint32_t N_centroids,
				   double *centroids, char *mask_centroids)
{
	uint32_t N_holes = 0;
	for (uint32_t i = 0; i < N_centroids; i++) {
		bool not_inside = !nb_msh3trg_is_vtx_inside(msh,
							    centroids[i * 2],
							    centroids[i*2+1]);
		if (not_inside) {
			mask_centroids[i] = 1;
			N_holes += 1;
		}
	}
	return N_holes;
}

static double *get_holes(uint32_t N_holes, uint32_t N_centroids,
			 const double *centroids, const char *mask_centroids)
{
	double *holes = NULL;
	if (0 < N_holes) {
		holes = nb_allocate_mem(2 * N_holes * sizeof(*holes));
		uint32_t i = 0;
		for (uint32_t j = 0; j < N_centroids; j++) {
			if (1 == mask_centroids[j]) {
				memcpy(&(holes[i*2]),
				       &(centroids[j*2]),
				       2 * sizeof(*holes));
				i += 1;
			}
		}
	}
	return holes;
}

void nb_msh3trg_build_model_disabled_elems(const void *msh3trg,
					   const bool *elems_enabled,
					   nb_model_t *model,
					   uint32_t *N_input_vtx,
					   uint32_t **input_vtx)
{
	/* Count segments and mark vertices used */
	model->M = 0;
	
	uint32_t N_nod = nb_msh3trg_get_N_nodes(msh3trg);
	char* vertices_used = nb_soft_allocate_mem(N_nod);
	memset(vertices_used, 0, N_nod);

	char* vertices_bndr = nb_soft_allocate_mem(N_nod);
	memset(vertices_bndr, 0, N_nod);

	uint32_t N_elems = nb_msh3trg_get_N_elems(msh3trg);
	for (uint32_t i = 0; i < N_elems; i++){
		if (!elems_enabled[i])
			continue;
		uint32_t v1 = nb_msh3trg_elem_get_adj(msh3trg, i, 0);
		uint32_t v2 = nb_msh3trg_elem_get_adj(msh3trg, i, 1);
		uint32_t v3 = nb_msh3trg_elem_get_adj(msh3trg, i, 2);
		uint32_t nid = nb_msh3trg_elem_get_ngb(msh3trg, i, 0);
		if (nid == N_elems) {
			model->M += 1;
			vertices_bndr[v1] = 1;
			vertices_bndr[v2] = 1;
			vertices_used[v1] = 1;
			vertices_used[v2] = 1;
		} else if (!elems_enabled[nid]) {
			model->M += 1;
			vertices_used[v1] = 1;
			vertices_used[v2] = 1;
		}

		nid = nb_msh3trg_elem_get_ngb(msh3trg, i, 1);
		if (nid == N_elems) {
			model->M += 1;
			vertices_bndr[v2] = 1;
			vertices_bndr[v3] = 1;
			vertices_used[v2] = 1;
			vertices_used[v3] = 1;
		} else if (!elems_enabled[nid]) {
			model->M += 1;
			N_input_vtx[0] += 1;
			vertices_used[v2] = 1;
			vertices_used[v3] = 1;
		}

		nid = nb_msh3trg_elem_get_ngb(msh3trg, i, 2);
		if (nid == N_elems) {
			model->M += 1;
			vertices_bndr[v3] = 1;
			vertices_bndr[v1] = 1;
			vertices_used[v3] = 1;
			vertices_used[v1] = 1;
 		} else if (!elems_enabled[nid]) {
			model->M += 1;
			vertices_used[v3] = 1;
			vertices_used[v1] = 1;
		}
	}
	/* Count vertices */
	model->N = 0;
	for(uint32_t i = 0; i < N_nod; i++){
		if(vertices_used[i]) 
			model->N += 1;
	}
	nb_soft_free_mem(N_nod, vertices_used);

	N_input_vtx[0] = 0;
	for (uint32_t i = 0; i < N_nod; i++) {
		if (vertices_bndr[i]) 
			N_input_vtx[0] += 1;
	}

	/* Allocate segments and vertices */
	nb_model_alloc_vertices(model);
	nb_model_alloc_edges(model);

	input_vtx[0] = nb_allocate_mem(N_input_vtx[0] * 
				       sizeof(uint32_t));

	/* Set vertices and segments */ 
	uint32_t idx_memsize = N_nod * sizeof(uint32_t);
	uint32_t* vertices_idx = nb_soft_allocate_mem(idx_memsize);
	memset(vertices_idx, 0, idx_memsize);

	for (uint32_t i = 0; i < N_nod; i++)
		vertices_idx[i] = N_nod;
  
	uint32_t real_vtx_cnt = 0;
	uint32_t vtx_counter = 0;
	uint32_t sgm_counter = 0;
	for (uint32_t i = 0; i < N_elems; i++) {
		if(!elems_enabled[i])
			continue;
		uint32_t v1 = nb_msh3trg_elem_get_adj(msh3trg, i, 0);
		uint32_t v2 = nb_msh3trg_elem_get_adj(msh3trg, i, 1);
		uint32_t v3 = nb_msh3trg_elem_get_adj(msh3trg, i, 2);

		/* Check side 1 */
		uint32_t nid = nb_msh3trg_elem_get_ngb(msh3trg, i, 0);
		bool include_side = false;
		if (nid == N_elems)
			include_side = true;
		else if (!elems_enabled[nid])
			include_side = true;
		if (include_side) {
			if (vertices_idx[v1] == N_nod) {
				model->vertex[vtx_counter * 2] =
					nb_msh3trg_node_get_x(msh3trg, v1);
				model->vertex[vtx_counter*2+1] =
					nb_msh3trg_node_get_y(msh3trg, v1);
				vertices_idx[v1] = vtx_counter++;
				if(vertices_bndr[v1])
					input_vtx[0][real_vtx_cnt++] = 
						vertices_idx[v1];
			}
			if(vertices_idx[v2] == N_nod) {
				model->vertex[vtx_counter * 2] =
					nb_msh3trg_node_get_x(msh3trg, v2);
				model->vertex[vtx_counter*2+1] =
					nb_msh3trg_node_get_y(msh3trg, v2);
				vertices_idx[v2] = vtx_counter++;
				if(vertices_bndr[v2])
					input_vtx[0][real_vtx_cnt++] =
						vertices_idx[v2];
			}
      
			model->edge[sgm_counter * 2] = vertices_idx[v1];
			model->edge[sgm_counter*2+1] = vertices_idx[v2];
			sgm_counter += 1;
		}

		/* Check side 2 */
		nid = nb_msh3trg_elem_get_ngb(msh3trg, i, 1);
		include_side = false;
		if (nid == N_elems)
			include_side = true;
		else if (!elems_enabled[nid])
			include_side = true;
		if (include_side) {
 			if (vertices_idx[v2] == N_nod) {
				model->vertex[vtx_counter * 2] =
					nb_msh3trg_node_get_x(msh3trg, v2);
				model->vertex[vtx_counter*2+1] =
					nb_msh3trg_node_get_y(msh3trg, v2);
				vertices_idx[v2] = vtx_counter++;
				if (vertices_bndr[v2])
					input_vtx[0][real_vtx_cnt++] =
						vertices_idx[v2];
			}
			if (vertices_idx[v3] == N_nod) {
				model->vertex[vtx_counter * 2] =
					nb_msh3trg_node_get_x(msh3trg, v3);
				model->vertex[vtx_counter*2+1] =
					nb_msh3trg_node_get_y(msh3trg, v3);
				vertices_idx[v3] = vtx_counter++;
				if (vertices_bndr[v3])
					input_vtx[0][real_vtx_cnt++] = 
						vertices_idx[v3];
			}
      
			model->edge[sgm_counter * 2] = vertices_idx[v2];
			model->edge[sgm_counter*2+1] = vertices_idx[v3];
			sgm_counter += 1;
		}

		/* Check side 3 */
		nid = nb_msh3trg_elem_get_ngb(msh3trg, i, 2);
		include_side = false;
		if (nid == N_elems)
			include_side = true;
		else if (!elems_enabled[nid])
			include_side = true;
		if (include_side) {
			if (vertices_idx[v3] == N_nod) {
				model->vertex[vtx_counter * 2] =
					nb_msh3trg_node_get_x(msh3trg, v3);
				model->vertex[vtx_counter*2+1] =
					nb_msh3trg_node_get_y(msh3trg, v3);
				vertices_idx[v3] = vtx_counter++;
				if(vertices_bndr[v3])
					input_vtx[0][real_vtx_cnt++] = 
						vertices_idx[v3];
			}
			if (vertices_idx[v1] == N_nod) {
				model->vertex[vtx_counter * 2] =
					nb_msh3trg_node_get_x(msh3trg, v1);
				model->vertex[vtx_counter*2+1] =
					nb_msh3trg_node_get_y(msh3trg, v1);
				vertices_idx[v1] = vtx_counter++;
				if (vertices_bndr[v1])
					input_vtx[0][real_vtx_cnt++] =
						vertices_idx[v1];
			}
      
			model->edge[sgm_counter * 2] = vertices_idx[v3];
			model->edge[sgm_counter*2+1] = vertices_idx[v1];
			sgm_counter += 1;
		}
	}
	nb_soft_free_mem(idx_memsize, vertices_idx);
	nb_soft_free_mem(N_nod, vertices_bndr);
}

void nb_msh3trg_centroid_iteration(void *msh, uint32_t max_iter,
				   /* density can be NULL */
				   double (*density)(const double[2],
						     const void *data),
				   const void *density_data)
{
	;/* PENDING */
}
