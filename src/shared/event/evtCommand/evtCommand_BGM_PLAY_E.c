#include "shared/event/evtCommand/decls.h"

u32 evtCommand_BGM_PLAY_E(void) {
    s32 eventId;
    s32 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtPlayEventBgm(eventId, fade);
    return 1;
}
