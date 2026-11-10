/* Declarations of smt3-decomp src/smt3/effect/effSpline.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFSPLINE_H
#define SHARED_EFFECT_EFFSPLINE_H

#include "common.h"

#include "sdfFrame.h"

#include "newbattle.h"

#include "kernel.h"

#include "ps2asm.h"

#include "nb.h"

#include "model.h"

#include "motion.h"

/* Cubic spline fitted over a point list; dds3AllocSplineHandle takes 0x2C. */
typedef struct dds3Spline_t {
    void *handle;
    void *coefHandle;
    s32 count;
    s16 single;
    s16 pad0E;
    void *points;
    f32 *coefs;
    f32 *coefB;
    f32 *coefC;
    s32 unk20;
    f32 unk24;
    f32 unk28;
} dds3Spline_t;

/* Walks a point list at a fixed step; dds3_1B1238 takes 0x18. */
typedef struct dds3SplineWalk_t {
    void *handle;
    s32 count;
    f32 *points;
    s32 index;
    f32 t;
    f32 step;
} dds3SplineWalk_t;

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

void dds3AllocSplinePoints(dds3Spline_t *spline, s32 flattenEqualComponents);

void dds3BuildSplinePoints(f32 *tangents, f32 *points, u32 count);

void dds3BuildSplineCoefs(dds3Spline_t *spline, f32 *tangents);

void SolveTridiagonal(f32 *solution, f32 *rhs, s32 count);

void dds3BuildSplineCoefsSkipDup(dds3Spline_t *spline, f32 *tangents);

void func_001B1370(sdfVector4_t *vertex, dds3SplineWalk_t *channel, s32 index, f32 t);

void func_001B0A60(sdfVector4_t *vertex, dds3Spline_t *primitive, s32 recordIndex, f32 t);

dds3Spline_t *dds3AllocSplineHandle(f32 *points, u32 count, s32 flattenEqualComponents);

void dds3FreeSplineHandle(dds3Spline_t *pSpline);

s32 dds3_1B09C0(sdfVector4_t *pVertex, dds3Spline_t *pSpline);

void func_001B0A60(sdfVector4_t *vertex, dds3Spline_t *primitive, s32 recordIndex, f32 t);

/* vu0 routine: sample the curve into vf10. */
void func_001B0B90(dds3Spline_t *primitive, s32 recordIndex, f32 t);

void func_001B0CC0(dds3Spline_t *pSpline);

void func_001B0CD0(dds3Spline_t *pSpline, f32 value);

void dds3AllocSplinePoints(dds3Spline_t *primitive, s32 flattenEqualComponents);

void dds3BuildSplinePoints(f32 *tangents, f32 *points, u32 count);

void SolveTridiagonal(f32 *solution, f32 *rhs, s32 count);

void dds3BuildSplineCoefs(dds3Spline_t *primitive, f32 *tangents);

void dds3BuildSplineCoefsSkipDup(dds3Spline_t *primitive, f32 *tangents);

void *dds3_1B1238(void *rows, u32 count);

s32 dds3_1B12B0(dds3SplineWalk_t *pWalk);

s32 dds3_1B12D0(void *vertex, dds3SplineWalk_t *channel);

void func_001B1370(sdfVector4_t *vertex, dds3SplineWalk_t *channel, s32 recordIndex, f32 t);

void func_001B1478(dds3SplineWalk_t *pWalk);

void func_001B1488(dds3SplineWalk_t *pWalk, f32 step);

#endif
