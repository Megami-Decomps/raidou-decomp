#include "shared/event/evtCommand/decls.h"

u32 evtCommand_LOAD_SOUND_E(void)
{
    if (scrGetCommandTimer() == 0) {
        evtLoadEventBgm(scrGetIntPara(0));
    } else if (evtCheckEventBgmLoaded(scrGetIntPara(0)) != 0) {
        return 1;
    }
    return 0;
}
