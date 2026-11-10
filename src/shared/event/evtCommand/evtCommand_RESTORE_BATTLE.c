#include "shared/event/evtCommand/decls.h"

s32 evtCommand_RESTORE_BATTLE(void)
{
    s32 eventId;
    s32 requestMode;

    eventId = scrGetIntPara(0);
    requestMode = scrGetIntPara(1);
    CallBattleSub(eventId, requestMode);
    return 1;
}
