#include "shared/sdk/libc/decls.h"

f64 _ulp(f64 x)
{
    libcDoubleBits_t input;
    libcDoubleBits_t result;
    s32 exponent;

    input.value = x;
    exponent = (input.word[1] & 0x7FF00000) - 0x03400000;
    if (exponent > 0) {
        result.word[1] = exponent;
        result.word[0] = 0;
    } else {
        exponent = -exponent >> 20;
        if (exponent < 20) {
            result.word[1] = 0x80000 >> exponent;
            result.word[0] = 0;
        } else {
            result.word[1] = 0;
            exponent -= 20;
            result.word[0] = exponent >= 31 ? 1 : 1 << (31 - exponent);
        }
    }
    return result.value;
}
