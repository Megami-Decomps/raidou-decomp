#ifndef SDK_LIBM_H
#define SDK_LIBM_H

#include "common.h"

f32 fabsf(f32 x);

s32 __ieee754_rem_pio2f(f32 x, f32 *y);

f32 atanf(f32 x);

f32 __kernel_sinf(f32 x, f32 y, s32 useTail);

f32 __kernel_cosf(f32 x, f32 tail);

f32 floorf(f32 x);

f32 __kernel_tanf(f32 x, f32 y, s32 selector);

s32 __kernel_rem_pio2f(f32 *x, f32 *y, s32 e0, s32 nx, s32 precision, const s32 *ipio2);

#endif
