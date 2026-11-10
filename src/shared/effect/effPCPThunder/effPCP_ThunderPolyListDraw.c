#include "shared/effect/effPCPThunder/decls.h"

void effPCP_ThunderPolyListDraw(effPCP_ThunderPolyList_t *hData) {
    s32 i = 0;
    s32 segmentCount = hData->head.count - 1;
    s32 j;
    s32 fragmentCount;
    u32 color;
    f32 (*points)[4] = hData->head.points;
    effPCP_ThunderPoly_t *work;
    effPCP_ThunderPoly_t *previous;
    parCell_t *cell;

    for (; i < segmentCount; i++) {
        work = hData->handles[i];
        SDF_VU0_COPYVECTOR(work->head.start, points[0]);
        SDF_VU0_COPYVECTOR(work->head.end, points[1]);
        points++;
        fragmentCount = work->head.number;
        color = effMagaCylinderCalcColor(hData->color, work->color);
        cell = work->hPoly->cells;
        for (j = 0; j < fragmentCount; j++) {
            if (i > 0) {
                previous = hData->handles[i - 1];
                SDF_FRAME_WHEN(dds3GetKernelCount(), effPCP_ThunderPolyListPartRefresh(work, j,
                    previous->hPoly->cells[j].history + previous->hPoly->cells[j].vertexCount - 5));
            } else {
                SDF_FRAME_WHEN(dds3GetKernelCount(), effPCP_ThunderPolyPartRefresh(work, j));
            }
            cell[j].color = color;
        }
        work->state.updateCount++;
        dds3TrackPolyDraw(work->hPoly);
    }
}
