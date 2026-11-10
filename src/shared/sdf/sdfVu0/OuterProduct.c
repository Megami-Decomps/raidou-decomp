#include "shared/sdf/sdfVu0/decls.h"

void OuterProduct(void *aReVector, const void *aVectorA, const void *aVectorB) {
    SDF_VU0_LOADVECTOR(aVectorA);
    SDF_VU0_LOADVECTOR_B(aVectorB);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR(aReVector);
}
