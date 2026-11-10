#include "shared/sdf/sdfMat/decls.h"

sdfMatUvwParm_t *sdfMatGetMultiTexUvwParm(sdfMat_t *aMat)
{
    sdfMatUvwParm_t *parm = aMat->multiTexUvwParm;

    if (parm == NULL) {
        parm = sdfMatAllocTexUvwParm();
        aMat->multiTexUvwParm = parm;
    }
    return parm;
}
