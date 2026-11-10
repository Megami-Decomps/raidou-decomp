#ifndef DDS_SDF_QUATERNION_H

#define DDS_SDF_QUATERNION_H
#include "common.h"

/* RenderWare XYZ axis used by the native SDF matrix builders. */
typedef struct RwV3d
{
    f32 x;
    f32 y;
    f32 z;
} RwV3d;

typedef char RwV3dSizeAssert[(sizeof(RwV3d) == 0xC) ? 1 : -1];

/* Normalize the supplied axis and write an XYZW quaternion. The native
 * conversion uses nonnegative square roots for both half-angle components. */
void sdfQuatFromAxisAngle(f32 *out, f32 x, f32 y, f32 z, f32 angle);

/* Write three axis components and twice acos(W), using the native small
 * denominator fallback when sqrt(1-W*W) is near zero. */
void sdfQuatToAxisAngle(f32 *axis, f32 *angle, const f32 *quaternion);

void sdfQuatMultiply(f32 *output, f32 *left, f32 *right);

#endif /* DDS_SDF_QUATERNION_H */
