#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matMultiDiffuseSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetMultiDiffuse(aThis->mat, sdfMotInterpolateKeyColor(&itf));
}
