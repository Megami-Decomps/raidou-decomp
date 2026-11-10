#ifndef DDS_SND_SEQUENCE_VOLUME_PAN_H

#define DDS_SND_SEQUENCE_VOLUME_PAN_H
#include "common.h"

/* Local packet object; only the first 0xC bytes are sent to the sound worker. */
typedef struct SndSequenceVolumePanPacket {
    u32 trackId;          /* +0x00 */
    u32 opaque04;         /* +0x04 */
    u16 volume;           /* +0x08 */
    u16 pan;              /* +0x0A */
    u32 opaqueNotSent0C;  /* +0x0C */
} SndSequenceVolumePanPacket;

#endif /* DDS_SND_SEQUENCE_VOLUME_PAN_H */
