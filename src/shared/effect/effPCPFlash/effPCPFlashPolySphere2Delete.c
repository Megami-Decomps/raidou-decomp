#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphere2Delete(effPCPFlashPolySphere2_tag *hData)
{
    dds3FlashPolyList2Delete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}
