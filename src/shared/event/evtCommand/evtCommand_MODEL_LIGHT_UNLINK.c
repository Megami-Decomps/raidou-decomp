#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_LIGHT_UNLINK(void)
{
    s32 key;
    void *unit;
    s32 time;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    time = scrGetIntPara(1);
    evtUnlinkLightObjectBasic(unit, time);
    return 1;
}
