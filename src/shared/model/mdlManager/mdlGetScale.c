#include "shared/model/mdlManager/decls.h"

void mdlGetScale(mdlObj_t *aHandle)
{
    SDF_VU0_LOADVECTOR_BARRIER(&aHandle->inner->scale);
}
