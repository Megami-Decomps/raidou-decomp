#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_modelVisibleInitBlend(sdfMotNodeModelFlag_t *aThis)
{
    aThis->blendFrom = ((aThis->modelBase.modelNode->flags >> 4) ^ 1) & 1;
}
