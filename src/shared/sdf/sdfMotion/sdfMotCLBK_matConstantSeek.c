#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matConstantSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetConstant(aThis->mat, sdfMotInterpolateKeyFloat(&itf));
}
