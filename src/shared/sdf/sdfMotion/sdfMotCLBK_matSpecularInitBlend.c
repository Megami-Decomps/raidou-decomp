#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matSpecularInitBlend(sdfMotNodeMatColor_t *aThis)
{
    aThis->blendSrc = aThis->base.mat->specular;
}
