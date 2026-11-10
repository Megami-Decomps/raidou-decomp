#ifndef DDS_EFF_OWNER_RECORDS_H

#define DDS_EFF_OWNER_RECORDS_H
#include "common.h"

struct EffectSlotSet;

typedef struct EffectRecord {
    void *owner;
    s32 slot;
    struct EffectRecord *prev;
    struct EffectRecord *next;
} EffectRecord;

typedef struct EffectOwnerRecord {
    struct EffectSlotSet *owner;
    EffectRecord *entries[16];
} EffectOwnerRecord;

/* Per-slot effect data; only its sixteen-bucket index is recovered. */
typedef struct EffectSlot {
    u8 opaque00[0x28];
    s32 bucketIndex; /* 0x28: indexes EffectOwnerRecord.entries. */
    u8 opaque2C[0x54];
} EffectSlot;

typedef struct EffectSlotOwner {
    u8 opaque00[0x10];
    EffectSlot *slots; /* 0x10 */
} EffectSlotOwner;

typedef char EffectRecordSizeCheck[sizeof(EffectRecord) == 0x10 ? 1 : -1];

typedef char EffectOwnerRecordSizeCheck[sizeof(EffectOwnerRecord) == 0x44 ? 1 : -1];

typedef char EffectSlotSizeCheck[sizeof(EffectSlot) == 0x80 ? 1 : -1];

typedef char EffectSlotOwnerSizeCheck[sizeof(EffectSlotOwner) == 0x14 ? 1 : -1];

#endif
