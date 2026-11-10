#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetMultiTex(sdfMat_t *aMat, sdfTexHandle_t *aTex)
{
    aMat->multiTexHandle = aTex;
    aMat->duty |= 0x30;
}
