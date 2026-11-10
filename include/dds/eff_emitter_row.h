#ifndef DDS_EFF_EMITTER_ROW_H

#define DDS_EFF_EMITTER_ROW_H
#include "common.h"

typedef struct EffEmitterRow {
    s32 age;    /* 0x00 */
    f32 speed;  /* 0x04 */
    f32 angle;  /* 0x08 */
    f32 radius; /* 0x0C */
    f32 offset; /* 0x10 */
    f32 width;  /* 0x14 */
} EffEmitterRow;

typedef char EffEmitterRowSizeCheck[(sizeof(EffEmitterRow) == 0x18) ? 1 : -1];

#endif
