#include "shared/sdf/sdfMotion/decls.h"

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerScaleBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate) {
    sdfMotGetKeyIf_t keyInterval;

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    SDF_VU0_LOADVECTOR_UNALIGN(keyInterval.key0);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.key1);
    VU0_LERP_VF10_COPY(keyInterval.rate);
    SDF_VU0_LOADVECTOR(&aThis->blendFrom);
    VU0_LERP_VF10_W(aBlendRate);
    SDF_VU0_STOREVECTOR(&aThis->modelBase.modelNode->scale);
}
