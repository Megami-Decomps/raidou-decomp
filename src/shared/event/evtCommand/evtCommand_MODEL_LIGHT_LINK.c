#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_LIGHT_LINK(void)
{
    s32 key;
    void *unit;
    void *target;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    target = evtSearchBasic(9, scrGetIntPara(2));
    if (target == NULL) {
        return 1;
    }
    evtLinkLightObjectBasic(unit, target, scrGetIntPara(1));
    return 1;
}
