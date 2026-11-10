#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matDiffuseSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetDiffuse(aThis->mat, sdfMotInterpolateKeyColor(&itf));
}
