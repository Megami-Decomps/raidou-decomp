#include "shared/effect/effPCPFlash/decls.h"

effPCPFlashPolySphereExp_tag *effPCPFlashPolySphereExpCreate(effPCPFlashPolySphereExpParam_t *pParam)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(pParam->particleCount * sizeof(effPCPFlashPolySphereExpPart_tag) + sizeof(effPCPFlashPolySphereExp_tag));
    effPCPFlashPolySphereExp_tag *work = EE_ADDR_AS(effPCPFlashPolySphereExp_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    u32 range;
    u32 i;

    memcpy(work, pParam, 0x4C);
    work->parts = (effPCPFlashPolySphereExpPart_tag *)(work + 1);
    work->allocationHandle = handle;
    work->abgr = EFF_NEUTRAL_COLOR;
    work->scale = 1.0f;
    work->updateCount = 0;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = dds3FlashPolyListCreate(work->particleCount);
    work->hPoly = record;
    record->blend = work->drawMode;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
    }
    return work;
}
