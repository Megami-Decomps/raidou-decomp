#include "shared/effect/effBlur_Filter/decls.h"

/* Clone a blur template into a fresh allocation. */
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam) {
    effBlurTemplate_t *dst = sdfChipAllocDirect(sizeof(effBlurTemplate_t));

    dst->texture = dds3BillboardGetGeneralTex(2);
    memcpy(&dst->body, pParam, sizeof(*pParam));
    return dst;
}
