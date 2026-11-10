#include "shared/event/evtSound/decls.h"

s32 evtLoadEventBgm(s32 eventId)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    soundIndex = evtGetSoundIndex(eventId, 0);
    p2sdLoadSMGFile(soundIndex);
    return soundIndex;
}
