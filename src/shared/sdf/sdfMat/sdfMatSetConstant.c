#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio)
{
    aMat->constant = aRatio;
    aMat->duty = aMat->duty | 3;
}
