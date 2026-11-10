#include "shared/model/mdlManager/decls.h"

void mdlSetRotate(mdlObj_t *aHandle)
{
    SDF_VU0_STOREVECTOR(&aHandle->inner->rotate);
    ToMatrix();
    SDF_VU0_STORE_ROW0(&aHandle->inner->tm.m[0]);
    SDF_VU0_STORE_ROW1(&aHandle->inner->tm.m[4]);
    SDF_VU0_STORE_ROW2(&aHandle->inner->tm.m[8]);
}
