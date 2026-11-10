#include "shared/sdk/libkernl/decls.h"

s32 kCopy_1(u8 *dst, const u8 *src, u32 count)
{
    u32 i;

    i = 0;
    if (count != 0) {
        do {
            *dst++ = *src++;
            i++;
        } while (i < count);
    }
    return 0;
}
