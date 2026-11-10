#include "shared/sdf/sdfVu0/decls.h"

/* vu0 routine: rotate the vf28/vf29 pair by angle (vf28 = vf28*c + vf29*s, vf29 = vf29*c - vf28*s) */
void RotateMatrixZ(f32 aAngle)
{
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    VU0_ROTATE_BASIS_Z(c, s);
}
