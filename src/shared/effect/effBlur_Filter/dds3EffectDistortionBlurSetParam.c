#include "shared/effect/effBlur_Filter/decls.h"

/* Update scale parameters but preserve the allocated destination slot count. */
void dds3EffectDistortionBlurSetParam(effBlurScaleWork_t *hDistortionBlur, dds3Effect_RippleBlurParam_t *pParam) {
    u32 count = hDistortionBlur->params.number;

    hDistortionBlur->params = *pParam;
    hDistortionBlur->params.number = count;
}
