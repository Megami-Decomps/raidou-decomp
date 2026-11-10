#include "shared/effect/effPCPFlash/decls.h"

effPCPFlashPolySphere2Exp_tag *effPCPFlashPolySphere2ExpCreate(effPCPFlashPolySphere2ExpParam_t *pParam)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(pParam->particleCount * sizeof(effPCPFlashPolyRingExpPart_tag) + sizeof(effPCPFlashPolySphere2Exp_tag));
    effPCPFlashPolySphere2Exp_tag *work = EE_ADDR_AS(effPCPFlashPolySphere2Exp_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    u32 i;

    memcpy(work, pParam, 0x3C);
    work->parts = (effPCPFlashPolyRingExpPart_tag *)(work + 1);
    work->abgr = EFF_NEUTRAL_COLOR;
    work->allocationHandle = handle;
    work->scale = 1.0f;
    work->updateCount = 0;
    record = dds3FlashPolyList2Create(work->particleCount);
    work->hPoly = record;
    record->blend = work->drawMode;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].frame = 0;
    }
    return work;
}
