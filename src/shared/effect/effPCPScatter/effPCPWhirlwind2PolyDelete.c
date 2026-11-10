#include "shared/effect/effPCPScatter/decls.h"

void effPCPWhirlwind2PolyDelete(effPCPWhirlwind2Poly_tag *hData)
{
    dds3WhirlwindPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}
