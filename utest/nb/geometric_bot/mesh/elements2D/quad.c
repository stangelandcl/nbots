#include <stdbool.h>
#include <stdlib.h>
#include <alloca.h>

#include <CUnit/Basic.h>

#include "nb/geometric_bot.h"

static int suite_init(void);
static int suite_clean(void);

static void test_load_from_mesh(void);

void cunit_nb_geometric_bot_mesh_elements2D_quad(void)
{
	CU_pSuite suite = CU_add_suite("nb/geometric_bot/mesh/elements2D/quad.c",
				       suite_init, suite_clean);
	CU_add_test(suite, "load_from_mesh()", test_load_from_mesh);
}

static int suite_init(void)
{
	return 0;
}

static int suite_clean(void)
{
	return 0;
}

static void test_load_from_mesh(void)
{
	vcn_model_t *model = vcn_model_create_polygon(1, 0, 0, 6);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 NB_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					 0.1);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	
	uint32_t size = nb_mshquad_get_memsize();
	nb_mshquad_t *quad = alloca(size);
	nb_mshquad_init(quad);
	nb_mshquad_load_from_mesh(quad, mesh);
	vcn_mesh_destroy(mesh);
	
	/* TEMPORAL FAIL: Produce different triangles each time */
	uint32_t N_elems = nb_mshquad_get_N_elems(quad);
	uint32_t N_edg = nb_mshquad_get_N_edges(quad);
	CU_ASSERT(600 < N_elems && 760 > N_elems);
	CU_ASSERT(1200 < N_edg && 1400 > N_edg);

	nb_mshquad_finish(quad);
}
