#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matConstantBlendSeek(sdfMotNodeMatFloat_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    f32 to;
    f32 from;

    sdfMotGetKey(&aThis->matBase.nodeBase, aFrame, &itf);
    to = sdfMotInterpolateKeyFloat(&itf);
    from = aThis->blendFrom;
    sdfMatSetConstant(aThis->matBase.mat, from + to * aBlendRate - from * aBlendRate);
}
