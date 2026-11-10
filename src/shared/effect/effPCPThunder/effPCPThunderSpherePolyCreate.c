#include "shared/effect/effPCPThunder/decls.h"

effPCPThunderSpherePoly_t *effPCPThunderSpherePolyCreate(effPCPThunderSpherePolyParam_t *source) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(source->cellCount * sizeof(effPCPThunderSpherePolyPart_t) + sizeof(effPCPThunderSpherePoly_t));
    effPCPThunderSpherePoly_t *work = (effPCPThunderSpherePoly_t *)sdfMemLock(allocation);
    u32 cellIndex;

    work->head = *source;
    work->pPart = (effPCPThunderSpherePolyPart_t *)(work + 1);
    work->width_base = source->width;
    work->height_base = source->height;
    work->allocationHandle = allocation;
    work->hPoly = dds3TrackPolyCreate(work->head.cellCount, work->head.perCell, 0, PAR_CELL_TOPOLOGY_FIVE_VECTOR);
    dds3TrackPolyVC5Set_AcuteW(work->hPoly, work->head.color_center, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->hPoly, work->head.systemParam);
    for (cellIndex = 0; cellIndex < work->head.cellCount; cellIndex++) {
        work->pPart[cellIndex].wait = 0;
        work->pPart[cellIndex].refresh = 0;
        work->pPart[cellIndex].abgr = 0;
    }
    work->abgr = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}
