#include "shared/effect/effPCPFlash/decls.h"

effPCPFlashPolySphere_tag *effPCPFlashPolySphereCreate(effPCPFlashPolySphereParam_t *src) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(src->number * sizeof(effPCPFlashPolySpherePart_tag) + sizeof(effPCPFlashPolySphere_tag));
    effPCPFlashPolySphere_tag *work = EE_ADDR_AS(effPCPFlashPolySphere_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    u32 range;
    u32 particleIndex;

    memcpy(work, src, 0x40);
    work->pPart = (effPCPFlashPolySpherePart_tag *)(work + 1);
    work->allocationHandle = handle;
    work->abgr = EFF_NEUTRAL_COLOR;
    work->scale = 1.0f;
    work->frame = 0;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = dds3FlashPolyListCreate(work->particleCount);
    work->hPoly = record;
    record->blend = work->drawMode;
    range = work->randomRange;
    for (particleIndex = 0; particleIndex < work->particleCount; particleIndex++) {
        work->pPart[particleIndex].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
    }
    return work;
}
