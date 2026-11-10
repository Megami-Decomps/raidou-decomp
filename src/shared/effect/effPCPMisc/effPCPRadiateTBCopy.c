#include "shared/effect/effPCPMisc/decls.h"

effPCPRadiateTB_tag *effPCPRadiateTBCopy(effPCPRadiateTB_tag *src) {
    u32 count = src->param.count;
    sdfMemHandle_t *handle = sdfMemAllocDirect(count * 32 + 0xA4);
    effPCPRadiateTB_tag *work = EE_ADDR_AS(void *, sdfMemLock(handle));
    effEveBEDParams_t place;
    effPCPRadiateTBPart_tag *entry;
    s32 life;
    u32 i;

    work->param = src->param;
    entry = (effPCPRadiateTBPart_tag *)(work + 1);
    work->hMem = handle;
    work->pPart = entry;
    work->abgr = 0x80808080;
    work->original = 0;
    work->hBED = src->hBED;
    work->ownerB = src->ownerB;
    place.center[0] = 0;
    place.center[1] = 0;
    place.center[2] = 0;
    place.center[3] = 0;
    place.rot[0] = 0;
    place.rot[1] = 0;
    place.rot[2] = 0;
    place.rot[3] = 1.0f;
    place.r = 100.0f;
    place.h = 100.0f;
    place.scale = 1.0f;
    place.abgr = 0x80808080;
    life = work->param.delaySpread;
    for (i = 0; i < count; i++) {
        entry->pThunderPoly = effPCP_ThunderPolyCreate(&src->param.fragmentParams);
        entry->eventA = effEveBEDCutCreate(src->hBED, 2, &place);
        entry->eventB = effEveBEDCutCreate(src->ownerB, 2, &place);
        entry->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % life);
        entry++;
    }
    return work;
}
