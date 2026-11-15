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

extern f32 __kernel_tanf(f32 x, f32 tail, s32 reciprocal);
extern s32 __ieee754_rem_pio2f(f32 x, f32 *remainder);
f32 tanf(f32 x);
s32 __ieee754_rem_pio2f(f32 x, f32 *y);
f32 __kernel_tanf(f32 x, f32 y, s32 selector);

f32 tanf(f32 x)
{
    f32 y[2];
    s32 magnitude;
    s32 quadrant;

    GET_FLOAT_WORD(magnitude, x);
    magnitude &= 0x7FFFFFFF;
    if (magnitude <= 0x3F490FDA) {
        return __kernel_tanf(x, 0.0f, 1);
    }
    quadrant = __ieee754_rem_pio2f(x, y);
    return __kernel_tanf(y[0], y[1], 1 - ((quadrant & 1) << 1));
}
