#include "shared/model/mdlManager/decls.h"

f32 mdlGetMotionSpeedEx(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;
    f32 result;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    result = 0.0f;
    if (matched != NULL)
    {
        result = matched->speed;
    }
    return result;
}
