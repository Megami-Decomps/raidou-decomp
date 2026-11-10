#include "shared/effect/effPCPScatter/decls.h"

void dds3ScatterPolyListCopyTex(dds3ScatterPolyList_tag *hPoly, dds3ScatterPolyList_tag *hPolySrc)
{
    dds3ScatterPolyListTexResource_tag *res;

    res = dds3ScatterPolyListTexResourceCopy(hPolySrc->sharedResource);
    hPoly->sharedResource = res;
}
