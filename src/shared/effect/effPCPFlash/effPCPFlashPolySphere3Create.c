#include "shared/effect/effPCPFlash/decls.h"

effPCPFlashPolySphere3_tag *effPCPFlashPolySphere3Create(effFlashRadialStripParam_t *pParam)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(pParam->particleCount * sizeof(effPCPFlashPolySphere3Part_tag) + sizeof(effPCPFlashPolySphere3_tag));
    effPCPFlashPolySphere3_tag *work = EE_ADDR_AS(effPCPFlashPolySphere3_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    u32 range;
    u32 i;

    memcpy(work, pParam, 0xD0);
    work->parts = (effPCPFlashPolySphere3Part_tag *)(work + 1);
    work->allocationHandle = handle;
    work->abgr = EFF_NEUTRAL_COLOR;
    work->updateCount = 0;
    work->scale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = dds3FlashPolyList3Create(work->particleCount * 2);
    record->scale = 1.0f;
    record->blend = work->drawMode;
    work->hPoly = record;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
    }
    return work;
}
