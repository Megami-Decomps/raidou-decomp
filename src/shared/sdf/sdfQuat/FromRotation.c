#include "shared/sdf/sdfQuat/decls.h"

/* vf10 initially holds the axis; write the sine-scaled axis and cosine W. */
void FromRotation(f32 aAngle)
{
    f32 halfAngle = aAngle * 0.5f;
    f32 trigValue = sdfSin(halfAngle);
    SDF_VU0_SCALEVECTOR_V0(trigValue);
    trigValue = sdfCos(halfAngle);
    VU0_SEQ_FROM_ROTATION(trigValue);
}
