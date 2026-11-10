#include "shared/interface/frWrap/decls.h"

/* Decode entryIndex's bit span through the resource's six-byte prefix-tree records.
 * Write terminal bytes to decodedBytes and return/cache the count. The font atlas
 * uses these bytes as pixels; neither entry nor output bounds are checked. */
s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex, void *resourceBase, frImagePtrs_t *segments) {
    frBitRange_t range;
    s32 produced;
    u8 *table = segments->treeNodes;
    u16 *bitWords;
    u32 wordIndex;
    u32 start;
    s32 remaining;
    u32 state;
    u16 *root;
    u16 *node;

    memcpy(&range.start, (u8 *)segments->entryBitOffsets + entryIndex * 4, 4);
    memcpy(&range.end, (u8 *)segments->entryBitOffsets + entryIndex * 4 + 4, 4);
    produced = 0;
    frGetFontHeader(EE_ADDR(resourceBase));
    start = range.start;
    bitWords = segments->packedBitstream;
    wordIndex = start >> 4;
    remaining = range.end - start;
    state = (bitWords[wordIndex] | 0x10000) >> (start & 0xF);
    root = (u16 *)(table + 2);
    node = root;
    do {
        u16 index = node[state & 1];

        node = (u16 *)(table + index * 6 + 2);
        if (*node == 0) {
            decodedBytes[produced++] = ((u8 *)node)[2];
            node = root;
        }
        state >>= 1;
        if (state == 1) {
            wordIndex++;
            state = bitWords[wordIndex] | 0x10000;
        }
        remaining--;
    } while (remaining != 0);
    frFontRowBuf = produced;
    return produced;
}
