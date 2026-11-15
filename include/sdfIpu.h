#ifndef SDFIPU_H
#define SDFIPU_H

#include "common.h"
#include "sdf_movie_types.h"
#include "sdf_resource.h"

void sdfIpuFree(sdfIpuDecodeCb_t *cb);
void sdfIpuInit(void);
void sdfIpuSetOutputTex(sdfIpuDecodeCb_t *node, struct sdfIpuInitParm_t *params, u8 *sourceData, s32 sourceSize, sdfTexHandle_t *source);

#endif
