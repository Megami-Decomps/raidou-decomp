#include "shared/sdf/sdfMat/decls.h"

void sdfMatCopyStat(sdfMat_t *aDstMat, sdfMat_t *aSrcMat)
{
    sdfMatUvwParm_t *parm;

    aDstMat->duty = 0xFF;
    aDstMat->ambient = aSrcMat->ambient;
    aDstMat->diffuse = aSrcMat->diffuse;
    aDstMat->constant = aSrcMat->constant;
    aDstMat->multiTexDiffuse = aSrcMat->multiTexDiffuse;
    aDstMat->flags = aSrcMat->flags;
    aDstMat->texHandle = aSrcMat->texHandle;
    aDstMat->specular = aSrcMat->specular;
    aDstMat->refmap = aSrcMat->refmap;
    parm = aSrcMat->uvwParm;
    if (parm != NULL) {
        *sdfMatGetTexUvwParm(aDstMat) = *parm;
    }
    parm = aSrcMat->multiTexUvwParm;
    if (parm != NULL) {
        *sdfMatGetMultiTexUvwParm(aDstMat) = *parm;
    }
    aDstMat->fdBias = aSrcMat->fdBias;
    aDstMat->fdSoften = aSrcMat->fdSoften;
}
