#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetTexUvw(sdfMat_t *aMat, f32 aOffsetX, f32 aOffsetY, f32 aTileX, f32 aTileY,
                     f32 aAngle)
{
    f32 *values = sdfMatGetTexUvwParm(aMat)->offset;

    values[0] = aOffsetX;
    values[1] = aOffsetY;
    values[2] = aTileX;
    values[3] = aTileY;
    values[4] = aAngle;
    aMat->duty |= 0xC;
}
