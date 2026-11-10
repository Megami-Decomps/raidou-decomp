#include "shared/effect/effPCPThunder/decls.h"

void effPCP_ThunderRadiatePolyDraw(effPCP_ThunderRadiatePoly_t *hData) {
    s32 i = 0;
    s32 cellCount = hData->head.number;
    effPCP_ThunderRadiatePolyPart_t *cell = hData->pPart;
    parCell_t *renderCells = hData->hPoly->cells;

    if (cellCount > 0) {
        do {
            if (cell->delayFrames == 0 && SDF_FRAME_DUE(dds3GetKernelCount())) {
                if (cell->activeFrames != 0) {
                    effPCP_ThunderRadiatePolyPartRefresh(hData, i);
                    cell->activeFrames--;
                } else if (renderCells[i].color & EFF_THUNDER_ALPHA_MASK) {
                    renderCells[i].color -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effPCP_ThunderRadiatePolyDrawOff(hData, i);
                    dds3TrackPolyListInit(hData->hPoly, i);
                }
            } else {
                cell->delayFrames -= SDF_FRAME_DUE(dds3GetKernelCount());
            }
            cell++;
            i++;
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(hData->hPoly);
}
