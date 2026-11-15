#ifndef SDFCOREDRAW_H
#define SDFCOREDRAW_H

#include "common.h"
#include "model.h"

void sdfCoreDrawInit(s32 buffSize);
void sdfCoreDrawSetZBias(f32 z);
void sdfCoreDrawSetLight(u8 *light);
void sdfCoreDrawSetFog(u8 *fog);
void *sdfCorePacketGen(sdfModelNode_t *node, void *packetData, u32 attrMask,
                       u32 color, f32 soften, f32 bias, s32 frame, s32 reverse);

#endif
