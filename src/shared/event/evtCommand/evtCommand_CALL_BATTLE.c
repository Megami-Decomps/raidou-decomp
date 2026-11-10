#include "shared/event/evtCommand/decls.h"

s32 evtCommand_CALL_BATTLE(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    CallBattleSub(eventId, 0);
    return 1;
}
