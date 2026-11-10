#ifndef DDS_EVT_SOUND_VARIATION_H

#define DDS_EVT_SOUND_VARIATION_H
#include "common.h"

/* Signed selector added to an event sound's packed track ID.
 * evtTransEventBgm preserves the existing negative value as a no-track ID. */
typedef s32 EvtSoundVariation;

/* Packed sequence/variation identifier consumed by the sound engine. */
typedef s32 EvtPackedSoundTrackId;

#endif /* DDS_EVT_SOUND_VARIATION_H */
