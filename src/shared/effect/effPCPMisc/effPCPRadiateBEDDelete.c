#include "shared/effect/effPCPMisc/decls.h"

void effPCPRadiateBEDDelete(effPCPRadiateBED_tag *hData) {
    u32 i = 0;
    u32 count = hData->param.count;
    effPCPRadiateBEDPart_tag *entry = hData->pPart;

    if (count != 0) {
        do {
            effEveBEDCutDelete(entry->hBEDCut);
            entry++;
            i++;
        } while (i < count);
    }
    if (hData->owner->active == 0) {
        effEveBEDDelete(hData->owner);
    }
    sdfMemFreeDirect(hData->handle);
}
