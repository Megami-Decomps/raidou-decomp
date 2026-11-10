#include "shared/sdk/libc/decls.h"

_Bigint *_d2b(_reent *r, f64 d, s32 *e, s32 *bitsOut) {
    libcDoubleBits_t value;
    _Bigint *b;
    u32 highMantissa;
    u32 *limbs;
    s32 exponent;
    s32 trailingBits;
    s32 wordCount;

    b = _Balloc(r, 1);
    value.value = d;
    limbs = b->x;
    highMantissa = value.word[1] & 0xFFFFF;
    value.word[1] &= 0x7FFFFFFF;
    exponent = value.word[1] >> 20;
    if (exponent != 0) {
        highMantissa |= 0x100000;
    }
    /* Target-specific allocation aid: the real raw low-word snapshot uses
     * $5 only through the test and stack copy, before _lo0bits can clobber it.
     * This constrains reconstruction, not the historical source spelling. */
    {
        FAKEMATCH_ASM_REG(u32, rawLowWord, "$5") = value.word[0];
        FAKEMATCH_ASM_LIVE(rawLowWord);
        if (rawLowWord != 0) {
            u32 lowWord = rawLowWord;
            trailingBits = _lo0bits(&lowWord);
            if (trailingBits != 0) {
                limbs[0] = lowWord | (highMantissa << (32 - trailingBits));
                highMantissa >>= trailingBits;
            } else {
                limbs[0] = lowWord;
            }
            wordCount = b->wds = (limbs[1] = highMantissa) != 0 ? 2 : 1;
        } else {
            trailingBits = _lo0bits(&highMantissa) + 32;
            limbs[0] = highMantissa;
            wordCount = b->wds = 1;
        }
    }
    if (exponent != 0) {
        *e = (exponent + trailingBits) - 0x433;
        *bitsOut = 0x35 - trailingBits;
    } else {
        *e = trailingBits - 0x432;
        *bitsOut = (wordCount << 5) - _hi0bits(limbs[wordCount - 1]);
    }
    return b;
}
