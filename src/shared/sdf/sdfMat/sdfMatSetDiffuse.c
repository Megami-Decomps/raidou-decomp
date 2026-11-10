#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetDiffuse(sdfMat_t *aMat, u32 aColor)
{
    aMat->diffuse = aColor;
    aMat->duty |= 3;
}
