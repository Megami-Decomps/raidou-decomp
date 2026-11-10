#include "shared/sdf/sdfVu0/decls.h"

void MultMatrix(void *aMatrix) {
    SDF_VU0_LOADMATRIX_B(aMatrix);
    MultMatrixNoArg();
}
