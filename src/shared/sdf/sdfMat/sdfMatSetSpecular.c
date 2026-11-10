#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetSpecular(sdfMat_t *aMat, u32 aColor)
{
    aMat->specular = aColor;
    aMat->duty |= 3;
}
