#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetMultiMethod(sdfMat_t *aMat, u32 aMethod)
{
    aMat->multiTexMethod = aMethod;
    aMat->duty |= 0x30;
}
