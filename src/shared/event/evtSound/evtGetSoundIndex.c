#include "shared/event/evtSound/decls.h"

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
