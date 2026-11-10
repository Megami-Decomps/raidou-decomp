#include "shared/interface/frWrap/decls.h"

/* Resolve three signed, cumulative relative offsets without validating bounds. */
void frGetFontImagePtrs(frRelativeImage_t *block, frImagePtrs_t *out) {
    s32 firstOffset = block->firstOffset;
    s32 secondOffset = firstOffset + block->secondDelta;
    s32 thirdOffset = secondOffset + block->thirdDelta;

    out->treeNodes = (u8 *)block + firstOffset;
    out->entryBitOffsets = (u8 *)block + secondOffset;
    out->packedBitstream = (u8 *)block + thirdOffset;
}
