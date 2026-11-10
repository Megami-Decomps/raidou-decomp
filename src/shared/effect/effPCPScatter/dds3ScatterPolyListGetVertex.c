#include "shared/effect/effPCPScatter/decls.h"

u128 *dds3ScatterPolyListGetVertex(dds3ScatterPolyList_tag *hPoly, s32 index)
{
    return (u128 *)(hPoly->recordBase + index * EFF_SCATTER_RECORD_BYTES);
}
