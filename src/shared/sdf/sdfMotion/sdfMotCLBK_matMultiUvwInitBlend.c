#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matMultiUvwInitBlend(sdfMotNodeMatUvw_t *aThis)
{
    aThis->blendFrom = *(sdfMotUvwKey_t *)sdfMatGetMultiTexUvwParm(aThis->base.mat);
}
