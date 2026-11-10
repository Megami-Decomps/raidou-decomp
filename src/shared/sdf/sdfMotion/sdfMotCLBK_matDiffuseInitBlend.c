#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matDiffuseInitBlend(sdfMotNodeMatColor_t *aThis)
{
    aThis->blendSrc = aThis->base.mat->diffuse;
}
