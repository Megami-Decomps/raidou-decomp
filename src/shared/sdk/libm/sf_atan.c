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

f32 atanf(f32 x)
{
    s32 inputWord;
    s32 magnitude;
    s32 reduction;
    f32 z;
    f32 w;
    f32 even;
    f32 odd;
    f32 result;

    GET_FLOAT_WORD(inputWord, x);
    magnitude = inputWord & 0x7FFFFFFF;
    if (magnitude > 0x507FFFFF) {
        if (inputWord > 0) {
            return D_r1_00467924 + D_r1_00467934;
        }
        return -D_r1_00467924 - D_r1_00467934;
    }

    if (magnitude <= 0x3EDFFFFF) {
        if (magnitude < 0x31000000 && 1.0f < x + 1.0e30f) {
            return x;
        }
        reduction = -1;
    } else {
        x = fabsf(x);
        if (magnitude <= 0x3F97FFFF) {
            if (magnitude <= 0x3F2FFFFF) {
                reduction = 0;
                x = (2.0f * x - 1.0f) / (x + 2.0f);
            } else {
                reduction = 1;
                x = (x - 1.0f) / (x + 1.0f);
            }
        } else if (magnitude <= 0x401BFFFF) {
            reduction = 2;
            x = (x - 1.5f) / (1.5f * x + 1.0f);
        } else {
            reduction = 3;
            x = -1.0f / x;
        }
    }

    z = x * x;
    w = z * z;
    even = z * (D_r1_00467938[0] + w * (D_r1_00467938[2] + w * (D_r1_00467938[4] +
                  w * (D_r1_00467938[6] + w * (D_r1_00467938[8] +
                  w * D_r1_00467938[10])))));
    odd = w * (D_r1_00467938[1] + w * (D_r1_00467938[3] + w * (D_r1_00467938[5] +
                 w * (D_r1_00467938[7] + w * D_r1_00467938[9]))));

    if (reduction < 0) {
        return x - x * (even + odd);
    }
    result = D_r1_00467918[reduction] - ((x * (even + odd) - D_r1_00467928[reduction]) - x);
    if (inputWord < 0) {
        result = -result;
    }
    return result;
}

INCLUDE_RODATA(const s32, "sdk/libm/sf_atan", D_r1_00467918);

INCLUDE_RODATA(const s32, "sdk/libm/sf_atan", D_r1_00467924);

INCLUDE_RODATA(const s32, "sdk/libm/sf_atan", D_r1_00467928);

INCLUDE_RODATA(const s32, "sdk/libm/sf_atan", D_r1_00467934);

INCLUDE_RODATA(const s32, "sdk/libm/sf_atan", D_r1_00467938);

