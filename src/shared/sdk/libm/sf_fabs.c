#include "common.h"

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

extern f32 fabsf(f32 x);
f32 fabsf(f32 x);

f32 fabsf(f32 x)
{
    s32 word;
    GET_FLOAT_WORD(word, x);
    SET_FLOAT_WORD(x, (u32)word & 0x7FFFFFFFU);
    return x;
}
