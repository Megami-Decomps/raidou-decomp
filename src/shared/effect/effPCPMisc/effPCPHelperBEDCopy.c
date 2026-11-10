#include "shared/effect/effPCPMisc/decls.h"

effPCPHelperBED_tag *effPCPHelperBEDCopy(effPCPHelperBED_tag *src) {
    effPCPHelperBED_tag *work;

    work = effPCPHelperBEDCreate(&src->param, 0, 0);
    work->hHelper = dds3EffectPartCopy(src->hHelper);
    work->hBED = src->hBED;
    effPCPHelperInit(work);
    return work;
}
