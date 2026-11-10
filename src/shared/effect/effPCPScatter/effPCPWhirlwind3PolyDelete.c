#include "shared/effect/effPCPScatter/decls.h"

void effPCPWhirlwind3PolyDelete(effPCPWhirlwind3Poly_tag *hData)
{
    dds3WhirlwindPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->hMem);
}
