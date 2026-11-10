#include "shared/effect/effMagatuhi/decls.h"

/* Invoke the primary type callback with the payload; no NULL or type bounds check. */
void dds3MagatuhiParDraw(dds3MagatuhiPar_tag *hData) {
    dds3MagatuhiDrawTable[hData->type].handler(hData->pHandle);
}
