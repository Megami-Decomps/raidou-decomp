#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matUvwBlendSeek(sdfMotNodeMatUvw_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    sdfMotUvwKey_t dst;
    sdfMotUvwKey_t key;

    sdfMotGetKey(&aThis->base.nodeBase, aFrame, &itf);
    sdfMotInterpolateKeyUvw(&itf, &key);
    sdfMotInterpolateKeyUvwSub(dst.parm, aThis->blendFrom.parm, key.parm, aBlendRate);
    sdfMatSetTexUvwByPtr(aThis->base.mat, &dst);
}
