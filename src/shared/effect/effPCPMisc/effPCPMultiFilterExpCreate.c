#include "shared/effect/effPCPMisc/decls.h"

effPCPMultiBlurExp2_tag *effPCPMultiFilterExpCreate(effPCPMultiFilterExpParam_t *params) {
    effPCPMultiBlurExp2_tag *work;

    work = sdfChipAllocDirect(sizeof(effPCPMultiBlurExp2_tag));
    work->resource.rectangle = dds3EffectPartialMultiFilterCreate(&params->res);
    work->frame = 0;
    work->color = 0x80808080;
    work->flags = params->type;
    work->duration = params->life;
    work->fadeIn = params->fade_in;
    work->fadeOut = params->fade_out;
    work->startExtent = params->area_s;
    work->endExtent = params->area_e;
    work->baseColor = params->res.Filter.rgba;
    return work;
}
