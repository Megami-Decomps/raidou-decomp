#include "shared/sdf/sdfVu0/decls.h"

/* vu0 routine: rotate the vf29/vf30 pair by angle (vf29 = vf29*c + vf30*s, vf30 = vf30*c - vf29*s) */
void RotateMatrixX(f32 aAngle)
{
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    VU0_ROTATE_BASIS_X(c, s);
}
