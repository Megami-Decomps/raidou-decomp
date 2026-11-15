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

extern f32 __kernel_sinf(f32 x, f32 tail, s32 reduced);
f32 __kernel_sinf(f32 x, f32 y, s32 useTail);

f32 __kernel_sinf(f32 x, f32 y, s32 useTail)
{
    s32 magnitude;
    f32 z, v, r;

    GET_FLOAT_WORD(magnitude, x);
    magnitude &= 0x7FFFFFFF;
    if (magnitude < 0x32000000) {
        if ((s32)x == 0)
            return x;
    }
    z = x * x;
    v = z * x;
    r = 0x1.1111120000000p-7f + z * (-0x1.a01a020000000p-13f + z * (0x1.71de340000000p-19f + z * (-0x1.ae5e680000000p-26f + z * 0x1.5d93a40000000p-33f)));
    if (useTail == 0)
        return x + v * (-0x1.5555540000000p-3f + z * r);
    return x - ((z * (0.5f * y - v * r) - y) - v * -0x1.5555540000000p-3f);
}
