#include "shared/effect/effPCPScatter/decls.h"

void dds3WhirlwindPolyListCopyTex(dds3WhirlwindPolyList_tag *hPoly, dds3WhirlwindPolyList_tag *hPolySrc) {
    dds3ScatterPolyListTexResource_tag *shared;

    shared = dds3ScatterPolyListTexResourceCopy(hPolySrc->sharedResource);
    hPoly->sharedResource = shared;
}
