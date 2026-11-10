#include "shared/effect/effPCPMisc/decls.h"

void effPCPHama01Delete(effPCPHama01_tag *hData) {
    s32 pairIndex;
    effPartHandle_t **pairHandleCursor;
    effPartHandle_t **sharedHandleCursor;

    pairHandleCursor = &hData->pair[0][0];
    sharedHandleCursor = hData->shared;
    for (pairIndex = 7; pairIndex >= 0; pairIndex--) {
        dds3EffectPartDelete(*sharedHandleCursor);
        sharedHandleCursor++;
        dds3EffectPartDelete(pairHandleCursor[1]);
        dds3EffectPartDelete(pairHandleCursor[0]);
        pairHandleCursor += 2;
    }
    sdfChipFreeDirect(hData);
}
