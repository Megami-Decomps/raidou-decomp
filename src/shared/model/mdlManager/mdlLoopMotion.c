#include "shared/model/mdlManager/decls.h"

void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber)
{
    mdlMotion(aHandle, aGroup, aNumber, 1, 0.0f, 0.0f);
}
