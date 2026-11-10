#include "shared/effect/effPCPBezier/decls.h"

void effPCPBezierBillDelete(effPCPBezierBill_tag *hData) {
    u32 i = 0;
    u32 count = hData->param.number;
    effPCPBezierBillPart_tag *slot = hData->pPart;

    if (count != 0) {
        do {
            i++;
            dds3BezierTrackPolyDelete(slot->hBztp);
            effEveBEDCutDelete(slot->hBEDCut);
            slot++;
        } while (i < count);
    }
    if (hData->original != 0) {
        effEveBEDDelete(hData->hBED);
    }
    sdfMemFreeDirect(hData->hMem);
}
