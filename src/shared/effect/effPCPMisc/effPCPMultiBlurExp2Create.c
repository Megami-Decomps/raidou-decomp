#include "shared/effect/effPCPMisc/decls.h"

effPCPMultiBlurExp2_tag *effPCPMultiBlurExp2Create(effPCPMultiBlurExp2Param_t *pParam) {
    effPCPMultiBlurExp2_tag *work;

    work = sdfChipAllocDirect(sizeof(effPCPMultiBlurExp2_tag));
    work->resource.blur = dds3EffectPartialMultiBlurCreate(&pParam->MultiBlur);
    work->frame = 0;
    work->color = 0x80808080;
    work->flags = pParam->type;
    work->duration = pParam->life;
    work->fadeIn = pParam->fade_in;
    work->fadeOut = pParam->fade_out;
    work->startExtent = pParam->area_s;
    work->endExtent = pParam->area_e;
    work->baseColor = pParam->MultiBlur.Blur.abgr;
    return work;
}
