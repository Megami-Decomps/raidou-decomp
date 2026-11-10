#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetMultiDiffuse(sdfMat_t *aMat, u32 aColor)
{
    aMat->multiTexDiffuse = aColor;
    aMat->duty |= 0x30;
}
