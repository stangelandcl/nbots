#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <alloca.h>
#include <math.h>

#include "nb/image_bot.h"
#include "nb/graphics_bot/drawing_tools.h"

#include "../../palette_struct.h"

#include "rasterizer.h"
#include "font_rasterizer.h"

#include "pix_drawing.h"

#define TURTLE_STATIC_MEMSIZE 10       /* turtle_step */
#define TURTLE_DYNAMIC_MEMINCREASE 15  /* turtle_step */
#define PIXMASK_STACK_MEMSIZE 250      /* Bytes */

#define ANTIALIASING true

#define POW2(a) ((a)*(a))

enum {
	SOLID, GRAD, TRG
};

enum {
	MOVE_TO, LINE_TO, QCURVE_TO,
	QRCURVE_TO, CURVE_TO, CLOSE_PATH
};

typedef struct {
	uint8_t type;
	float x, y;
	float data[4];
} turtle_step;

typedef struct {
	uint16_t N;
	uint16_t N_alloc;
	turtle_step static_mem[TURTLE_STATIC_MEMSIZE];
	turtle_step *dynamic_mem;
} turtle_struct;

typedef struct {
	uint8_t source_type:4;
	nb_graphics_grad_t grad_type:4;
	float vtx[6];
	nb_graphics_palette_t *pal;
	nb_graphics_palette_t color;
} source_t;

typedef struct {
	const char *type;
	uint16_t size;	
} font_t;

typedef struct {
	vcn_image_t *img;
	turtle_struct *turtle;
	float line_width;
	source_t *source;
	font_t *font;
} context_t;

static void turtle_clear(turtle_struct *turtle);
static void turtle_add(turtle_struct *turtle, uint8_t type,
		       float x, float y, float v1, float v2,
		       float v3, float v4);
static turtle_step *turtle_ref_step(turtle_struct *turtle, uint16_t i);
static void turtle_reset(turtle_struct *turtle);
static void set_pixel(int x, int y, uint8_t i, void* context);
static void draw_line(context_t *c, int x0, int y0, int x1, int y1);
static void draw_qcurve(context_t *c, int x0, int y0, int x1, int y1,
			int cx, int cy);
static void draw_qrcurve(context_t *c, int x0, int y0, int x1, int y1,
			 int cx, int cy, float w);
static void draw_curve(context_t *c, int x0, int y0, int x1, int y1,
		       float c0x, float c0y, float c1x, float c1y);
static void source_set_rgba(source_t *source, uint8_t r,
			    uint8_t g, uint8_t b, uint8_t a);
static void source_set_grad(source_t *source,
			    nb_graphics_grad_t grad,
			    float x1, float y1,
			    float x2, float y2,
			    nb_graphics_palette_t *pal);
static void source_set_trg(source_t *source,
			   float x1, float y1,
			   float x2, float y2,
			   float x3, float y3,
			   const uint8_t rgba1[4],
			   const uint8_t rgba2[4],
			   const uint8_t rgba3[4]);
static void source_get_color(source_t *source, int x, int y, uint8_t pix[4]);
static void source_get_color_solid(source_t *source, uint8_t pix[4]);
static void source_get_color_grad(source_t *source, int x, int y,
				  uint8_t pix[4]);
static void source_get_color_grad_linear(source_t *source, int x, int y,
					 uint8_t pix[4]);
static void source_get_color_grad_radial(source_t *source, int x, int y,
					 uint8_t pix[4]);
static void source_get_color_trg(source_t *source, int x, int y,
				 uint8_t pix[4]);
static void get_barycentric_coordinates(float x1, float y1, float x2, float y2,
					float x3, float y3, float x, float y,
					float lambda[3]);

void* nb_graphics_pix_create_context(int width, int height)
{
	uint32_t ctx_size = sizeof(context_t);
	uint32_t img_size = sizeof(vcn_image_t);
	uint32_t pix_size = 4 * width * height;
	uint32_t trt_size = sizeof(turtle_struct);
	uint16_t src_size = sizeof(source_t);
	uint16_t fnt_size = sizeof(font_t);
	uint32_t memsize = ctx_size + img_size + pix_size +
		trt_size + src_size + fnt_size;
	char *memblock = malloc(memsize);
	context_t *ctx = (void*) memblock;

	ctx->img = (void*) (memblock + ctx_size);
	vcn_image_init(ctx->img);
	ctx->img->width = width;
	ctx->img->height = height;
	ctx->img->comp_x_pixel = 4;
	ctx->img->pixels = (void*) (memblock + ctx_size + img_size);
	memset(ctx->img->pixels, 0, pix_size);

	ctx->turtle = (void*) (memblock + ctx_size + img_size + pix_size);
	memset(ctx->turtle, 0, trt_size);

	ctx->line_width = 1.0;
	
	ctx->source = (void*) (memblock + ctx_size + img_size +
			       pix_size + trt_size);
	source_set_rgba(ctx->source, 0, 0, 0, 255);
	
	ctx->font = (void*) (memblock + ctx_size + img_size +
			       pix_size + trt_size + src_size);
	ctx->font->type = "Mono";
	ctx->font->size = 18;

	return ctx;
}

void nb_graphics_pix_destroy_context(void *ctx)
{
	context_t *c = ctx;
	turtle_clear(c->turtle);
	free(ctx);
}

static void turtle_clear(turtle_struct *turtle)
{
	if (turtle->N_alloc > 0)
		free(turtle->dynamic_mem);
	memset(turtle, 0, sizeof(turtle_struct));
}

void nb_graphics_pix_export_context(const void *ctx, const char *filename)
{
	const context_t *c = ctx;
	vcn_image_write(c->img, filename);
}

void nb_graphics_pix_move_to(void *ctx, float x, float y)
{
	context_t *c = ctx;
	turtle_add(c->turtle, MOVE_TO, x, y,
		   0, 0, 0, 0);
}

static void turtle_add(turtle_struct *turtle, uint8_t type,
		       float x, float y, float v1, float v2,
		       float v3, float v4)
{
	if (turtle->N < TURTLE_STATIC_MEMSIZE) {
		turtle->static_mem[turtle->N].type = type;
		turtle->static_mem[turtle->N].x = x;
		turtle->static_mem[turtle->N].y = y;
		turtle->static_mem[turtle->N].data[0] = v1;
		turtle->static_mem[turtle->N].data[1] = v2;
		turtle->static_mem[turtle->N].data[2] = v3;
		turtle->static_mem[turtle->N].data[3] = v4;
		turtle->N += 1;
	} else {
		uint16_t N = turtle->N - TURTLE_STATIC_MEMSIZE;
		if (N >= turtle->N_alloc) {
			turtle->N_alloc += TURTLE_DYNAMIC_MEMINCREASE;
			uint16_t memsize =
				sizeof(turtle_step) * turtle->N_alloc;
			turtle->dynamic_mem = realloc(turtle->dynamic_mem,
						      memsize);
		}
		turtle->dynamic_mem[N].type = type;
		turtle->dynamic_mem[N].x = x;
		turtle->dynamic_mem[N].y = y;
		turtle->dynamic_mem[N].data[0] = v1;
		turtle->dynamic_mem[N].data[1] = v2;
		turtle->dynamic_mem[N].data[2] = v3;
		turtle->dynamic_mem[N].data[3] = v4;
		turtle->N += 1;
	}
}

void nb_graphics_pix_line_to(void *ctx, float x, float y)
{
	context_t *c = ctx;
	turtle_add(c->turtle, LINE_TO, x, y,
		   0, 0, 0, 0);
}

void nb_graphics_pix_qcurve_to(void *ctx, float x, float y,
			       float xcontrol, float ycontrol)
{
	context_t *c = ctx;
	turtle_add(c->turtle, QCURVE_TO, x, y,
		   xcontrol, ycontrol, 0, 0);
}

void nb_graphics_pix_qrcurve_to(void *ctx, float x, float y,
				float xcontrol, float ycontrol, float w)
{
	context_t *c = ctx;
	turtle_add(c->turtle, QRCURVE_TO, x, y,
		   xcontrol, ycontrol, w, 0);
}
void nb_graphics_pix_curve_to(void *ctx, float x, float y,
			      float x0_control, float y0_control,
			      float x1_control, float y1_control)
{
	context_t *c = ctx;
	turtle_add(c->turtle, CURVE_TO, x, y,
		   x0_control, y0_control,
		   x1_control, y1_control);
}

void nb_graphics_pix_close_path(void *ctx)
{
	context_t *c = ctx;
	turtle_add(c->turtle, CLOSE_PATH,
		   0, 0, 0, 0, 0, 0);
}

void nb_graphics_pix_set_line_width(void *ctx, float w)
{
	context_t *c = ctx;
	c->line_width = w;
}

void nb_graphics_pix_set_source_rgb(void *ctx,
				    uint8_t r, uint8_t g, uint8_t b)
{
	context_t *c = ctx;
	source_set_rgba(c->source, r, g, b, 255);
}

void nb_graphics_pix_set_source_rgba(void *ctx, uint8_t r, uint8_t g,
				     uint8_t b, uint8_t a)
{
	context_t *c = ctx;
	source_set_rgba(c->source, r, g, b, a);
}

static void source_set_rgba(source_t *source, uint8_t r,
			    uint8_t g, uint8_t b, uint8_t a)
{
	memset(source, 0, sizeof(source_t));
	source->source_type = SOLID;
	source->pal = &(source->color);
	nb_graphics_palette_add_rgba(source->pal, 0.0,
				     r, g, b, a);
}
void nb_graphics_pix_set_source_grad(void *ctx,
				     nb_graphics_grad_t grad,
				     float x1, float y1,
				     float x2, float y2,
				     nb_graphics_palette_t *pal)
{
	context_t *c = ctx;
	source_set_grad(c->source, grad, x1, y1, x2, y2, pal);
}

static void source_set_grad(source_t *source,
			    nb_graphics_grad_t grad,
			    float x1, float y1,
			    float x2, float y2,
			    nb_graphics_palette_t *pal)
{
	memset(source, 0, sizeof(source_t));
	source->source_type = GRAD;
	source->grad_type = grad;
	source->pal = pal;
	source->vtx[0] = x1;
	source->vtx[1] = y1;
	source->vtx[2] = x2 - x1;
	source->vtx[3] = y2 - y1;
	source->vtx[4] = sqrt(POW2(source->vtx[2]) + 
			      POW2(source->vtx[3]));
	source->vtx[2] /= source->vtx[4];
	source->vtx[3] /= source->vtx[4];
}

void nb_graphics_pix_set_source_trg(void *ctx,
				    float x1, float y1,
				    float x2, float y2,
				    float x3, float y3,
				    const uint8_t rgba1[4],
				    const uint8_t rgba2[4],
				    const uint8_t rgba3[4])
{
	context_t *c = ctx;
	source_set_trg(c->source, x1, y1, x2, y2, x3, y3,
		       rgba1, rgba2, rgba3);
}

static void source_set_trg(source_t *source,
			   float x1, float y1,
			   float x2, float y2,
			   float x3, float y3,
			   const uint8_t rgba1[4],
			   const uint8_t rgba2[4],
			   const uint8_t rgba3[4])
{
	memset(source, 0, sizeof(source_t));
	source->source_type = TRG;
	source->pal = &(source->color);
	source->vtx[0] = x1;
	source->vtx[1] = y1;
	source->vtx[2] = x2;
	source->vtx[3] = y2;
	source->vtx[4] = x3;
	source->vtx[5] = y3;
	nb_graphics_palette_add_rgba(source->pal, 0.0, rgba1[0],
				     rgba1[1], rgba1[2], rgba1[3]);
	nb_graphics_palette_add_rgba(source->pal, 0.5, rgba2[0],
				     rgba2[1], rgba2[2], rgba2[3]);
	nb_graphics_palette_add_rgba(source->pal, 1.0, rgba3[0],
				     rgba3[1], rgba3[2], rgba3[3]);
}

void nb_graphics_pix_fill(void *ctx)
{
	;
	/* PENDING */
}

void nb_graphics_pix_fill_preserve(void *ctx)
{
	;
	/* PENDING */
}

void nb_graphics_pix_stroke(void *ctx)
{
	context_t *c = ctx;
	int x0, y0;
	int x, y;
	for (uint16_t i = 0; i < c->turtle->N; i++) {
		turtle_step *step = turtle_ref_step(c->turtle, i);
		int sx = (int)(step->x + 0.5);
		int sy = (int)(step->y + 0.5);
		switch (step->type) {
		case MOVE_TO:
			x0 = sx;
			y0 = sy;
			break;
		case LINE_TO:
			draw_line(c, x, y, sx, sy);
			break;
		case QCURVE_TO:
			draw_qcurve(c, x, y, sx, sy,
				    (int)(step->data[0] + 0.5),
				    (int)(step->data[1] + 0.5));
			break;
		case QRCURVE_TO:
			draw_qrcurve(c, x, y, sx, sy,
				     (int)(step->data[0] + 0.5),
				     (int)(step->data[1] + 0.5),
				     step->data[2]);
			break;
		case CURVE_TO:
			draw_curve(c, x, y, sx, sy,
				   step->data[0],
				   step->data[1],
				   step->data[2],
				   step->data[3]);
			break;
		case CLOSE_PATH:
			sx = x0;
			sy = y0;
			draw_line(c, x, y, x0, y0);
			break;
		default:
			sx = x0;
			sy = y0;
			draw_line(c, x, y, x0, y0);
		}
		x = sx;
		y = sy;
	}
	turtle_reset(c->turtle);
}

static turtle_step *turtle_ref_step(turtle_struct *turtle, uint16_t i)
{
	void *step;
	if (i < TURTLE_STATIC_MEMSIZE)
		step = &(turtle->static_mem[i]);
	else
		step = &(turtle->dynamic_mem[i - TURTLE_STATIC_MEMSIZE]);
	return step;
}

static void turtle_reset(turtle_struct *turtle)
{
	turtle->N = 0;
}

static void set_pixel(int x, int y, uint8_t i, void* context)
{
	context_t *c = context;
	if (x >= 0 && x < c->img->width &&
	    y >= 0 && y < c->img->height) {
		uint8_t pix[4];
		source_get_color(c->source, x, y, pix);
		pix[3] = (uint8_t)(pix[3] * (i/255.0f));
		vcn_image_blend_pixel_rgba(c->img, y, x, pix);
	}
}


static void source_get_color(source_t *source, int x, int y, 
			     uint8_t pix[4])
{
	switch (source->source_type) {
	case SOLID:
		source_get_color_solid(source, pix);
		break;
	case GRAD:
		source_get_color_grad(source, x, y, pix);
		break;
	case TRG:
		source_get_color_trg(source, x, y, pix);
		break;
	default:
		source_get_color_solid(source, pix);
	}
}

static void source_get_color_solid(source_t *source, uint8_t pix[4])
{
	nb_graphics_palette_get_rgba(source->pal, 0.0, pix);
}

static void source_get_color_grad(source_t *source, int x, int y,
				  uint8_t pix[4])
{
	switch (source->grad_type) {
	case NB_LINEAR:
		source_get_color_grad_linear(source, x, y, pix);
		break;
	case NB_RADIAL:
		source_get_color_grad_radial(source, x, y, pix);
		break;
	default:
		source_get_color_grad_linear(source, x, y, pix);
	}
}

static void source_get_color_grad_linear(source_t *source, int x, int y,
					 uint8_t pix[4])
{
	x = x - source->vtx[0];
	y = y - source->vtx[1];
	float factor = x * source->vtx[2] + y * source->vtx[3];
	factor = factor / source->vtx[4];
	nb_graphics_palette_get_rgba(source->pal, factor, pix);
}

static void source_get_color_grad_radial(source_t *source, int x, int y,
					 uint8_t pix[4])
{
	x = x - source->vtx[0];
	y = y - source->vtx[1];
	float factor = sqrt(POW2(x) + POW2(y));
	factor = factor / source->vtx[4];
	nb_graphics_palette_get_rgba(source->pal, factor, pix);
}

static void source_get_color_trg(source_t *source, int x, int y,
				 uint8_t pix[4])
{
	float lambda[3];
	get_barycentric_coordinates(source->vtx[0], source->vtx[1],
				    source->vtx[2], source->vtx[3],
				    source->vtx[4], source->vtx[5],
				    x, y, lambda);
	uint8_t pix1[4], pix2[4], pix3[4];
	nb_graphics_palette_get_rgba(source->pal, 0.0, pix1);
	nb_graphics_palette_get_rgba(source->pal, 0.5, pix2);
	nb_graphics_palette_get_rgba(source->pal, 1.0, pix3);

	for (int j = 0; j < 4; j++)
		pix[j] = (uint8_t) (lambda[0] * pix1[j] +
				    lambda[1] * pix2[j] +
				    lambda[2] * pix3[j]);
}

static void get_barycentric_coordinates(float x1, float y1, float x2, float y2,
					float x3, float y3, float x, float y,
					float lambda[3])
{
	float detT = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
	float x_x3 = x - x3;
	float y_y3 = y - y3;
	lambda[0] = ((y2 - y3) * x_x3 + (x3 - x2) * y_y3) / detT;
	lambda[1] = ((y3 - y1) * x_x3 + (x1 - x3) * y_y3) / detT;
	lambda[2] = 1.0f - lambda[1] - lambda[0];
}

static void draw_line(context_t *c, int x0, int y0, int x1, int y1)
{
	nb_graphics_rasterizer_line(x0, y0, x1, y1,
				    ANTIALIASING,
				    set_pixel, c);
}

static void draw_qcurve(context_t *c, int x0, int y0, int x1, int y1,
			int cx,	int cy)
{
	nb_graphics_rasterizer_quad_bezier(x0, y0, x1, y1, cx, cy,
					   ANTIALIASING,
					   set_pixel, c);
}

static void draw_qrcurve(context_t *c, int x0, int y0, int x1, int y1,
			 int cx, int cy, float w)
{
	nb_graphics_rasterizer_quad_rational_bezier(x0, y0, x1, y1,
						    cx, cy, w,
						    ANTIALIASING,
						    set_pixel, c);
}

static void draw_curve(context_t *c, int x0, int y0, int x1, int y1,
		       float c0x, float c0y, float c1x, float c1y)
{
	nb_graphics_rasterizer_cubic_bezier(x0, y0, x1, y1,
					    c0x, c0y, c1x, c1y,
					    ANTIALIASING,
					    set_pixel, c);
}

void nb_graphics_pix_set_font_type(void *ctx, const char *type)
{
	context_t *c = ctx;
	c->font->type = type;
}

void nb_graphics_pix_set_font_size(void *ctx, uint16_t size)
{
	context_t *c = ctx;
	c->font->size = size;
}

void nb_graphics_pix_show_text(void *ctx, const char *str)
{
	/* PENDING */;
}

void nb_graphics_pix_get_text_attr(const void *ctx, const char *label,
				   nb_graphics_text_attr_t *attr)
{
	/* PENDING */;
}
