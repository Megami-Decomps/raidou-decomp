#ifndef DDS_EVT_BLEND_VEC_COLOR_H

#define DDS_EVT_BLEND_VEC_COLOR_H
#include "common.h"

typedef struct EvtBlendVecColor {
    f32 vec[4];
    u32 color;
} EvtBlendVecColor;

typedef char EvtBlendVecColorSizeCheck[(sizeof(EvtBlendVecColor) == 0x14) ? 1 : -1];

#endif
