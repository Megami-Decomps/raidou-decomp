#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetFaceDivParm(sdfMat_t *aMat, f32 aBias, f32 aSoften)
{
    aMat->fdBias = aBias;
    aMat->fdSoften = aSoften;
    aMat->duty = aMat->duty | 0xC0;
}
