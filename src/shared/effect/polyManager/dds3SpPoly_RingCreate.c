#include "shared/effect/polyManager/decls.h"

dds3SpPoly_Ring_t3 *dds3SpPoly_RingCreate(dds3SpPoly_Basic_t *templateHead) {
    s32 tailSize = sizeof(dds3SpPoly_Ring_t3) - sizeof(dds3SpPoly_Basic_t);
    u32 size = templateHead->number * sizeof(dds3SpPoly_RingPhysics_t) + sizeof(dds3SpPoly_Ring_t3);
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size);
    dds3SpPoly_Ring_t3 *band = EE_ADDR_AS(dds3SpPoly_Ring_t3 *, sdfMemLock(allocation));

    memset(band, 0, size);
    memcpy(band, templateHead, templateHead->templateSize);
    memcpy(&band->spawnDelayStep, (u8 *)templateHead + templateHead->templateSize, tailSize);
    band->head.templateSize = sizeof(dds3SpPoly_Basic_t);
    band->pPhysics = (dds3SpPoly_RingPhysics_t *)(band + 1);
    band->hMem = allocation;
    if (((dds3SpPolyRingUnion_t *)band)->band.vertex < 3) {
        ((dds3SpPolyRingUnion_t *)band)->band.vertex = 3;
    }
    band->hTrackPoly = dds3TrackPolyCreate(band->head.number, band->vertex, 1, 0);
    dds3TrackPolyVCSet(band->hTrackPoly, 0, band->dispatchArgA, band->dispatchArgB);
    dds3SpPoly_RingAllBuffInit(band);
    SDF_VU0_LOADMATRIX(band->head.matrix);
    SDF_VU0_STOREMATRIX(band->head.matrixCopy);
    dds3TrackPolySetMode(band->hTrackPoly, band->head.mode);
    return band;
}
