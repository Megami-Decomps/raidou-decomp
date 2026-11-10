#include "shared/effect/effEvent/decls.h"

/* Release slot storage, every duplicated parameter and the backing allocation in native order. */
void effAPPoint4Delete(effAPPoint4_tag *hData) {
    effAPPoint4Part_tag *recordCursor;
    u32 recordIndex = 0;
    u32 recordCount;

    dds3Bezier4PDelete(hData->pBezier4P);
    recordCount = hData->param.count;
    recordCursor = hData->pPart;
    if (recordCount != 0) {
        do {
            dds3EffectPartDelete(recordCursor->hPart);
            recordCursor++;
            recordIndex++;
        } while (recordIndex < recordCount);
    }
    sdfMemFreeDirect(hData->hMem);
}
