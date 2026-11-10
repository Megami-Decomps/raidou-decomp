#include "shared/effect/effPCPMisc/decls.h"

void effPCPSpiralBEDDelete(effPCPSpiralBED_tag *hData) {
    u32 i = 0;
    u32 count = hData->param.count;
    effPCPSpiralBEDPart_tag *entry = hData->pPart;

    if (count != 0) {
        do {
            effEveBEDCutDelete(entry->hBEDCut);
            entry++;
            i++;
        } while (i < count);
    }
    if (hData->hBED->active == 0) {
        effEveBEDDelete(hData->hBED);
    }
    sdfMemFreeDirect(hData->handle);
}
