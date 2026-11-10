#include "shared/event/evtCommand/decls.h"

u32 evtCommand_BGM_TRANS_E(void) {
    s32 eventId;
    s32 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtTransEventBgm(eventId, fade);
    return 1;
}
