#include "shared/effect/effEvent/decls.h"

/* Clone the four default work templates; only the staggered slot count is overridden. */
void Eve_EffectInit(void) {
    hEve_PartialMultiBlur = dds3EffectPartialMultiBlurCreate((dds3Effect_PartialMultiBlurParam_t *)mblurDefaultParam);
    hEve_DistortionBlur = dds3EffectDistortionBlurCreate((dds3Effect_DistortionBlurParam_t *)dblurDefaultParam);
    hEve_PartialMultiFilter = dds3EffectPartialMultiFilterCreate((dds3Effect_PartialMultiFilterParam_t *)mfilterDefaultParam);
    hEve_RippleBlur = dds3EffectRippleBlurCreate((dds3Effect_RippleBlurParam_t *)rblurDefaultParam);
    Eve_RippleBlurGetParams()->params.number = EFF_EVENT_STAGGERED_SLOT_COUNT;
}
