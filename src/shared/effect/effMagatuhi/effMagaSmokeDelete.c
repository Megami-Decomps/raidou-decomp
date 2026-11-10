#include "shared/effect/effMagatuhi/decls.h"

/* Release the drift history owner before its states-plus-work allocation. */
void effMagaSmokeDelete(effMagaSmoke_tag *pData) {
    effMagaCylinderFree(pData->pMagatuhi);
    sdfMemFreeDirect(pData->hMem);
}
