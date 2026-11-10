#include "shared/effect/effPCPThunder/decls.h"

effPCP_ThunderPoly_t *effPCP_ThunderPolyCreate(effPCP_ThunderPolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->number * sizeof(effPCP_ThunderPolyPart_t) + sizeof(effPCP_ThunderPoly_t));
    effPCP_ThunderPoly_t *work = (effPCP_ThunderPoly_t *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->pPart = (effPCP_ThunderPolyPart_t *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->hPoly = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_FIVE_VECTOR);
    dds3TrackPolyVC5Set_AcuteW(work->hPoly, work->head.color_center, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->hPoly, work->head.blend);
    for (i = 0; i < work->head.number; i++) {
        effPCP_ThunderPolyDrawOff(work, i);
    }
    work->state.updateCount = 0;
    work->color = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}
