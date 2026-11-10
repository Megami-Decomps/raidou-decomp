#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matUvw2BlendSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetTexUvwByPtr(aThis->mat, itf.key0);
}
