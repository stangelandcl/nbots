#ifndef __VCN_GEOMETRIC_BOT_MODEL_EXPORTER_CAIRO_H__
#define __VCN_GEOMETRIC_BOT_MODEL_EXPORTER_CAIRO_H__

#include <stdint.h>
#include <stdbool.h>
#include "vcn/geometric_bot.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @brief Export a PNG image of the model.
   * @param[in] model Model to be displayed in the image.
   * @param[in] filename Name of the PNG file.
   * @param[in] width Width of the image.
   * @param[in] height of the image.
   * @param[in] include_numbering Enable or Disable the ids of vertices and
   * segments.
   */
  void vcn_model_export_png(const vcn_model_t *const model,
			    const char* filename,
			    int width, int height,
			    bool include_numbering);

  /**
   * @brief Export an EPS image of the model.
   * @param[in] model Model to be displayed in the image.
   * @param[in] filename Name of the EPS file.
   * @param[in] width Width of the image.
   * @param[in] height of the image.
   * @param[in] include_numbering Enable or Disable the ids of vertices and
   * segments.
   */
  void vcn_model_export_eps(const vcn_model_t *const model,
			    const char* filename,
			    int width, int height,
			    bool include_numbering);

#ifdef __cplusplus
}
#endif

#endif
