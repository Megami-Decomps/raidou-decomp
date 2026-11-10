#include "shared/basic/dds3Basic/decls.h"

/* vu0 routine: distance between the two positions, against the right radius. */
s32 dds3ComparePosPoseSet2(dds3PoseSet_t *left, dds3PoseSet_t *right)
{
    f32 distance;
    f32 rightRadius;

    SDF_VU0_LOADVECTOR(&left->pos);
    SDF_VU0_LOADVECTOR_B(&right->pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    distance = sdfFabs(distance);
    rightRadius = right->r;
    if (distance - rightRadius < 0.0f) {
        return 1;
    }
    return 0;
}
