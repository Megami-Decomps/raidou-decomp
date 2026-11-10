#ifndef DDS_BTL_VEC3_H

#define DDS_BTL_VEC3_H
#include "common.h"

typedef struct BtlVec3 {
    f32 x;
    f32 y;
    f32 z;
} BtlVec3;

typedef char BtlVec3_size[(sizeof(BtlVec3) == 0x0C) ? 1 : -1];

#endif
