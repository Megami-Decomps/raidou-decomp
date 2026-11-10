#include "shared/sdk/libc/decls.h"

s32 _malloc_trim_r(_reent *r, u32 pad)
{
    s64 topSize;
    s64 extra;
    void *newBreak;

    __malloc_lock(r);
    topSize = __malloc_av_.top->size & ~3U;
    /* Page rounding uses 64 bits; break increments and chunk words narrow. */
    extra = (((((u64)topSize - pad + 0xFEF) >> 12) - 1) << 12);
    if (extra < 0x1000) {
        __malloc_unlock(r);
        return 0;
    }
    if (_sbrk_r(r, 0) != (u8 *)__malloc_av_.top + (s32)topSize) {
        __malloc_unlock(r);
        return 0;
    }
    if (_sbrk_r(r, -(s32)extra) == (void *)-1) {
        newBreak = _sbrk_r(r, 0);
        topSize = (s32)(EE_ADDR(newBreak) - EE_ADDR(__malloc_av_.top));
        if (topSize >= 16) {
            __malloc_current_mallinfo.arena =
                EE_ADDR(newBreak) - EE_ADDR(__malloc_sbrk_base);
            __malloc_av_.top->size = topSize | 1;
        }
        __malloc_unlock(r);
        return 0;
    }
    __malloc_av_.top->size = (topSize - extra) | 1;
    __malloc_current_mallinfo.arena -= (s32)extra;
    __malloc_unlock(r);
    return 1;
}
