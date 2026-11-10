#include "shared/effect/effPCPFlash/decls.h"

effPCPFlashPolySphere2_tag *effPCPFlashPolySphere2Create(effPCPFlashPolySphere2Param_t *pParam)
{
    sdfMemHandle_t *allocation = sdfMemAllocDirect(pParam->number * sizeof(effPCPFlashPolySphere2Part_tag) + sizeof(effPCPFlashPolySphere2_tag));
    effPCPFlashPolySphere2_tag *work = EE_ADDR_AS(effPCPFlashPolySphere2_tag *, sdfMemLock(allocation));
    dds3FlashPolyList_t *record;
    u32 i;

    memcpy(work, pParam, 0x30);
    work->pPart = (effPCPFlashPolySphere2Part_tag *)(work + 1);
    work->abgr = EFF_NEUTRAL_COLOR;
    work->allocationHandle = allocation;
    work->scale = 1.0f;
    work->frame = 0;
    record = dds3FlashPolyList2Create(work->particleCount);
    work->hPoly = record;
    record->blend = work->drawMode;
    for (i = 0; i < work->particleCount; i++) {
        work->pPart[i].frame = 0;
    }
    return work;
}
