#include "shared/sdf/sdfQuat/decls.h"

/* vu0 routine: shared quaternion registers are the input/output interface. */
f32 EulerAngleAxisY(void)
{
    f32 rotatedZ;
    f32 rotatedX;
    ToMatrix();
    /* PEXEW exchanges words 0 and 2: output 0 is Z, while output 1 is X. */
    SDF_VU0_LOADVECTOR_BARRIER(sdfUnitXVector);
    SDF_VU0_APPLYMATRIX_XYZ();
    VU0_SEQ_EULER_ANGLE_AXIS_Y(rotatedZ, rotatedX);
    return sdfAtan2(rotatedZ, rotatedX);
}
