#include "shared/event/evtCommand/decls.h"

s32 evtCommand_CALL_BATTLE2(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    CallBattleSub2(eventId);
    return 1;
}
