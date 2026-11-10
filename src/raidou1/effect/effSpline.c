#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effSpline.c */
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

/* Walks a point list at a fixed step; effCreateChannel takes 0x18. */
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

void effSampleChannelBezier(sdfVector4_t *vertex, dds3SplineWalk_t *channel, s32 index, f32 t);

void effSplCalc(sdfVector4_t *vertex, dds3Spline_t *primitive, s32 recordIndex, f32 t);

dds3Spline_t *dds3AllocSplineHandle(f32 *points, u32 count, s32 flattenEqualComponents);

void dds3FreeSplineHandle(dds3Spline_t *pSpline);

s32 effSplRun(sdfVector4_t *pVertex, dds3Spline_t *pSpline);

void effSplCalc(sdfVector4_t *vertex, dds3Spline_t *primitive, s32 recordIndex, f32 t);

/* vu0 routine: sample the curve into vf10. */
void effSplCalcRegist(dds3Spline_t *primitive, s32 recordIndex, f32 t);

void effSplRewind(dds3Spline_t *pSpline);

void effSplSetStep(dds3Spline_t *pSpline, f32 value);

void dds3AllocSplinePoints(dds3Spline_t *primitive, s32 flattenEqualComponents);

void dds3BuildSplinePoints(f32 *tangents, f32 *points, u32 count);

void SolveTridiagonal(f32 *solution, f32 *rhs, s32 count);

void dds3BuildSplineCoefs(dds3Spline_t *primitive, f32 *tangents);

void dds3BuildSplineCoefsSkipDup(dds3Spline_t *primitive, f32 *tangents);

void *effCreateChannel(void *rows, u32 count);

s32 effReleaseInterpolationChannel(dds3SplineWalk_t *pWalk);

s32 effAdvanceChanCursor(void *vertex, dds3SplineWalk_t *channel);

void effSampleChannelBezier(sdfVector4_t *vertex, dds3SplineWalk_t *channel, s32 recordIndex, f32 t);

void effClearChanCursor(dds3SplineWalk_t *pWalk);

void effSetChanStep(dds3SplineWalk_t *pWalk, f32 step);

extern u8 nbCameraStartReadWaitActive;

extern u32 nbCameraStartReadWaitFrames;

extern u8 nbCameraStartAnimWaitActive;

extern u32 nbCameraStartAnimWaitFrames;

extern s32 nbCameraStartBAFrame;

extern f32 nbCameraEditDist;

extern f32 nbCameraEditUp;

extern s8 nbCameraCRSetPlayerFormRequestFlag;

extern s32 nbCameraState_CommandRoot_OrderIndex;

extern s8 nbCameraPressRunFlag;

extern u32 nbCameraPressThrough;

extern s32 nbCameraBossCutFlag;

extern f32 nbCameraBossVecTable[1];

extern s32 nbCameraBoss_BSARI_MASEHA_Frame;

extern s32 nbCameraBoss_BSARI_MASEHA_Type;

extern s32 nbCameraBoss_BSARI_SIMANO_Type;

extern f32 nbCameraBoss_BSARI_SIMANO_Rate;

extern u32 nbCameraBoss_Kagu_MJINKO_Type;

extern u8 nbCameraBoss_Pish_MUHENK_Type;

extern s32 nbCameraBoss_Pish_MUHENK_Frame;

extern u32 nbCameraBoss_BSBERU_E915_Frame;

extern f32 nbCameraBoss_BSBERU_E915_Rate;

extern f32 nbCameraBoss_Beru_HAENO1_DirY[1];

extern s32 nbCameraBoss_BSLUSI_Type;

extern f32 D_0054F3D0;

extern f32 D_0054F3D4;

extern char *nbCameraHelperViewerMode;

extern void *nbCameraHelperViewerCurrent;

extern sdfMemHandle_t *nbCameraHelperViewerStateHandle;

extern sdfMemHandle_t *nbCameraHelperViewerTargetHandle;

extern u8 nbCameraHelperViewerShowFrame;

extern u8 nbCameraHelperViewerShowLines;

extern char D_0054F3F0[8];
/* End of declarations from src/smt3/effect/effSpline.c */

dds3Spline_t *dds3AllocSplineHandle(f32 *points, u32 count, s32 flattenEqualComponents) {
    sdfMemHandle_t *primaryResource = sdfMemAllocDirect(0x2C);
    f32 step = 0.05f;
    dds3Spline_t *primitive = (dds3Spline_t *)sdfMemLock(primaryResource);
    sdfMemHandle_t *coefficientResource;
    f32 (*coefficients)[3];

    primitive->handle = primaryResource;
    primitive->count = count;
    primitive->points = points;
    primitive->unk20 = 0;
    primitive->unk24 = 0;
    primitive->unk28 = step;
    if (count < 3) {
        primitive->coefs = NULL;
        primitive->single = 1;
    } else {
        coefficientResource = sdfMemAllocDirect(count * 36);
        coefficients = (f32 (*)[3])sdfMemLock(coefficientResource);
        primitive->handle = primaryResource;
        primitive->coefHandle = coefficientResource;
        primitive->single = 0;
        primitive->count = count;
        primitive->points = points;
        primitive->coefs = (f32 *)coefficients;
        primitive->coefB = (f32 *)(coefficients + count);
        primitive->coefC = (f32 *)(coefficients + count * 2);
        primitive->unk20 = 0;
        primitive->unk24 = 0;
        primitive->unk28 = step;
        dds3AllocSplinePoints(primitive, flattenEqualComponents);
    }
    return primitive;
}

void dds3FreeSplineHandle(dds3Spline_t *pSpline)
{
    if (pSpline == NULL) {
        return;
    }
    if (pSpline->coefs != NULL) {
        sdfMemFreeDirect(pSpline->coefHandle);
    }
    sdfMemFreeDirect(pSpline->handle);
}

INCLUDE_ASM(const s32, "effect/effSpline", func_001F5F38);

INCLUDE_ASM(const s32, "effect/effSpline", func_001F5FD8);

INCLUDE_ASM(const s32, "effect/effSpline", func_001F6108);

INCLUDE_ASM(const s32, "effect/effSpline", func_001F6238);

INCLUDE_ASM(const s32, "effect/effSpline", func_001F6248);

void dds3AllocSplinePoints(dds3Spline_t *primitive, s32 flattenEqualComponents) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(primitive->count * 12);
    f32 *tangentData = (f32 *)sdfMemLock(handle);

    dds3BuildSplinePoints(tangentData, primitive->points, primitive->count);
    if (flattenEqualComponents == 0) {
        dds3BuildSplineCoefs(primitive, tangentData);
    } else {
        dds3BuildSplineCoefsSkipDup(primitive, tangentData);
    }
    sdfMemFreeDirect(handle);
}

void dds3BuildSplinePoints(f32 *tangents, f32 *points, u32 count) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(count * sizeof(f32));
    f32 *rhs = (f32 *)sdfMemLock(handle);
    f32 (*pts)[3] = (f32 (*)[3])points;
    s32 component;
    s32 i;

    for (component = 0; component < 3; component++) {
        rhs[0] = (pts[1][component] - pts[0][component]) * 3.0f;
        rhs[count - 1] = (pts[count - 1][component] - pts[count - 2][component]) * 3.0f;
        for (i = 1; i <= (s32)count - 2; i++) {
            rhs[i] = ((pts[i][component] - pts[i - 1][component]) +
                      (pts[i + 1][component] - pts[i][component])) * 3.0f;
        }
        SolveTridiagonal(tangents, rhs, count);
        tangents += count;
    }
    sdfMemFreeDirect(handle);
}

void SolveTridiagonal(f32 *solution, f32 *rhs, s32 count) {
    s32 byteCount = count * sizeof(f32);
    void *handle = sdfMemAllocDirect(byteCount);
    f32 *upper;
    s32 i;
    f32 denominator;
    f32 *upperEnd;
    f32 *solutionEnd;
    f32 *rhsEnd;

    count -= 2;
    upper = (f32 *)sdfMemLock(handle);
    denominator = 2.0f;

    solution[0] = rhs[0] / denominator;
    for (i = 1; i <= count; i++) {
        upper[i] = 1.0f / denominator;
        denominator = 4.0f - upper[i];
        solution[i] = (rhs[i] - solution[i - 1]) / denominator;
    }
    upperEnd = (f32 *)((u8 *)upper + byteCount);
    solutionEnd = (f32 *)((u8 *)solution + byteCount);
    rhsEnd = (f32 *)((u8 *)rhs + byteCount);
    upperEnd[-1] = 1.0f / denominator;
    denominator = 2.0f - upperEnd[-1];
    solutionEnd[-1] = (rhsEnd[-1] - solutionEnd[-2]) / denominator;
    for (i = count; i >= 0; i--) {
        solution[i] -= upper[i + 1] * solution[i + 1];
    }
    sdfMemFreeDirect(handle);
}

void dds3BuildSplineCoefs(dds3Spline_t *primitive, f32 *tangents) {
    f32 *cubic = primitive->coefs;
    f32 *quadratic = primitive->coefB;
    f32 *linear = primitive->coefC;
    f32 *points = primitive->points;
    s32 count = primitive->count;
    s32 segment;
    for (segment = 0; segment < count - 1; segment++) {
        s32 component;
        for (component = 0; component < 3; component++) {
            f32 endValue = points[segment * 3 + component + 3];
            f32 startValue = points[segment * 3 + component];
            f32 startTangent = tangents[component * count + segment];
            f32 endTangent = tangents[component * count + segment + 1];
            cubic[segment * 3 + component] = 2.0f * (startValue - endValue) +
                (startTangent + endTangent);
            quadratic[segment * 3 + component] = (endValue - startValue) * 3.0f -
                (2.0f * startTangent + endTangent);
            linear[segment * 3 + component] = startTangent;
        }
    }
}

void dds3BuildSplineCoefsSkipDup(dds3Spline_t *primitive, f32 *tangents) {
    f32 *cubic = primitive->coefs;
    f32 *quadratic = primitive->coefB;
    f32 *linear = primitive->coefC;
    f32 *points = primitive->points;
    s32 count = primitive->count;
    s32 segment;
    s32 component;

    for (segment = 0; segment < count - 1; segment++) {
        for (component = 0; component < 3; component++) {
            f32 startValue = points[segment * 3 + component];
            f32 endValue = points[segment * 3 + component + 3];

            if (startValue == endValue) {
                cubic[segment * 3 + component] = 0.0f;
                quadratic[segment * 3 + component] = 0.0f;
                linear[segment * 3 + component] = 0.0f;
            } else {
                f32 startTangent = tangents[component * count + segment];
                f32 endTangent = tangents[component * count + segment + 1];

                cubic[segment * 3 + component] = 2.0f * (startValue - endValue) + (startTangent + endTangent);
                quadratic[segment * 3 + component] = (endValue - startValue) * 3.0f - (2.0f * startTangent + endTangent);
                linear[segment * 3 + component] = startTangent;
            }
        }
    }
}
