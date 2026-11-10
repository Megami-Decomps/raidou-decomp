#include "shared/model/mdlEffect/decls.h"

/* vu0 routine: transform an anchor, then offset it away from the eye. */
void mdlEffect_P2A_D3P_GetPos(sdfModelRootNode_t *model, dds3ModelEffectCtrlr_t *effect, f32 *vect) {
    mdlMarkPos_t *mapPosition = effect->payload.part.mapPositionRecord;
    sdfModelNode_t *drawNode = sdfModelSearchNode(model, mapPosition->nodeId);
    f32 scale = effect->payload.part.anchorScale;

    SDF_VU0_LOADMATRIX(&drawNode->tm);
        SDF_VU0_LOADVECTOR( &mapPosition->position);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR_C( aViewEye);
    SDF_VU0_SUBVECTOR_A_C();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_BITS(scale);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM( vect);
}
