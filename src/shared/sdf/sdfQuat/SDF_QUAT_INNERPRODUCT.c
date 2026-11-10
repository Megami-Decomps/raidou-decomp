#include "shared/sdf/sdfQuat/decls.h"

/* vu0 routine: return the four-lane dot product of vf10 and vf11. */
f32 SDF_QUAT_INNERPRODUCT(void)
{
    f32 dotProduct;
    VU0_QUAT_DOT_VF10_VF11(dotProduct);
    return dotProduct;
}
