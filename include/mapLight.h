#ifndef MAPLIGHT_H
#define MAPLIGHT_H

#include "common.h"
#include "sdf_types.h"

/* Map lights as mapGetLightParam fills them (0x70 bytes); HD appends bloom settings. */
typedef struct mapLightParam_t {
    sdfVector4_t dir0;
    f32 col0[3];
    u32 rgb0;
    sdfVector4_t dir1;
    f32 col1[3];
    u32 rgb1;
    sdfVector4_t dir2;
    f32 col2[3];
    u32 rgb2;
    f32 cola[3];
    u32 rgba;
} mapLightParam_t;

void mapGetLightParam(mapLightParam_t *param, f32 x, f32 y, f32 z);

#endif
