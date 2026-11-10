#ifndef DDS_SND_WORK_H

#define DDS_SND_WORK_H
#include "common.h"

#include "snd_track_slot.h"

#define SND_WORK_CHANNEL_COUNT 16

#define SND_WORK_IOP_BUFFER_COUNT 16

#define SND_WORK_TRACK_SLOT_COUNT 13

typedef struct SndChannel {
    u8 index;
    u8 unk1;
    s16 unk2;
    u32 unk4;
    u32 iopWorkAddress;
    u32 unkC;
} SndChannel;

typedef struct SndIopBuffer {
    s32 size;
    u32 iopAddress; /* IOP address returned by sdfSndIopAlloc. */
} SndIopBuffer;

/* Prefix and arrays of the MIDI sound state. The IOP image header identifies
 * the EE-side work image and the byte count copied from its start. */
typedef struct SndWork {
    s32 header;
    s32 bufferCount;
    u32 iopBufferAddress;
    u32 iopBufferBytes;
    SndChannel channels[SND_WORK_CHANNEL_COUNT];
    SndIopBuffer buffers[SND_WORK_IOP_BUFFER_COUNT];
    SndTrackSlot slots[SND_WORK_TRACK_SLOT_COUNT];
    struct SndWork *eeWorkAddress;
    u32 eeWorkByteCount;
    u32 opaque200;
    u32 opaque204;
    u32 opaque208;
    u32 opaque20C;
} SndWork;

/* sceSifSetDma descriptor: four words filled before one descriptor is submitted. */
typedef struct SifDmaTransfer {
    u32 source;
    s32 destination;
    s32 size;
    s32 attributes;
} SifDmaTransfer;

typedef char SndChannelSizeAssert[(sizeof(SndChannel) == 0x10) ? 1 : -1];

#endif
