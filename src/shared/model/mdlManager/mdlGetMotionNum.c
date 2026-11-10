#include "shared/model/mdlManager/decls.h"

s32 mdlGetMotionNum(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched == NULL)
    {
        return -1;
    }
    return matched->cutNumber;
}
