#include "shared/sdf/sdfMotion/decls.h"

void sdfMotionChange(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType)
{
    sdfMotionChangeBlend(aCtrlr, aCutNum, aType, 0.0f, 0.0f);
}
