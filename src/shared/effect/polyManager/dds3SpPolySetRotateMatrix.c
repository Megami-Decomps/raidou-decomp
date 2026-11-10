#include "shared/effect/polyManager/decls.h"

/* vu0 routine: compose the new rotation with the retained source matrix. */
void dds3SpPolySetRotateMatrix(dds3SpPoly_Basic_t *hData, void *pMatrix) {
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_LOADMATRIX_B(hData->matrixCopy);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX(hData->matrix);
}
