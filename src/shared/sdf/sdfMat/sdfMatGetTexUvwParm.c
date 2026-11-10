#include "shared/sdf/sdfMat/decls.h"

sdfMatUvwParm_t *sdfMatGetTexUvwParm(sdfMat_t *aMat)
{
    sdfMatUvwParm_t *parm = aMat->uvwParm;

    if (parm == NULL) {
        parm = sdfMatAllocTexUvwParm();
        aMat->uvwParm = parm;
    }
    return parm;
}
