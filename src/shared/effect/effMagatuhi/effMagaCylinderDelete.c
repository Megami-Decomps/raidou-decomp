#include "shared/effect/effMagatuhi/decls.h"

/* Release the ring history owner before the work/state allocation. */
void effMagaCylinderDelete(effMagaCylinder_tag *pData) {
    effMagaCylinderFree(pData->pMagatuhi);
    sdfMemFreeDirect(pData->hMem);
}
