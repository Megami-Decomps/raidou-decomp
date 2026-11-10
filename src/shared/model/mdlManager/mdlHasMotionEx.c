#include "shared/model/mdlManager/decls.h"

s32 mdlHasMotionEx(mdlObj_t *aHandle, s32 aGroup)
{
    return mdlGetMotionCtrlr(aHandle, aGroup) != NULL;
}
