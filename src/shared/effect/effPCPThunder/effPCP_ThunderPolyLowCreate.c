#include "shared/effect/effPCPThunder/decls.h"

effPCP_ThunderPoly_t *effPCP_ThunderPolyLowCreate(effPCP_ThunderPolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->number * sizeof(effPCP_ThunderPolyPart_t) + sizeof(effPCP_ThunderPoly_t));
    effPCP_ThunderPoly_t *work = (effPCP_ThunderPoly_t *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->pPart = (effPCP_ThunderPolyPart_t *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->state.secondarySystem = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_TRIANGLE);
    dds3TrackPolyVCSet(work->state.secondarySystem, 2, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->state.secondarySystem, work->head.blend);
    work->hPoly = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(work->hPoly, 2, work->head.color_center, work->head.color_center);
    dds3TrackPolySetMode(work->hPoly, work->head.blend);
    for (i = 0; i < work->head.number; i++) {
        effPCP_ThunderPolyLowDrawOff(work, i);
    }
    work->color = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}
