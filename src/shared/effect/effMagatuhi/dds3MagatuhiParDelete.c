#include "shared/effect/effMagatuhi/decls.h"

/* Invoke the release callback, then free the outer work; no callback NULL check. */
void dds3MagatuhiParDelete(dds3MagatuhiPar_tag *hData) {
    dds3MagatuhiDeleteTable[hData->type].handler(hData->pHandle);
    sdfChipFreeDirect(hData);
}
