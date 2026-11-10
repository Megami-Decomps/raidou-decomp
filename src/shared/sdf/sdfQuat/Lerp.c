#include "shared/sdf/sdfQuat/decls.h"

/* vu0 routine: shared quaternion registers are the input/output interface. */
void Lerp(f32 aRatio)
{
    f32 dotProduct;
    VU0_QUAT_DOT_VF10_VF11(dotProduct);
    if (dotProduct < 0.0f) {
        VU0_NEGATE_VF11_TO_VF12();
    } else {
        SDF_VU0_COPYVECTOR_B_TO_C();
    }
    {
        f32 firstWeight = 1.0f - aRatio;
        VU0_SET_WEIGHTS_VF2X_VF3X(firstWeight, aRatio);
        VU0_WEIGHTED_SUM_VF10_VF12();
    }
    Normalize();
}
