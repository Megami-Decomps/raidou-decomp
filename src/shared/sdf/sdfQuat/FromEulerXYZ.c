#include "shared/sdf/sdfQuat/decls.h"

/* vu0 routine: the composed quaternion is left in the shared register. */
void FromEulerXYZ(f32 aRotateX, f32 aRotateY, f32 aRotateZ)
{
    /* VU lqc2 reads this scratch through its address; retain retail store order. */
    volatile f32 quaternion[4] __attribute__((aligned(16)));
    f32 half;
    f32 cx;
    f32 sx;
    f32 cy;
    f32 sy;
    f32 cz;
    f32 sz;

    half = sdfClipAngleValuePI(-aRotateX) * 0.5f;
    cx = sdfCos(half);
    sx = sdfSin(half);
    half = sdfClipAngleValuePI(-aRotateY) * 0.5f;
    cy = sdfCos(half);
    sy = sdfSin(half);
    half = sdfClipAngleValuePI(-aRotateZ) * 0.5f;
    cz = sdfCos(half);
    sz = sdfSin(half);
    quaternion[0] = sz * sy * cx + cz * cy * sx;
    quaternion[1] = cz * sy * cx - sz * cy * sx;
    quaternion[2] = sz * cy * cx + cz * sy * sx;
    quaternion[3] = cz * cy * cx - sz * sy * sx;
    SDF_VU0_LOADVECTOR((f32 *)quaternion);
}
