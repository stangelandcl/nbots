#include <stdlib.h>

#include "nb/geometric_bot/utils2D.h"
#include "nb/geometric_bot/point2D.h"

inline vcn_point2D_t* vcn_point2D_create(void)
{
	return calloc(1, sizeof(vcn_point2D_t));
}

inline void vcn_point2D_destroy(void *point_ptr)
{
	free(point_ptr);
}

inline int8_t vcn_point2D_compare(const void *const p1_ptr,
				  const void *const p2_ptr)
{
	const vcn_point2D_t *const p1 = p1_ptr;
	const vcn_point2D_t *const p2 = p2_ptr;
	return (vcn_utils2D_get_dist(p1->x, p2->x) < NB_GEOMETRIC_TOL_POW2) ?
	  0 : 1;
}