#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetRefMap(sdfMat_t *aMat, u32 aColor)
{
    aMat->refmap = aColor;
    aMat->duty |= 3;
}
