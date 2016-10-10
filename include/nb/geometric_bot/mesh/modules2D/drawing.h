#ifndef __NB_GEOMETRIC_BOT_MESH_MODULES2D_DRAWING_H__
#define __NB_GEOMETRIC_BOT_MESH_MODULES2D_DRAWING_H__

#include <stdint.h>
#include <stdbool.h>
#include "nb/geometric_bot/mesh/partition.h"

void nb_mesh_draw(const nb_mesh_t *mesh, const char* filename,
		   int width, int height);

void nb_dewall_draw(const nb_mesh_t *mesh, const char* filename,
		     int width, int height, uint8_t axe, double alpha,
		     uint32_t N, void *vtx_array);

#endif
