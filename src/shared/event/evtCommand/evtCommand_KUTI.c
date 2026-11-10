#include "shared/event/evtCommand/decls.h"

u32 evtCommand_KUTI(void) {
    evtUnit_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 firstSlot = scrGetIntPara(1);
        s32 secondSlot = scrGetIntPara(2);
        evtSetUnitKutiMotion(unit, firstSlot, secondSlot);
        evtRunUnitKutiMotion(EE_ADDR(unit));
        evtKutiUnit = EE_ADDR(unit);
    }
    {
        s32 window = scrGetMessageHandle();
        if (window < 0) {
            return 1;
        }
        itfMesMngSetKutiCallback(window, evtRunKutiMotion);
    }
    return 1;
}
