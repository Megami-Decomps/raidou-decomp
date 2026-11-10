#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matDiffuseBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    u32 to;
    u32 color;

    sdfMotGetKey(&aThis->base.nodeBase, aFrame, &itf);
    to = sdfMotInterpolateKeyColor(&itf);
    VU0_RGBA_LERP(color, aThis->blendSrc, to, aBlendRate, 0.5f);
    sdfMatSetDiffuse(aThis->base.mat, color);
}
