#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matAmbientSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetAmbient(aThis->mat, sdfMotInterpolateKeyColor(&itf));
}
