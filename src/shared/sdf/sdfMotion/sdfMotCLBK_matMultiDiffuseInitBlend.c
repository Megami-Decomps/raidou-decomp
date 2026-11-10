#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matMultiDiffuseInitBlend(sdfMotNodeMatColor_t *aThis)
{
    aThis->blendSrc = aThis->base.mat->multiTexDiffuse;
}
