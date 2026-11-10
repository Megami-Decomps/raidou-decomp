#include "shared/effect/effPCPScatter/decls.h"

dds3ScatterPolyListTexResource_tag *dds3ScatterPolyListTexResourceCopy(dds3ScatterPolyListTexResource_tag *hSplTexResSrc)
{
    hSplTexResSrc->refCount++;
    return hSplTexResSrc;
}
