#include "shared/sdk/libkernl/decls.h"

s32 kCopy_3(u32 *dst, const u32 *src, u32 count)
{
    u32 i;

    count >>= 2;
    for (i = 0; i < count; i++) {
        dst[i] = src[i];
    }
    return 0;
}
