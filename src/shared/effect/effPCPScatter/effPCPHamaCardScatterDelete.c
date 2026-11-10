#include "shared/effect/effPCPScatter/decls.h"

void effPCPHamaCardScatterDelete(effPCPHamaCardScatter_tag *hData)
{
    u32 duplicatedHandleIndex;
    u32 count;

    if (hData->hAttachMem != 0) {
        count = hData->AttachNum;
        duplicatedHandleIndex = 0;
        if (count != 0) {
            do {
                dds3EffectPartDelete(hData->pAttach[duplicatedHandleIndex]);
                duplicatedHandleIndex++;
            } while (duplicatedHandleIndex < count);
        }
        sdfMemFreeDirect(hData->hAttachMem);
    }
    dds3ScatterPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->hMem);
}
