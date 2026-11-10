#include "shared/effect/effPCPThunder/decls.h"

void effPCP_ThunderPolyDraw(effPCP_ThunderPoly_t *hData) {
    s32 i = 0;
    parSystem_t *system = hData->hPoly;
    s32 count = hData->head.number;
    u32 restartLimit = SDF_FRAMES(hData->head.restartFrameLimit);
    u32 tintColor = hData->color;
    u32 updateCount = hData->state.updateCount;
    effPCP_ThunderPolyPart_t *fragment = hData->pPart;
    parCell_t *renderCells = system->cells;
    s32 canRestart;

    if (count > 0) {
        canRestart = updateCount < restartLimit;
        do {
            if (fragment->wait == 0 && SDF_FRAME_DUE(dds3GetKernelCount())) {
                if (fragment->refresh != 0) {
                    effPCP_ThunderPolyPartRefresh(hData, i);
                    fragment->refresh--;
                } else if (fragment->abgr & EFF_THUNDER_ALPHA_MASK) {
                    fragment->abgr += EFF_THUNDER_ALPHA_WRAP_ADD;
                } else if (restartLimit == 0 || canRestart) {
                    effPCP_ThunderPolyDrawOff(hData, i);
                    dds3TrackPolyListInit(hData->hPoly, i);
                } else {
                    renderCells[i].vertexCount = 0;
                }
            } else {
                fragment->wait -= SDF_FRAME_DUE(dds3GetKernelCount());
            }
            renderCells[i].color = effMagaCylinderCalcColor(fragment->abgr, tintColor);
            i++;
            fragment++;
        } while (i < count);
        updateCount = hData->state.updateCount;
    }
    hData->state.updateCount = updateCount + 1;
    dds3TrackPolyDraw(hData->hPoly);
}
