#include "shared/sdf/sdfModel/decls.h"

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelMakeMarkPosMatrix(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark) {
    sdfModelNode_t *drawNode = sdfModelSearchNode(aRoot, aMark->nodeId);
    u8 *vector;

    vector = (u8 *)&aMark->dir;
    SDF_VU0_LOADVECTOR(vector);
    SDF_VU0_SET_ROW2();
    SDF_VU0_COPYVECTOR_A_TO_B();
    vector = (u8 *)&aMark->up;
    SDF_VU0_LOADVECTOR(vector);
    SDF_VU0_NEGVECTOR_XYZ();
    SDF_VU0_SET_ROW1();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW0();
    SDF_VU0_LOAD_ROW3(&aMark->pos);
    SDF_VU0_MATRIX_ROW3_W_SET_ONE();
    MultMatrixRev(&drawNode->tm);
}
