/* Compile from build dir:
  export LD_LIBRARY_PATH=libs/nbots/shared/debug:libs/nbots_cairo/shared/debug
  export FLAGS="-std=c99 -g -I ../include/"
  export FLIBS="-L libs/nbots/shared/debug -L libs/nbots_cairo/shared/debug"
  export LIBS="-lm -lnbots -lnbots_cairo -lcunit"
  gcc $FLAGS $FLIBS ../cunit_share/debug_tests_by_hand.c -o dbg_by_hand $LIBS
 */

#include "nbots.h"

#include "../utest/nb/geometric_bot/mesh/elements2D/polygons.c"/* PASTE MODULE TO DEBUG*/

#include <CUnit/Basic.h>

int main()
{

	if (CUE_SUCCESS == CU_initialize_registry()) {
		cunit_nb_geometric_bot_mesh_elements2D_poly();/* CALL CUNIT SUITE TO DEBUG */
		/* Run all tests using the CUnit Basic interface */
		CU_basic_set_mode(CU_BRM_VERBOSE);
		CU_basic_run_tests();
	} else {
		printf("ERROR: Initializing CUnit registry...");
	}
	CU_cleanup_registry();
	return CU_get_error();
}