#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_modelRotInitBlend(sdfMotNodeModelVec_t *aThis)
{
    SDF_VU0_COPYVECTOR(&aThis->blendFrom, &aThis->modelBase.modelNode->rotate);
}
