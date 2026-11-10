#include "shared/sdk/libkernl/decls.h"

s32 kCopy(u32 *dst, const u32 *src, u32 count)
{
    u32 i;

    count >>= 2;
    for (i = 0; i < count; i++) {
        *dst++ = *src++;
    }
    return 0;
}
