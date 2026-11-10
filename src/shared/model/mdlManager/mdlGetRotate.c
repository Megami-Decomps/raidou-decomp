#include "shared/model/mdlManager/decls.h"

void mdlGetRotate(mdlObj_t *aHandle)
{
    SDF_VU0_LOADVECTOR_BARRIER(&aHandle->inner->rotate);
}
