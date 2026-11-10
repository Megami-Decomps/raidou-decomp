#include "shared/effect/effPCPMisc/decls.h"

void effPCPAgiFireballDelete(effPCPAgiFireball_tag *hData) {
    u32 groupIndex;
    u32 handleIndex;
    u32 groupHandleCount;

    if (hData->source == NULL) {
        dds3EffectPartDelete(hData->headHandle);
        for (handleIndex = 0; handleIndex < 5; handleIndex++) {
            dds3EffectPartDelete(hData->handleA[handleIndex]);
        }
        for (groupIndex = 0; groupIndex < 3; groupIndex++) {
            if (hData->alloc[groupIndex] != 0) {
                groupHandleCount = hData->count * hData->param.TrackCount[groupIndex];
                for (handleIndex = 0; handleIndex < groupHandleCount; handleIndex++) {
                    if (hData->list[groupIndex][handleIndex] != 0) {
                        dds3EffectPartDelete(hData->list[groupIndex][handleIndex]);
                    }
                }
                sdfMemFreeDirect(hData->alloc[groupIndex]);
            }
        }
        for (handleIndex = 0; handleIndex < 5; handleIndex++) {
            dds3EffectPartDelete(hData->handleB[handleIndex]);
        }
        dds3EffectPartDelete(hData->tailHandle);
    }
    sdfChipFreeDirect(hData);
}
