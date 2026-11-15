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

extern f32 __kernel_tanf(f32 x, f32 tail, s32 reciprocal);
extern const f32 __kernel_tanfTable[];
f32 __kernel_tanf(f32 x, f32 y, s32 selector);
extern const f32 __kernel_tanfTable[14];

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
    r = __kernel_tanfTable[1] + w * (__kernel_tanfTable[3] + w *
        (__kernel_tanfTable[5] + w * (__kernel_tanfTable[7] + w *
        (__kernel_tanfTable[9] + w * __kernel_tanfTable[11]))));
    v = z * (__kernel_tanfTable[2] + w * (__kernel_tanfTable[4] + w *
        (__kernel_tanfTable[6] + w * (__kernel_tanfTable[8] + w *
        (__kernel_tanfTable[10] + w * __kernel_tanfTable[12])))));
    s = z * x;
    r = y + z * (s * (r + v) + y);
    r += __kernel_tanfTable[0] * s;
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

INCLUDE_RODATA(const s32, "sdk/libm/kf_tan", __kernel_tanfTable);

