#include "shared/effect/effPCPScatter/decls.h"

void dds3ScatterPolyListSetTex(dds3ScatterPolyList_tag *hPoly, u32 pTex)
{
    dds3ScatterPolyListTexResource_tag *res;

    res = dds3ScatterPolyListTexResourceCreate(pTex);
    hPoly->sharedResource = res;
}
