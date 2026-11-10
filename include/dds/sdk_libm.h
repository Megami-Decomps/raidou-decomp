#ifndef DDS_SDK_LIBM_H

#define DDS_SDK_LIBM_H
/* SDK math library (libm); single-precision entry points plus the double-precision ones the game links. One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

#ifndef SDK_SKIP_acosf

#endif

#ifndef SDK_SKIP_asinf

f32 asinf(f32 x);

#endif

#ifndef SDK_SKIP_atan2f

#endif

#ifndef SDK_SKIP_atanf

#endif

#ifndef SDK_SKIP_cosf

#endif

#ifndef SDK_SKIP_sinf

#endif

#ifndef SDK_SKIP_tanf

#endif

#ifdef VERSION_DDS2

#ifndef SDK_SKIP_cos

f64 cos(f64 x);

#endif

#ifndef SDK_SKIP_sin

f64 sin(f64 x);

#endif

#ifndef SDK_SKIP_sqrt

f64 sqrt(f64 x);

#endif

#endif

#endif
