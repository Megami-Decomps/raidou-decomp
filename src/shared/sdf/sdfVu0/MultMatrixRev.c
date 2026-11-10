#include "shared/sdf/sdfVu0/decls.h"

void MultMatrixRev(const void *aMatrix) {
    SDF_VU0_LOADMATRIX_B(aMatrix);
    MultMatrixRevNoArg();
}
