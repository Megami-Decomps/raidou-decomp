#include "shared/effect/effPCPBoss/decls.h"

void effPCPCrackDelete(effPCPCrack_t *hData)
{
    u32 groupIndex;

    if (hData->groupsHandle != NULL) {
        for (groupIndex = 0; groupIndex < hData->MarkCount; groupIndex++) {
            dds3FlashPolyListDelete(hData->pPart[groupIndex].hFlash);
        }
        sdfMemFreeDirect(hData->groupsHandle);
    }
    dds3TrackPolyDelete(hData->system);
    dds3EffectPartDelete(hData->part);
    sdfChipFreeDirect(hData);
}
