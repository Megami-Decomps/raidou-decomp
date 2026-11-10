#include "shared/effect/effBlur_Filter/decls.h"

/* Clone rectangle parameters and select a fresh source handle. */
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam) {
    dds3Effect_PartialMultiFilter_t *dst = sdfChipAllocDirect(sizeof(dds3Effect_PartialMultiFilter_t));

    dst->hAlphaTex = dds3BillboardGetGeneralTex(0);
    memcpy(&dst->param, pParam, sizeof(*pParam));
    return dst;
}
