#ifndef DDS_SND_SLOT_H

#define DDS_SND_SLOT_H
#include "common.h"

struct FileRequest;

struct sdfMemHandle_t;

typedef struct SoundSlotTableEntry {
    s16 resourceOffset;
    u16 fileId;
} SoundSlotTableEntry;

/* Shared motion-SE cache, allocated as 0x108 bytes by DDS1 001F3E70 and
 * DDS2 00204B00. The key and work subobject begin at +0x08. */
typedef struct SoundSlotWork {
    s32 id;
    u32 refCount;
    u32 pendingPackedTrackPosition; /* Owner +0x10: key passed to packed-track load queries. */
    s32 pendingSlot;
    struct FileRequest *fileRequests[0x1D];
    struct sdfMemHandle_t *resourceHandles[0x1D];
} SoundSlotWork;

/* SoundSlotOwner flag bits observed across file and packed-track loading. */
typedef enum SoundSlotOwnerFlag {
    SOUND_SLOT_FILE_LOAD_PENDING = 1,
    SOUND_SLOT_FILES_READY = 2,
    SOUND_SLOT_TRACK_LOAD_REQUESTED = 4,
    SOUND_SLOT_TRACK_LOADING = 8,
    SOUND_SLOT_TRACK_READY = 0x10
} SoundSlotOwnerFlag;

typedef struct SoundSlotOwner {
    u32 flags; /* SoundSlotOwnerFlag masks; other bits remain unclassified. */
    s32 category;
    SoundSlotWork work;
    struct SoundSlotOwner *prev;
    struct SoundSlotOwner *next;
} SoundSlotOwner;

typedef char SoundSlotWorkSizeCheck[(sizeof(SoundSlotWork) == 0xF8) ? 1 : -1];

typedef char SoundSlotOwnerSizeCheck[(sizeof(SoundSlotOwner) == 0x108) ? 1 : -1];

SoundSlotOwner *sndFindListNodeForChannel(s32 category, s32 id);

void sndReleaseSlotOwner(SoundSlotOwner *owner);

#endif
