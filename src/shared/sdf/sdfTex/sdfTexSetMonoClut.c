#include "shared/sdf/sdfTex/decls.h"

void sdfTexSetMonoClut(sdfTexHandle_t *aHandle)
{
    s32 bytesPerEntry;
    s32 entryCount;
    u8 *out;
    u8 *src;
    u32 color;
    u32 r;
    u32 g;
    u32 b;

    if (aHandle->CLUTFormat == 0) {
        bytesPerEntry = 4;
        entryCount = (u32)aHandle->clutDataSize >> 2;
    } else {
        bytesPerEntry = 2;
        entryCount = (u32)aHandle->clutDataSize >> 1;
    }
    out = aHandle->monoClutData;
    if (out == NULL) {
        out = sdfAllocAuto(entryCount);
        aHandle->monoClutData = out;
    }
    src = aHandle->clutData;
    do {
        if (bytesPerEntry == 4) {
            color = *(u32 *)src;
            src += 4;
        } else {
            u16 packed = *(u16 *)src;

            src += 2;
            /* 1555 to 8888: pext5 spreads the five-bit fields, then the top
             * three bits of each are copied down into the low bits. */
            MMI_PEXT5(color, packed);
            color |= (color >> 5) & 0x070707;
        }
        g = (color >> 8) & 0xFF;
        r = color & 0xFF;
        b = (color >> 16) & 0xFF;
        entryCount--;
        *out = ((r * 77) >> 8) + ((g * 150) >> 8) + ((b * 29) >> 8);
        out++;
    } while (entryCount != 0);
}
