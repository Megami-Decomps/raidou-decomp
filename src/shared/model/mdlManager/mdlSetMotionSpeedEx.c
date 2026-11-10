#include "shared/model/mdlManager/decls.h"

void mdlSetMotionSpeedEx(mdlObj_t *aHandle, s32 aGroup, f32 speed)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched != NULL)
    {
        matched->speed = speed;
    }
}
