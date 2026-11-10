#include "shared/model/mdlManager/decls.h"

void mdlSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber)
{
    mdlMotion(aHandle, aGroup, aNumber, 0, 0.0f, 0.0f);
}
