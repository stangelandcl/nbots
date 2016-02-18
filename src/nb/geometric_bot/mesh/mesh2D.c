/******************************************************************************
 *   Geometric Bot: Geometric tesselations for numerical analysis.            *
 *   2011-2015 Victor Eduardo Cardoso Nungaray                                *
 *   Twitter: @victore_cardoso                                                *
 *   email: victorc@cimat.mx                                                  *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "nb/math_bot.h"
#include "nb/container_bot.h"
#include "nb/geometric_bot/point2D.h"
#include "nb/geometric_bot/utils2D.h"
#include "nb/geometric_bot/knn/bins2D.h"
#include "nb/geometric_bot/knn/bins2D_iterator.h"
#include "nb/geometric_bot/model/model2D.h"
#include "nb/geometric_bot/mesh/constrained_delaunay.h"
#include "nb/geometric_bot/mesh/mesh2D.h"

#include "../model/model2D_struct.h"
#include "ruppert/ruppert.h"
#include "mesh2D_structs.h"

static void init_tasks(vcn_mesh_t *mesh);
static void copy_tasks(vcn_mesh_t *copy, const vcn_mesh_t *const mesh);
static void null_task(const vcn_mesh_t *const mesh);
static inline void set_angle_constraint(vcn_mesh_t *mesh, double angle);
static void remove_triangles_propagate(vcn_mesh_t *const mesh,
				       msh_trg_t* trg);

static void remove_concavities_triangles(vcn_mesh_t* mesh);
static void remove_holes_triangles(vcn_mesh_t* mesh, double* holes,
				   uint32_t N_holes);

static void delete_trg_in_holes(vcn_mesh_t *mesh,
				const vcn_model_t *const restrict model);
static bool size_constraints_allow_refine(const vcn_mesh_t *const mesh);

/* Compare functions */
static int compare_sgmA_isSmallerThan_sgmB(const void *const sgmA, 
					   const void *const sgmB);
static int compare_sgmA_isTheSameThan_sgmB(const void *const  sgmA, 
					   const void *const  sgmB);
static int compare_trgA_isBetterThan_trgB(const void *const  trgA, 
					  const void *const  trgB);
static int compare_trgA_isSmallerThan_trgB(const void *const  trgA,
					   const void *const  trgB);
static int compare_id(const void* const ptrA,
		      const void* const ptrB);
static int compare_vtx(const void* const vtxA,
		       const void* const vtxB);
static int compare_sgm_by_dist_and_by_vtx_pointer
				(const void* const sgmA,
				 const void* const sgmB);

/* Hash functions */
static uint32_t hash_key_vtx(const void *const  vertex);
static uint32_t hash_key_trg(const void *const  triangle);

vcn_mesh_t* vcn_mesh_create(void)
{
	vcn_mesh_t *mesh = calloc(1, sizeof(*mesh));
	mesh->ug_vtx = vcn_bins2D_create(1.0);

	mesh->ht_edge = nb_container_create(NB_HASH);
	nb_container_set_key_generator(mesh->ht_edge, hash_key_edge);
	nb_container_set_comparer(mesh->ht_edge, compare_edge);
	nb_container_set_destroyer(mesh->ht_edge, free);

	mesh->ht_trg = nb_container_create(NB_HASH);
	nb_container_set_key_generator(mesh->ht_trg, hash_key_trg);
	nb_container_set_destroyer(mesh->ht_trg, free);
	mesh->scale = 1.0;

	set_angle_constraint(mesh, NB_MESH_MAX_ANGLE);

	init_tasks(mesh);

	mesh->refiner_type = NB_MESH_REFINE_RUPPERT;
	return mesh;
}

static void init_tasks(vcn_mesh_t *mesh)
{
	mesh->do_after_insert_trg = null_task;
	mesh->do_after_insert_vtx = null_task;
}

static void copy_tasks(vcn_mesh_t *copy, const vcn_mesh_t *const mesh)
{
	copy->do_after_insert_trg = mesh->do_after_insert_trg;
	copy->do_after_insert_vtx = mesh->do_after_insert_vtx;
}

void vcn_mesh_clear(vcn_mesh_t *mesh)
{
	free(mesh->input_vtx);
	mesh->input_vtx = NULL;
	mesh->N_input_vtx = 0;
	if (mesh->N_input_sgm > 0) {
		for (uint32_t i = 0; i < mesh->N_input_sgm; i++) {
			msh_edge_t* sgm = mesh->input_sgm[i];
			while (NULL != sgm) {
				msh_edge_t* to_free = sgm;
				sgm = medge_subsgm_next(sgm);
				medge_destroy_subsgm_attribute(to_free);
			}
		}
		free(mesh->input_sgm);
		mesh->N_input_sgm = 0;
	}
	vcn_bins2D_clear(mesh->ug_vtx);
	nb_container_clear(mesh->ht_trg);
	nb_container_clear(mesh->ht_edge);

	init_tasks(mesh);
}

void vcn_mesh_destroy(vcn_mesh_t* mesh)
{
	vcn_mesh_clear(mesh);
	vcn_bins2D_destroy(mesh->ug_vtx);
	nb_container_destroy(mesh->ht_trg);
	nb_container_destroy(mesh->ht_edge);
	free(mesh);
}

static void null_task(const vcn_mesh_t *const mesh)
{
	; /* NULL statement */
}

void vcn_mesh_set_task(vcn_mesh_t *mesh, int type,
		       void (*task)(const vcn_mesh_t *const))
{
	switch (type) {
	case NB_MESH_TASK_AFTER_INSERT_TRG:
		mesh->do_after_insert_trg = task;
		break;
	case NB_MESH_TASK_AFTER_INSERT_VTX:
		mesh->do_after_insert_vtx = task;
		break;
	}
}

void vcn_mesh_set_size_constraint(vcn_mesh_t *mesh, int type, 
				 uint32_t value)
{
	switch (type) {
	case NB_MESH_SIZE_CONSTRAINT_MAX_VTX:
		mesh->max_vtx = value;
		break;
	case NB_MESH_SIZE_CONSTRAINT_MAX_TRG:
		mesh->max_trg = value;
		break;
	}
}

void vcn_mesh_unset_size_constraint(vcn_mesh_t *mesh, int type)
{
	switch (type) {
	case NB_MESH_SIZE_CONSTRAINT_MAX_VTX:
		mesh->max_vtx = 0;
		break;
	case NB_MESH_SIZE_CONSTRAINT_MAX_TRG:
		mesh->max_trg = 0;
		break;
	}
}

uint32_t vcn_mesh_get_size_constraint(const vcn_mesh_t *mesh, int type)
{
	uint32_t val;
	switch (type) {
	case NB_MESH_SIZE_CONSTRAINT_MAX_VTX:
		val = mesh->max_vtx;
		break;
	case NB_MESH_SIZE_CONSTRAINT_MAX_TRG:
		val = mesh->max_trg;
		break;
	default:
		val = 0;
	}
	return val;
}

void vcn_mesh_set_geometric_constraint(vcn_mesh_t *mesh, int type, 
				      double value)
{
	switch (type) {
	case NB_MESH_GEOM_CONSTRAINT_MIN_ANGLE:
		set_angle_constraint(mesh, value);
		break;
	case NB_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH:
		mesh->max_edge_length = value;
		break;
	case NB_MESH_GEOM_CONSTRAINT_MAX_SUBSGM_LENGTH:
		mesh->max_subsgm_length = value;
		break;
	}
}

void vcn_mesh_unset_geometric_constraint(vcn_mesh_t *mesh, int type)
{
	switch (type) {
	case NB_MESH_GEOM_CONSTRAINT_MIN_ANGLE:
		set_angle_constraint(mesh, 0.0);
		break;
	case NB_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH:
		mesh->max_edge_length = 0.0;
		break;
	case NB_MESH_GEOM_CONSTRAINT_MAX_SUBSGM_LENGTH:
		mesh->max_subsgm_length = 0.0;
		break;
	}
}

static inline void set_angle_constraint(vcn_mesh_t *mesh, double angle)
{
	mesh->min_angle = angle;
	if (0.0 < angle)
		mesh->cr2se_ratio = 1.0 / (2.0 * sin(angle));
	else
		mesh->cr2se_ratio = 1e6;
}

double vcn_mesh_get_geometric_constraint(const vcn_mesh_t *mesh, int type)
{
	double val;
	switch (type) {
	case NB_MESH_GEOM_CONSTRAINT_MIN_ANGLE:
		val = mesh->min_angle;
		break;
	case NB_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH:
		val = mesh->max_edge_length;
		break;
	case NB_MESH_GEOM_CONSTRAINT_MAX_SUBSGM_LENGTH:
		val = mesh->max_subsgm_length;
		break;
	default:
		val = 0;
	}
	return val;
}

inline void vcn_mesh_set_density(vcn_mesh_t* mesh,
				 double (*density)(const double x[2],
						   const void *data),
				 const void *density_data)
{
	mesh->density = density;
	mesh->density_data = density_data;
}

inline void vcn_mesh_unset_density(vcn_mesh_t* mesh)
{
	mesh->density = NULL;
	mesh->density_data = NULL;
}

inline void vcn_mesh_set_refiner(vcn_mesh_t *mesh, int type)
{
	if (type >= 0 && type < NB_MESH_REFINE_DEFAULT)
		mesh->refiner_type = type;
	else
		mesh->refiner_type = NB_MESH_REFINE_DEFAULT;
}

inline int vcn_mesh_get_refiner(const vcn_mesh_t *const mesh)
{
	return mesh->refiner_type;
}

inline bool vcn_mesh_is_empty(const vcn_mesh_t *const mesh)
{
	return nb_container_is_empty(mesh->ht_trg);
}

bool vcn_mesh_is_vtx_inside(const vcn_mesh_t *const restrict mesh,
				 const double *const restrict vtx){
	msh_vtx_t v; 
	v.x[0] = mesh->scale * (vtx[0] - mesh->xdisp);
	v.x[1] = mesh->scale * (vtx[1] - mesh->ydisp);
	msh_trg_t* enveloping_trg = mesh_locate_vtx(mesh, &v);
	return (enveloping_trg != NULL);
}

void vcn_mesh_get_vertices(vcn_mesh_t* mesh, double* vertices)
{
	vcn_bins2D_iter_t* iter = vcn_bins2D_iter_create();
	vcn_bins2D_iter_set_bins(iter, mesh->ug_vtx);
	int i = 0;
	while (vcn_bins2D_iter_has_more(iter)) {
		const msh_vtx_t* vtx = vcn_bins2D_iter_get_next(iter);
		mesh_get_extern_scale_and_disp(mesh, vtx->x,
					       &(vertices[i * 2]));
		i++;
	}
	vcn_bins2D_iter_destroy(iter);
}

inline uint32_t vcn_mesh_get_N_vtx(const vcn_mesh_t *const mesh)
{
	return vcn_bins2D_get_length(mesh->ug_vtx);
}

inline uint32_t vcn_mesh_get_N_trg(const vcn_mesh_t *const mesh)
{
	return nb_container_get_length(mesh->ht_trg);
}

inline uint32_t vcn_mesh_get_N_edg(const vcn_mesh_t *const mesh)
{
	return nb_container_get_length(mesh->ht_edge);
}

double vcn_mesh_get_area(const vcn_mesh_t *const mesh)
{
	double area = 0.0;
	nb_iterator_t* iter = nb_iterator_create();
	nb_iterator_set_container(iter, mesh->ht_trg);
	while (nb_iterator_has_more(iter)) {
		msh_trg_t* trg = (msh_trg_t*) nb_iterator_get_next(iter);
		area += vcn_utils2D_get_2x_trg_area(trg->v1->x,
						    trg->v2->x,
						    trg->v3->x);
	}
	nb_iterator_destroy(iter);
	return (0.5 * area) / vcn_math_pow2(mesh->scale);
}



static void remove_triangles_propagate
                   (vcn_mesh_t *const restrict mesh, 
		    msh_trg_t* trg)
{
	mesh_substract_triangle(mesh, trg);
	trg->attr = (attr_t*)0x1;
	if (NULL != trg->t1) {
		msh_trg_t* nb_trg = trg->t1;
		mtrg_vanish_from_neighbour(trg, nb_trg);
		if (!medge_is_subsgm(trg->s1) && nb_trg->attr != (attr_t*)0x1)
			remove_triangles_propagate(mesh, nb_trg);
	}
	if (NULL != trg->t2) {
		msh_trg_t* nb_trg = trg->t2;
		mtrg_vanish_from_neighbour(trg, nb_trg);
		if (!medge_is_subsgm(trg->s2) && nb_trg->attr != (attr_t*)0x1)
			remove_triangles_propagate(mesh, nb_trg);
	}
	if (trg->t3 != NULL) {
		msh_trg_t* nb_trg = trg->t3;
		mtrg_vanish_from_neighbour(trg, nb_trg);
		if (!medge_is_subsgm(trg->s3) && nb_trg->attr != (attr_t*)0x1)
			remove_triangles_propagate(mesh, nb_trg);
	}
	free(trg);
}

static inline int compare_sgmA_isSmallerThan_sgmB
                                 (const void *const restrict sgmA, 
				  const void *const restrict sgmB)
{
	const uint64_t lA = 
		(uint64_t)(1e8 * medge_get_computed_length((msh_edge_t*)sgmA));
	const uint64_t lB = 
		(uint64_t)(1e8 * medge_get_computed_length((msh_edge_t*)sgmB));
	if (lB > lA)
		return 1;
	else if (lA > lB)
		return -1;
	return 0;
}

static inline int compare_sgmA_isTheSameThan_sgmB
                                 (const void *const restrict sgmA, 
				  const void *const restrict sgmB)
{
	const msh_edge_t *const restrict A = (msh_edge_t*)sgmA;
	const msh_edge_t *const restrict B = (msh_edge_t*)sgmB;
	if (A->v1 == B->v1 && A->v2 == B->v2)
		return 0;
	if (sgmA < sgmB)
		return 1;
	if (sgmA > sgmB) 
		return -1;
	return 0;
}

static inline int compare_trgA_isBetterThan_trgB
                                (const void *const restrict trgA,
				 const void *const restrict trgB)
{
	const uint64_t QA = 
		(uint64_t)(1e6 * mtrg_get_computed_quality((msh_trg_t*)trgA));
	const uint64_t QB = 
		(uint64_t)(1e6 * mtrg_get_computed_quality((msh_trg_t*)trgB));
	if (QA > QB)
		return 1;
	else if (QB > QA)
		return -1;  
	return 0;
}

static inline int compare_trgA_isSmallerThan_trgB
                                (const void *const restrict trgA, 
				 const void *const restrict trgB)
{
	const uint64_t SA = 
		(uint64_t)(1e8 * mtrg_get_computed_size((msh_trg_t*)trgA));
	const uint64_t SB = 
		(uint64_t)(1e8 * mtrg_get_computed_size((msh_trg_t*)trgB));
	if (SA < SB)
		return 1;
	else if (SB < SA)
		return -1;  
	return 0;
}

static inline int compare_id(const void* const ptrA,
			     const void* const ptrB)
{
	uint32_t* A = (uint32_t*) ptrA;
	uint32_t* B = (uint32_t*) ptrB;
	if (A[0] == B[0])
		return 0;
	if(A[0] > B[0])
		return 1;
	return -1;
}

static inline int compare_vtx(const void* const vtxA,
			      const void* const vtxB)
{
	msh_vtx_t* vA = (msh_vtx_t*) vtxA;
	msh_vtx_t* vB = (msh_vtx_t*) vtxB;
	if (vcn_utils2D_get_dist2(vA->x, vB->x) < NB_GEOMETRIC_TOL)
		return 0;
	return 1;
}

static inline int compare_sgm_by_dist_and_by_vtx_pointer
                                            (const void* const sgmA,
					     const void* const sgmB)
{
	int comp1 = compare_sgmA_isSmallerThan_sgmB(sgmA, sgmB);
	if (0 == comp1) {
		return compare_sgmA_isTheSameThan_sgmB(sgmA, sgmB);
	}
	return -comp1;
}

static inline uint32_t hash_key_vtx(const void *const  vertex)
{
	const msh_vtx_t *const vtx = vertex;
	return (uint32_t)((int)(vtx->x[0] * 73856093) ^
			  (int)(vtx->x[1] * 19349663));
}

static inline uint32_t hash_key_trg(const void *const restrict triangle)
{
	const msh_trg_t *const restrict trg = triangle;
	return (uint32_t)
		((int)((trg->v1->x[0]/((trg->v1->x[1]!=0.0)?
				       trg->v1->x[1]:13.13))*73856093) ^ 
		 (int)((trg->v2->x[1]/((trg->v2->x[0]!=0.0)?
				       trg->v2->x[0]:17.17))*19349663) ^
		 (int)((trg->v3->x[0]/((trg->v3->x[1]!=0.0)?
				       trg->v3->x[1]:23.23))*83492791));
}

static void remove_concavities_triangles(vcn_mesh_t* mesh)
{
	/* (BIG OPPORTUNITY TO MAKE IT FAST) */
	msh_trg_t* trg = NULL;
	nb_iterator_t* iter = nb_iterator_create();
	nb_iterator_set_container(iter, mesh->ht_trg);
	while (nb_iterator_has_more(iter)) {
		trg = (msh_trg_t*)nb_iterator_get_next(iter);
		if ((trg->t1 == NULL && !medge_is_subsgm(trg->s1)) || 
		    (trg->t2 == NULL && !medge_is_subsgm(trg->s2)) ||
		    (trg->t3 == NULL && !medge_is_subsgm(trg->s3))) {
			remove_triangles_propagate(mesh, trg);
			nb_iterator_restart(iter);
		}
	}
	nb_iterator_destroy(iter);
}

static void remove_holes_triangles(vcn_mesh_t* mesh, double* holes,
				   uint32_t N_holes)
{
	/* (BIG OPPORTUNITY TO MAKE IT FAST) */
	for (uint32_t i=0; i < N_holes; i++) {
		msh_trg_t* trg = NULL;
		nb_iterator_t* iter = nb_iterator_create();
		nb_iterator_set_container(iter, mesh->ht_trg);
		while (nb_iterator_has_more(iter)) {
			/* REFACTOR next line */
			trg = (msh_trg_t*) nb_iterator_get_next(iter);
			if (vcn_utils2D_pnt_lies_in_trg(trg->v1->x,
							trg->v2->x,
							trg->v3->x,
							&(holes[i*2])))
			{
				remove_triangles_propagate(mesh, trg);
				break;
			}
		}
		nb_iterator_destroy(iter);
	}
}

void vcn_mesh_refine(vcn_mesh_t *restrict mesh)
{
	switch (mesh->refiner_type) {
	case NB_MESH_REFINE_RUPPERT:
		vcn_ruppert_refine(mesh);
		break;
	case NB_MESH_REFINE_CHEW:
		printf("Chew refinement is not implemented yet\n");
		break;
	default:
		vcn_ruppert_refine(mesh);
	}
}

bool vcn_mesh_insert_vtx(vcn_mesh_t *restrict mesh, const double vertex[2])
{
	bool inserted;
	switch (mesh->refiner_type) {
	case NB_MESH_REFINE_RUPPERT:
		inserted = vcn_ruppert_insert_vtx(mesh, vertex);
		break;
	case NB_MESH_REFINE_CHEW:
		inserted = false;
		printf("Chew refinement is not implemented yet\n");
		break;
	default:
		inserted = vcn_ruppert_insert_vtx(mesh, vertex);
	}
	return inserted;
}

vcn_mesh_t* vcn_mesh_clone(const vcn_mesh_t* const mesh)
{
	vcn_mesh_t* clone = calloc(1, sizeof(*clone));

	clone->scale = mesh->scale;
	clone->xdisp = mesh->xdisp;
	clone->ydisp = mesh->ydisp;

	clone->max_vtx = mesh->max_vtx;
	clone->max_trg = mesh->max_trg;
	
	clone->min_angle = mesh->min_angle;
	clone->cr2se_ratio = mesh->cr2se_ratio;
	clone->max_edge_length = mesh->max_edge_length;
	clone->max_subsgm_length = mesh->max_subsgm_length;
	
	clone->density = mesh->density;
	clone->density_data = mesh->density_data;

	clone->refiner_type = mesh->refiner_type;

	copy_tasks(clone, mesh);

	/* Clone arrays to relate the mesh with the input */
	clone->N_input_vtx = mesh->N_input_vtx;
	clone->input_vtx = 
		calloc(clone->N_input_vtx, sizeof(*(clone->input_vtx)));
	clone->N_input_sgm = mesh->N_input_sgm;
	clone->input_sgm =
		calloc(clone->N_input_sgm, sizeof(*(clone->input_sgm)));

	/* Clone grid of vertices */
	uint32_t N_vertices = vcn_bins2D_get_length(mesh->ug_vtx);
	msh_vtx_t** vertices = malloc(N_vertices * sizeof(*vertices));
	double bins_size = vcn_bins2D_get_size_of_bins(mesh->ug_vtx);
	clone->ug_vtx = vcn_bins2D_create(bins_size);
	uint32_t i = 0;
	vcn_bins2D_iter_t* giter = vcn_bins2D_iter_create();
	vcn_bins2D_iter_set_bins(giter, mesh->ug_vtx);
	while (vcn_bins2D_iter_has_more(giter)) {
		msh_vtx_t* vtx = (msh_vtx_t*) vcn_bins2D_iter_get_next(giter);
		/* Create the vertex clone */
		msh_vtx_t* vtx_clone = vcn_point2D_create();
		memcpy(vtx_clone->x, vtx->x, 2 * sizeof(*(vtx->x)));
		vtx_clone->attr = vtx->attr;

		/* Set an index to the original vertex */
		void** attr = malloc(2 * sizeof(*attr));
		uint32_t *id = malloc(sizeof(*id));
		id[0] = i++;
		attr[0] = id;
		attr[1] = vtx->attr;
		vtx->attr = attr;
		/* Set cloned and original vertices to the built-in hash table */
		vertices[id[0]] = vtx_clone;
		/* Insert the clone vertex into the clone grid */
		vcn_bins2D_insert(clone->ug_vtx, vtx_clone);
	}
	/* Create a built-in hash table to relate original and cloned segments
	 * and triangles */
	uint32_t N_triangles = nb_container_get_length(mesh->ht_trg);
	msh_trg_t** triangles = malloc(N_triangles * sizeof(*triangles));
	i = 0;
	nb_iterator_t* trg_iter = nb_iterator_create();
	nb_iterator_set_container(trg_iter, mesh->ht_trg);
	while (nb_iterator_has_more(trg_iter)) {
		msh_trg_t* trg = (msh_trg_t*) nb_iterator_get_next(trg_iter);
		/* Clone the triangle */
		msh_trg_t* trg_clone = calloc(1, sizeof(*trg_clone));
		trg_clone->attr = trg->attr;
		/* Set an index to the original */
		void** attr = malloc(2 * sizeof(*attr));
		uint32_t *id = malloc(sizeof(*id));
		id[0] = i++;
		attr[0] = id;
		attr[1] = trg->attr;
		trg->attr = attr;
		/* Set cloned and original to the built-in hash table */
		triangles[id[0]] = trg_clone;
	}

	uint32_t N_segments = nb_container_get_length(mesh->ht_edge);
	msh_edge_t** segments = malloc(N_segments * sizeof(*segments));
	i = 0;
	nb_iterator_t* sgm_iter = nb_iterator_create();
	nb_iterator_set_container(sgm_iter, mesh->ht_edge);
	while (nb_iterator_has_more(sgm_iter)) {
		msh_edge_t* sgm = (msh_edge_t*) nb_iterator_get_next(sgm_iter);
		/* Clone the segment */
		msh_edge_t* sgm_clone = calloc(1, sizeof(*sgm_clone));

		/* Set an index to the original */
		void** attr = malloc(2 * sizeof(*attr));
		uint32_t *id = malloc(sizeof(*id));
		id[0] = i++;
		attr[0] = id;
		attr[1] = sgm->attr;
		sgm->attr = attr;
		/* Set cloned and original to the built-in hash table */
		segments[id[0]] = sgm_clone;
	}

	/* Clone data structures and hash tables used to handle mesh */
	clone->ht_trg = nb_container_create(NB_HASH);
	nb_container_set_key_generator(clone->ht_trg, hash_key_trg);
	nb_container_set_destroyer(clone->ht_trg, free);
	nb_iterator_restart(trg_iter);
	while (nb_iterator_has_more(trg_iter)) {
		const msh_trg_t *trg = nb_iterator_get_next(trg_iter);
		msh_trg_t *trg_clone = 
			triangles[((uint32_t*)((void**)trg->attr)[0])[0]];
		trg_clone->v1 = vertices[((uint32_t*)((void**)trg->v1->attr)[0])[0]];
		trg_clone->v2 = vertices[((uint32_t*)((void**)trg->v2->attr)[0])[0]];
		trg_clone->v3 = vertices[((uint32_t*)((void**)trg->v3->attr)[0])[0]];

		trg_clone->s1 = segments[((uint32_t*)((void**)trg->s1->attr)[0])[0]];
		trg_clone->s2 = segments[((uint32_t*)((void**)trg->s2->attr)[0])[0]];
		trg_clone->s3 = segments[((uint32_t*)((void**)trg->s3->attr)[0])[0]];

		if (NULL != trg->t1)
			trg_clone->t1 = triangles[((uint32_t*)((void**)trg->t1->attr)[0])[0]];
		if (NULL != trg->t2)
			trg_clone->t2 = triangles[((uint32_t*)((void**)trg->t2->attr)[0])[0]];
		if (NULL != trg->t3)
			trg_clone->t3 = triangles[((uint32_t*)((void**)trg->t3->attr)[0])[0]];
		nb_container_insert(clone->ht_trg, trg_clone);
	}
	clone->ht_edge = nb_container_create(NB_HASH);
	nb_container_set_key_generator(clone->ht_edge, hash_key_edge);
	nb_container_set_destroyer(clone->ht_edge, free);
	nb_container_set_comparer(clone->ht_edge, compare_edge);
	nb_iterator_restart(sgm_iter);
	while (nb_iterator_has_more(sgm_iter)) {
		const msh_edge_t *sgm = nb_iterator_get_next(sgm_iter);
		msh_edge_t *sgm_clone = segments[((uint32_t*)((void**)sgm->attr)[0])[0]];
		sgm_clone->v1 = vertices[((uint32_t*)((void**)sgm->v1->attr)[0])[0]];
		sgm_clone->v2 = vertices[((uint32_t*)((void**)sgm->v2->attr)[0])[0]];
		if (NULL != sgm->t1)
			sgm_clone->t1 = triangles[((uint32_t*)((void**)sgm->t1->attr)[0])[0]];
		if (NULL != sgm->t2)
			sgm_clone->t2 = triangles[((uint32_t*)((void**)sgm->t2->attr)[0])[0]];
		attr_t* sgm_attr = (attr_t*)((void**)sgm->attr)[1];
		if (NULL != sgm_attr) {
			if (1 == sgm_attr->id) {
				/* The segment is forming the input */
				input_sgm_attr_t* input_sgm_attr = sgm_attr->data;
				msh_edge_t* prev_clone = NULL;
				msh_edge_t* prev = input_sgm_attr->prev;
				if (NULL != prev)
					prev_clone = segments[((uint32_t*)((void**)prev->attr)[0])[0]];
				msh_edge_t* next_clone = NULL;
				msh_edge_t* next = input_sgm_attr->next;
				if(NULL != next)
					next_clone = segments[((uint32_t*)((void**)next->attr)[0])[0]];
				medge_set_as_subsgm(sgm_clone, medge_subsgm_idx(sgm),
						    prev_clone, next_clone);
			}
		}
		nb_container_insert(clone->ht_edge, sgm_clone);
	}
	for (uint32_t i = 0; i < clone->N_input_vtx; i++)
		clone->input_vtx[i] = 
			vertices[((uint32_t*)((void**)mesh->input_vtx[i]->attr)[0])[0]];

	for (uint32_t i = 0; i < clone->N_input_sgm; i++)
		clone->input_sgm[i] = 
			segments[((uint32_t*)((void**)mesh->input_sgm[i]->attr)[0])[0]];  

	/* Free memory */
	free(vertices);
	free(segments);
	free(triangles);

	nb_iterator_restart(sgm_iter);
	while (nb_iterator_has_more(sgm_iter)) {
		msh_edge_t* sgm = (msh_edge_t*) nb_iterator_get_next(sgm_iter);
		void** attr = sgm->attr;
		sgm->attr = attr[1];
		free(attr[0]);
		free(attr);
	}
	nb_iterator_destroy(sgm_iter);

	nb_iterator_restart(trg_iter);
	while (nb_iterator_has_more(trg_iter)) {
		msh_trg_t* trg = (msh_trg_t*) nb_iterator_get_next(trg_iter);
		void** attr = trg->attr;
		trg->attr = attr[1];
		free(attr[0]);
		free(attr);
	}
	nb_iterator_destroy(trg_iter);

	vcn_bins2D_iter_restart(giter);
	while (vcn_bins2D_iter_has_more(giter)) {
		msh_vtx_t* vtx = (msh_vtx_t*) vcn_bins2D_iter_get_next(giter);
		void** attr = vtx->attr;
		vtx->attr = attr[1];
		free(attr[0]);
		free(attr);
	}
	vcn_bins2D_iter_destroy(giter);

	/* Return clone */
	return clone;
}

void vcn_mesh_generate_from_model(vcn_mesh_t *mesh,
				  const vcn_model_t *const restrict model)
{
	vcn_mesh_get_constrained_delaunay(mesh, model->N, model->vertex,
					  model->M, model->edge);

	delete_trg_in_holes(mesh, model);
	remove_concavities_triangles(mesh);
	
	if (size_constraints_allow_refine(mesh))
		vcn_mesh_refine(mesh);
}

static void delete_trg_in_holes(vcn_mesh_t *mesh,
				const vcn_model_t *const restrict model)
{
	if (0 < model->H) {
		double *holes = calloc(2 * model->H, sizeof(*holes));
		for (uint32_t i = 0; i < model->H; i++) {
			holes[i * 2] = mesh->scale * (model->holes[i * 2] - mesh->xdisp);
			holes[i*2+1] = mesh->scale * (model->holes[i*2+1] - mesh->ydisp);
		}
		remove_holes_triangles(mesh, holes, model->H);
		free(holes);
	}
}

static inline bool size_constraints_allow_refine(const vcn_mesh_t *const mesh)
{
	bool refine = false;
	if (vcn_bins2D_get_length(mesh->ug_vtx) < mesh->max_vtx ||
	    0 == mesh->max_vtx) {
		if (nb_container_get_length(mesh->ht_trg) < mesh->max_trg ||
		    0 == mesh->max_trg)
			refine = true;
	}
	return refine;
}