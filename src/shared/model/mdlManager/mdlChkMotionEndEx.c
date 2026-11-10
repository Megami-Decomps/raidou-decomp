#include "shared/model/mdlManager/decls.h"

s32 mdlChkMotionEndEx(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched == NULL)
    {
        return 2;
    }
    return matched->stat == 5;
}
