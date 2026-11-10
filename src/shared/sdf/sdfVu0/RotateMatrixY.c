#include "shared/sdf/sdfVu0/decls.h"

/* vu0 routine: rotate the vf28/vf30 pair by angle (vf28 = vf28*c - vf30*s, vf30 = vf30*c + vf28*s) */
void RotateMatrixY(f32 aAngle)
{
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    VU0_ROTATE_BASIS_Y(c, s);
}
