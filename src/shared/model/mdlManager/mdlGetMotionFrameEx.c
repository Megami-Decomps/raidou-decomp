#include "shared/model/mdlManager/decls.h"

s32 mdlGetMotionFrameEx(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched == NULL)
    {
        return 0;
    }
    return (s32)matched->frame;
}
