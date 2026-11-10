#include "shared/event/evtSound/decls.h"

s32 evtPlaySe(s32 eventno, s32 seno)
{
    s32 soundIndex;

    if ((u32)(eventno - 0x258) >= 0x100) {
        return 0;
    }
    soundIndex = evtGetSoundIndex(eventno, seno);
    p2sdPlaySE(soundIndex, 0x7F, 0x3F);
    return soundIndex;
}
