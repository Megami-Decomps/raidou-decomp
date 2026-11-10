#include "shared/effect/effPCPScatter/decls.h"

u32 *dds3ScatterPolyListGetVC(dds3ScatterPolyList_tag *hPoly, s32 index)
{
    return (u32 *)(hPoly->auxRecordBase + index * EFF_SCATTER_AUX_RECORD_BYTES);
}
