#include "shared/sdf/sdfModel/decls.h"

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelEvalTm2(sdfModelRootNode_t *aRoot, s32 aFrameId) {
    u128 rootMatrix[4];
    void *transform = &aRoot->tm;
    f32 *scale;
    sdfModelNodeList_t *list;

    SDF_VU0_LOADMATRIX(transform);
    scale = (f32 *)&aRoot->scale;
    SDF_VU0_LOADVECTOR_BARRIER(scale);
    SDF_VU0_SCALE_XYZ_REV();
    SDF_VU0_STOREMATRIX_MEMOPS(rootMatrix[0], rootMatrix[1], rootMatrix[2], rootMatrix[3]);
    list = aRoot->nodeList;
    sdfModelEvalNode(((sdfModelNode_t **)list->items)[0], (const f32 (*)[4])rootMatrix, aFrameId);
}
