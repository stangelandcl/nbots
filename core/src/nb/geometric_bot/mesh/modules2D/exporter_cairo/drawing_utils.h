#ifndef __NB_GEOMETRIC_BOT_MESH_MODULES2D_EXPORTER_CAIRO_DRAWING_UTILS_H__
#define __NB_GEOMETRIC_BOT_MESH_MODULES2D_EXPORTER_CAIRO_DRAWING_UTILS_H__

#define _NB_COLOR_BLACK {0.0, 0.0, 0.0}
#define _NB_COLOR_BLUE {0.0, 0.0, 1.0}
#define _NB_COLOR_SKY_BLUE {0.1, 0.1, 1.0}

typedef struct {
	int width;
	int height;
	double center[2];
	double zoom;
} camera_t;

#define set_center_and_zoom nb_drawing_utils_set_center_and_zoom

void nb_drawing_utils_set_center_and_zoom(camera_t *cam, const double box[4],
					  double width, double height);

#endif
