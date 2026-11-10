#include "shared/sdf/sdfModel/decls.h"

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelMakeMarkPos(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark) {
    sdfModelNode_t *drawNode = sdfModelSearchNode(aRoot, aMark->nodeId);

    SDF_VU0_LOADMATRIX(&drawNode->tm);
    SDF_VU0_LOADVECTOR(&aMark->pos);
    SDF_VU0_APPLYMATRIX_XYZ1();
}
