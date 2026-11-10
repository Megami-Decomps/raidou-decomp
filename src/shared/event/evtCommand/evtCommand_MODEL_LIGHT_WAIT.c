#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_LIGHT_WAIT(void)
{
    s32 key;
    void *unit;
    s32 stat;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    stat = evtUnitLightAnimeStat(unit);
    return stat == 0;
}
