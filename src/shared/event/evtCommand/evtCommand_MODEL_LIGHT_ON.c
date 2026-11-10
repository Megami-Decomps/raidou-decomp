#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_LIGHT_ON(void) {
    s32 worldObjectKey;
    evtUnit_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    evtUnitLightOn(unit);
    return 1;
}
