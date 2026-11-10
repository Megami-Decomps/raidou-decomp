/* Declarations of smt3-decomp src/smt3/effect/effMath.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFMATH_H
#define SHARED_EFFECT_EFFMATH_H

#include "common.h"

#include "sdfFrame.h"

#include "sdf_types.h"

#include "ps2asm.h"

#include "fpu.h"

/* The owner is stored after its contiguous cubic segments. */
typedef struct
{
    f32 controlPoints[4][3];
    f32 t;
    f32 step;
} dds3Bezier4PPart_t;

typedef struct
{
    void *pPart;
    u32 number;
    void *allocation;
} effBezier4P_t;

extern void *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(void *aHandle);

extern void sdfMemFreeDirect(void *aHandle);

#define EFF_RGBA_UNIT 0x3C000000

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct
{
    u32 word;
    u8 pad04[12];
} effColorWork_t;

/* Only the projection block from +0x30 on is read here: a 4x4 matrix, then the
 * viewport half-extent and centre as one quadword each. */
typedef struct
{
    u8 pad00[0x30];
    f32 matrix[16];
    sdfVector4_t scale;
    sdfVector4_t offset;
    u8 pad90[0x10];
} dds3ScreenParam_t;

extern dds3ScreenParam_t dds3ScreenParam;

extern f32 D_002FA240[16];

extern sdfVector4_t aViewEye;

extern sdfVector4_t aViewDown;

extern void MultMatrixRev(f32 *aMatrix);

/* vu0 routine: project the world point in vf10 to screen space */
void effScreenOffset(void);

/* vu0 routine: measure the projected camera-right offset. */
s32 effPCPBlurScreenOffset(f32 scale);

/* vu0 routine: blend two colours, `amount` of `from` */
u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

/* vu0 routine: unpack two colours, multiply and pack */
u32 effMagaCylinderCalcColor(u32 color, u32 scale);

/* vu0 routine: distance from `to` to the line through `from` along `dir` */
f32 effLinePointLength(sdfVector4_t *pDir, sdfVector4_t *pLinePos, sdfVector4_t *pPos);

effBezier4P_t *dds3Bezier4PCreate(s32 number);

void dds3Bezier4PDelete(effBezier4P_t *pEffBezier4P);

s32 dds3Bezier4PPartRun(effBezier4P_t *table, s32 slotIndex, f32 *out);

s32 nbCameraLerpVec(dds3Bezier4PPart_t *slot, f32 *out);

void func_001AC068(effBezier4P_t *table, s32 slotIndex);

dds3Bezier4PPart_t *dds3Bezier4PGetPart(effBezier4P_t *pEffBezier4P, s32 index);

#endif
