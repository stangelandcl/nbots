#include <stdlib.h>

#include "nb/memory_bot.h"
#include "nb/geometric_bot/utils2D.h"
#include "nb/geometric_bot/point2D.h"

inline nb_point2D_t* nb_point2D_create(void)
{
	return nb_allocate_zero_mem(sizeof(nb_point2D_t));
}

inline void nb_point2D_destroy(void *point_ptr)
{
	nb_free_mem(point_ptr);
}

inline int8_t nb_point2D_compare(const void *const p1_ptr,
				  const void *const p2_ptr)
{
	const nb_point2D_t *const p1 = p1_ptr;
	const nb_point2D_t *const p2 = p2_ptr;
	double dist = nb_utils2D_get_dist(p1->x, p2->x);
	return (dist < NB_GEOMETRIC_TOL_POW2) ?	0 : 1;
}
