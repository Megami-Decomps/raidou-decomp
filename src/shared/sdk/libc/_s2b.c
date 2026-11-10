#include "shared/sdk/libc/decls.h"

/* Ordinary source reconstruction of the newlib decimal-string to Bigint helper. */
_Bigint *_s2b(_reent *r, const char *s, s32 nd0, s32 nd, u32 y9)
{
    _Bigint *b;
    s32 n;
    s32 k;
    s32 i;
    s32 capacity;

    n = (nd + 8) / 9;
    k = 0;
    capacity = 1;
    while (capacity < n) {
        capacity <<= 1;
        k++;
    }

    b = _Balloc(r, k);
    b->x[0] = y9;
    b->wds = 1;

    i = 9;
    if (nd0 > 9) {
        s += 9;
        do {
            b = _multadd(r, b, 10, *s++ - '0');
        } while (++i < nd0);
        s++;
    } else {
        s += 10;
    }

    while (i < nd) {
        b = _multadd(r, b, 10, *s++ - '0');
        ++i;
    }
    return b;
}
