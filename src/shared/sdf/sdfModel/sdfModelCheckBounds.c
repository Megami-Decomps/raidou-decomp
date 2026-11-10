#include "shared/sdf/sdfModel/decls.h"

/* vu0 routine: native SDK vector/matrix operations around model state. */
s32 sdfModelCheckBounds(const f32 *aMinMax) {
    s32 flags;
    s32 result;

    VU0_CLIP_TEST_BOX_CORNERS(flags, aMinMax);
    result = flags & 0x3F;
    result &= (flags >> 12) & (flags >> 6);
    result &= flags >> 18;
    return result;
}
