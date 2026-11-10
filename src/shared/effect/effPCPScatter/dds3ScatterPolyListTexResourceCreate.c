#include "shared/effect/effPCPScatter/decls.h"

dds3ScatterPolyListTexResource_tag *dds3ScatterPolyListTexResourceCreate(u32 pTex)
{
    dds3ScatterPolyListTexResource_tag *res;

    res = sdfChipAllocDirect(sizeof(dds3ScatterPolyListTexResource_tag));
    res->textureHandle = sdfTexSet(EE_ADDR_AS(TMX_HEADER_t *, (pTex)));
    res->refCount = 1;
    return res;
}
