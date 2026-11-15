#ifndef MAPFILTER_H
#define MAPFILTER_H

#include "common.h"

typedef struct mapFilter_t {
    s16 idx;
    s16 alphaOfs;
    s16 anime;
    s16 mode;
    s32 fogFar;
    s16 fogNear;
    s16 fogMin;
    s16 fogMax;
    s16 colorR;
    s16 colorG;
    s16 colorB;
    f32 skyScale;   /* mapSKYBGEDIT: horizontal scroll divisor */
    f32 skyOffset;  /* mapSKYBGEDIT: vertical offset in screen heights */
} mapFilter_t;

typedef char mapFilter_t_size_must_be_0x20[(sizeof(mapFilter_t) == 0x20) ? 1 : -1];

extern mapFilter_t mapData33[64];

#endif /* MAPFILTER_H */
