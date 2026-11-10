#ifndef DDS_SDF_MODEL_PACKET_CALLBACK_H

#define DDS_SDF_MODEL_PACKET_CALLBACK_H
#include "sdf_draw.h"

struct sdfGrOt_t;

static inline void sdfBuildModelPacketsVia(
        void (*build)(struct sdfGrOt_t **, dds_sdfModelRootNode_t *),
        struct sdfGrOt_t **surfaces, dds_sdfModelRootNode_t *model) {
    build(surfaces, model);
}

#endif
