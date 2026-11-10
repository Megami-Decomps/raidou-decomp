#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matAmbientInitBlend(sdfMotNodeMatColor_t *aThis)
{
    aThis->blendSrc = aThis->base.mat->ambient;
}
