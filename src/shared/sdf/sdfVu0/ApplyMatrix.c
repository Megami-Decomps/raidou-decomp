#include "shared/sdf/sdfVu0/decls.h"

void ApplyMatrix(void *aReVector, const void *aVector) {
    SDF_VU0_LOADVECTOR(aVector);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(aReVector);
}
