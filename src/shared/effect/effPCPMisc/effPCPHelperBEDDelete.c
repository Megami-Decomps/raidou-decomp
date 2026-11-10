#include "shared/effect/effPCPMisc/decls.h"

void effPCPHelperBEDDelete(effPCPHelperBED_tag *hData) {
    u32 i;
    u32 count;
    effPCPHelperBEDPart_tag *entry;

    dds3EffectPartDelete(hData->hHelper);
    count = hData->count;
    entry = hData->pPart;
    for (i = 0; i < count; i++) {
        if (entry->hBEDCut != NULL) {
            effEveBEDCutDelete(entry->hBEDCut);
        }
        entry++;
    }
    if (hData->hBED->active == 0) {
        effEveBEDDelete(hData->hBED);
    }
    sdfMemFreeDirect(hData->hPartMem);
    sdfChipFreeDirect(hData);
}
