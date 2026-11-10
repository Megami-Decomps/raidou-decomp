#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matUvw2Seek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetTexUvwByPtr(aThis->mat, itf.key0);
}
