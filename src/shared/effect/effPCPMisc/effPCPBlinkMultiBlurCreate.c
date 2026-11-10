#include "shared/effect/effPCPMisc/decls.h"

effPCPBlinkMultiBlur_tag *effPCPBlinkMultiBlurCreate(effPCPBlinkMultiBlurParam_t *params) {
    effPCPBlinkMultiBlur_tag *ref = sdfChipAllocDirect(sizeof(*ref));

    ref->frame = 0;
    if (BlinkMultiBlurCount == 0) {
        pBlinkMultiBlurBase = sdfChipAllocDirect(sizeof(effPCPBlinkMultiBlurBase_tag));
        pBlinkMultiBlurBase->hBlur = dds3EffectPartialMultiBlurCreate(&params->res);
        pBlinkMultiBlurBase->abgr_base = params->res.Blur.abgr;
        pBlinkMultiBlurBase->frame = 0;
        pBlinkMultiBlurBase->abgr = 0x80808080;
        pBlinkMultiBlurBase->pos[0] = 0;
        pBlinkMultiBlurBase->pos[1] = 0;
        pBlinkMultiBlurBase->pos[2] = 0;
        pBlinkMultiBlurBase->area = 0;
        pBlinkMultiBlurBase->blink = params->count;
        pBlinkMultiBlurBase->blink_base = params->count;
        pBlinkMultiBlurBase->blink_abgr[0] = params->colors[0];
        pBlinkMultiBlurBase->blink_abgr[1] = params->colors[1];
        pBlinkMultiBlurBase->ratio = params->scale;
        pBlinkMultiBlurBase->area_min = params->minSize;
    }
    BlinkMultiBlurCount++;
    return ref;
}
