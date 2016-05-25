#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <CUnit/Basic.h>

#include  "nb/math_bot.h"
#include  "nb/geometric_bot/mesh/mesh2D.h"
#include  "nb/geometric_bot/model/modules2D/clipper.h"

#define INPUTS_DIR "../../../../utest/nb/geometric_bot/model/modules2D/clipper_inputs"
#define OUTPUT_DIR "../../../"

#include  "nb/geometric_bot/model/modules2D/exporter_cairo.h" /* TEMPORAL */
#include  "nb/geometric_bot/mesh/modules2D/exporter_cairo.h" /* TEMPORAL */
static int TEMPORAL_ = 0; /* TEMPORAL */		      /* TEMPORAL */
static void TEMPORAL(const vcn_model_t *const model)	      /* TEMPORAL */
{							      /* TEMPORAL */
	char label[100];				      /* TEMPORAL */
	sprintf(label, "%s/TEMP_MODEL_%02i.png",
		OUTPUT_DIR, TEMPORAL_++);                    /* TEMPORAL */
	vcn_model_export_png(model, label, 1000, 800, false);
	vcn_mesh_t *mesh = vcn_mesh_create();
	vcn_mesh_generate_from_model(mesh, model);
	sprintf(label, "%s/TEMP_MODEL_%02i_MSH.png", OUTPUT_DIR,
		TEMPORAL_++);	                             /* TEMPORAL */
	vcn_mesh_save_png(mesh, label, 1000, 800);  /* TEMPORAL */
	vcn_mesh_destroy(mesh);
}                                                             /* TEMPORAL */

static int suite_init(void);
static int suite_clean(void);

static void test_get_combination_squares(void);
static void test_get_intersection_squares(void);
static void test_get_union_squares(void);
static void test_get_substractionA_squares(void);
static void test_get_substractionB_squares(void);
static void test_get_difference_squares(void);
static void test_get_combination_cross(void);
static void test_get_intersection_cross(void);
static void test_get_union_cross(void);
static void test_get_substractionA_cross(void);
static void test_get_substractionB_cross(void);
static void test_get_difference_cross(void);
static void test_get_combination_c(void);
static void test_get_intersection_c(void);
static void test_get_union_c(void);
static void test_get_substractionA_c(void);
static void test_get_substractionB_c(void);
static void test_get_difference_c(void);
static void test_get_combination_b(void);
static void test_get_intersection_b(void);
static void test_get_union_b(void);
static void test_get_substractionA_b(void);
static void test_get_substractionB_b(void);
static void test_get_difference_b(void);
static void test_get_combination_g(void);
static void test_get_intersection_g(void);
static void test_get_union_g(void);
static void test_get_substractionA_g(void);
static void test_get_substractionB_g(void);
static void test_get_difference_g(void);
static void test_get_combination_h(void);
static void test_get_intersection_h(void);
static void test_get_union_h(void);
static void test_get_substractionA_h(void);
static void test_get_substractionB_h(void);
static void test_get_difference_h(void);

void cunit_nb_geometric_bot_model2D_clipper(void)
{
	CU_pSuite suite = CU_add_suite("nb/geometric_bot/model/modules2D/clipper.c",
				       suite_init, suite_clean);
	CU_add_test(suite,
		    "get_combination() of squares",
		    test_get_combination_squares);
	CU_add_test(suite, "get_intersection() of squares",
		    test_get_intersection_squares);
	CU_add_test(suite, "get_union() of squares",
		    test_get_union_squares);
	CU_add_test(suite, "get_substraction() of squares A",
		    test_get_substractionA_squares);
	CU_add_test(suite, "get_substraction() of squares B",
		    test_get_substractionB_squares);
	CU_add_test(suite, "get_difference() of squares B",
		    test_get_difference_squares);
	CU_add_test(suite, "get_combination() of cross",
		    test_get_combination_cross);
	CU_add_test(suite, "get_intersection() of cross",
		    test_get_intersection_cross);
	CU_add_test(suite, "get_union() of cross",
		    test_get_union_cross);
	CU_add_test(suite, "get_substraction() of cross A",
		    test_get_substractionA_cross);
	CU_add_test(suite, "get_substraction() of cross B",
		    test_get_substractionB_cross);
	CU_add_test(suite, "get_difference() of cross",
		    test_get_difference_cross);
	CU_add_test(suite, "get_combination() of c",
		    test_get_combination_c);
	CU_add_test(suite, "get_intersection() of c",
		    test_get_intersection_c);
	CU_add_test(suite, "get_union() of c", test_get_union_c);
	CU_add_test(suite, "get_substraction() of c A",
		    test_get_substractionA_c);
	CU_add_test(suite, "get_substraction() of c B",
		    test_get_substractionB_c);
	CU_add_test(suite, "get_difference() of c",
		    test_get_difference_c);
	CU_add_test(suite, "get_combination() of b",
		    test_get_combination_b);
	CU_add_test(suite, "get_intersection() of b",
		    test_get_intersection_b);
	CU_add_test(suite, "get_union() of b", test_get_union_b);
	CU_add_test(suite, "get_substraction() of b A",
		    test_get_substractionA_b);
	CU_add_test(suite, "get_substraction() of b B",
		    test_get_substractionB_b);
	CU_add_test(suite, "get_difference() of b",
		    test_get_difference_b);
	return;/* TEMPORAL */

	CU_add_test(suite, "get_combination() of g",
		    test_get_combination_g);
	CU_add_test(suite, "get_intersection() of g",
		    test_get_intersection_g);
	CU_add_test(suite, "get_union() of g",
		    test_get_union_g);
	CU_add_test(suite, "get_substraction() of g A",
		    test_get_substractionA_g);
	CU_add_test(suite, "get_substraction() of g B",
		    test_get_substractionB_g);
	CU_add_test(suite, "get_difference() of g",
		    test_get_difference_g);
	CU_add_test(suite, "get_combination() of h",
		    test_get_combination_h);
	CU_add_test(suite, "get_intersection() of h",
		    test_get_intersection_h);
	CU_add_test(suite, "get_union() of h",
		    test_get_union_h);
	CU_add_test(suite, "get_substraction() of h A",
		    test_get_substractionA_h);
	CU_add_test(suite, "get_substraction() of h B",
		    test_get_substractionB_h);
	CU_add_test(suite, "get_difference() of h",
		    test_get_difference_h);
}

static int suite_init(void)
{
	return 0;
}

static int suite_clean(void)
{
	return 0;
}

static void test_get_combination_squares(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_squares_1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_squares_2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_combination(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_intersection_squares(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_squares_1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_squares_2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_intersection(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_union_squares(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_squares_1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_squares_2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_union(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionA_squares(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_squares_1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_squares_2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionB_squares(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_squares_2.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_squares_1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_difference_squares(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_squares_1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_squares_2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_difference(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_combination_cross(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_cross_1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_cross_2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_combination(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_intersection_cross(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_cross_1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_cross_2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_intersection(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_union_cross(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_cross_1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_cross_2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_union(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionA_cross(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_cross_1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_cross_2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionB_cross(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_cross_2.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_cross_1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_difference_cross(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_cross_1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_cross_2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_difference(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_combination_c(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_F1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_E1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_combination(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_intersection_c(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_F1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_E1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_intersection(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_union_c(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_F1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_E1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_union(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionA_c(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_F1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_E1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionB_c(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_E1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_F1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_difference_c(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_F1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_E1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_difference(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_combination_b(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_B1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_C1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_combination(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_intersection_b(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_B1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_C1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_intersection(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_union_b(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_B1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_C1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_union(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionA_b(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_B1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_C1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionB_b(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_C1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_B1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_difference_b(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_B1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_C1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_difference(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_combination_g(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_G1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_G2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_combination(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_intersection_g(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_G1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_G2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_intersection(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_union_g(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_G1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_G2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_union(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionA_g(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_G1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_G2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionB_g(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_G2.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_G1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_difference_g(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_G1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_G2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_difference(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_combination_h(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_H1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_H2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_combination(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_intersection_h(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_H1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_H2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_intersection(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_union_h(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_H1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_H2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_union(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionA_h(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_H1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_H2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_substractionB_h(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_H2.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_H1.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_substraction(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}

static void test_get_difference_h(void)
{
	char input_name[256];
	sprintf(input_name, "%s/combining_H1.psl", INPUTS_DIR);
	vcn_model_t *model1 = vcn_model_load(input_name);
	sprintf(input_name, "%s/combining_H2.psl", INPUTS_DIR);
	vcn_model_t *model2 = vcn_model_load(input_name);
	vcn_model_t *model = vcn_model_create();
	vcn_model_get_difference(model, model1, model2, 0);
	TEMPORAL(model); /* TEMPORAL */
	vcn_model_destroy(model1);
	vcn_model_destroy(model2);
	vcn_model_destroy(model);
	CU_ASSERT(false);
}