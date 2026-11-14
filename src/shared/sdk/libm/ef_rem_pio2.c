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

s32 __ieee754_rem_pio2f(f32 x, f32 *y)
{
    f32 z, w, t, r, fn;
    f32 tx[3];
    s32 i, j, n, ix, hx;
    s32 e0, nx;
    u32 high;

    GET_FLOAT_WORD(hx, x);
    ix = hx & 0x7FFFFFFF;
    if (ix <= 0x3F490FD8) {
        y[0] = x;
        y[1] = 0.0f;
        return 0;
    }

    if (ix <= 0x4016CBE3) {
        if (hx > 0) {
            z = x - 0x1.921f000000000p+0f;
            if ((ix & 0xFFFFFFF0) != 0x3FC90FD0) {
                y[0] = z - 0x1.6a88840000000p-17f;
                y[1] = (z - y[0]) - 0x1.6a88840000000p-17f;
            } else {
                z -= 0x1.6a87fe0000000p-17f;
                y[0] = z - 0x1.0b46100000000p-34f;
                y[1] = (z - y[0]) - 0x1.0b46100000000p-34f;
            }
            return 1;
        }
        z = x + 0x1.921f000000000p+0f;
        if ((ix & 0xFFFFFFF0) != 0x3FC90FD0) {
            y[0] = z + 0x1.6a88840000000p-17f;
            y[1] = (z - y[0]) + 0x1.6a88840000000p-17f;
        } else {
            z += 0x1.6a87fe0000000p-17f;
            y[0] = z + 0x1.0b46100000000p-34f;
            y[1] = (z - y[0]) + 0x1.0b46100000000p-34f;
        }
        return -1;
    }

    if (ix <= 0x43490F80) {
        t = fabsf(x);
        n = (s32)(t * 0x1.45f3080000000p-1f + 0x1.0000000000000p-1f);
        fn = (f32)n;
        r = t - fn * 0x1.921f000000000p+0f;
        w = fn * 0x1.6a88840000000p-17f;
        if (n < 32 && (ix & 0xFFFFFF00) != (u32)npio2_hw[n - 1]) {
            y[0] = r - w;
        } else {
            j = ix >> 23;
            y[0] = r - w;
            GET_FLOAT_WORD(high, y[0]);
            i = j - ((high >> 23) & 0xFF);
            if (i > 8) {
                t = r;
                w = fn * 0x1.6a87fe0000000p-17f;
                r = t - w;
                w = fn * 0x1.0b46100000000p-34f - ((t - r) - w);
                y[0] = r - w;
                GET_FLOAT_WORD(high, y[0]);
                i = j - ((high >> 23) & 0xFF);
                if (i > 25) {
                    t = r;
                    w = fn * 0x1.0b46000000000p-34f;
                    r = t - w;
                    w = fn * 0x1.1a62620000000p-54f - ((t - r) - w);
                    y[0] = r - w;
                }
            }
        }
        y[1] = (r - y[0]) - w;
        if (hx < 0) {
            y[0] = -y[0];
            y[1] = -y[1];
            return -n;
        }
        return n;
    }

    e0 = (ix >> 23) - 134;
    SET_FLOAT_WORD(z, ix - (e0 << 23));
    for (i = 0; i < 2; i++) {
        tx[i] = (f32)(s32)z;
        z = (z - tx[i]) * 0x1.0000000000000p+8f;
    }
    tx[2] = z;
    nx = 3;
    while (tx[nx - 1] == 0.0f) {
        nx--;
    }
    n = __kernel_rem_pio2f(tx, y, e0, nx, 2, two_over_pi);
    if (hx < 0) {
        y[0] = -y[0];
        y[1] = -y[1];
        return -n;
    }
    return n;
}

INCLUDE_RODATA(const s32, "sdk/libm/ef_rem_pio2", D_r1_00467C50);

INCLUDE_RODATA(const s32, "sdk/libm/ef_rem_pio2", D_r1_00467F64);

