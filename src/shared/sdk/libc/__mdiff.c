#include "shared/sdk/libc/decls.h"

/* The shared pointer and limb intermediates have qualified allocator controls;
 * see docs/levers/cross-block-locals.md, "Retain bigint subtraction values". */
_Bigint *__mdiff(_reent *r, _Bigint *a, _Bigint *b) {
    s32 sign = __mcmp(a, b);
    _Bigint *result;
    u32 *srcA;
    u32 *srcB;
    u32 *dst;
    u32 *endA;
    u32 *endB;
    s32 countA;
    s32 countB;
    s32 borrow;
    s32 low;
    u32 limb;

    if (sign == 0) {
        result = _Balloc(r, 0);
        result->wds = 1;
        result->x[0] = 0;
        return result;
    }
    if (sign < 0) {
        result = a;
        a = b;
        b = result;
        sign = 1;
    } else {
        sign = 0;
    }

    result = _Balloc(r, a->k);
    result->sign = sign;
    srcA = a->x;
    srcB = b->x;
    dst = result->x;
    countA = a->wds;
    countB = b->wds;
    endA = srcA + countA;
    endB = srcB + countB;
    borrow = 0;
    do {
        u32 wordB;
        limb = *srcA++;
        wordB = *srcB++;
        low = (limb & 0xFFFF) - (wordB & 0xFFFF) + borrow;
        borrow = low >> 16;
        limb = (limb >> 16) - (wordB >> 16);
        limb += borrow;
        ((u16 *)dst)[0] = low;
        ((u16 *)dst)[1] = limb;
        dst++;
        borrow = (s32)limb >> 16;
    } while (srcB < endB);
    while (srcA < endA) {
        u32 word = *srcA++;
        low = (word & 0xFFFF) + borrow;
        borrow = low >> 16;
        limb = (word >> 16) + borrow;
        ((u16 *)dst)[0] = low;
        ((u16 *)dst)[1] = limb;
        dst++;
        borrow = (s32)limb >> 16;
    }
    dst--;
    while (*dst == 0) {
        dst--;
        countA--;
    }
    result->wds = countA;
    return result;
}
