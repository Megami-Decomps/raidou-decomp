#include "shared/effect/effPCPMisc/decls.h"

effPCPBlurExp_tag *effPCPBlurExpCreate(effPCPBlurExpParam_t *pParam) {
    effPCPBlurExp_tag *work = sdfChipAllocDirect(sizeof(effPCPBlurExp_tag));

    work->param = *pParam;
    work->abgr = 0x80808080;
    work->frame = 0;
    work->abgr_base = pParam->Blur.abgr;
    return work;
}
