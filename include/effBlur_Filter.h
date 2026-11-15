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

#include "sdf_resource.h"

typedef struct EffBlurTemplate {
    dds3Effect_PartialMultiBlurParam_t body;
    struct sdfTexHandle_t *texture;
} EffBlurTemplate;

typedef char EffBlurTemplateSizeCheck[sizeof(EffBlurTemplate) == 0x30 ? 1 : -1];

typedef struct EffBlurScatterSlot {
    s32 delay;
    f32 angle;
    dds3Effect_Blur_t quad;
} EffBlurScatterSlot;

typedef struct EffBlurScatterWork {
    dds3Effect_DistortionBlurParam_t params;
    struct sdfTexHandle_t *texture;
    struct sdfMemHandle_t *allocation;
    EffBlurScatterSlot *slots;
} EffBlurScatterWork;

typedef char EffBlurScatterSlotSizeCheck[sizeof(EffBlurScatterSlot) == 0x30 ? 1 : -1];

typedef char EffBlurScatterWorkSizeCheck[sizeof(EffBlurScatterWork) == 0x38 ? 1 : -1];

void effBlurStepScatterSlotsAndDraw(EffBlurScatterWork *work);

void effBlurDrawFramebufferQuad(const dds3Effect_Blur_t *source);

void effDrawBlurFixedPointRectangle(EffBlurTemplate *owner);

#endif
