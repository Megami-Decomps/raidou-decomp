#ifndef EFFBLUR_FILTER_H
#define EFFBLUR_FILTER_H

#include "common.h"
#include "effect.h"

effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *pParam);
void dds3EffectFilter(void *);
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam);
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam);
effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *pParam);
void dds3EffectRippleBlurReset(effBlurScaleWork_t *hRippleBlur);
void effBlurStepScaleSlotsAndDraw(void *);
f32 effBlurGetBlurValid(void);
void effBlurSetBlurValid(f32 val);

#include "sdf_resource.h"

typedef struct dds3Effect_PartialMultiBlur_t {
    dds3Effect_PartialMultiBlurParam_t body;
    struct sdfTexHandle_t *texture;
} dds3Effect_PartialMultiBlur_t;

typedef char dds3Effect_PartialMultiBlurSizeCheck[sizeof(dds3Effect_PartialMultiBlur_t) == 0x30 ? 1 : -1];

typedef char effBlurScatterSlotSizeCheck[sizeof(effBlurScatterSlot_t) == 0x30 ? 1 : -1];

typedef char effBlurScatterWorkSizeCheck[sizeof(effBlurScatterWork_t) == 0x38 ? 1 : -1];

void effBlurStepScatterSlotsAndDraw(effBlurScatterWork_t *work);

void effBlurDrawFramebufferQuad(const dds3Effect_Blur_t *source);
void effBlurBuildSamplingQuad(const dds3Effect_Blur_t *source, effBlurPrimTriStrip_t *quad, u8 fixedPointCoordinates);
void effPCPDistortionBlurExpDeleteSub(effBlurScatterWork_t *hDistortionBlur);
void effPCPRippleBlurExpDeleteSub(effBlurScaleWork_t *hRippleBlur);
void effBTLSysEncountDeleteSub(dds3Effect_PartialMultiFilter_t *hPartialMultiFilter);

void effResourceQuadDraw(effResourceRectDrawParams_t *params, sdfTexHandle_t *resource, u8 gsCoordinates);
void dds3EffectBlurDrawFrameMaskReset(sdfDrawTag_t *list);
void dds3EffectBlurDrawAlphaMulti(sdfDrawTag_t *list, const dds3Effect_Blur_t *source, u8 fixedPointCoordinates);
void effCallNullDrawProc(const dds3Effect_Blur_t *source, sdfTexHandle_t *texture, u8 fixedPointCoordinates);
void effDrawBlurRectangle(const dds3Effect_Blur_t *source);
void effCallNullDrawProc2(effBlurTemplate_t *owner);
void effStopSub(effBlurTemplate_t *owner);
void effPCPMultiFilterExpDrawSub(dds3Effect_PartialMultiFilter_t *owner);

void effDrawBlurFixedPointRectangle(dds3Effect_PartialMultiBlur_t *owner);

#endif
