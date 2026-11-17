#ifndef FLDDBG_H
#define FLDDBG_H

#include "common.h"

/* View of the debug state at fldDbg through the last recovered field. */
typedef struct fldDbg_t {
    s32 menuOn;         /* 0x00 */
    s32 menuCur;        /* 0x04 */
    s32 infoOn;         /* 0x08 */
    s32 hitViewOn;      /* 0x0C: nonzero tints the pac map sprites */
    s32 hitOn;          /* 0x10 */
    s32 camOn;          /* 0x14 */
    s32 bgDrawOn;       /* 0x18: set by mapBgDrawOnOff; nonzero runs the sky BG drawing */
    s32 bgCngOn;        /* 0x1C */
    s32 skyCngOn;       /* 0x20 */
    s32 ctnOn;          /* 0x24 */
    s32 mizuEffOn;      /* 0x28 */
    s32 encountOn;      /* 0x2C */
    s32 encountBtnOn;   /* 0x30 */
    s32 eveHitOn;       /* 0x34 */
    s32 zoomTestOn;     /* 0x38 */
    s32 sousaAllMSKOn;  /* 0x3C */
    s32 tmyDrawType;    /* 0x40 */
    s32 moonSpeed;      /* 0x44 */
    f32 skyBgSize;      /* 0x48: copy of mapFilter_t.skyScale while SKY BG EDIT runs */
    f32 skyBgY;         /* 0x4C */
    s32 word50;         /* 0x50: raidou2 only, cleared by fldNewGameStartSub */
} fldDbg_t;

extern fldDbg_t fldDbg;

#endif /* FLDDBG_H */
