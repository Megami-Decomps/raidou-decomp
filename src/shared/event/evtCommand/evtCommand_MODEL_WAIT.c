#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_WAIT(void)
{
    s32 key;
    void *unit;
    u32 ret = 1;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    if (unit == NULL) {
        return ret;
    }
    return evtCheckUnitWait(unit) != 0;
}
