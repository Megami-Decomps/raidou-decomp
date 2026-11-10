#include "shared/effect/polyManager/decls.h"

dds3SpPolyOldDesc_t *dds3SpPoly_Ring_Old_Create(dds3SpPolyOldDesc_t *src) {
    s32 tailSize = sizeof(dds3SpPolyOldCellParam_t);
    dds3SpPolyOldDesc_t *desc = sdfChipAllocDirect(src->count * 4 + 0xF0);

    memset(desc, 0, 0xF0);
    memcpy(desc, src, src->headerSize);
    memcpy(&desc->cells, (u8 *)src + src->headerSize, tailSize);
    desc->headerSize = 0xC0;
    desc->cells.cellWords = (u32 *)(desc + 1);
    if (desc->cells.verticesPerCell < 3) {
        desc->cells.verticesPerCell = 3;
    }
    desc->cells.cellSystem = dds3TrackPolyCreate(desc->count, desc->cells.verticesPerCell, 1, TRACK_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(desc->cells.cellSystem, 0,
                   desc->cells.dispatchArgA,
                   desc->cells.dispatchArgB);
    dds3SpPoly_Ring_Old_BuffInit((dds3SpPoly_Ring_Old_t3 *)desc);
    return desc;
}
