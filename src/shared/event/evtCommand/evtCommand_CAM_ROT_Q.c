#include "shared/event/evtCommand/decls.h"

s32 evtCommand_CAM_ROT_Q(void) {
    void *effectUnit;
    sdfVector4_t quaternion;

    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetUnitFollowCamera();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    quaternion.x = scrGetFloatPara(1);
    quaternion.y = scrGetFloatPara(2);
    quaternion.z = scrGetFloatPara(3);
    quaternion.w = scrGetFloatPara(4);
    dds3SetRotPoseSet(effectUnit, (u128 *)&quaternion);
    return 1;
}
