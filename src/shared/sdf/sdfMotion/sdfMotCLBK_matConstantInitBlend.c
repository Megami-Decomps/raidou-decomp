#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_matConstantInitBlend(sdfMotNodeMatFloat_t *aThis)
{
    aThis->blendFrom = aThis->matBase.mat->constant;
}
