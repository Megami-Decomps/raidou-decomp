#ifndef DDS_SDF_MAT4_H

#define DDS_SDF_MAT4_H
#include "common.h"

typedef struct SdfMat4 {
    f32 m[16];
} SdfMat4;

typedef char SdfMat4SizeCheck[(sizeof(SdfMat4) == 0x40) ? 1 : -1];

#endif /* DDS_SDF_MAT4_H */
