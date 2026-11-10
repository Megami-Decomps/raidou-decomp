#ifndef DDS_SND_TRACK_SLOT_H

#define DDS_SND_TRACK_SLOT_H
#include "common.h"

/* The mixer lookup and balance query share this eight-byte MIDI track slot.
 * Only the ID and two balance-query components are interpreted here. */
typedef struct SndTrackSlot {
    s32 id; /* 0x00 */
    u8 balanceFirstComponent; /* 0x04 */
    u8 balanceSecondComponent; /* 0x05 */
    u8 unknown_06[2];
} SndTrackSlot;

typedef char SndTrackSlot_size_must_be_8[
    (sizeof(SndTrackSlot) == 8) ? 1 : -1];

#endif /* DDS_SND_TRACK_SLOT_H */
