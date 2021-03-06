#include <stdbool.h>
#include <stdlib.h>

#include "cunit/Basic.h"

#include "nb/memory_bot.h"
#include "nb/geometric_bot.h"

static int suite_init(void);
static int suite_clean(void);

static void test_load_from_tessellator2D(void);

void cunit_nb_mshpoly(void)
{
	CU_pSuite suite =
		CU_add_suite("nb/geometric_bot/mesh/mesh2D/"\
			     "elements2D/mshpoly.c",
			     suite_init, suite_clean);
	CU_add_test(suite, "load_from_mesh()", test_load_from_tessellator2D);
}

static int suite_init(void)
{
	return 0;
}

static int suite_clean(void)
{
	return 0;
}

static void test_load_from_tessellator2D(void)
{
	nb_model_t *model = nb_model_create_polygon(1, 0, 0, 6);
	nb_tessellator2D_t* mesh = nb_tessellator2D_create();
	nb_tessellator2D_set_geometric_constraint(mesh,
					  NB_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					  0.05);
	nb_tessellator2D_generate_from_model(mesh, model);
	nb_model_destroy(model);

	void *poly = nb_allocate_on_stack(nb_mshpoly_get_memsize());
	nb_mshpoly_init(poly);
	nb_mshpoly_load_from_tessellator2D(poly, mesh);
	nb_tessellator2D_destroy(mesh);

	/* TEMPORAL FAIL: Produce different triangles each time */
	uint32_t N_elems = nb_mshpoly_get_N_elems(poly);
	uint32_t N_edges = nb_mshpoly_get_N_edges(poly);
	CU_ASSERT(2000 < N_elems && 2900 > N_elems);
	CU_ASSERT(7500 < N_edges && 8300 > N_edges);

	nb_mshpoly_finish(poly);
}
