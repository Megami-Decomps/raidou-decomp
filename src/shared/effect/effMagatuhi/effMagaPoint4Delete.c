#include "shared/effect/effMagatuhi/decls.h"

/* Release Bezier slots, their history owner, then the backing allocation. */
void effMagaPoint4Delete(effMagaPoint4_tag *pData) {
    dds3Bezier4PDelete(pData->pBezier4P);
    effMagaCylinderFree(pData->pMagatuhi);
    sdfMemFreeDirect(pData->hMem);
}
