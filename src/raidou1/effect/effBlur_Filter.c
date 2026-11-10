#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effBlur_Filter.c */
#include "common.h"

#include "sdk_libgcc.h"

#include "libsdf.h"

#include "effect.h"

#include "sdfFrame.h"

extern sdfGrOt_t sdfGrOtTable[];

extern u64 *effPkt2DAllocF4(u32 prim);

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

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

void *sdfChipAllocDirect(s32 aSize);

void sdfDrawTagLink(struct sdfDrawTag_t *aTag, addr_t aPacket);

void sdfDrawTagInit(struct sdfDrawTag_t *a);

void sdfChipFreeDirect(void *aPtr);

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

extern u32 dds3GetFrameID(void);

extern u32 dds3NullDrawProc(void);

extern void sdfGrSetFrameMaskReset(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, addr_t aPacket);

extern void sdfGrLinkDispFrameTex(sdfDrawTag_t *aTag, addr_t aEnv, void *aPkt);

extern void sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);

extern void sdfGrSetFrameMaskRGB(const u8 *aEnv, void *aHdr, s32 aContext);

extern f32 sdfSin(f32 aRadius);

extern sdfGrPacketHeader_t *effPkt2DAllocTriStrip(s32 num, s32 prim);

extern void *effPkt2DGetPrimPtr(void *pPkt);

/* The frame buffers the blur samples from; only the mask source is named. */
typedef struct effBlurDrawBank_t {
    u8 pad0000[0x1300];
    u8 frameMaskSource[0x8E0];
} effBlurDrawBank_t;

extern effBlurDrawBank_t dds3DrawEnv[];

void dds3EffectBlurDrawQuad(const dds3Effect_Blur_t *pBlur, effBlurPrimTriStrip_t *pPrim);

void dds3EffectBlurDrawFrameMask(sdfDrawTag_t *list, s32 blendControl, sdfTexHandle_t *texture);

void dds3EffectBlurDrawAlphaMulti(sdfDrawTag_t *list, const dds3Effect_Blur_t *source);

void dds3EffectBlurDrawFrameMaskReset(sdfDrawTag_t *list);

void dds3EffectBlurDraw(const dds3Effect_Blur_t *source, sdfTexHandle_t *texture);

void dds3EffectFilterDrawTexQuad(effResourceRectDrawParams_t *source, sdfTexHandle_t *texture);

/* Build the alpha-multiply tri-strip: a flat grey quad over the blur rect. */
void dds3EffectBlur_AlphaMultiParmSet(const dds3Effect_Blur_t *pBlur, effBlurPrimTriStrip_t *pSpriteParm);

/* Borrow the display frame as a texture, set the two blend states and the
   sampling window, then draw the source quad through it. */
void dds3EffectBlur(const dds3Effect_Blur_t *source);

/* Point the sampler at this frame's mask, queue the blend state the blur
   needs, then hand the display frame back as the second source. */
void dds3EffectBlurDrawFrameMask(sdfDrawTag_t *list, s32 blendControl, sdfTexHandle_t *texture);

/* Two tri-strips: the alpha-multiply pass, then the blurred source quad. */
void dds3EffectBlurDrawAlphaMulti(sdfDrawTag_t *list, const dds3Effect_Blur_t *source);

/* Restore the frame mask this frame's blur borrowed, then queue the list. */
void dds3EffectBlurDrawFrameMaskReset(sdfDrawTag_t *list);

void dds3EffectBlurDraw(const dds3Effect_Blur_t *source, sdfTexHandle_t *texture);

void dds3EffectBlurSetRect(dds3Effect_PartialMultiBlurParam_t *work);

/* Clone a blur template into a fresh allocation. */
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam);

void dds3EffectPartialMultiBlurDelete(effBlurTemplate_t *hPartialMultiBlur);

void dds3EffectPartialMultiBlurDraw(effBlurTemplate_t *work);

/* Copy only the serialized parameters, leaving the owned work tail intact. */
void dds3EffectPartialMultiBlurSetParam(effBlurTemplate_t *hPartialMultiBlur, const dds3Effect_PartialMultiBlurParam_t *pParam);

/* Supply a source handle for the first blur variant. */
void dds3EffectPartialMultiBlurSetTex(effBlurScatterWork_t *hPartialMultiBlur, struct sdfTexHandle_t *hTex);

/* Acquire the first variant's source handle from the effect manager. */
void dds3EffectPartialMultiBlurSetTexDef(effBlurScatterWork_t *hPartialMultiBlur);

void dds3EffectDistBlurPartInit(effBlurScatterWork_t *pParam, effBlurScatterSlot_t *pPart);

effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *pParam);

/* Release the first variant's owned effect resource. */
void dds3EffectDistortionBlurDelete(effBlurScatterWork_t *hDistortionBlur);

/* Each live slot waits out its delay, then fades as its angle leaves zero.
 * Adapted from the matched DDS/redthing1 effBlurStepScatterSlotsAndDraw.
 * Keep SMT3's two-argument renderer and canonical soft-double helpers:
 * dpcmp compares doubles; the pi and reciprocal literals are promoted floats. */
void dds3EffectDistortionBlurDraw(effBlurScatterWork_t *work);

/* Update scale parameters but preserve the allocated destination slot count. */
void dds3EffectDistortionBlurSetParam(effBlurScaleWork_t *hDistortionBlur, dds3Effect_RippleBlurParam_t *pParam);

/* The scale variant has its own source-handle setter. */
void func_001A2668(effBlurScaleWork_t *work, struct sdfTexHandle_t *sourceHandle);

/* Both acquisition callbacks request selector 2; the second factory uses 3. */
void dds3EffectDistortionBlurSetTex(effBlurScaleWork_t *work);

/* Size the slot's quad from its current phase, about the work centre. */
void dds3EffectRipBlurPartAreaSet(effBlurScaleWork_t *pParam, effBlurScaleSlot_t *pPart);

/* Reset a slot for a new burst: zero phase and angle, then copy the colour, the two spare fields and the centre from the work parameters. */
void dds3EffectRipBlurPartInit(effBlurScaleWork_t *pParam, effBlurScaleSlot_t *pPart);

/* Reset every slot, then stagger its initial phase by -spacing * index. */
void dds3EffectRippleBlurReset(effBlurScaleWork_t *hRippleBlur);

/* Allocate an owner and slot array from only the serialized parameter prefix. */
effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *pParam);

/* Release the scale variant's allocation, including its inline slot array. */
void dds3EffectRippleBlurDelete(effBlurScaleWork_t *hRippleBlur);

/* Phase-driven sibling of the distortion update; adapted from DDS/redthing1
 * effBlurStepScaleSlotsAndDraw. SMT3 preserves the slot's RGB and uses the
 * two-argument renderer. Extract then attenuate the same alpha quantity. */
void dds3EffectRippleBlurDraw(effBlurScaleWork_t *work);

/* Queue blend state and a uniformly colored GS quad from two source corners. */
void dds3EffectFilter(dds3Effect_Filter_t *source);

/* One textured quad over the source rectangle, tinted by its own RGBA. */
void dds3EffectFilterDrawTexQuad(effResourceRectDrawParams_t *source, sdfTexHandle_t *texture);

/* Clone rectangle parameters and select a fresh source handle. */
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam);

void dds3EffectPartialMultiFilterDelete(dds3Effect_PartialMultiFilter_t *hPartialMultiFilter);

void dds3EffectPartialMultiFilterDraw(dds3Effect_PartialMultiFilter_t *hPartialMultiFilter);
/* End of declarations from src/smt3/effect/effBlur_Filter.c */

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F81D0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8278);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F82F0);

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

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8488);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F84A0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8518);

/* Copy only the serialized parameters, leaving the owned work tail intact. */
void dds3EffectPartialMultiBlurSetParam(effBlurTemplate_t *hPartialMultiBlur, const dds3Effect_PartialMultiBlurParam_t *pParam) {
    hPartialMultiBlur->body = *pParam;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F85F8);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8600);

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

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8850);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8868);

/* Update scale parameters but preserve the allocated destination slot count. */
void dds3EffectDistortionBlurSetParam(effBlurScaleWork_t *hDistortionBlur, dds3Effect_RippleBlurParam_t *pParam) {
    u32 count = hDistortionBlur->params.number;

    hDistortionBlur->params = *pParam;
    hDistortionBlur->params.number = count;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8B10);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8B18);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8B48);

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

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8D88);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8DA0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F8F90);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F91A8);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectFilter);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F95E0);

/* Clone rectangle parameters and select a fresh source handle. */
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam) {
    dds3Effect_PartialMultiFilter_t *dst = sdfChipAllocDirect(sizeof(dds3Effect_PartialMultiFilter_t));

    dst->hAlphaTex = dds3BillboardGetGeneralTex(0);
    memcpy(&dst->param, pParam, sizeof(*pParam));
    return dst;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F9880);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001F9898);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455968);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455970);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455978);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455988);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455998);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_004559A8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_004559B8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_004559C8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_004559D8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_004559E8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_004559F8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455A08);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455A18);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455A28);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455A38);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455A48);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455A58);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455A68);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455A78);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455A88);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455A98);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455AA8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455AB8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455AC8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455AD8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455AE8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455AF8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455B08);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455B18);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455B28);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455B38);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455B48);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455B58);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455B68);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455B78);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455B88);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455B98);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455BA8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455BB8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455BC8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455BD8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455BE8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455BF8);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455C08);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455C18);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455C28);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455C38);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00455C48);

