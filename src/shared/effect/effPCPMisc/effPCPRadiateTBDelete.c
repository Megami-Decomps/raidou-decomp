#include "shared/effect/effPCPMisc/decls.h"

void effPCPRadiateTBDelete(effPCPRadiateTB_tag *hData) {
    u32 i = 0;
    u32 count = hData->param.count;
    effPCPRadiateTBPart_tag *entry = hData->pPart;

    if (count != 0) {
        do {
            effEveBEDCutDelete(entry->eventA);
            effEveBEDCutDelete(entry->eventB);
            effPCP_ThunderPolyDelete(entry->pThunderPoly);
            entry++;
            i++;
        } while (i < count);
    }
    if (hData->hBED->active == 0) {
        effEveBEDDelete(hData->hBED);
    }
    if (hData->ownerB->active == 0) {
        effEveBEDDelete(hData->ownerB);
    }
    sdfMemFreeDirect(hData->hMem);
}
