#include "shared/sdf/sdfTex/decls.h"

s32 sdfTexGetTexelBitCount(s32 aFormat)
{
    s32 bitCount;

    switch (aFormat) {
    case 19:
        bitCount = 8;
        break;
    case 20:
        bitCount = 4;
        break;
    case 2:
    case 10:
        bitCount = 16;
        break;
    default:
        bitCount = 32;
        break;
    }
    return bitCount;
}
