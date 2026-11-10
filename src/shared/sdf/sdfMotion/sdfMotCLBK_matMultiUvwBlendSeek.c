#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matMultiUvwBlendSeek(sdfMotNodeMatUvw_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    sdfMotUvwKey_t key;
    sdfMotUvwKey_t dst;

    sdfMotGetKey(&aThis->base.nodeBase, aFrame, &itf);
    sdfMotInterpolateKeyUvw(&itf, &key);
    sdfMotInterpolateKeyUvwSub(dst.parm, aThis->blendFrom.parm, key.parm, aBlendRate);
    sdfMatSetMultiTexUvwByPtr(aThis->base.mat, &dst);
}
