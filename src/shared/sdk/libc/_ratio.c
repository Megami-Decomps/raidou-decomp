#include "shared/sdk/libc/decls.h"

f64 _ratio(_Bigint *a, _Bigint *b)
{
    libcDoubleBits_t numerator;
    libcDoubleBits_t denominator;
    s32 aExponent;
    s32 bExponent;
    s32 difference;

    numerator.value = _b2d(a, &aExponent);
    denominator.value = _b2d(b, &bExponent);
    difference = aExponent - bExponent + 32 * (a->wds - b->wds);
    if (difference > 0) {
        numerator.word[1] += difference * 0x100000U;
    } else {
        denominator.word[1] -= difference * 0x100000U;
    }
    return numerator.value / denominator.value;
}
