#ifndef __NB_GRAPHICS_BOT_DRAWING_UTILS_H__
#define __NB_GRAPHICS_BOT_DRAWING_UTILS_H__

#include <stdint.h>
#include "nb/graphics_bot/drawing_tools.h"

static double _NB_RGB_BLACK[4] = {0.0, 0.0, 0.0};
static double _NB_RGB_RED[3] = {0.0, 0.0, 1.0};
static double _NB_RGB_BLUE[3] = {0.0, 0.0, 1.0};

typedef enum {
	NB_RAINBOW,
	NB_SUNSET,
	NB_FRENCH
} nb_palette_preset;

typedef struct nb_palette_s vcn_palette_t;/* DEPRECATED (TEMPORAL) */
typedef struct nb_palette_s nb_palette_t;

void nb_graphics_cam_fit_box(camera_t *cam, const double box[4],
			     double width, double height);

vcn_palette_t* vcn_palette_create();
vcn_palette_t* vcn_palette_create_preset(nb_palette_preset preset);
void vcn_palette_destroy(vcn_palette_t* palette);
void vcn_palette_clear(vcn_palette_t* palette);
void vcn_palette_add_colour(vcn_palette_t* palette, float tic,
			    uint8_t r, uint8_t g, uint8_t b);
void vcn_palette_get_colour(const vcn_palette_t *const palette,
			    float factor,
			    uint8_t rgb[3]);

void nb_graphics_draw_palette(nb_graphics_context_t *g, 
			      const vcn_palette_t *const palette,
			      float x, float y, float w, float h,
			      float border, double min_v, double max_v);

#endif
