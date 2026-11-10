#include "shared/effect/effPCPScatter/decls.h"

void effPCPWhirlwindPolyDelete(effPCPWhirlwindPoly_tag *hData)
{
    dds3WhirlwindPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}
