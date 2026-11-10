#include "shared/sdk/libc/decls.h"

/* Qualified allocator controls for the two count lifetimes are recorded in
 * docs/levers/cross-block-locals.md, "Retain bigint limb and bit counts". */
_Bigint *_lshift(_reent *r, _Bigint *b, s32 shift) {
    _Bigint *b1;
    u32 *src;
    u32 *dst;
    u32 *end;
    u32 carry;
    s32 n;
    s32 wds;
    s32 bitCount;
    s32 count;

    n = shift >> 5;
    count = b->maxwds;
    wds = n + b->wds + 1;
    bitCount = b->k;
    if (count < wds) {
        do {
            count *= 2;
            bitCount++;
        } while (count < wds);
    }
    b1 = _Balloc(r, bitCount);
    dst = b1->x;
    count = 0;
    while (count < n) {
        *dst++ = 0;
        count++;
    }
    src = b->x;
    shift &= 0x1F;
    end = src + b->wds;
    if (shift != 0) {
        bitCount = 0x20 - shift;
        carry = 0;
        do {
            *dst++ = (*src << shift) | carry;
            carry = *src++ >> bitCount;
        } while (src < end);
        *dst = carry;
        if (carry != 0) {
            wds++;
        }
    } else {
        do {
            *dst++ = *src++;
        } while (src < end);
    }
    b1->wds = wds - 1;
    _Bfree(r, b);
    return b1;
}
