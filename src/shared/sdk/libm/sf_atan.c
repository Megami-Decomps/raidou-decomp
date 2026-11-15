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

extern f32 atanf(f32 x);
extern const f32 atanfTable[];
extern const f32 atanfConst;
extern const f32 atanfTable2[];
extern const f32 atanfConst2;
extern const f32 atanfTable3[];
f32 atanf(f32 x);
extern const f32 atanfTable[3];
extern const f32 atanfConst __attribute__((section(".rodata")));
extern const f32 atanfTable2[3];
extern const f32 atanfConst2 __attribute__((section(".rodata")));
extern const f32 atanfTable3[18];

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
            return atanfConst + atanfConst2;
        }
        return -atanfConst - atanfConst2;
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
    even = z * (atanfTable3[0] + w * (atanfTable3[2] + w * (atanfTable3[4] +
                  w * (atanfTable3[6] + w * (atanfTable3[8] +
                  w * atanfTable3[10])))));
    odd = w * (atanfTable3[1] + w * (atanfTable3[3] + w * (atanfTable3[5] +
                 w * (atanfTable3[7] + w * atanfTable3[9]))));

    if (reduction < 0) {
        return x - x * (even + odd);
    }
    result = atanfTable[reduction] - ((x * (even + odd) - atanfTable2[reduction]) - x);
    if (inputWord < 0) {
        result = -result;
    }
    return result;
}

INCLUDE_RODATA(const s32, "sdk/libm/sf_atan", atanfTable);

INCLUDE_RODATA(const s32, "sdk/libm/sf_atan", atanfConst);

INCLUDE_RODATA(const s32, "sdk/libm/sf_atan", atanfTable2);

INCLUDE_RODATA(const s32, "sdk/libm/sf_atan", atanfConst2);

INCLUDE_RODATA(const s32, "sdk/libm/sf_atan", atanfTable3);

