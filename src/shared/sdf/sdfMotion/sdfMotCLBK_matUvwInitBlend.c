#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matUvwInitBlend(sdfMotNodeMatUvw_t *aThis)
{
    aThis->blendFrom = *(sdfMotUvwKey_t *)sdfMatGetTexUvwParm(aThis->base.mat);
}
