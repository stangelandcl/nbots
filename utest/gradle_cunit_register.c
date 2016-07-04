#include "nb.h"

#include "nb/container_bot_utests.h"
#include "nb/geometric_bot_utests.h"
#include "nb/image_bot_utests.h"
#include "nb/pde_bot_utests.h"
#include "nb/graphics_bot_utests.h"

#include "gradle_cunit_register.h"


void gradle_cunit_register()
{
	cunit_suites_nb_container_bot();
	cunit_suites_nb_geometric_bot();
	cunit_suites_nb_image_bot();
	cunit_suites_nb_pde_bot();
	cunit_suites_nb_graphics_bot();
}
