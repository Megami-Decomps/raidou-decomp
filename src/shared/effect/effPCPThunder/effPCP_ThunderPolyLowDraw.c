#include "shared/effect/effPCPThunder/decls.h"

void effPCP_ThunderPolyLowDraw(effPCP_ThunderPoly_t *hData) {
    s32 index = 0;
    parSystem_t *secondarySystem = hData->state.secondarySystem;
    s32 fragmentCount = hData->head.number;
    parSystem_t *primarySystem = hData->hPoly;
    parCell_t *primaryCells = primarySystem->cells;
    u32 tintColor = hData->color;
    effPCP_ThunderPolyPart_t *fragment = hData->pPart;
    parCell_t *secondaryCells = secondarySystem->cells;
    s32 packedColor;

    if (fragmentCount > 0) {
        do {
            if (fragment->wait == 0 && SDF_FRAME_DUE(dds3GetKernelCount())) {
                if (fragment->refresh != 0) {
                    effPCP_ThunderPolyLowPartRefresh(hData, index);
                    fragment->refresh--;
                } else if (fragment->abgr & EFF_THUNDER_ALPHA_MASK) {
                    fragment->abgr -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effPCP_ThunderPolyLowDrawOff(hData, index);
                    dds3TrackPolyListInit(hData->state.secondarySystem, index);
                    dds3TrackPolyListInit(hData->hPoly, index);
                }
            } else {
                fragment->wait -= SDF_FRAME_DUE(dds3GetKernelCount());
            }
            packedColor = effMagaCylinderCalcColor(fragment->abgr, tintColor);
            secondaryCells[index].color = packedColor;
            primaryCells[index].color = packedColor;
            fragment++;
            index++;
        } while (index < fragmentCount);
    }
    dds3TrackPolyDraw(hData->state.secondarySystem);
    dds3TrackPolyDraw(hData->hPoly);
}
