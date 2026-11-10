#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_MOVE_MOT(void)
{
    s32 key;
    void *unit;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    if (unit == NULL) {
        return 1;
    }
    evtSetUnitMotion(unit, scrGetIntPara(1), 5, 7, 1);
    printf(evtMsgModelMoveMotDeprecated);
    return 1;
}
