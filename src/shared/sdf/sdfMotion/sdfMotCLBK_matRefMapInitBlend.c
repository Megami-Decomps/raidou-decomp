#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matRefMapInitBlend(sdfMotNodeMatColor_t *aThis)
{
    aThis->blendSrc = aThis->base.mat->refmap;
}
