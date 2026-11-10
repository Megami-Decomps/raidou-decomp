#include "shared/effect/effEvent/decls.h"

/* Drop the shared texture reference count; zero dispatches both resource handles.
 * The retained allocation is released afterwards, regardless of that count. */
void effEVECombParSmokeDelete(effEVECombParSmoke_t *pCombParSmoke) {
    EffEveCPSmokeCount = EffEveCPSmokeCount - 1;
    if (EffEveCPSmokeCount == 0) {
        dds3BillboardDelete(EE_ADDR_AS(dds3Billboard_t *, hEffEveCPSmokeBill01Tex));
        dds3BillboardDelete(EE_ADDR_AS(dds3Billboard_t *, hEffEveCPSmokeBill02Tex));
    }
    sdfMemFreeDirect(pCombParSmoke->hMem);
}
