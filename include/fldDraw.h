#ifndef FLDDRAW_H
#define FLDDRAW_H

#include "common.h"

/* One sdf2DAllocSprite primitive: colour, then two corners. */
typedef struct fldSpriteCorner_t {
    s32 u;
    s32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} fldSpriteCorner_t;

typedef struct fldSpriteVertex_t {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    fldSpriteCorner_t corner[2];
} fldSpriteVertex_t;

void fldDrawFrameSprite(void);
void fldDrwZFree(void);
void fldPoint3To2(f32 *x2, f32 *y2, f32 x3, f32 y3, f32 z3);
s32 fldPoint3ToZ(f32 x3, f32 y3, f32 z3);
void rsetOtOfs(s32 otofs);
void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
void rsetAlpha(s32 mode);
void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b, u32 a);

extern s32 rsetFrameMaskOtOffset;
extern s32 nbData4;
extern addr_t mapEffEplInitWork;
extern s32 fldDbgFmtHex;
extern struct sdfMat_t *fldData9;
void nbCallSetOt4(f32 x, f32 y, f32 z, f32 size, s32 alpha);
#ifdef VERSION_RAIDOU2
s32 fldEveHitTaskFldSub(f32 x, f32 z);
void fldCallSetOt(f32 x, f32 y, f32 z, f32 size, s32 alpha);
#endif
void nbCallGetY(f32 x, f32 y, f32 z, f32 size, s32 alpha);

#endif
