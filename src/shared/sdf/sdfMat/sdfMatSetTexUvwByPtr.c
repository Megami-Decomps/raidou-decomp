#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetTexUvwByPtr(sdfMat_t *aMat, const f32 *aParm)
{
    f32 *values = sdfMatGetTexUvwParm(aMat)->offset;

    values[0] = aParm[0];
    values[1] = aParm[1];
    values[2] = aParm[2];
    values[3] = aParm[3];
    values[4] = aParm[4];
    aMat->duty |= 0xC;
}
