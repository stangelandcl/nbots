#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "test_library.h"
#include "test_add.h"

#define INPUTS_DIR "../tests/vcn/geometric_bot/mesh/mesh2D_UT_inputs"

#include  "vcn/geometric_bot/mesh/modules2D/exporter_cairo.h" /* TEMPORAL */
static int TEMPORAL_ = 0; /* TEMPORAL */		      /* TEMPORAL */
static void TEMPORAL(const vcn_mesh_t *const mesh)	      /* TEMPORAL */
{							      /* TEMPORAL */
	char label[100];				      /* TEMPORAL */
	sprintf(label, "TEMP_%02i.png", TEMPORAL_++);	      /* TEMPORAL */
	vcn_mesh_save_png(mesh, label, 1000, 800);	      /* TEMPORAL */
}                                                             /* TEMPORAL */

static bool check_generate_from_model_angle_constraint(void);
static bool check_generate_from_model_length_constraint(void);
static bool check_generate_from_model_huge_scale(void);
static bool check_generate_from_model_tiny_scale(void);
static bool check_generate_from_model_simple_square(void);
static bool check_generate_from_model_subsgm_constraint(void);
static bool check_generate_from_model_small_local_feature(void);
static bool check_generate_from_model_small_localf_v2(void);
static bool check_generate_from_model_subsgm_const_v2(void);
static bool check_generate_from_model_holes(void);
static bool check_generate_from_model_small_angles(void);
static bool check_generate_from_model_quasi_linear(void);

static double density_func(const double *const x, const void * const data);

inline int vcn_test_get_driver_id(void)
{
	return VCN_DRIVER_UNIT_TEST;
}

void vcn_test_load_tests(void *tests_ptr)
{
	vcn_test_add(tests_ptr, check_generate_from_model_angle_constraint,
		     "Check generate_from_model() with angle constraint");
	vcn_test_add(tests_ptr, check_generate_from_model_length_constraint,
		     "Check generate_from_model() with length constraint");
	vcn_test_add(tests_ptr, check_generate_from_model_huge_scale,
		     "Check generate_from_model() with huge scale");
	vcn_test_add(tests_ptr, check_generate_from_model_tiny_scale,
		     "Check generate_from_model() with tiny scale");
	vcn_test_add(tests_ptr, check_generate_from_model_simple_square,
		     "Check generate_from_model() of simple square");
	vcn_test_add(tests_ptr, check_generate_from_model_subsgm_constraint,
		     "Check generate_from_model() with sub-segment constraint");
	vcn_test_add(tests_ptr, check_generate_from_model_small_local_feature,
		     "Check generate_from_model() with small local feature");
	vcn_test_add(tests_ptr, check_generate_from_model_small_localf_v2,
		     "Check generate_from_model() with small local feat. v2");
	vcn_test_add(tests_ptr, check_generate_from_model_subsgm_const_v2,
		     "Check generate_from_model() with sub-segment const. v2");
	vcn_test_add(tests_ptr, check_generate_from_model_holes,
		     "Check generate_from_model() with holes");
	vcn_test_add(tests_ptr, check_generate_from_model_small_angles,
		     "Check generate_from_model() with small angles");
	vcn_test_add(tests_ptr, check_generate_from_model_quasi_linear,
		     "Check generate_from_model() with quasi linear segments");
}

static bool check_generate_from_model_angle_constraint(void)
/* Test that the algorithm terminates */
{
	char input_name[256];
	sprintf(input_name, "%s/Triangle.psl", INPUTS_DIR);
	vcn_model_t *model = vcn_model_load(input_name);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (39 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (79 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_length_constraint(void)
{
	vcn_model_t *model = vcn_model_create_polygon(20, 0, 0, 100);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					 1.0);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	//printf("\nCIRCLE: %i %i\n", vcn_mesh_get_N_trg(mesh),/* TEMPORAL */
	//     vcn_mesh_get_N_edg(mesh));                 /* TEMPORAL */
	bool trg_ok = (6468 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (9812 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_huge_scale(void)
{
	vcn_model_t *model = vcn_model_create_polygon(2e13, 0, 0, 100);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					 1e12);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (6468 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (9812 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_tiny_scale(void)
{
	vcn_model_t *model = vcn_model_create_polygon(2e-13, 0, 0, 100);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					 1e-14);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (6468 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (9812 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_simple_square(void)
{
	char input_name[256];
	sprintf(input_name, "%s/Square.psl", INPUTS_DIR);
	vcn_model_t *model = vcn_model_load(input_name);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					 0.5);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (39 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (79 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_subsgm_constraint(void)
{
	char input_name[256];
	sprintf(input_name, "%s/Square.psl", INPUTS_DIR);
	vcn_model_t *model = vcn_model_load(input_name);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					 0.5);
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_SUBSGM_LENGTH,
					 0.05);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (39 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (79 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_small_local_feature(void)
{
	char input_name[256];
	sprintf(input_name, "%s/Rectangle.psl", INPUTS_DIR);
	vcn_model_t *model = vcn_model_load(input_name);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					 5.0);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (39 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (79 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_small_localf_v2(void)
{
	char input_name[256];
	sprintf(input_name, "%s/Rectangle_with_two_nodges.psl", INPUTS_DIR);
	vcn_model_t *model = vcn_model_load(input_name);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					 2.0);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (39 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (79 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_subsgm_const_v2(void)
{
	char input_name[256];
	sprintf(input_name, "%s/Medieval_Ax.psl", INPUTS_DIR);
	vcn_model_t *model = vcn_model_load(input_name);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_SUBSGM_LENGTH,
					 0.5);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (39 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (79 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_holes(void)
{
	char input_name[256];
	sprintf(input_name, "%s/CIMAT_Logo.psl", INPUTS_DIR);
	vcn_model_t *model = vcn_model_load(input_name);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_SUBSGM_LENGTH,
					 0.3);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (39 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (79 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_small_angles(void)
{
	char input_name[256];
	sprintf(input_name, "%s/Spokes.psl", INPUTS_DIR);
	vcn_model_t *model = vcn_model_load(input_name);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_geometric_constraint(mesh,
					 VCN_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					 10);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (39 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (79 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_quasi_linear(void)
{
	return false; /* TEMPORAL: Fails on delaunay */
	char input_name[256];
	sprintf(input_name, "%s/Short_cantilever.psl", INPUTS_DIR);
	vcn_model_t *model = vcn_model_load(input_name);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (39 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (79 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}

static bool check_generate_from_model_size_constraint(void)
{
	char input_name[256];
	sprintf(input_name, "%s/Zacatecas.psl", INPUTS_DIR);
	vcn_model_t *model = vcn_model_load(input_name);
	vcn_mesh_t* mesh = vcn_mesh_create();
	vcn_mesh_set_size_constraint(mesh,
				     VCN_MESH_SIZE_CONSTRAINT_,
				     8);
	vcn_mesh_set_geometric_constraint(mesh,
					  VCN_MESH_GEOM_CONSTRAINT_MAX_EDGE_LENGTH,
					  8);
	vcn_mesh_set_geometric_constraint(mesh,
					  VCN_MESH_GEOM_CONSTRAINT_MAX_SUBSGM_LENGTH,
					  5);
	vcn_mesh_generate_from_model(mesh, model);
	vcn_model_destroy(model);
	bool trg_ok = (39 == vcn_mesh_get_N_trg(mesh));
	bool edg_ok = (79 == vcn_mesh_get_N_edg(mesh));
	TEMPORAL(mesh); /* TEMPORAL */
	vcn_mesh_destroy(mesh);
	return trg_ok && edg_ok;
}
static vcn_mesh_t* test14(char* name, const char* input_dir)
{
  char input_name[256];
  sprintf(input_name, "%s/Zacatecas.psl", input_dir);
  sprintf(name, "Zacatecas map");
  vcn_model_t *model = vcn_model_load(input_name);
  double max_edge[2] = {8.0, 5.0};
  vcn_mesh_t* mesh = 
    vcn_mesh_create_from_model(model, 0, 3000,
			       VCN_ANGLE_MAX,
			       VCN_DENSITY_MAX, max_edge);
  vcn_model_destroy(model);
    
  return mesh;
}
*/

/**
 * @brief Test vcn_mesh_is_vtx_inside().
 */
/*
static vcn_mesh_t* test15(char* name, const char* input_dir)
{
  char input_name[256];
  sprintf(input_name, "%s/Zacatecas.psl", input_dir);
  sprintf(name, "Zacatecas map into a Grid");
  vcn_model_t *model = vcn_model_load(input_name);
  vcn_mesh_t* mesh = 
    vcn_mesh_create_from_model(model, 0, 0,
			       VCN_ANGLE_UNC,
			       VCN_DENSITY_CDT, NULL);
  vcn_mesh_t *mesh_CDT = vcn_mesh_clone(mesh);
  double box[4];
  vcn_model_get_enveloping_box(model, box);
  double xstep = (box[2] - box[0]) / 49.0;
  double ystep = (box[3] - box[1]) / 49.0;
  for (register int i = 0; i < 50; i++) {
    for (register int j = 0; j < 50; j++) {
      double vtx[2];
      vtx[0] = box[0] + i * xstep;
      vtx[1] = box[1] + j * ystep;
      if (vcn_mesh_is_vtx_inside(mesh_CDT, vtx))
	vcn_mesh_insert_vertex(mesh, vtx);
    }
  }
  vcn_mesh_destroy(mesh_CDT);
  vcn_model_destroy(model);
  return mesh;
}
*/

/**
 * @brief Test the density function.
 */
/*
static vcn_mesh_t* test16(char* name, const char* input_dir)
{
  sprintf(name, "External density function");
  vcn_model_t* model = vcn_model_create_rectangle(-2.0 * M_PI, -2.0 * M_PI,
						  2.0 * M_PI, 2.0 * M_PI);
  vcn_mesh_t* mesh = 
    vcn_mesh_create_from_model(model, 0, 0, VCN_ANGLE_MAX,
			       density_func, NULL);
  vcn_model_destroy(model);

  return mesh;
}
*/

/**
 * @brief Test the PNG format in a color picture.
 */
/*
static vcn_mesh_t* test17(char* name, const char* input_dir)
{
  char input_name[256];
  sprintf(input_name, "%s/eye_raw.jpg", input_dir);
  sprintf(name, "Eye picture");
  vcn_density_img_t* data = vcn_density_img_create(input_name, 1.0, 
						   0.0, 0.0, 0.2);
  vcn_model_t* model = 
    vcn_model_create_rectangle(0.0, 0.0,
			       vcn_density_img_get_width(data),
			       vcn_density_img_get_height(data));
  vcn_mesh_t* mesh = 
    vcn_mesh_create_from_model(model, 0, 0, 0.2, VCN_DENSITY_IMG, data);
  vcn_model_destroy(model);
  vcn_density_img_destroy(data);

  return mesh;
}
*/

/**
 * @brief Test the JPEG format.
 */
/*
static vcn_mesh_t* test18(char* name, const char* input_dir)
{
  char input_name[256];
  sprintf(input_name, "%s/gnome.jpg", input_dir);
  sprintf(name, "Gnome logo");
  vcn_density_img_t* data = vcn_density_img_create(input_name, 1.0, 
						   0.0, 0.0, 1.0);
  vcn_model_t* model = 
    vcn_model_create_rectangle(0.0, 0.0,
			       vcn_density_img_get_width(data),
			       vcn_density_img_get_height(data));
  vcn_mesh_t* mesh = 
    vcn_mesh_create_from_model(model, 0, 0,
			       VCN_ANGLE_MAX,
			       VCN_DENSITY_IMG, data);
  vcn_model_destroy(model);
  vcn_density_img_destroy(data);

  return mesh;
}
*/

/**
 * @brief Test a grayscale PNG image.
 */
/*
static vcn_mesh_t* test19(char* name, const char* input_dir)
{
  char input_name[256];
  sprintf(input_name, "%s/women.png", input_dir);
  sprintf(name, "Women picture");

  vcn_density_img_t* data = vcn_density_img_create(input_name, 1.0, 
						   0.0, 0.0, 2.0);
  vcn_model_t* model = 
    vcn_model_create_rectangle(0.0, 0.0,
			       vcn_density_img_get_width(data),
			       vcn_density_img_get_height(data));
  vcn_mesh_t* mesh = 
    vcn_mesh_create_from_model(model, 0, 0, 0.2, VCN_DENSITY_IMG, data);
  vcn_model_destroy(model);
  vcn_density_img_destroy(data);

  return mesh;
}
*/

/**
 * @brief Test JPG black and white image.
 */
/*
static vcn_mesh_t* test20(char* name, const char* input_dir)
{
  char input_name[256];
  sprintf(input_name, "%s/hand.jpg", input_dir);
  sprintf(name, "Hand and baby hand");

  vcn_density_img_t* data = vcn_density_img_create(input_name, 1.0, 
						   0.0, 0.0, 0.2);
  vcn_model_t* model =
    vcn_model_create_rectangle(0.0, 0.0,
			       vcn_density_img_get_width(data),
			       vcn_density_img_get_height(data));
  vcn_mesh_t* mesh = 
    vcn_mesh_create_from_model(model, 0, 0, 0.2, VCN_DENSITY_IMG, data);
  vcn_model_destroy(model);
  vcn_density_img_destroy(data);

  return mesh;
}
*/


static inline double density_func(const double *const x, 
				  const void * const data)
{
  return 10.0 * (1.0 + sin(x[0]) * cos(x[1]));
}
