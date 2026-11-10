#include "shared/effect/effPCPMisc/decls.h"

effPCPFilterExp_tag *effPCPFilterExpCreate(effPCPFilterExpParam_t *pParam) {
    effPCPFilterExp_tag *work = sdfChipAllocDirect(sizeof(effPCPFilterExp_tag));

    work->param = *pParam;
    work->abgr = 0x80808080;
    work->frame = 0;
    work->abgr_base = pParam->Filter.rgba;
    return work;
}
