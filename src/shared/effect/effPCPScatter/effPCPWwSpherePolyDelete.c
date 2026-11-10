#include "shared/effect/effPCPScatter/decls.h"

void effPCPWwSpherePolyDelete(effPCPWwSpherePoly_tag *hData)
{
    dds3WhirlwindPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->hMem);
}
