#ifndef FLDDBG_H
#define FLDDBG_H

#include "common.h"

/* View of the debug state at fldDbg through the last recovered field. */
typedef struct fldDbg_t {
    u8 unk00[0x0C];
    s32 spriteTint;     /* 0x0C: nonzero tints the pac map sprites */
    u8 unk10[8];
    s32 skyEnable;      /* 0x18: nonzero runs the sky BG drawing */
    u8 unk1C[0x2C];
    f32 skyScale;       /* 0x48: copy of mapFilter_t.skyScale while SKY BG EDIT runs */
    f32 skyOffset;      /* 0x4C */
} fldDbg_t;

extern fldDbg_t fldDbg;

#endif /* FLDDBG_H */
