#include "shared/effect/effMagatuhi/decls.h"

/* Release the orbit history owner before the work/state allocation. */
void effMagaSpiralDelete(effMagaSpiral_tag *pData) {
    effMagaCylinderFree(pData->pMagatuhi);
    sdfMemFreeDirect(pData->hMem);
}
