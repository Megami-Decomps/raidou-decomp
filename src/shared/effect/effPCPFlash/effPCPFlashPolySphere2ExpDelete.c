#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphere2ExpDelete(effPCPFlashPolySphere2Exp_tag *hData)
{
    dds3FlashPolyList2Delete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}
