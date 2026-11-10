#ifndef DDS_ITF_TEXT_VECTOR_H

#define DDS_ITF_TEXT_VECTOR_H
#include "common.h"

/* Four signed fixed-point components used by interface quad colors. */
typedef struct TextVector {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} TextVector;

typedef char TextVector_size_must_be_0x10[
    sizeof(TextVector) == 0x10 ? 1 : -1];

#endif
