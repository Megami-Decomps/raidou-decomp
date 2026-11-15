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

#endif
