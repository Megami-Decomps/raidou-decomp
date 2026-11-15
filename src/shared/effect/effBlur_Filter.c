#include "common.h"
#include "sdk_libgcc.h"
#include "libsdf.h"
#include "effect.h"
#include "sdfFrame.h"

extern u8 dds3EffectBase[];
void dds3EffectBlur(const dds3Effect_Blur_t *source);
sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(u8 *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(u8 *name);
extern void *memcpy(void *a, const void *b, size_t c);
struct sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
void *sdfChipAllocDirect(s32 aSize);
u32 sdfMemLock(struct sdfMemHandle_t *aHandle);
extern u32 dds3NullDrawProc(void);

/* The frame buffers the blur samples from; only the mask source is named. */
typedef struct effBlurDrawBank_t {
    u8 pad0000[0x1300];
    u8 frameMaskSource[0x8E0];
} effBlurDrawBank_t;

void dds3EffectBlurSetRect(dds3Effect_PartialMultiBlurParam_t *work);
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam);
void dds3EffectPartialMultiBlurSetParam(effBlurTemplate_t *hPartialMultiBlur, const dds3Effect_PartialMultiBlurParam_t *pParam);
void dds3EffectDistBlurPartInit(effBlurScatterWork_t *pParam, effBlurScatterSlot_t *pPart);
effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *pParam);
void dds3EffectDistortionBlurSetParam(effBlurScaleWork_t *hDistortionBlur, dds3Effect_RippleBlurParam_t *pParam);
void dds3EffectRipBlurPartInit(effBlurScaleWork_t *pParam, effBlurScaleSlot_t *pPart);
void dds3EffectRippleBlurReset(effBlurScaleWork_t *hRippleBlur);
effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *pParam);
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam);

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_effBlurDrawBank_t 1

#include "dds/btl_effect_position.h"
#include "dds/eff.h"
#include "dds/eff_math.h"
#include "dds/sdf_trig.h"
#include "dds/sdk_ps2.h"
#include "dds/sdf_gs_header.h"
#include "dds/bill_object_api.h"
#include "dds/sdf_packet_list.h"
#include "dds/common.h"
#include "dds/dds3_background_fade_api.h"
#include "dds/sdf_vu0.h"
#include "dds/dds3_draw_buffer.h"
#include "dds/dds3_frame_draw_buffer.h"
#include "dds/sdf_texture_draw_packet.h"
#include "dds/sdf.h"
#include "dds/sdf_chip.h"
#include "dds/eff_blur.h"
#include "dds/pcp_vu0.h"

typedef struct {
    u8 pad00[0x124];
    u16 kind; /* 0x124: kind 0x109 selects separate slots for the two resources */
} BlurActor;

typedef dds3Effect_Blur_t BlurSource;

struct sdfDrawTag_t;

struct sdfGrRefPacketHeader_t;

#include "dds/sdf_packet_alloc.h"

/* Queue blend setup and both rectangle packets, then finish with the filter draw. */
void effDrawBlurSource(const BlurSource *source, sdfTexHandle_t *resource, u8 fixedPointCoordinates);
void effDrawBlurFixedPointRectangle(EffBlurTemplate *owner);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectBlurDrawAlphaMulti);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectBlurDrawFrameMaskReset);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effCallNullDrawProc);

void dds3EffectBlurSetRect(dds3Effect_PartialMultiBlurParam_t *work) {
    s32 x = work->Blur.center[0] + 0x100;
    s32 y = work->Blur.center[1] + 0xE0;
    s32 extent = work->area;

    work->Blur.xy[0][0] = x - extent;
    work->Blur.xy[0][1] = y - extent;
    work->Blur.xy[1][0] = x + extent;
    work->Blur.xy[1][1] = y + extent;
    dds3EffectBlur(&work->Blur);
}

/* Clone a blur template into a fresh allocation. */
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam) {
    effBlurTemplate_t *dst = sdfChipAllocDirect(sizeof(effBlurTemplate_t));

    dst->texture = dds3BillboardGetGeneralTex(2);
    memcpy(&dst->body, pParam, sizeof(*pParam));
    return dst;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effStopSub);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effCallNullDrawProc2);

/* GS coordinates use sixteenth-pixel X and half-height Y; extent is halved for Y. */
void effDrawBlurFixedPointRectangle(EffBlurTemplate *owner) {
    s32 centerX, centerY, halfExtent;

    if (dds3NullDrawProc() == 0) {
        centerX = owner->body.Blur.center[0] + 0x1000;
        centerY = (owner->body.Blur.center[1] + 0xE00) >> 1;
        halfExtent = owner->body.area;
        owner->body.Blur.xy[0][0] = centerX - halfExtent;
        owner->body.Blur.xy[1][0] = centerX + halfExtent;
        halfExtent >>= 1;
        owner->body.Blur.xy[0][1] = centerY - halfExtent;
        owner->body.Blur.xy[1][1] = centerY + halfExtent;
        effDrawBlurSource(&owner->body.Blur, owner->texture, 1);
    }
}

/* Copy only the serialized parameters, leaving the owned work tail intact. */
void dds3EffectPartialMultiBlurSetParam(effBlurTemplate_t *hPartialMultiBlur, const dds3Effect_PartialMultiBlurParam_t *pParam) {
    hPartialMultiBlur->body = *pParam;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effLeaf56);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectDistortionBlurSetTex);

void dds3EffectDistBlurPartInit(effBlurScatterWork_t *pParam, effBlurScatterSlot_t *pPart) {
    dds3Effect_Blur_t *quad = &pPart->quad;
    f32 spread;
    s32 halfSize;
    s32 centerX;
    s32 centerY;
    pPart->delay = SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % (pParam->params.interval + 1));
    pPart->angle = -3.14159265f;
    quad->rotate = pParam->params.rotate;
    quad->abgr = pParam->params.abgr;
    quad->blend = pParam->params.blend;
    spread = pParam->params.area;
    halfSize = pParam->params.width;
    quad->center[0] = pParam->params.center[0] +
        (s32)(spread * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f));
    halfSize >>= 1;
    quad->center[1] = pParam->params.center[1] +
        (s32)(spread * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f));
    centerX = quad->center[0] + 256;
    quad->xy[0][0] = centerX - halfSize;
    quad->xy[1][0] = centerX + halfSize;
    centerY = quad->center[1] + 224;
    quad->xy[0][1] = centerY - halfSize;
    quad->xy[1][1] = centerY + halfSize;
}

effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *pParam)
{
    struct sdfMemHandle_t *allocation;
    effBlurScatterWork_t *work;
    effBlurScatterSlot_t *slot;
    s32 slotIndex;

    allocation = sdfMemAllocDirect(sizeof(effBlurScatterWork_t) + 100 * sizeof(effBlurScatterSlot_t));
    work = EE_ADDR_AS(effBlurScatterWork_t *, sdfMemLock(allocation));
    work->params = *pParam;
    work->allocation = allocation;
    work->slots = (effBlurScatterSlot_t *)(work + 1);
    work->texture = dds3BillboardGetGeneralTex(2);
    slot = work->slots;
    for (slotIndex = 0; slotIndex < 100; slotIndex++, slot++) {
        dds3EffectDistBlurPartInit(work, slot);
        slot->angle = 3.14159265f + 1.0f;
    }
    return work;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effPCPDistortionBlurExpDeleteSub);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effBlurStepScatterSlotsAndDraw);

/* Update scale parameters but preserve the allocated destination slot count. */
void dds3EffectDistortionBlurSetParam(effBlurScaleWork_t *hDistortionBlur, dds3Effect_RippleBlurParam_t *pParam) {
    u32 count = hDistortionBlur->params.number;

    hDistortionBlur->params = *pParam;
    hDistortionBlur->params.number = count;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effLeaf57);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectPartialMultiBlurSetTexDef);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectRipBlurPartAreaSet);

/* Reset a slot for a new burst: zero phase and angle, then copy the colour, the two spare fields and the centre from the work parameters. */
void dds3EffectRipBlurPartInit(effBlurScaleWork_t *pParam, effBlurScaleSlot_t *pPart) {
    dds3Effect_Blur_t *quad = &pPart->quad;

    pPart->phase = 0.0f;
    pPart->angle = 0.0f;
    quad->abgr = pParam->params.abgr;
    quad->blend = pParam->params.blend;
    quad->rotate = pParam->params.rotate;
    quad->center[0] = pParam->params.center[0];
    quad->center[1] = pParam->params.center[1];
}

/* Reset every slot, then stagger its initial phase by -spacing * index. */
void dds3EffectRippleBlurReset(effBlurScaleWork_t *hRippleBlur) {
    effBlurScaleSlot_t *slot = hRippleBlur->slots;
    s32 count = hRippleBlur->params.number;
    s32 i;

    for (i = 0; i < count; i++, slot++) {
        dds3EffectRipBlurPartInit(hRippleBlur, slot);
        slot->phase = -(hRippleBlur->params.interval * (f32)i);
    }
}

/* Allocate an owner and slot array from only the serialized parameter prefix. */
effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *pParam) {
    s32 count = pParam->number;
    void *allocation = sdfMemAllocDirect(count * 0x30 + 0x38);
    effBlurScaleWork_t *work = EE_ADDR_AS(effBlurScaleWork_t *, sdfMemLock(allocation));
    effBlurScaleSlot_t *slot;
    s32 i = 0;

    /* Copy through size, leaving the source handle, allocation and slots owned
       by this new work rather than reading a live-work tail from src. */
    memcpy(work, pParam, 0x2C);
    work->allocation = allocation;
    work->slots = (effBlurScaleSlot_t *)(work + 1);
    work->texture = dds3BillboardGetGeneralTex(3);
    slot = work->slots;
    while (i < count) {
        dds3EffectRipBlurPartInit(work, slot);
        slot->phase = -(work->params.interval * (f32)i);
        i++;
        slot++;
    }
    return work;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effPCPRippleBlurExpDeleteSub);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effBlurStepScaleSlotsAndDraw);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effBlurBuildSamplingQuad);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effBlurDrawFramebufferQuad);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectFilter);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effResourceQuadDraw);

/* Clone rectangle parameters and select a fresh source handle. */
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam) {
    dds3Effect_PartialMultiFilter_t *dst = sdfChipAllocDirect(sizeof(dds3Effect_PartialMultiFilter_t));

    dst->hAlphaTex = dds3BillboardGetGeneralTex(0);
    memcpy(&dst->param, pParam, sizeof(*pParam));
    return dst;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effBTLSysEncountDeleteSub);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effPCPMultiFilterExpDrawSub);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", effBlurStepScatterSlotsAndDrawTable);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", effBlurStepScatterSlotsAndDrawTable2);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrTestBed);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3Str012402D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrDamageD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrMltestD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrBtest0D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrTest2D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrTestD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrTestTmx2);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrWw02Tmx);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSmoke08Tmx);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSmoke07Tmx);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSmokeTmx);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSpview3P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSpview2P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSpviewP2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrMulti00P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrStage12P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrTestP2A2);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrMakatora020P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRing01D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRball01D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRball02D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrBball01D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki21BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki20BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki11BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki10BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki01BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki00BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki01D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki00D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrHouden0P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrDikira20P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSse3D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSse2D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSse1D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSse0D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSsf3D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSsf2D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSsf1D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSsf0D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRingf2D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRingf1D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRingf0D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSltfireP2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrHcTestPb);

