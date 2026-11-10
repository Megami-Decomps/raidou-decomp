#include "shared/effect/effPCPMisc/decls.h"

void effPCPRadiateThunderDelete(effPCPRadiateThunder_tag *hData) {
    u32 releaseIndex = 0;
    u32 releaseCount = hData->param.number;
    effPCPRadiateThunderPart_tag *fragmentEntryCursor = hData->pPart;
    effPartHandle_t **duplicateHandleBase;
    effPartHandle_t **duplicateHandleCursor;

    if (releaseCount != 0) {
        do {
            effPCP_ThunderPoly_t *currentHandle = fragmentEntryCursor->pThunderPoly;
            fragmentEntryCursor++;
            releaseIndex++;
            effPCP_ThunderPolyDelete(currentHandle);
        } while (releaseIndex < releaseCount);
    }
    duplicateHandleBase = hData->phAttach;
    releaseCount = releaseCount * 4;
    if (duplicateHandleBase != NULL) {
        duplicateHandleCursor = duplicateHandleBase;
        releaseIndex = 0;
        if (releaseCount != 0) {
            do {
                effPartHandle_t *currentHandle = *duplicateHandleCursor++;
                if (currentHandle != 0) {
                    dds3EffectPartDelete(currentHandle);
                }
                releaseIndex++;
            } while (releaseIndex < releaseCount);
        }
        sdfMemFreeDirect(hData->hAttachMem);
    }
    sdfMemFreeDirect(hData->hMem);
}
