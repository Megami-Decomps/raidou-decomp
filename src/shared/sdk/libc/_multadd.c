#include "shared/sdk/libc/decls.h"

_Bigint *_multadd(_reent *r, _Bigint *b, s32 m, s32 a)
{
    s32 wds = b->wds;
    u32 *x = b->x;
    s32 i = 0;
    u32 lo;
    u32 hi;
    _Bigint *b1;

    do {
        lo = (*x & 0xffff) * m + a;
        hi = (*x >> 16) * m + (lo >> 16);
        a = hi >> 16;
        *x++ = (hi << 16) + (lo & 0xffff);
    } while (++i < wds);
    if (a != 0) {
        if (wds >= b->maxwds) {
            b1 = _Balloc(r, b->k + 1);
            memcpy(&b1->sign, &b->sign, b->wds * sizeof(u32) + 2 * sizeof(s32));
            _Bfree(r, b);
            b = b1;
        }
        b->x[wds++] = a;
        b->wds = wds;
    }
    return b;
}
