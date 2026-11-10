#include "shared/basic/dds3Basic/decls.h"

s32 dds3ComparePosPoseSet(dds3PoseSet_t *left, dds3PoseSet_t *right)
{
    f32 distance;
    f32 leftRadius;
    f32 rightRadius;

    SDF_VU0_LOADVECTOR(&left->pos);
    SDF_VU0_LOADVECTOR_B(&right->pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    leftRadius = left->r;
    rightRadius = right->r;
    if (fabsf(distance) - (leftRadius + rightRadius) < 0.0f) {
        return 1;
    }
    return 0;
}
