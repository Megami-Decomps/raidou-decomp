#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matRefMapSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetRefMap(aThis->mat, sdfMotInterpolateKeyColor(&itf));
}
