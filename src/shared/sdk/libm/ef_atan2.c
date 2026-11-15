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

extern f32 __ieee754_atan2f(f32 y, f32 x);
extern f32 atanf(f32 x);
f32 __ieee754_atan2f(f32 y, f32 x);
f32 atanf(f32 x);

f32 __ieee754_atan2f(f32 y, f32 x)
{
    s32 hx;
    s32 hy;
    s32 ix;
    s32 iy;
    s32 quadrant;
    s32 exponentDifference;
    f32 angle;

    GET_FLOAT_WORD(hx, x);
    ix = hx & 0x7FFFFFFF;
    GET_FLOAT_WORD(hy, y);
    iy = hy & 0x7FFFFFFF;

    if (hx == 0x3F800000) {
        return atanf(y);
    }

    quadrant = (s32)(((u32)hy >> 31) | ((hx >> 30) & 2));
    if (iy <= 0x007FFFFF) {
        switch (quadrant) {
        case 0:
        case 1:
            return y;
        case 2:
            return 3.1415926f;
        case 3:
            return -3.1415926f;
        }
    }

    if (ix <= 0x007FFFFF) {
        return hy < 0
                   ? -1.5707963f
                   : 1.5707963f;
    }

    exponentDifference = (iy - ix) >> 23;
    if (exponentDifference >= 61) {
        angle = 1.5707963f;
    } else if (hx < 0 && exponentDifference < -60) {
        angle = 0.0f;
    } else {
        angle = atanf(fabsf(y / x));
    }

    switch (quadrant) {
    case 0:
        return angle;
    case 1: {
        s32 signWord;
        GET_FLOAT_WORD(signWord, angle);
        SET_FLOAT_WORD(angle, (u32)signWord ^ 0x80000000U);
        return angle;
    }
    case 2:
        return 3.1415926f - (angle - 1.5099578831723193e-7f);
    default:
        return (angle - 1.5099578831723193e-7f) - 3.1415926f;
    }
}
