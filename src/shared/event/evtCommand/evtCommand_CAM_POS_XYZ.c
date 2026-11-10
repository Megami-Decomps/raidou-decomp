#include "shared/event/evtCommand/decls.h"

s32 evtCommand_CAM_POS_XYZ(void) {
    void *effectUnit;
    sdfVector4_t position;

    memset(&position, 0, sizeof(position));
    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetUnitFollowCamera();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    position.x = scrGetFloatPara(1);
    position.y = scrGetFloatPara(2);
    position.z = scrGetFloatPara(3);
    dds3SetPosPoseSet(effectUnit, (u128 *)&position);
    return 1;
}
