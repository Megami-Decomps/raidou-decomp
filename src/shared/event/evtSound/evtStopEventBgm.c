#include "shared/event/evtSound/decls.h"

s32 evtStopEventBgm(s32 eventId, s32 variation)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    soundIndex = evtGetSoundIndex(eventId, variation);
    p2sdStopBGM(soundIndex);
    return soundIndex;
}
