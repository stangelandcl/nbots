#include <stdbool.h>
#include <stdlib.h>
#include <alloca.h>

#include <CUnit/Basic.h>

#include "nb/graphics_bot.h"
#include "nb/geometric_bot.h"

static int suite_init(void);
static int suite_clean(void);

static void test_load_from_mesh(void);

void cunit_nb_geometric_bot_mesh_elements2D_triangles(void)
{
	CU_pSuite suite = CU_add_suite("nb/geometric_bot/mesh/elements2D/triangles.c",
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
	nb_model_t *model  = alloca(vcn_model_get_memsize());
	model->N = 5;
	model->M = 5;
	model->H = 0;
	double vtx[10] = {-4, -1,
			  2.2, -2.6,
			  3.9, -2.7,
			  4, 1,
			  -4.0, 1};
	model->vertex = vtx;
	uint32_t edge[10] = {0, 1,
			     1, 2,
			     2, 3,
			     3, 4,
			     4, 0};
	model->edge = edge;
	model->holes = NULL;
	
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_generate_from_model(mesh, model);

	vcn_msh3trg_t *msh3trg = vcn_mesh_get_msh3trg(mesh, true, true, true, true, true);

	vcn_mesh_destroy(mesh);
	
	CU_ASSERT(9 == msh3trg->N_vertices);
	CU_ASSERT(15 == msh3trg->N_edges);
	CU_ASSERT(7 == msh3trg->N_triangles);

	vcn_msh3trg_destroy(msh3trg);
}
