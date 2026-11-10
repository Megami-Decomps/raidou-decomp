#include "shared/model/mdlManager/decls.h"

void mdlSetScale(mdlObj_t *aHandle)
{
    SDF_VU0_STOREVECTOR(&aHandle->inner->scale);
}
