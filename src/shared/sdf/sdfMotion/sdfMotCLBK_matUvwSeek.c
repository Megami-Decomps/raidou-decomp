#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matUvwSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;
    sdfMatUvwParm_t uvw;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMotInterpolateKeyUvw(&itf, &uvw);
    sdfMatSetTexUvwByPtr(aThis->mat, &uvw);
}
