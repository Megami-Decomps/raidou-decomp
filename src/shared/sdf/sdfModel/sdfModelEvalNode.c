#include "shared/sdf/sdfModel/decls.h"

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelEvalNode(sdfModelNode_t *aNode, const f32 (*aParentTm)[4], s32 aFrameId) {
    f32 *xAxis = &aNode->localNoScaledTm.m[0];
    f32 *yAxis;
    f32 *zAxis;
    f32 *scale;
    f32 *translation;
    f32 (*transformed)[4];
    u8 *workspace;
    sdfModelNode_t *child;

    SDF_VU0_LOAD_ROW0_BARRIER(xAxis);
    yAxis = &aNode->localNoScaledTm.m[4];
    SDF_VU0_LOAD_ROW1_BARRIER(yAxis);
    zAxis = &aNode->localNoScaledTm.m[8];
    SDF_VU0_LOAD_ROW2_BARRIER(zAxis);
    scale = (f32 *)&aNode->scale;
    SDF_VU0_LOADVECTOR_BARRIER(scale);
    SDF_VU0_SCALE_XYZ();
    translation = (f32 *)&aNode->pos;
    SDF_VU0_LOAD_ROW3_BARRIER(translation);
    SDF_VU0_LOADMATRIX_B(aParentTm);
    MultMatrixRevNoArg();
    transformed = (f32 (*)[4])aNode->tm.m;
    SDF_VU0_STOREMATRIX(transformed);
    workspace = aNode->tmPacket;
    if (workspace != NULL) {
        sdf3DPrimSetTmPacket((void *)(workspace + aFrameId * sizeof(sdfModelTmImage_t)));
    }
    child = aNode->children;
    if (child == 0) {
        return;
    }
    do {
        sdfModelEvalNode(child, (const f32 (*)[4])transformed, aFrameId);
        child = child->next;
    } while (child != aNode->children);
}
