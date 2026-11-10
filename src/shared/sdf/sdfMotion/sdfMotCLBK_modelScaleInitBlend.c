#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_modelScaleInitBlend(sdfMotNodeModelVec_t *aThis)
{
    SDF_VU0_COPYVECTOR(&aThis->blendFrom, &aThis->modelBase.modelNode->scale);
}
