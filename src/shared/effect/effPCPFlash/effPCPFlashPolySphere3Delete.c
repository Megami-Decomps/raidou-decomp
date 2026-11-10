#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphere3Delete(effPCPFlashPolySphere3_tag *hData)
{
    dds3FlashPolyList3Delete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}
