#include "common.h"
#include "kernel.h"
#include "sdfSound.h"
#include "evtSound.h"

extern char evtPackNameFormat[];

#include "sdk_libc.h"

s32 evtGetSoundIndex(s32 eventId, s32 variation);
s32 evtLoadEventBgm(s32 eventId);
s32 evtCheckEventBgmLoaded(s32 eventId);
void evtTransEventBgm(s32 eventId, s32 variation);
s32 evtFadeInEventBgm(s32 eventId, s32 variation);
s32 evtStopEventBgm(s32 eventId, s32 variation);
s32 evtPlaySe(s32 eventno, s32 seno);
void evtMakeEventPackName(s32 index, char *buffer);

s32 evtGetSoundIndex(s32 eventId, s32 variation)
{
    s32 index;

    index = 0xC7;
    if (eventId != 0x31F) {
        index = eventId - 0x259;
        if (eventId >= 0x320) {
            index = (eventId < 0x384) ? (eventId - 0x258) : (eventId - 0x29E);
        }
    }
    return ((index + 0x100) << 16) + variation;
}

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

s32 evtCheckEventBgmLoaded(s32 eventId)
{
    if ((u32)(eventId - 0x258) >= 0x100) {
        return 1;
    }
    return p2sdLoadCheckMidi(evtGetSoundIndex(eventId, 0)) == 1;
}

INCLUDE_ASM(const s32, "event/evtSound", evtPlayEventBgm);

void evtTransEventBgm(s32 eventId, s32 variation)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return;
    }
    soundIndex = -1;
    if (variation >= 0) {
        soundIndex = evtGetSoundIndex(eventId, variation);
    }
    printf("Event BGM trans :%08X\n", soundIndex);
    p2sdSetBgmTrack(soundIndex);
}

s32 evtFadeInEventBgm(s32 eventId, s32 variation)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    soundIndex = evtGetSoundIndex(eventId, variation);
    printf("Event BGM fade in play :%08X\n", soundIndex);
    p2sdPlayBGMFade(soundIndex);
    return soundIndex;
}

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

INCLUDE_ASM(const s32, "event/evtSound", evtStopSe);

void evtMakeEventPackName(s32 index, char *buffer)
{
    sprintf(buffer, evtPackNameFormat, index);
}

char evtPackNameFormat[8] = "e%03d_p";
