#include "shared/sdf/sdfMotion/decls.h"

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerRotXYZBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate) {
    sdfMotGetKeyIf_t keyInterval;
    volatile sdfVector4_t euler;
    sdfModelNode_t *drawNode;
    f32 (*matrix)[4];

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    SDF_VU0_LOADVECTOR_UNALIGN(keyInterval.key0);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.key1);
    VU0_LERP_VF10_VF11(keyInterval.rate);
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_STOREVECTOR_NOMEM(&euler);
    FromEuler(euler.x, euler.y, euler.z);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&aThis->blendFrom);
    Lerp(aBlendRate);
    drawNode = aThis->modelBase.modelNode;
    SDF_VU0_STOREVECTOR_NOMEM(&drawNode->rotate);
    matrix = (f32 (*)[4])drawNode->localNoScaledTm.m;
    ToMatrix();
    SDF_VU0_STORE_ROW0_NOMEM(matrix[0]);
    SDF_VU0_STORE_ROW1_NOMEM(matrix[1]);
    SDF_VU0_STORE_ROW2_NOMEM(matrix[2]);
}
