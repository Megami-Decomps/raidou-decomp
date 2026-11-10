#include "shared/model/mdlManager/decls.h"

void mdlSetPosition(mdlObj_t *aHandle)
{
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&aHandle->inner->tm.m[12]);
}
