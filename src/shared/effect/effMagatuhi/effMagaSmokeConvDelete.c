#include "shared/effect/effMagatuhi/decls.h"

/* Release math state, the history owner, then the backing allocation. */
void effMagaSmokeConvDelete(effMagaSmokeConv_tag *pData) {
    dds3Bezier4PDelete(pData->pBezier4P);
    effMagaCylinderFree(pData->pMagatuhi);
    sdfMemFreeDirect(pData->hMem);
}
