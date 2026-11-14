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

f32 __kernel_tanf(f32 x, f32 y, s32 selector)
{
    s32 originalWord;
    s32 magnitude;
    s32 reciprocalWord;
    s32 resultWord;
    f32 z, w, r, v, s, a, t;

    GET_FLOAT_WORD(originalWord, x);
    magnitude = originalWord & 0x7FFFFFFF;
    if (magnitude < 0x31800000) {
        if ((s32)x == 0) {
            if (((u32)magnitude | ((u32)selector + 1U)) == 0) {
                return 1.0f / fabsf(x);
            }
            return selector == 1 ? x : -1.0f / x;
        }
    }
    if (magnitude >= 0x3F2CA140) {
        if (originalWord < 0) {
            x = -x;
            y = -y;
        }
        z = 0x1.921fb40000000p-1f - x;
        w = 0x1.4442ce0000000p-25f - y;
        x = z + w;
        y = 0.0f;
    }
    z = x * x;
    w = z * z;
    r = D_r1_004681F8[1] + w * (D_r1_004681F8[3] + w *
        (D_r1_004681F8[5] + w * (D_r1_004681F8[7] + w *
        (D_r1_004681F8[9] + w * D_r1_004681F8[11]))));
    v = z * (D_r1_004681F8[2] + w * (D_r1_004681F8[4] + w *
        (D_r1_004681F8[6] + w * (D_r1_004681F8[8] + w *
        (D_r1_004681F8[10] + w * D_r1_004681F8[12])))));
    s = z * x;
    r = y + z * (s * (r + v) + y);
    r += D_r1_004681F8[0] * s;
    w = x + r;
    if (magnitude >= 0x3F2CA140) {
        v = (f32)selector;
        return (f32)(1 - ((originalWord >> 30) & 2)) *
            (v - 2.0f * (x - ((w * w) / (w + v) - r)));
    }
    if (selector == 1) {
        return w;
    }
    /* Truncate the working tangent value before reciprocal compensation. */
    z = w;
    GET_FLOAT_WORD(resultWord, z);
    SET_FLOAT_WORD(z, resultWord & 0xFFFFF000);
    v = r - (z - x);
    a = -1.0f / w;
    GET_FLOAT_WORD(reciprocalWord, a);
    SET_FLOAT_WORD(t, reciprocalWord & 0xFFFFF000);
    s = t * z + 1.0f;
    return t + a * (s + t * v);
}

INCLUDE_RODATA(const s32, "sdk/libm/kf_tan", D_r1_004681F8);

