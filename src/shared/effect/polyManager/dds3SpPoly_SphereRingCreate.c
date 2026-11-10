#include "shared/effect/polyManager/decls.h"

dds3SpPoly_SphereRing_t3 *dds3SpPoly_SphereRingCreate(dds3SpPoly_SphereRing_t3 *source) {
    s32 tailBytes = sizeof(dds3SpPoly_SphereRing_t3) - sizeof(dds3SpPoly_Basic_t);
    s32 size = source->head.number * sizeof(dds3SpPoly_SphereRingPhysics_t) + sizeof(dds3SpPoly_Basic_t) + tailBytes;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size);
    dds3SpPoly_SphereRing_t3 *clone = EE_ADDR_AS(dds3SpPoly_SphereRing_t3 *, sdfMemLock(allocation));

    memset(clone, 0, size);
    memcpy(clone, source, source->head.templateSize);
    memcpy(&clone->spawnDelayStep, (u8 *)source + source->head.templateSize, tailBytes);
    clone->head.templateSize = 0xC0;
    clone->pPhysics = (dds3SpPoly_SphereRingPhysics_t *)(clone + 1);
    clone->hMem = allocation;
    if (((dds3SpPolyRingUnion_t *)clone)->arc.vertex < 3) {
        ((dds3SpPolyRingUnion_t *)clone)->arc.vertex = 3;
    }
    clone->hTrackPoly = dds3TrackPolyCreate(clone->head.number, clone->vertex, 1, 0);
    dds3TrackPolyVCSet(clone->hTrackPoly, 0, clone->dispatchArgA, clone->dispatchArgB);
    dds3SpPoly_SphereRingAllBuffInit(clone);
    SDF_VU0_LOADMATRIX(clone->head.matrix);
    SDF_VU0_STOREMATRIX(clone->head.matrixCopy);
    dds3TrackPolySetMode(clone->hTrackPoly, clone->head.mode);
    return clone;
}
