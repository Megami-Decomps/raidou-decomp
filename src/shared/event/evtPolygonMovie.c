#include "common.h"
#include "kernel_draw.h"
#include "kernel.h"
#include "libsdf.h"
#include "effect.h"
#include "event.h"
#include "evtPolygonMovie.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "sdk_libc.h"
#include "sdfMemory.h"
#include "fileManager.h"
#include "fld.h"
#include "sdfDelayedResrc.h"
#include "dds3KernelCore.h"
#include "code_r1_001C4B78.h"
#include "evtEventViewer.h"
#include "sdfFile.h"
#include "dds3KernelDraw.h"
#include "sdfChip.h"
#include "itfMesManager.h"
#include "code_r1_001CF808.h"
#include "dds3MoverBasic.h"
#include "dds3PathBasic.h"
#include "dds3UnitObjectBasic.h"
#include "dds3ObjectBase.h"
#include "model.h"

/* 3D screen scale and offset that mapBgSetScrollReset resets. */
extern f32 sdf3DScreenAdjustScaleX;
extern f32 sdf3DScreenAdjustScaleY;
extern f32 sdf3DScreenAdjustOffsetX;
extern f32 sdf3DScreenAdjustOffsetY;

extern const char SetPMFileNameStrEvent03030303E0303Pm1[];
extern const char SetPMFileNameStrEvent03030303E0303Pm2[];
extern const char SetPMFileNameStrEvent03030303E0303Pm3[];
extern const char evtStrZikkiPlayModeEventViewer[];

extern f32 evtSceneLightDefaultColor[4];
extern f32 evtSceneLightDefaultDir[4];
extern f32 evtSceneLightDefaultAmbient[4];

typedef struct fileRequest fileRequest_t;

typedef struct pmdCameraTable_t {
    s32 NameIndex, Reserve1, Reserve2, Reserve3;
} pmdCameraTable_t;

f32 evtHokanFloat(s32 hokan, f32 ratio, f32 f1, f32 f2);
void evtHokanSceneFog(s32 hokan, f32 ratio, evtBlendVecColor_t *key1, evtBlendVecColor_t *key2, evtBlendVecColor_t *d);
void evtHokanBlur2(s32 hokan, f32 ratio, dds3Effect_Blur_t *key1, dds3Effect_Blur_t *key2, dds3Effect_Blur_t *d);
void evtHokanMultBlur(s32 hokan, f32 ratio, dds3Effect_PartialMultiBlurParam_t *key1, dds3Effect_PartialMultiBlurParam_t *key2, dds3Effect_PartialMultiBlurParam_t *d);
void evtHokanDistBlur(s32 hokan, f32 ratio, dds3Effect_DistortionBlurParam_t *key1, dds3Effect_DistortionBlurParam_t *key2, dds3Effect_DistortionBlurParam_t *d);
void evtHokanRipBlur(s32 hokan, f32 ratio, dds3Effect_RippleBlurParam_t *key1, dds3Effect_RippleBlurParam_t *key2, dds3Effect_RippleBlurParam_t *d);
void evtHokanMultFilter(s32 hokan, f32 ratio, dds3Effect_PartialMultiFilterParam_t *key1, dds3Effect_PartialMultiFilterParam_t *key2, dds3Effect_PartialMultiFilterParam_t *d);
void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d);
typedef void (*evtFrameFunc_t)(evtPolygonMovieWork_t *, pmdFrameTable_t *, pmdFrameTable_t *, s32);
u32 evtHokanColor(s32 hokan, f32 ratio, u32 c1, u32 c2);
extern f32 evtHokanFogParam[4];
extern u32 evtSceneFogDefaultColor[4];
extern dds3Effect_Blur_t evtBlur2DefaultParam;
extern dds3Effect_PartialMultiBlurParam_t evtMultBlurDefaultParam;
extern dds3Effect_Filter_t evtFilterDefaultParam;
extern dds3Effect_PartialMultiFilterParam_t evtMultFilterDefaultParam;
extern dds3Effect_DistortionBlurParam_t evtDistBlurDefaultParam;
extern dds3Effect_RippleBlurParam_t evtRipBlurDefaultParam;

typedef struct polyMovieClip_t {
    u8 opaque_00[8];
    f32 duration;
    f32 position;
} polyMovieClip_t;

typedef struct polyMovieState_t {
    u8 opaque_00[4];
    polyMovieClip_t *clip;
} polyMovieState_t;

typedef struct polyMovieObject_t {
    u8 opaque_00[0x18];
    polyMovieState_t *state;
} polyMovieObject_t;

typedef struct evtScaledValue_t {
    u32 word00;
    u32 flags;
    f32 base;
    f32 scaled;
    u8 pad10[8];
    u8 pad18[4];
} evtScaledValue_t;

static inline void UnpackRGBAWordFloat(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK(rgba, unit);
}

static inline u32 PackRGBAWord(f32 unit) {
    u32 rgba[4];
    MMI_RGBA_PACK_UNIT(rgba[0], unit);
    return rgba[0];
}

/* One asm location for every arm's w set, so jump2 can cross-jump them. */
static inline void SetBlendVecW(f32 w)
{
    SDF_VU0_SETVECTOR_W(w);
}

s32 evtHokanInt(s32 hokan, f32 ratio, s32 i1, s32 i2);
u32 evtHokanUInt(s32 hokan, f32 ratio, u32 i1, u32 i2);

#include "include_asm.h"
#include "field.h"
#include "evtEventViewerMenu.h"

void evtCheckLoad(evtPolygonMovieWork_t *wdata) {
    if (wdata->loadFlags & 8) {
        return;
    }
    if (wdata->loadFlags & 2) {
        if (wdata->pm1Request != NULL && pacFileCheckEnd(wdata->pm1Request)) {
            wdata->pm1File = fileGetMemHandle(wdata->pm1Request);
            wdata->pm1Data = sdfMemLock(wdata->pm1File);
            fileHandleFree(wdata->pm1Request);
            wdata->pm1Request = NULL;
            wdata->loadFlags &= ~2;
        }
    } else if (wdata->loadFlags & 4) {
        if (wdata->pm2Request != NULL && pacFileCheckEnd(wdata->pm2Request)) {
            wdata->pm2Mem = fileGetMemHandle(wdata->pm2Request);
            wdata->pm2Data = sdfMemLock(wdata->pm2Mem);
            wdata->pm2Size = wdata->pm2Request->size;
            fileHandleFree(wdata->pm2Request);
            wdata->pm2Request = NULL;
            wdata->loadFlags &= ~4;
        }
    } else if (wdata->loadFlags & 0x10) {
        if (wdata->pm3Request != NULL && pacFileCheckEnd(wdata->pm3Request)) {
            wdata->pm3Mem = fileGetMemHandle(wdata->pm3Request);
            wdata->pm3Data = sdfMemLock(wdata->pm3Mem);
            wdata->pm3Size = wdata->pm3Request->size;
            fileHandleFree(wdata->pm3Request);
            wdata->pm3Request = NULL;
            wdata->loadFlags &= ~0x10;
        }
    } else if (!(wdata->loadFlags & 0x14)) {
        oldEventSceneUnload(wdata);
        GetWorkData1(wdata, wdata->pm1Data);
        evtGetWorkData2and3(wdata, wdata->pm2Cuts[wdata->seqIndex], wdata->pm3Cuts[wdata->seqIndex]);
        wdata->loadFlags |= 8;
    }
}

s32 evtCallGetProcessData13(dds3ProcessID_t *id) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);

    if (pInfo == NULL) {
        return 0;
    }
    if (pInfo->workdata == NULL) {
        return 0;
    }
    if (pInfo->workdata->loadFlags & 8) {
        return 1;
    }
    return 0;
}

s32 evtCheckRun(dds3ProcessID_t *id) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);

    if (evtCallGetProcessData13(id) == 0) {
        return 0;
    }
    if (pInfo->workdata->loadFlags & 1) {
        return 0;
    }
    if (evtCheckProcess() != 0) {
        return 1;
    }
    return 0;
}

/* NON_MATCHING: 22 of 25 words match.  */
#ifdef NON_MATCHING
s32 evtCheckSeqEnd(dds3ProcessID_t *id) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);
    u32 flags;

    if (pInfo == NULL) {
        return 1;
    }
    if (pInfo->workdata != NULL) {
        flags = pInfo->workdata->loadFlags;
        if (!(flags & 8)) {
            return 0;
        }
        if (!(flags & 1)) {
            if (pInfo->flags & 0x80) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtCheckSeqEnd);
#endif

extern dds3Effect_Blur_t evtBlur2DefaultParam2;

/* One asm location for every arm's w set, so jump2 can cross-jump them. */
static inline void evtSetBlendVecW(f32 w)
{
    VU0_SCALAR_OP_CLOBBER(w, "vmulx.w vf10, vf0, vf2x");
}

void evtHokanHeikouBlur(s32 enable, f32 t, const dds3Effect_Blur_t *a, const dds3Effect_Blur_t *b, dds3Effect_Blur_t *out);

f32 evtHokanFloat(s32 hokan, f32 ratio, f32 f1, f32 f2)
{
    if (hokan) {
        return f1 * (1.0f - ratio) + f2 * ratio;
    }
    return f1;
}

s32 evtHokanInt(s32 hokan, f32 ratio, s32 i1, s32 i2)
{
    s32 result = i1;

    if (hokan) {
        result = (s32)((f32)i1 * (1.0f - ratio) + (f32)i2 * ratio);
    }
    return result;
}

u32 evtHokanUInt(s32 hokan, f32 ratio, u32 i1, u32 i2)
{
    if (hokan) {
        return (u32)((f32)i1 * (1.0f - ratio) + (f32)i2 * ratio);
    }
    return i1;
}

/* vu0 routine: blend two RGBA8888 colours by t */
u32 evtHokanColor(s32 hokan, f32 ratio, u32 c1, u32 c2)
{
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;

    if (hokan) {
        color1[0] = c1;
        MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
        SDF_VU0_COPYVECTOR_A_TO_B();
        color2[0] = c2;
        MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
        SDF_VU0_SCALEVECTOR_BITS(ratio);
        SDF_VU0_ADDVECTOR();
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        blended[0] = packed;
        return blended[0];
    }
    return c1;
}

#ifdef NON_MATCHING
/* 38 of 101 words differ: same length and structure, but the loop counts up and the
 * registers are numbered differently (retail counts down from 2). */
/* vu0 routine: blend two scene-light records (colour and direction of three lights, ambient) by t */
void evtHokanSceneLight(s32 hokan, f32 ratio, pmdSceneLightTable_t *key1, pmdSceneLightTable_t *key2, pmdSceneLightTable_t *d) {
    s32 i;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    for (i = 0; i < 3; i++) {
        if (key1 != NULL) {
            SDF_VU0_LOADVECTOR(key1->color[i]);
        } else {
            SDF_VU0_LOADVECTOR(evtSceneLightDefaultColor);
        }
        SDF_VU0_SCALEVECTOR_V0(1.0f - ratio);
        if (key2 != NULL) {
            SDF_VU0_LOADVECTOR_B(key2->color[i]);
        } else {
            SDF_VU0_LOADVECTOR_B(evtSceneLightDefaultColor);
        }
        SDF_VU0_SCALEVECTOR_B_V0(ratio);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(d->color[i]);
        if (key1 != NULL) {
            SDF_VU0_LOADVECTOR(key1->direction[i]);
        } else {
            SDF_VU0_LOADVECTOR(evtSceneLightDefaultDir);
        }
        SDF_VU0_SCALEVECTOR_V0(1.0f - ratio);
        if (key2 != NULL) {
            SDF_VU0_LOADVECTOR_B(key2->direction[i]);
        } else {
            SDF_VU0_LOADVECTOR_B(evtSceneLightDefaultDir);
        }
        SDF_VU0_SCALEVECTOR_B_V0(ratio);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(d->direction[i]);
    }
    if (key1 != NULL) {
        SDF_VU0_LOADVECTOR(key1->ambient);
    } else {
        SDF_VU0_LOADVECTOR(evtSceneLightDefaultAmbient);
    }
    SDF_VU0_SCALEVECTOR_V0(1.0f - ratio);
    if (key2 != NULL) {
        SDF_VU0_LOADVECTOR_B(key2->ambient);
    } else {
        SDF_VU0_LOADVECTOR_B(evtSceneLightDefaultAmbient);
    }
    SDF_VU0_SCALEVECTOR_B_V0(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(d->ambient);
}
#else
INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtHokanSceneLight);
#endif

/* vu0 routine: blend a vector and an RGBA8888 colour by t (missing input =
 * default vector/colour) */
void evtHokanSceneFog(s32 hokan, f32 ratio, evtBlendVecColor_t *key1, evtBlendVecColor_t *key2, evtBlendVecColor_t *d)
{

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 != NULL) {
        SDF_VU0_SETVECTOR_X(key1->vec[0]);
        SDF_VU0_SETVECTOR_Y(key1->vec[1]);
        SDF_VU0_SETVECTOR_Z(key1->vec[2]);
        SetBlendVecW(key1->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (key2 != NULL) {
        SDF_VU0_SETVECTOR_X(key2->vec[0]);
        SDF_VU0_SETVECTOR_Y(key2->vec[1]);
        SDF_VU0_SETVECTOR_Z(key2->vec[2]);
        SetBlendVecW(key2->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_GETVECTOR_X(d->vec[0]);
    SDF_VU0_GETVECTOR_Y(d->vec[1]);
    SDF_VU0_GETVECTOR_Z(d->vec[2]);
    SDF_VU0_GETVECTOR_W(d->vec[3]);
    if (key1 != NULL) {
        UnpackRGBAWordFloat(key1->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(evtSceneFogDefaultColor[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (key2 != NULL) {
        UnpackRGBAWordFloat(key2->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(evtSceneFogDefaultColor[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(ratio);
    SDF_VU0_ADDVECTOR();
    d->color = PackRGBAWord(128.0f);
}

void evtHokanBlur2(s32 hokan, f32 ratio, dds3Effect_Blur_t *key1, dds3Effect_Blur_t *key2, dds3Effect_Blur_t *d)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtBlur2DefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtBlur2DefaultParam;
    }
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
    d->rotate = evtHokanFloat(hokan, ratio, key1->rotate, key2->rotate);
    d->scale = evtHokanFloat(hokan, ratio, key1->scale, key2->scale);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        d->center[positionOrCornerIndex] = evtHokanInt(hokan, ratio, key1->center[positionOrCornerIndex], key2->center[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->xy[positionOrCornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->xy[positionOrCornerIndex][coordinateIndex], key2->xy[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtHokanMultBlur(s32 hokan, f32 ratio, dds3Effect_PartialMultiBlurParam_t *key1, dds3Effect_PartialMultiBlurParam_t *key2, dds3Effect_PartialMultiBlurParam_t *d)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtMultBlurDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtMultBlurDefaultParam;
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    d->Blur.abgr = evtHokanColor(hokan, ratio, key1->Blur.abgr, key2->Blur.abgr);
    d->Blur.blend = key1->Blur.blend;
    d->Blur.rotate = evtHokanFloat(hokan, ratio, key1->Blur.rotate, key2->Blur.rotate);
    d->Blur.scale = evtHokanFloat(hokan, ratio, key1->Blur.scale, key2->Blur.scale);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        d->Blur.center[positionOrCornerIndex] = evtHokanInt(hokan, ratio, key1->Blur.center[positionOrCornerIndex], key2->Blur.center[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->Blur.xy[positionOrCornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->Blur.xy[positionOrCornerIndex][coordinateIndex], key2->Blur.xy[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d)
{
    s32 cornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtFilterDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtFilterDefaultParam;
    }
    for (cornerIndex = 0; cornerIndex < 2; cornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->xy[cornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->xy[cornerIndex][coordinateIndex], key2->xy[cornerIndex][coordinateIndex]);
        }
    }
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
}

void evtHokanMultFilter(s32 hokan, f32 ratio, dds3Effect_PartialMultiFilterParam_t *key1, dds3Effect_PartialMultiFilterParam_t *key2, dds3Effect_PartialMultiFilterParam_t *d)
{
    s32 centerCoordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtMultFilterDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtMultFilterDefaultParam;
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    for (centerCoordinateIndex = 0; centerCoordinateIndex < 2; centerCoordinateIndex++) {
        d->center[centerCoordinateIndex] = evtHokanInt(
            hokan, ratio, key1->center[centerCoordinateIndex], key2->center[centerCoordinateIndex]);
    }
    d->Filter.rgba = evtHokanColor(hokan, ratio, key1->Filter.rgba, key2->Filter.rgba);
    d->Filter.blendControl = key1->Filter.blendControl;
}

void evtHokanDistBlur(s32 hokan, f32 ratio, dds3Effect_DistortionBlurParam_t *key1, dds3Effect_DistortionBlurParam_t *key2, dds3Effect_DistortionBlurParam_t *d)
{
    s32 positionAxisIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtDistBlurDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtDistBlurDefaultParam;
    }
    d->number = evtHokanUInt(hokan, ratio, key1->number, key2->number);
    d->interval = evtHokanUInt(hokan, ratio, key1->interval, key2->interval);
    d->speed = evtHokanFloat(hokan, ratio, key1->speed, key2->speed);
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
    d->rotate = evtHokanFloat(hokan, ratio, key1->rotate, key2->rotate);
    d->distortion = evtHokanFloat(hokan, ratio, key1->distortion, key2->distortion);
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        d->center[positionAxisIndex] = evtHokanInt(hokan, ratio, key1->center[positionAxisIndex], key2->center[positionAxisIndex]);
    }
    d->width = evtHokanUInt(hokan, ratio, key1->width, key2->width);
}

void evtHokanRipBlur(s32 hokan, f32 ratio, dds3Effect_RippleBlurParam_t *key1, dds3Effect_RippleBlurParam_t *key2, dds3Effect_RippleBlurParam_t *d)
{
    s32 positionAxisIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtRipBlurDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtRipBlurDefaultParam;
    }
    d->number = evtHokanUInt(hokan, ratio, key1->number, key2->number);
    d->speed = evtHokanFloat(hokan, ratio, key1->speed, key2->speed);
    d->interval = evtHokanFloat(hokan, ratio, key1->interval, key2->interval);
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
    d->rotate = evtHokanFloat(hokan, ratio, key1->rotate, key2->rotate);
    d->power = evtHokanFloat(hokan, ratio, key1->power, key2->power);
    d->frequency = evtHokanFloat(hokan, ratio, key1->frequency, key2->frequency);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        d->center[positionAxisIndex] = evtHokanInt(hokan, ratio, key1->center[positionAxisIndex], key2->center[positionAxisIndex]);
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
}

void evtHokanHeikouBlur(s32 enable, f32 t, const dds3Effect_Blur_t *a, const dds3Effect_Blur_t *b, dds3Effect_Blur_t *out)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &evtBlur2DefaultParam2;
    }
    if (b == NULL) {
        b = &evtBlur2DefaultParam2;
    }
    out->abgr = evtHokanColor(enable, t, a->abgr, b->abgr);
    out->blend = a->blend;
    out->rotate = evtHokanFloat(enable, t, a->rotate, b->rotate);
    out->scale = evtHokanFloat(enable, t, a->scale, b->scale);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        out->center[positionOrCornerIndex] = evtHokanInt(enable, t, a->center[positionOrCornerIndex], b->center[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            out->xy[positionOrCornerIndex][coordinateIndex] = evtHokanInt(enable, t, a->xy[positionOrCornerIndex][coordinateIndex], b->xy[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtBlendParamsH(s32 enable, f32 t, const evtBlendKey_t *a, const evtBlendKey_t *b, evtBlendKey_t *out)
{
    s32 colorChannelIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    out->x = evtHokanInt(enable, t, a->x, b->x);
    out->w[0] = evtHokanInt(enable, t, a->w[0], b->w[0]);
    out->w[1] = evtHokanInt(enable, t, a->w[1], b->w[1]);
    out->w[2] = evtHokanInt(enable, t, a->w[2], b->w[2]);
    out->w[3] = evtHokanInt(enable, t, a->w[3], b->w[3]);
    out->flagWord = a->flagWord;
    for (colorChannelIndex = 0; colorChannelIndex < 3; colorChannelIndex++) {
        out->y[colorChannelIndex] = evtHokanInt(enable, t, a->y[colorChannelIndex], b->y[colorChannelIndex]);
        out->z[colorChannelIndex] = evtHokanInt(enable, t, a->z[colorChannelIndex], b->z[colorChannelIndex]);
    }
}

void evtRunUnitMode(dds3Basic_t *unit, PMD_UNIT mode, u32 addflag, u32 downflag) {
    switch (mode) {
    case PMD_UNIT_DISP_ON:
        dds3GetUnitHandleObjectBasic(unit)->owner->flags &= ~1;
        break;
    case PMD_UNIT_DISP_OFF:
        dds3GetUnitHandleObjectBasic(unit)->owner->flags |= 1;
        break;
    case PMD_UNIT_SHADOW_OFF:
        dds3ObjectBaseOffFlag(unit, 0x400);
        dds3ObjectBaseOffFlag(unit, 0x200);
        dds3ObjectBaseOffFlag(unit, 0x4000);
        dds3ObjectBaseOffFlag(unit, 0x8000);
        break;
    case PMD_UNIT_MSHADOW_ON:
        dds3ObjectBaseOnFlag(unit, 0x400);
        dds3ObjectBaseOffFlag(unit, 0x200);
        if (addflag != 0) {
            dds3ObjectBaseOnFlag(unit, addflag);
        }
        if (downflag != 0) {
            dds3ObjectBaseOffFlag(unit, downflag);
        }
        break;
    case PMD_UNIT_VSHADOW_ON:
        dds3ObjectBaseOffFlag(unit, 0x400);
        dds3ObjectBaseOnFlag(unit, 0x200);
        if (addflag != 0) {
            dds3ObjectBaseOnFlag(unit, addflag);
        }
        if (downflag != 0) {
            dds3ObjectBaseOffFlag(unit, downflag);
        }
        break;
    case PMD_UNIT_MASK_ON:
        dds3UnitObjectBasicSetOt(unit, 0);
        break;
    case PMD_UNIT_MASK_OFF:
        dds3UnitObjectBasicSetOt(unit, 11);
        mode = PMD_UNIT_DISP_ON;
        break;
    }
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtSetFlag);

void evtResetFlag(dds3ProcessID_t *id, PMD_FLAG flag)
{
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);

    pInfo->workdata->loadFlags &= ~flag;
}

s32 evtStopMover(dds3Basic_t *mover, s32 sw, s32 baseframe, s32 nowframe) {
    dds3PathBasicControl_t *clip;
    s32 elapsedFrames;
    s32 duration;

    elapsedFrames = nowframe - baseframe;
    if (elapsedFrames < 0) {
        elapsedFrames = 0;
    }
    clip = ((dds3MoverBasicWorkData_t *)mover->workdata)->pPathControl;
    if (clip != NULL) {
        duration = (s32)clip->frameMax * 2;
        if (duration <= elapsedFrames) {
            elapsedFrames = duration;
        }
        dds3PathBasicControlSetRatio(clip, (f32)elapsedFrames / (f32)duration);
        if (sw == 0) {
            dds3PathBasicPauseOn(clip);
        } else {
            dds3PathBasicPauseOff(clip);
        }
    }
#ifdef FIX_BUGS
    return 0;
#endif
}

void evtMoverFrameSet(dds3Basic_t *mover, s32 sw, s32 baseframe, s32 nowframe, s32 offset) {
    dds3PathBasicControl_t *clip;

    clip = ((dds3MoverBasicWorkData_t *)mover->workdata)->pPathControl;
    if (clip != NULL) {
        s32 elapsedFrames = nowframe - baseframe + offset;
        s32 duration;

        if (elapsedFrames < 0) {
            elapsedFrames = 0;
        }
        duration = (s32)clip->frameMax * 2;
        if (duration <= elapsedFrames) {
            elapsedFrames = duration;
        }
        clip->frame = (f32)elapsedFrames * 0.5f;
    }
}

evtPolygonMovieWork_t *InitPolygonMovieWorkData(void) {
    sdfMemHandle_t *mem;
    evtPolygonMovieWork_t *work;

    mem = sdfMemAllocDirect(sizeof(evtPolygonMovieWork_t));
    work = sdfMemLock(mem);
    if (work == NULL) {
        return NULL;
    }
    memset(work, 0, sizeof(evtPolygonMovieWork_t));
    work->mem = mem;
    sdf3DScreenAdjustScaleX = 1.0f;
    sdf3DScreenAdjustScaleY = 1.0f;
    sdf3DScreenAdjustOffsetX = 0.0f;
    sdf3DScreenAdjustOffsetY = 0.0f;
    return work;
}

evtPolygonMovieWork_t *GetWorkData1(evtPolygonMovieWork_t *wdata, pmdHeader_t *pm1data) {
    u32 *block;
    s32 i;

    if (pm1data == NULL) {
        return NULL;
    }
    wdata->unk010 = 0;
    wdata->MesHandle = -1;
    block = sdfChipAllocDirect(0x1FC);
    wdata->chipBlock = block;
    block += 126;
    for (i = 126; i >= 0; i--) {
        *block-- = 0;
    }
    wdata->nameCount1 = 0;
    wdata->stageCount = 0;
    wdata->unitCount = 0;
    wdata->effectCount = 0;
    wdata->tmxCount = 0;
    wdata->entryTable1 = pm1data->entries;
    wdata->header1 = pm1data;
    wdata->nameTable1 = NULL;
    wdata->stageTable = NULL;
    wdata->stageF1 = NULL;
    wdata->stageF2 = NULL;
    wdata->stageTB = NULL;
    wdata->unitTable = NULL;
    wdata->type9Data = NULL;
    wdata->effectTable = NULL;
    wdata->type8Data = NULL;
    wdata->tmxTable = NULL;
    wdata->pm1Type23Data = NULL;
    for (i = 0; i < wdata->header1->count; i++) {
        switch (wdata->entryTable1[i].type) {
            case 2:
                wdata->stageTable = (pmdStageTable_t *)((u8 *)pm1data + wdata->entryTable1[i].offset);
                wdata->stageCount = wdata->entryTable1[i].value;
                break;
            case 10:
                wdata->stageF1 = (u8 *)pm1data + wdata->entryTable1[i].offset;
                break;
            case 11:
                wdata->stageF2 = (u8 *)pm1data + wdata->entryTable1[i].offset;
                break;
            case 12:
                wdata->stageTB = (u8 *)pm1data + wdata->entryTable1[i].offset;
                break;
            case 3:
                wdata->unitTable = (pmdUnitTable_t *)((u8 *)pm1data + wdata->entryTable1[i].offset);
                wdata->unitCount = wdata->entryTable1[i].value;
                break;
            case 9:
                wdata->type9Data = (u8 *)pm1data + wdata->entryTable1[i].offset;
                break;
            case 1:
                wdata->nameTable1 = (char (*)[32])((u8 *)pm1data + wdata->entryTable1[i].offset);
                wdata->nameCount1 = wdata->entryTable1[i].value;
                break;
            case 6:
                wdata->messageData = (u8 *)pm1data + wdata->entryTable1[i].offset;
                if (wdata->entryTable1[i].value == 0) {
                    wdata->MesHandle = -1;
                } else {
                    wdata->MesHandle = itfMesMngInitialize(wdata->messageData);
                }
                break;
            case 7:
                wdata->effectTable = (pmdEffectTable_t *)((u8 *)pm1data + wdata->entryTable1[i].offset);
                wdata->effectCount = wdata->entryTable1[i].value;
                break;
            case 8:
                wdata->type8Data = (u8 *)pm1data + wdata->entryTable1[i].offset;
                break;
            case 22:
                wdata->tmxTable = (pmdTmxTable_t *)((u8 *)pm1data + wdata->entryTable1[i].offset);
                wdata->tmxCount = wdata->entryTable1[i].value;
                break;
            case 23:
                wdata->pm1Type23Data = (u8 *)pm1data + wdata->entryTable1[i].offset;
                break;
            case 4:
            case 5:
                break;
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
                break;
        }
    }
    return wdata;
}

evtPolygonMovieWork_t *evtGetWorkData2and3(evtPolygonMovieWork_t *wdata, pmdHeader_t *pm2cut, pmdHeader_t *pm3cut) {
    s32 i;

    wdata->header2 = pm2cut;
    if (pm2cut != NULL) {
        wdata->entryTable2 = pm2cut->entries;
    } else {
        wdata->entryTable2 = NULL;
    }
    wdata->nameTable = NULL;
    wdata->nameCount2 = 0;
    wdata->cutInfo = NULL;
    wdata->frameTable = NULL;
    wdata->frameCount = 0;
    wdata->cameraTable = NULL;
    wdata->cameraCount = 0;
    wdata->slightTable = NULL;
    wdata->slightCount = 0;
    wdata->sfogTable = NULL;
    wdata->sfogCount = 0;
    wdata->blur2Table = NULL;
    wdata->blur2Count = 0;
    wdata->mblurTable = NULL;
    wdata->mblurCount = 0;
    wdata->dblurTable = NULL;
    wdata->dblurCount = 0;
    wdata->filterTable = NULL;
    wdata->filterCount = 0;
    wdata->mfilterTable = NULL;
    wdata->mfilterCount = 0;
    wdata->rblurTable = NULL;
    wdata->rblurCount = 0;
    wdata->objectTable = NULL;
    wdata->objectCount = 0;
    wdata->hblurTable = NULL;
    wdata->hblurCount = 0;
    wdata->rainDataTable = NULL;
    wdata->rainDataCount = 0;
    if (pm2cut == NULL) {
        return wdata;
    }
    for (i = 0; i < wdata->header2->count; i++) {
        switch (wdata->entryTable2[i].type) {
            case 0:
                wdata->cutInfo = (pmdCutInfo_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                break;
            case 4:
                if (wdata->header2->kind == 4) {
                    wdata->frameTable2 = NULL;
                    wdata->frameTable = (pmdFrameTable_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                } else {
                    wdata->frameTable = NULL;
                    wdata->frameTable2 = (pmdFrameTable2_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                }
                wdata->frameCount = wdata->entryTable2[i].value;
                break;
            case 1:
                wdata->nameTable = (char (*)[32])((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->nameCount2 = wdata->entryTable2[i].value;
                break;
            case 5:
                wdata->cameraTable = (u8 *)pm2cut + wdata->entryTable2[i].offset;
                wdata->cameraCount = wdata->entryTable2[i].value;
                break;
            case 13:
                wdata->slightTable = (pmdSceneLightTable_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->slightCount = wdata->entryTable2[i].value;
                break;
            case 14:
                wdata->sfogTable = (pmdSceneFogTable_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->sfogCount = wdata->entryTable2[i].value;
                break;
            case 15:
                wdata->blur2Table = (dds3Effect_Blur_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->blur2Count = wdata->entryTable2[i].value;
                break;
            case 16:
                wdata->mblurTable = (dds3Effect_PartialMultiBlurParam_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->mblurCount = wdata->entryTable2[i].value;
                break;
            case 17:
                wdata->dblurTable = (dds3Effect_DistortionBlurParam_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->dblurCount = wdata->entryTable2[i].value;
                break;
            case 18:
                wdata->filterTable = (dds3Effect_Filter_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->filterCount = wdata->entryTable2[i].value;
                break;
            case 19:
                wdata->mfilterTable = (dds3Effect_PartialMultiFilterParam_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->mfilterCount = wdata->entryTable2[i].value;
                break;
            case 20:
                wdata->rblurTable = (dds3Effect_RippleBlurParam_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->rblurCount = wdata->entryTable2[i].value;
                break;
            case 24:
                wdata->hblurTable = (pmdHeikouBlurTable_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->hblurCount = wdata->entryTable2[i].value;
                break;
            case 21:
                wdata->objectTable = (struct evtGroupMetadata_t *)((u8 *)pm2cut + wdata->entryTable2[i].offset);
                wdata->objectCount = wdata->entryTable2[i].value;
                break;
            case 25:
                wdata->rainDataTable = (u8 *)pm2cut + wdata->entryTable2[i].offset;
                wdata->rainDataCount = wdata->entryTable2[i].value;
                break;
            case 2:
            case 3:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 22:
            case 23:
                break;
        }
    }
    wdata->header3 = pm3cut;
    if (pm3cut != NULL) {
        wdata->entryTable3 = pm3cut->entries;
    } else {
        wdata->entryTable3 = NULL;
    }
    wdata->soundFrameTable = NULL;
    wdata->soundFrameCount = 0;
    if (pm3cut == NULL) {
        return wdata;
    }
    for (i = 0; i < wdata->header3->count; i++) {
        if (wdata->entryTable3[i].type == 4) {
            wdata->soundFrameTable = (pmdFrameTable2_t *)((u8 *)pm3cut + wdata->entryTable3[i].offset);
            wdata->soundFrameCount = wdata->entryTable3[i].value;
        }
    }
    return wdata;
}

void evtFreeWorkData1(evtPolygonMovieWork_t *wdata) {
    if (wdata == NULL) {
        return;
    }
    if (wdata->chipBlock != NULL) {
        sdfChipFreeDirect(wdata->chipBlock);
    }
    if (wdata->MesHandle >= 0) {
        itfMesMngRelease(wdata->MesHandle);
        wdata->MesHandle = -1;
    }
    fileSyncAll();
    if (wdata->pm1Request != NULL) {
        fileHandleFree(wdata->pm1Request);
    }
    if (wdata->pm1File != NULL) {
        sdfMemFreeDirect(wdata->pm1File);
    }
    if (scrCheckEnableMovieCancel() == 1) {
        scrGetAfterSkipBgmFlag();
    }
}

void evtFreeWorkData2and3(evtPolygonMovieWork_t *wdata) {
    if (wdata->pm2Request != NULL) {
        fileHandleFree(wdata->pm2Request);
    }
    if (wdata->pm3Request != NULL) {
        fileHandleFree(wdata->pm3Request);
    }
    if (wdata->pm2Mem != NULL) {
        sdfMemFreeDirect(wdata->pm2Mem);
        wdata->pm2Data = NULL;
    }
    if (wdata->pm3Mem != NULL) {
        sdfMemFreeDirect(wdata->pm3Mem);
        wdata->pm3Data = NULL;
    }
    sdfMemFreeDirect(wdata->mem);
}

sdfMemHandle_t *evtAllocMovieWork(pmdHeader_t **out, s32 *size)
{
    s32 header[16] = {0, 0, 0x32444D50, 0, 1, 0xB, 0, 0, 0, 0x10, 1, 0x30, 0, 999, 1000, 0};
    s32 headerBytes;
    sdfMemHandle_t *handle;
    void *block;

    headerBytes = 0x40;
    header[1] = headerBytes;
    if (size != NULL) {
        *size = headerBytes;
    }
    handle = sdfMemAllocDirect(headerBytes);
    block = EE_ADDR_AS(void *, sdfMemLock(handle));
    memcpy(block, header, headerBytes);
    *out = block;
    return handle;
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtCallMemset8);

void SetPMFileName(s32 eventno, s32 cutno, char *pm1, char *pm2, char *pm3) {
    sprintf(pm1, SetPMFileNameStrEvent03030303E0303Pm1, eventno / 10 * 10, eventno, eventno, cutno, eventno, cutno);
    sprintf(pm2, SetPMFileNameStrEvent03030303E0303Pm2, eventno / 10 * 10, eventno, eventno, cutno, eventno, cutno);
    sprintf(pm3, SetPMFileNameStrEvent03030303E0303Pm3, eventno / 10 * 10, eventno, eventno, cutno, eventno, cutno);
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtSetDofTblFileName);

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", SetPMFileNameStrEvent03030303E0303Pm1);

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", SetPMFileNameStrEvent03030303E0303Pm2);

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", SetPMFileNameStrEvent03030303E0303Pm3);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtGetPolygonMovieWorkData2);

dds3ProcessID_t *evtStart2(DDS3_PRIORITY priority, s32 eventno, s32 cutno, s32 seqno) {
    char pm1Name[0x40];
    char pm2Name[0x40];
    char pm3Name[0x40];
    sdfMemHandle_t *viewerMemory;
    void *viewerBytes;
    evtViewerInfo_t *viewer;
    evtPolygonMovieWork_t *work;
    dds3ProcessID_t *task;

    fldBgTexClear();
    while (sdfDelayedResrcCheck() != 0) {
    }
    dds3BackColor = 0x80000000;
    viewerMemory = sdfMemAllocDirect(0x2568);
    viewerBytes = sdfMemLock(viewerMemory);
    memset(viewerBytes, 0, 0x2568);
    viewer = viewerBytes;
    viewer->block = viewerMemory;
    SetPMFileName(eventno, cutno, pm1Name, pm2Name, pm3Name);
    work = InitPolygonMovieWorkData();
    work->eventNo = eventno;
    work->cutNo = cutno;
    work->seqIndex = seqno - 1;
    work->pm1Request = fileLoad(pm1Name);
    work->loadFlags |= 2;
    work->pm2Request = fileLoad(pm2Name);
    work->loadFlags |= 4;
    if (sdfFileCheckExist(pm3Name) != 0) {
        work->pm3Request = fileLoad(pm3Name);
        work->loadFlags |= 0x10;
    } else {
        work->pm3Request = NULL;
    }
    task = dds3StartProcess(evtStrZikkiPlayModeEventViewer, priority, 1, 1, evtCallGetProcessData11, (dds3ProcessFunc_t)evtEventViewerDestroyActiveAlternate, viewer);
    viewer->workdata = work;
    work->task = task;
    evtEventViewerInitCampFontResource(viewer);
    return task;
}

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", evtStrZikkiPlayModeEventViewer);

INCLUDE_SDATA(const s32, "event/evtPolygonMovie", evtCallMemset8Data);

