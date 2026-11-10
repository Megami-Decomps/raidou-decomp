#include "shared/effect/effPCPThunder/decls.h"

effPCP_ThunderRadiatePoly_t *effPCP_ThunderRadiatePolyCreate(effPCP_ThunderRadiatePolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->number * sizeof(effPCP_ThunderRadiatePolyPart_t) + sizeof(effPCP_ThunderRadiatePoly_t));
    effPCP_ThunderRadiatePoly_t *work = (effPCP_ThunderRadiatePoly_t *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->pPart = (effPCP_ThunderRadiatePolyPart_t *)(work + 1);
    work->hMem = allocationHandle;
    work->hPoly = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_SIX_VECTOR);
    dds3TrackPolyVC3Set_Acute(work->hPoly, work->head.color_center, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->hPoly, work->head.blend);
    for (i = 0; i < work->head.number; i++) {
        effPCP_ThunderRadiatePolyDrawOff(work, i);
    }
    return work;
}
