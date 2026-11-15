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

extern f32 __kernel_cosf(f32 x, f32 tail);
f32 __kernel_cosf(f32 x, f32 tail);

f32 __kernel_cosf(f32 x, f32 tail)
{
    s32 magnitude;
    f32 z;
    f32 r;
    f32 qx;
    f32 hz;
    f32 a;

    GET_FLOAT_WORD(magnitude, x);
    magnitude &= 0x7FFFFFFF;
    if (magnitude <= 0x31FFFFFF) {
        if ((s32)x == 0) {
            return 1.0f;
        }
    }
    z = x * x;
    r = z * (0x1.555554p-5f + z * (-0x1.6c16c0p-10f +
        z * (0x1.a01a00p-16f + z * (-0x1.27e4f6p-22f +
        z * (0x1.1ee9eap-29f + z * -0x1.8fae9cp-37f)))));
    if (magnitude <= 0x3E999999) {
        return 1.0f - (0.5f * z - (z * r - x * tail));
    }
    if (magnitude > 0x3F480000) {
        qx = 0.28125f;
    } else {
        SET_FLOAT_WORD(qx, magnitude - 0x01000000);
    }
    hz = 0.5f * z - qx;
    a = 1.0f - qx;
    return a - (hz - (z * r - x * tail));
}
