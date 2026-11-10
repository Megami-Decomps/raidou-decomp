#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetAmbient(sdfMat_t *aMat, u32 aColor)
{
    aMat->ambient = aColor;
    aMat->duty |= 3;
}
