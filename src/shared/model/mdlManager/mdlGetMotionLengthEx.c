#include "shared/model/mdlManager/decls.h"

u16 mdlGetMotionLengthEx(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched == NULL)
    {
        return 0;
    }
    return matched->frameLength;
}
