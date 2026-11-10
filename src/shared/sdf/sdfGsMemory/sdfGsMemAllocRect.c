#include "shared/sdf/sdfGsMemory/decls.h"

sdfGsMemSlice_t *sdfGsMemAllocRect(s32 aWidth, s32 aHeight, s32 aFormat, s32 aType, s32 aFromHead)
{
    s32 bitCount = sdfGsMemGetBitCount(aFormat);
    s32 words = (aWidth * aHeight * bitCount) >> 5;
    sdfGsMemSlice_t *slice;

    if (aFromHead != 0) {
        slice = sdfGsMemAllocFromHead(words, aType);
    } else {
        slice = sdfGsMemAllocFromTail(words, aType);
    }
    slice->w = aWidth;
    slice->h = aHeight;
    slice->pixelFormat = aFormat;
    return slice;
}
