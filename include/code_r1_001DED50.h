#ifndef CODE_R1_001_DED50_H
#define CODE_R1_001_DED50_H

#include "common.h"

/* Native billboard UVs are four ordered U/V corners, not a float vector. */
typedef struct BillTextureQuad {
    u16 components[8];
} BillTextureQuad;

typedef char BillTextureQuad_size_must_be_0x10[
    (sizeof(BillTextureQuad) == 0x10) ? 1 : -1];

#endif
