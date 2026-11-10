#include "shared/sdf/sdfMotion/decls.h"

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLerpRotQuatBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate) {
    sdfMotGetKeyIf_t keyInterval;
    sdfModelNode_t *drawNode;
    f32 (*matrix)[4];
    f32 *keyData;

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    keyData = keyInterval.key0;
    SDF_VU0_LOADSHORTVECTOR(keyData);
    keyData = keyInterval.key1;
    SDF_VU0_LOADSHORTVECTOR_B(keyData);
    Lerp(keyInterval.rate);
    drawNode = aThis->modelBase.modelNode;
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&aThis->blendFrom);
    Lerp(aBlendRate);
    SDF_VU0_STOREVECTOR_NOMEM(&drawNode->rotate);
    matrix = (f32 (*)[4])drawNode->localNoScaledTm.m;
    ToMatrix();
    SDF_VU0_STORE_ROW0_NOMEM(matrix[0]);
    SDF_VU0_STORE_ROW1_NOMEM(matrix[1]);
    SDF_VU0_STORE_ROW2_NOMEM(matrix[2]);
}
