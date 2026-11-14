#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdk/libm.c */
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

extern f32 __kernel_tanf(f32 x, f32 tail, s32 reciprocal);

extern s32 __ieee754_rem_pio2f(f32 x, f32 *remainder);

f32 cosf(f32 x);

f32 sinf(f32 x);

f32 tanf(f32 x);

extern f32 __ieee754_acosf(f32 x);

extern f32 __ieee754_atan2f(f32 y, f32 x);

f32 acosf(f32 x);

f32 atan2f(f32 y, f32 x);

extern f32 __ieee754_sqrtf(f32 x);

f32 __ieee754_acosf(f32 x);

extern f32 fabsf(f32 x);

extern f32 atanf(f32 x);

f32 __ieee754_atan2f(f32 y, f32 x);

extern const s32 two_over_pi[];

extern const s32 npio2_hw[];

extern s32 __kernel_rem_pio2f(f32 *x, f32 *y, s32 e0, s32 nx, s32 precision,
                              const s32 *twoOverPi);

s32 __ieee754_rem_pio2f(f32 x, f32 *y);

f32 __kernel_cosf(f32 x, f32 tail);

extern f32 floorf(f32 x);

extern f32 scalbnf(f32 x, s32 n);

extern const s32 D_r1_00468180[];

/* Readonly fragment also contains neighboring math constants. */
extern const f32 D_r1_00468190[];

s32 __kernel_rem_pio2f(f32 *x, f32 *y, s32 e0, s32 nx, s32 precision,
                       const s32 *ipio2);

f32 __kernel_sinf(f32 x, f32 y, s32 useTail);

extern const f32 D_r1_004681F8[];

f32 __kernel_tanf(f32 x, f32 y, s32 selector);

extern const f32 D_r1_00467918[];

extern const f32 D_r1_00467924;

extern const f32 D_r1_00467928[];

extern const f32 D_r1_00467934;

extern const f32 D_r1_00467938[];

f32 atanf(f32 x);

f32 fabsf(f32 x);

f32 floorf(f32 x);

extern f32 copysignf(f32 magnitude, f32 sign);

f32 scalbnf(f32 x, s32 n);

f32 copysignf(f32 magnitude, f32 sign);

extern const s32 two_over_pi[198];

extern const s32 npio2_hw[52];

extern const s32 D_r1_00468180[4];

extern const f32 D_r1_00468190[26];

extern const f32 D_r1_004681F8[14];

extern const f32 D_r1_00467918[3];

extern const f32 D_r1_00467924 __attribute__((section(".rodata")));

extern const f32 D_r1_00467928[3];

extern const f32 D_r1_00467934 __attribute__((section(".rodata")));

extern const f32 D_r1_00467938[18];
/* End of declarations from src/smt3/sdk/libm.c */

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
