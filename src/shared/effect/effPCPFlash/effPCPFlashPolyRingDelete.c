#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolyRingDelete(effPCPFlashPolyRing_tag *hData)
{
    dds3FlashPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}
