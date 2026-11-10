#include "shared/effect/effPCPThunder/decls.h"

effPCPThunderSpherePoly_t *effBTLBindCreateTrack(effPCPThunderSpherePolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->cellCount * sizeof(effPCPThunderSpherePolyPart_t) + sizeof(effPCPThunderSpherePoly_t));
    effPCPThunderSpherePoly_t *work = (effPCPThunderSpherePoly_t *)sdfMemLock(allocationHandle);
    u32 cellIndex;

    work->head = *parameters;
    work->pPart = (effPCPThunderSpherePolyPart_t *)(work + 1);
    work->width_base = parameters->width;
    work->height_base = parameters->height;
    work->allocationHandle = allocationHandle;
    work->hPoly = dds3TrackPolyCreate(work->head.cellCount, work->head.perCell, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(work->hPoly, 2, work->head.color_border, work->head.color_border);
    dds3TrackPolySetMode(work->hPoly, work->head.systemParam);
    for (cellIndex = 0; cellIndex < work->head.cellCount; cellIndex++) {
        work->pPart[cellIndex].wait = 0;
        work->pPart[cellIndex].refresh = 0;
        work->pPart[cellIndex].abgr = 0;
    }
    work->abgr = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}
