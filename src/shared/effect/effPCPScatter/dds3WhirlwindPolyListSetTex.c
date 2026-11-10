#include "shared/effect/effPCPScatter/decls.h"

void dds3WhirlwindPolyListSetTex(dds3WhirlwindPolyList_tag *hPoly, u32 pTex) {
    dds3ScatterPolyListTexResource_tag *shared;

    shared = dds3ScatterPolyListTexResourceCreate(pTex);
    hPoly->sharedResource = shared;
}
