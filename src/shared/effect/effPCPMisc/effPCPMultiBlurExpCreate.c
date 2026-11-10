#include "shared/effect/effPCPMisc/decls.h"

effPCPMultiBlurExp_tag *effPCPMultiBlurExpCreate(effPCPBlinkMultiBlurParam_t *params) {
    effPCPMultiBlurExp_tag *work = sdfChipAllocDirect(sizeof(effPCPMultiBlurExp_tag));

    work->hBlur = dds3EffectPartialMultiBlurCreate(&params->res);
    work->abgr_base = params->res.Blur.abgr;
    work->frame = 0;
    work->abgr = 0x80808080;
    work->pos[0] = 0.0f;
    work->pos[1] = 0.0f;
    work->pos[2] = 0.0f;
    work->area = 0.0f;
    work->blink = params->count;
    work->blink_abgr[0] = params->colors[0];
    work->blink_abgr[1] = params->colors[1];
    work->ratio = params->scale;
    work->area_min = params->minSize;
    return work;
}
