#include "shared/sdf/sdfQuat/decls.h"

/* As above, but write the second VU quaternion register (vf11). */
void FromRotation_B(f32 aAngle)
{
    f32 halfAngle = aAngle * 0.5f;
    f32 trigValue = sdfSin(halfAngle);
    SDF_VU0_SCALEVECTOR_B_V0(trigValue);
    trigValue = sdfCos(halfAngle);
    VU0_SEQ_FROM_ROTATION_B(trigValue);
}
