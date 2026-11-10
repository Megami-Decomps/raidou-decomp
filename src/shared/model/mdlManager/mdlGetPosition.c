#include "shared/model/mdlManager/decls.h"

void mdlGetPosition(mdlObj_t *aHandle)
{
    SDF_VU0_LOADVECTOR_BARRIER(&aHandle->inner->tm.m[12]);
}
