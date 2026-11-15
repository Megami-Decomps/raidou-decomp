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

extern f32 __kernel_cosf(f32 x, f32 tail);
extern f32 __kernel_sinf(f32 x, f32 tail, s32 reduced);
extern s32 __ieee754_rem_pio2f(f32 x, f32 *remainder);
f32 sinf(f32 x);
s32 __ieee754_rem_pio2f(f32 x, f32 *y);
f32 __kernel_cosf(f32 x, f32 tail);
f32 __kernel_sinf(f32 x, f32 y, s32 useTail);

f32 sinf(f32 x)
{
    f32 y[2];
    s32 magnitude;
    s32 quadrant;

    GET_FLOAT_WORD(magnitude, x);
    magnitude &= 0x7FFFFFFF;
    if (magnitude <= 0x3F490FD8) {
        return __kernel_sinf(x, 0.0f, 0);
    }
    quadrant = __ieee754_rem_pio2f(x, y);
    switch (quadrant & 3) {
    case 0: return __kernel_sinf(y[0], y[1], 1);
    case 1: return __kernel_cosf(y[0], y[1]);
    case 2: return -__kernel_sinf(y[0], y[1], 1);
    default: return -__kernel_cosf(y[0], y[1]);
    }
}
