#ifndef CODE_002F77E0_H
#define CODE_002F77E0_H

#include "common.h"

/* Partial layout of the msel work. */
typedef struct mselWork_t {
    u8 unk00[0x14];
    s32 cursor;
    u8 unk18[0xE8 - 0x18];
    u32 color[4];
} mselWork_t;

extern mselWork_t *gGlobal;

#endif
