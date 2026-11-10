/* Declarations of smt3-decomp src/smt3/effect/effBlur_Filter.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFBLUR_FILTER_H
#define SHARED_EFFECT_EFFBLUR_FILTER_H

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

#endif
