#ifndef __NB_GRAPHICS_BOT_DRAWING_TOOLS_ASY_ASY_DRAWING_H__
#define __NB_GRAPHICS_BOT_DRAWING_TOOLS_ASY_ASY_DRAWING_H__

#include <stdbool.h>
#include <stdint.h>

#include "nb/graphics_bot/drawing_tools.h"

void nb_graphics_asy_move_to(void *draw_ptr, double x, double y);

void nb_graphics_asy_line_to(void *draw_ptr, double x, double y);

void nb_graphics_asy_arc(void *draw_ptr,
			 double x, double y, double r,
			 double a0, double a1);

void nb_graphics_asy_set_circle(void *draw_ptr,
				double x, double y, double r);

void nb_graphics_asy_set_rectangle(void *draw_ptr, double x1, double y1,
				   double x2, double y2);

void nb_graphics_asy_close_path(void *draw_ptr);

void nb_graphics_asy_set_line_width(void *draw_ptr, double w);

void nb_graphics_asy_set_source_rgb(void *draw_ptr,
				    uint8_t r, uint8_t g, uint8_t b);

void nb_graphics_asy_set_source_rgba(void *draw_ptr, uint8_t r, uint8_t g,
				     uint8_t b, uint8_t a);

void nb_graphics_asy_set_source_grad(void *draw_ptr,
				     nb_graphics_grad_t grad,
				     double x1, double y1,
				     double x2, double y2,
				     nb_palette_t *pat);

void nb_graphics_asy_set_source_trg(void *draw_ptr,
				    double x1, double y1,
				    double x2, double y2,
				    double x3, double y3,
				    uint8_t r1, uint8_t g1, uint8_t b1,
				    uint8_t r2, uint8_t g2, uint8_t b2,
				    uint8_t r3, uint8_t g3, uint8_t b3);

void nb_graphics_asy_fill(void *draw_ptr);

void nb_graphics_asy_fill_preserve(void *draw_ptr);

void nb_graphics_asy_stroke(void *draw_ptr);

void nb_graphics_asy_set_font_type(void *draw_ptr, const char *type);

void nb_graphics_asy_set_font_size(void *draw_ptr, uint16_t size);

void nb_graphics_asy_show_text(void *draw_ptr, const char *str);
void nb_graphics_asy_get_text_attr(void *draw_ptr, const char *label,
				   nb_text_attr_t *attr);

#endif
