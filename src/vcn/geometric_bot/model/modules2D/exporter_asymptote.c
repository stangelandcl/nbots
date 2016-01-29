#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "vcn/container_bot/container.h"
#include "vcn/geometric_bot/utils2D.h"

#include "../model2D_struct.h"
#include "vcn/geometric_bot/model/model2D.h"
#include "vcn/geometric_bot/model/modules2D/simplifier.h"
#include "vcn/geometric_bot/model/modules2D/exporter_asymptote.h"

#define GET_PVTX(model, i) (&((model)->vertex[(i)*2]))
#define GET_1_EDGE_VTX(model, i) ((model)->edge[(i) * 2])
#define GET_2_EDGE_VTX(model, i) ((model)->edge[(i)*2+1])

static void write_in_file(FILE *fp, const vcn_model_t *const model,
			  vcn_container_t *wires);


void vcn_model_export_to_asymptote(const vcn_model_t *const model,
				   const char* filename)
{
	
	vcn_container_t* wires = vcn_model_generate_wires(model);
	
	FILE* fp = fopen(filename, "w");
	if (NULL != fp) {
		write_in_file(fp, model, wires);
		fclose(fp);
	} else {
		printf("ERROR: The file %s could not be created.\n", filename);
	}
	vcn_container_destroy(wires);
}

static void write_in_file(FILE *fp, const vcn_model_t *const model,
			  vcn_container_t *wires)
{
	fprintf(fp, "path[] modelContour()\n{\n");
	fprintf(fp, "  return\n");
	while (vcn_container_is_not_empty(wires)) {
		vcn_container_t* wire = vcn_container_delete_first(wires);
		while (vcn_container_is_not_empty(wire)) {
			uint32_t* edge = vcn_container_delete_first(wire);
			double* v1 = GET_PVTX(model, edge[0]);
			double* v2 = GET_PVTX(model, edge[1]);
			fprintf(fp, "    (%lf, %lf)--(%lf, %lf)^^\n",
				v1[0], v1[1], v2[0], v2[1]);
		}
		vcn_container_destroy(wire);
	}
	fprintf(fp, "}\n");
}
