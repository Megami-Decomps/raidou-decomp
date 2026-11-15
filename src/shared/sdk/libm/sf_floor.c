#include "common.h"
#include "sdk_libm.h"

#define GET_FLOAT_WORD(i, d)                    \
    do {                                        \
        union { f32 f; u32 u; } gf_u;             \
        gf_u.f = (d);                           \
        (i) = (s32)gf_u.u;                      \
    } while (0)

#define SET_FLOAT_WORD(d, i)                    \
    do {                                        \
        union { f32 f; u32 u; } sf_u;             \
        sf_u.u = (u32)(i);                      \
        (d) = sf_u.f;                           \
    } while (0)

extern f32 floorf(f32 x);
f32 floorf(f32 x);

f32 floorf(f32 x)
{
    s32 word;
    u32 magnitude;
    s32 exponent;
    u32 fractionMask;

    GET_FLOAT_WORD(word, x);
    magnitude = (u32)(word & 0x7FFFFFFF);
    exponent = (s32)(magnitude >> 23) - 127;

    if (exponent < 23) {
        if (exponent < 0) {
            if (0.0f < (x + 1.0e30f)) {
                if (word >= 0) {
                    word = 0;
                } else if (magnitude > 0x007FFFFFU) {
                    word = (s32)0xBF800000U;
                }
            }
        } else {
            fractionMask = 0x007FFFFF >> exponent;
            if ((word & fractionMask) == 0) {
                return x;
            }
            if (0.0f < (x + 1.0e30f)) {
                if (word < 0) {
                    word += (s32)0x00800000U >> exponent;
                }
                word &= ~fractionMask;
            }
        }
        SET_FLOAT_WORD(x, word);
        return x;
    }
    return x;
}
