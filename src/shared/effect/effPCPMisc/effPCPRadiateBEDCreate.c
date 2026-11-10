#include "shared/effect/effPCPMisc/decls.h"

effPCPRadiateBED_tag *effPCPRadiateBEDCreate(effPCPRadiateBEDParam_t *pParam, void *pBEDData) {
    u32 count = pParam->count;
    sdfMemHandle_t *handle = sdfMemAllocDirect(count * 24 + 0x11C);
    effPCPRadiateBED_tag *work = EE_ADDR_AS(void *, sdfMemLock(handle));
    effEveBEDParams_t place;
    effPCPRadiateBEDPart_tag *entry;
    s32 life;
    u32 i;

    work->param = *pParam;
    entry = (effPCPRadiateBEDPart_tag *)(work + 1);
    work->handle = handle;
    work->color = 0x80808080;
    work->original = 1;
    work->pPart = entry;
    work->scale = 1.0f;
    work->owner = effEveBEDCreate(pBEDData);
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
    life = work->param.timing;
    for (i = 0; i < count; i++) {
        entry->hBEDCut = effEveBEDCutCreate(work->owner, 2, &place);
        if (life > 0) {
            entry->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % life);
        } else {
            entry->frame = 0;
        }
        entry++;
    }
    return work;
}
