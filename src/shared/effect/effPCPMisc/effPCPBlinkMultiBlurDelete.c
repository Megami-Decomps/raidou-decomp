#include "shared/effect/effPCPMisc/decls.h"

void effPCPBlinkMultiBlurDelete(effPCPBlinkMultiBlur_tag *hData)
{
    sdfChipFreeDirect(hData);
    if (--BlinkMultiBlurCount != 0) {
        return;
    }
    dds3EffectPartialMultiBlurDelete(pBlinkMultiBlurBase->hBlur);
    sdfChipFreeDirect(pBlinkMultiBlurBase);
}
