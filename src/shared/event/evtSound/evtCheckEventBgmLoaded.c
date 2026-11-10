#include "shared/event/evtSound/decls.h"

s32 evtCheckEventBgmLoaded(s32 eventId)
{
    if ((u32)(eventId - 0x258) >= 0x100) {
        return 1;
    }
    return p2sdLoadCheckMidi(evtGetSoundIndex(eventId, 0)) == 1;
}
