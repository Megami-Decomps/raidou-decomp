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

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/ee_mmi.h"
#include "dds/pcp_vu0.h"
#include "dds/fld.h"

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

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtHokanSceneLight);

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

void evtBlendParamsH(s32 enable, f32 t, const EvtBlendKey *a, const EvtBlendKey *b, EvtBlendKey *out)
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

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtRunUnitMode);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtSetFlag);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtResetFlag);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtStopMover);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtMoverFrameSet);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", InitPolygonMovieWorkData);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", GetWorkData1);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtGetWorkData2and3);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtFreeWorkData1);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtFreeWorkData2and3);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtAllocMovieWork);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtCallMemset8);

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", evtAllocMovieWorkTable);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", SetPMFileName);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtSetDofTblFileName);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtGetPolygonMovieWorkData2);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtStart2);

INCLUDE_SDATA(const s32, "event/evtPolygonMovie", evtCallMemset8Data);

