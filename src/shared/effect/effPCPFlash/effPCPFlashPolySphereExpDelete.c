#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphereExpDelete(effPCPFlashPolySphereExp_tag *hData)
{
    dds3FlashPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}
