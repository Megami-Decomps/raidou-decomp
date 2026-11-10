#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetTex(sdfMat_t *aMat, sdfTexHandle_t *aTex)
{
    aMat->texHandle = aTex;
    aMat->duty |= 3;
}
