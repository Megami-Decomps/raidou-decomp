#include "common.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfMath.h"
#include "fileManager.h"
#include "sdfTex.h"
#include "sdf2D.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"

#include "kernel_draw.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "ps2asm.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "code_r1_002C8188.h"
#include "code_r1_002CAFD8.h"

/* Return the payload allocation descriptor. For normal file loads, freeing the
 * request wrapper does not free this allocation; its resource owner must keep
 * or release it separately. */

/* Free a completed request wrapper; return one while it remains pending. */

/* fileLoadEx stores userData without copying it. When callbackAddress is
 * nonzero, the manager calls it once after the device request closes, passing
 * the completed request and userData. The context must remain valid until then;
 * callback delivery does not free the request wrapper, which callers release
 * with fileHandleFree. */

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */

/* effDestroyPackedBatch has one normal outcome after freeing all owned storage. */
typedef s32 EffMappedResourceDestroyResult;

enum {
    EFF_MAPPED_RESOURCE_DESTROYED = 1
};

/* effTimedState_t.flags bits used by phase initialization and endpoint handling. */
enum EffTimedStateFlag {
    /* Selects incrementing from zero instead of decrementing from the endpoint. */
    EFF_TIMED_STATE_DIRECTION_FORWARD = 0x1,
    /* Initialize the phase when a resource is bound to the state. */
    EFF_TIMED_STATE_INITIALIZE_PHASE_ON_BIND = 0x2,
    /* At either endpoint, restart the work or reverse direction when ping-pong is set. */
    EFF_TIMED_STATE_RESTART_AT_ENDPOINT = 0x4,
    /* With restart-at-endpoint set, switch between forward and reverse. */
    EFF_TIMED_STATE_PING_PONG = 0x8,
};

void effResetSlotWork(struct sprHandle_t *owner, u32 slotIndex);

/* Serialized source bytes are borrowed; async mapped-resource outputs hold typed pointers. */
struct effMappedResource_t *effCreateMappedResource(const u8 *source);

/* Header for count 0x6C-byte records; allocation is retained as a resource handle. */
typedef struct effPayload_t {
    struct sdfMemHandle_t *allocation;
    u32 count;
    u8 *records;
} effPayload_t;

/* Indexed timed-effect table entry; the first word's meaning is unknown. */
typedef struct effRecordBucket_t {
    u32 unk_00;
    /* Negative results hold phase; nonnegative results are per-update deltas. */
    s32 (*step)(BdWork *, BdWork *, effTimedState_t *);
    u32 count;
    u8 *records;
} effRecordBucket_t;

typedef char effRecordBucketSizeCheck[sizeof(effRecordBucket_t) == 0x10 ? 1 : -1];

typedef struct effRecord_t {
    void *owner;
    s32 slot;
    struct effRecord_t *prev;
    struct effRecord_t *next;
} effRecord_t;

typedef struct effOwnerRecord_t {
    struct sprHandle_t *owner;
    effRecord_t *entries[16];
} effOwnerRecord_t;

/* Per-slot effect data; only its sixteen-bucket index is recovered. */
typedef struct effSlot_t {
    u8 opaque00[0x28];
    s32 bucketIndex; /* 0x28: indexes effOwnerRecord_t.entries. */
    u8 opaque2C[0x54];
} effSlot_t;

typedef struct effSlotOwner_t {
    u8 opaque00[0x10];
    effSlot_t *slots; /* 0x10 */
} effSlotOwner_t;

typedef char effRecordSizeCheck[sizeof(effRecord_t) == 0x10 ? 1 : -1];

typedef char effOwnerRecordSizeCheck[sizeof(effOwnerRecord_t) == 0x44 ? 1 : -1];

typedef char effSlotSizeCheck[sizeof(effSlot_t) == 0x80 ? 1 : -1];

typedef char effSlotOwnerSizeCheck[sizeof(effSlotOwner_t) == 0x14 ? 1 : -1];

/* Declared before first use (were implicit int). */

typedef struct effObjectFlag_t {
    u8 pad00[0xC];
    u32 flags;
} effObjectFlag_t;

extern effRecordBucket_t effData21[];

/* Relative offsets in an effect resource's table and its eight-byte entries. */
typedef struct effResourceTable_t {
    u8 pad00[0xC];
    u32 entryOffset;
    u8 pad10[4];
    u16 textureCount;
    u16 slotCount;
} effResourceTable_t;

typedef struct effResourceEntry_t {
    u32 unknown;
    u32 resourceOffset;
} effResourceEntry_t;

extern char D_003BD198[];

#define EFF_RESOURCE_PATH_BYTES 0x80

#define EFF_RESOURCE_KEEP_ALLOCATION 1

#define EFF_RESOURCE_TRANSIENT 0

#define EFF_OWNER_LIST_BYTES 0x44

#define EFF_RECORD_BUCKETS 16

#define EFF_RECORD_LAST_BUCKET 15

#define EFF_STATUS_RECORD_BYTES 0x24

#define EFF_PACKED_STATUS_HEADER_BYTES 0x20

#define EFF_STATUS_PARAM_STRIDE 0x18

#define EFF_STATUS_PARAM_HEADER_BYTES 4

#define EFF_SLOT_WORK_BYTES 0xA0

#define EFF_SLOT_PHASE_ZERO 0

#define EFF_SLOT_PHASE_ONE 0x10000

#define EFF_PAYLOAD_RECORD_BYTES 0x6C

#define EFF_RESOURCE_TABLE_ENTRY_BYTES 8

void effCompleteTransientResourceJob(fileRequest_t *job, u32 *outInstance);
void effCompleteRetainedResourceJob(fileRequest_t *job, u32 *outInstance);

void effCompleteMappedResourceJob(fileRequest_t *job,
                                  effMappedResource_t **outMappedResource);

effOwnerRecord_t *effCreateOwnerRecordList(sprHandle_t *owner);
void effInsertSlotRecord(void *owner, effOwnerRecord_t *list, effSlotOwner_t *work, s32 slotIndex);
s32 effRemoveSlotRecord(effOwnerRecord_t *list, effSlotOwner_t *work, s32 slotIndex);

/* Release all sixteen buckets; retain the original traversal that reads next after releasing a record. */
void effReleaseRecordBuckets();
u32 effDispatchRecordBuckets(u32 refresh, effOwnerRecord_t *list, s32 drawOption);
u32 effDestroyOwnerRecordList(u32 list);
u32 effConvertParamValue(u32 *kind, void *out, void *in, void *toFloat);
extern u32 effConvertParamValue(u32 *, void *, void *, void *);
u32 effSumRecordStatuses(const effMappedRecord_t *record);

/* Serialized file header; this is distinct from the live batch header below. */
typedef struct effMappedHeader_t {
    u8 pad_00[0x14];
    u32 count;        // 0x14
    u8 pad_18[8];
} effMappedHeader_t;    // 0x20

struct sdfMemHandle_t *effLoadMappedStatusRecords(const u8 *source, effMappedHeader_t *headerOut);

/* Build the live batch header from the serialized count and owned record array. */
void *effGetSlotWorkOrOverride(sprHandle_t *work, s32 slotIndex);
void effInitializeSlotPhase(effTimedState_t *effect);
extern sprHandle_t *effUpdateTimedStates(sprHandle_t *effect, u32 slot, void *entryData);

/* Rebuild a slot's live work from its source description, then prime the timed states. */
u8 *effResolveResourceSlots(sprHandle_t *set, u8 *resourceBytes, s32 clearAllSlots, s32 selectedSlot);
effPayload_t *effCreatePayload(u32 recordCount);
u32 effDestroyPayload(effPayload_t *payload);

#define EFF_SLOT_TEXTURE_REFERENCES_OWNED 0

#define EFF_SLOT_TEXTURE_REFERENCES_BORROWED 1

u32 effSlotTransitionClearTarget(effTimedState_t *record, u32 unused);
u32 effClampSlotPhaseAtEnd(sprHandle_t *owner, s32 index, effTimedState_t *effect);
u32 effClampSlotPhaseAtStart(sprHandle_t *owner, s32 index, effTimedState_t *effect);
extern u32 effResetRecordRun(u8 *work, u32 first, u32 unused);
sprHandle_t *effUpdateTimedStates(sprHandle_t *effect, u32 slot, void *entryData);
u32 effConfigureSlotResource(s32 work, s32 index, u32 value, u32 flags);

u32 effResetRecordRun(u8 *work, u32 first, u32 unused);
void effSelectPresetByKind(u32 mode, u32 value);

typedef struct SdfDrawPacket SdfDrawPacket;


extern void effSelectPresetByKind(u32, u32);
void itfRotateScreenOffset(s32 *outX, s32 *outY, s32 x, s32 y, s32 centerX, s32 centerY, f32 angle);
extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);


/* Load and instantiate a resource; only a zero keepAllocation releases the source allocation. */
sprHandle_t *effLoadIndexedResource(const char *base, const char *name, u32 keepAllocation) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    struct sdfMemHandle_t *allocation;
    sprHandle_t *instance;

    sprintf(path, D_003BD198, base, name);
    allocation = sdfDevLoadFileEx(path, &sourceAddress, 0);
    instance = effCreateResourceSlotSetFromAllocation(allocation, keepAllocation);
    if (keepAllocation == EFF_RESOURCE_TRANSIENT) {
        sdfMemFreeDirect(allocation);
    }
    return instance;
}

/* Publish the instance, release its source allocation, then clean up the completed file job. */
void effCompleteTransientResourceJob(fileRequest_t *job, u32 *outInstance) {
    struct sdfMemHandle_t *allocation;
    sprHandle_t *instance;

    allocation = fileGetMemHandle(job);
    instance = effCreateResourceSlotSetFromAllocation(allocation, EFF_RESOURCE_TRANSIENT);
    *outInstance = (u32)instance;
    sdfMemFreeDirect(allocation);
    fileHandleFree(job);
}

/* Publish the instance without releasing its source allocation, then clean up the file job. */
void effCompleteRetainedResourceJob(fileRequest_t *job, u32 *outInstance) {
    struct sdfMemHandle_t *allocation;
    sprHandle_t *instance;

    allocation = fileGetMemHandle(job);
    instance = effCreateResourceSlotSetFromAllocation(allocation, EFF_RESOURCE_KEEP_ALLOCATION);
    *outInstance = (u32)instance;
    fileHandleFree(job);
}

/* Clear the output first; only mode one selects the retained-allocation completion path. */
fileRequest_t *effRequestResourceByMode(const char *base, const char *name, u32 mode, void **outInstance) {
    char path[EFF_RESOURCE_PATH_BYTES];

    sprintf(path, D_003BD198, base, name);
    *outInstance = NULL;
    if (mode == EFF_RESOURCE_KEEP_ALLOCATION) {
        return fileLoadEx(path, 0, (u32)effCompleteRetainedResourceJob, (u32)outInstance);
    } else {
        return fileLoadEx(path, 0, (u32)effCompleteTransientResourceJob, (u32)outInstance);
    }
}

/* Build mapped records from the retained source address, then release the original file allocation. */
effMappedResource_t *effLoadMappedResource(const char *base, const char *name) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    effMappedResource_t *mappedResource;
    struct sdfMemHandle_t *allocation;

    sprintf(path, D_003BD198, base, name);
    allocation = sdfDevLoadFileEx(path, &sourceAddress, 0);
    mappedResource = effCreateMappedResource((const u8 *)sourceAddress);
    sdfMemFreeDirect(allocation);
    return mappedResource;
}

/* Publish mapped records before releasing their source allocation and completing the file job. */
void effCompleteMappedResourceJob(fileRequest_t *job,
                                  effMappedResource_t **outMappedResource) {
    struct sdfMemHandle_t *allocation;
    u32 sourceAddress;
    effMappedResource_t *mappedResource;

    allocation = fileGetMemHandle(job);
    sourceAddress = sdfMemLock(allocation);
    mappedResource = effCreateMappedResource((const u8 *)sourceAddress);
    *outMappedResource = mappedResource;
    sdfMemFreeDirect(allocation);
    fileHandleFree(job);
}

/* Initialize the output to zero and schedule mapped-record completion. */
void effRequestMappedResource(const char *base, const char *name,
                              effMappedResource_t **outMappedResource) {
    char path[EFF_RESOURCE_PATH_BYTES];

    sprintf(path, D_003BD198, base, name);
    *outMappedResource = 0;
    fileLoadEx(path, 0, (u32)effCompleteMappedResourceJob, (u32)outMappedResource);
}

/* Create an owner list with sixteen initially empty record buckets. */
effOwnerRecord_t *effCreateOwnerRecordList(sprHandle_t *owner) {
    effOwnerRecord_t *list = sdfChipAllocDirect(sizeof(effOwnerRecord_t));
    memset(list, 0, EFF_OWNER_LIST_BYTES);
    list->owner = owner;
    return list;
}

/* Clamp only the upper bucket bound and prepend; the former head's prev link is not rewritten. */
void effInsertSlotRecord(void *owner, effOwnerRecord_t *list, effSlotOwner_t *work, s32 slotIndex) {
    effSlot_t *slotData = &work->slots[slotIndex];
    s32 bucketIndex = slotData->bucketIndex;
    effRecord_t *record;

    if (bucketIndex >= EFF_RECORD_BUCKETS) {
        bucketIndex = EFF_RECORD_LAST_BUCKET;
    }
    record = sdfChipAllocDirect(sizeof(*record));
    record->prev = 0;
    record->owner = owner;
    record->slot = slotIndex;
    record->next = list->entries[bucketIndex];
    list->entries[bucketIndex] = record;
}

/* Remove the first matching slot from the raw bucket index; unlike insertion, this does not clamp it. */
s32 effRemoveSlotRecord(effOwnerRecord_t *list, effSlotOwner_t *work, s32 slotIndex) {
    effSlot_t *slotData = &work->slots[slotIndex];
    s32 bucketIndex = slotData->bucketIndex;
    effRecord_t *record = list->entries[bucketIndex];

    while (record != 0) {
        if (record->slot == slotIndex) {
            if (record->prev != 0) {
                record->prev->next = record->next;
            }
            if (record->next != 0) {
                record->next->prev = record->prev;
            }
            if (record == list->entries[bucketIndex]) {
                list->entries[bucketIndex] = record->next;
            }
            sdfChipFreeDirect(record);
            return 1;
        }
        record = record->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_r1_002C8188", effReleaseRecordBuckets);

/* Dispatch every bucket record and optionally refresh its owner/slot lookup. */
u32 effDispatchRecordBuckets(u32 refresh, effOwnerRecord_t *list, s32 drawOption) {
    effRecord_t **bucketHead = list->entries;
    s32 bucketCountdown = EFF_RECORD_LAST_BUCKET;

    do {
        effRecord_t *record = *bucketHead;
        while (record != 0) {
            itfDrawGridWithResolvedSlot(0, 0, 0, 0, list->owner, record->slot, drawOption);
            if (refresh != 0) {
                itfGridLookupValueOrDefault(list->owner, record->slot);
            }
            record = record->next;
        }
        ++bucketHead;
    } while (--bucketCountdown >= 0);
    return 1;
}

/* Release bucket records before the owner list. */
u32 effDestroyOwnerRecordList(u32 list) {
    effReleaseRecordBuckets((effOwnerRecord_t *)list);
    sdfChipFreeDirect((void *)list);
    return 1;
}

/* Convert a 4-byte effect parameter between its int and float kinds. Kind 0
 * converts (float to int, or int to float when toFloat is set); kind 2 copies
 * a float through. Returns the converted size, or 0 for other kinds. */
u32 effConvertParamValue(u32 *kind, void *out, void *in, void *toFloat) {
    s32 intValue;
    f32 floatValue;

    if (toFloat == NULL) {
        switch (*kind) {
        case 0:
            if (in != NULL) {
                intValue = *(f32 *)in;
            }
            if (out != NULL) {
                memcpy(out, &intValue, 4);
            }
            return 4;
        case 2:
            if (in != NULL) {
                floatValue = *(f32 *)in;
            }
            if (out != NULL) {
                memcpy(out, &floatValue, 4);
            }
            return 4;
        }
    } else {
        switch (*kind) {
        case 0:
            if (in != NULL) {
                floatValue = *(s32 *)in;
            }
            if (out != NULL) {
                memcpy(out, &floatValue, 4);
            }
            return 4;
        case 2:
            if (in != NULL) {
                floatValue = *(f32 *)in;
            }
            if (out != NULL) {
                memcpy(out, &floatValue, 4);
            }
            return 4;
        }
    }
    return 0;
}

/* Sum the status-storage byte requirements for the selected record category. */
u32 effSumRecordStatuses(const effMappedRecord_t *record) {
    effRecordBucket_t *group = &effData21[record->category];
    u32 statusBytes = 0;
    u32 recordIndex;

    for (recordIndex = 0; recordIndex < group->count; recordIndex++) {
        statusBytes += effConvertParamValue((u32 *)(group->records + recordIndex * EFF_STATUS_PARAM_STRIDE + EFF_STATUS_PARAM_HEADER_BYTES), 0, 0, 0);
    }
    return statusBytes;
}

/* Copy packed headers and status data into live records, zero-filling extra status capacity.
 * The required-size calculation uses the first record, not the current row.
 * Returns the record-allocation handle, not its retained address.
 */
struct sdfMemHandle_t *effLoadMappedStatusRecords(const u8 *source, effMappedHeader_t *headerOut) {
    effMappedHeader_t header;
    struct sdfMemHandle_t *allocation;
    effMappedRecord_t *records;
    u32 recordIndex = 0;
    u32 statusBytes;

    memcpy(&header, source, sizeof(header));
    source += sizeof(header);
    allocation = sdfMemAllocDirect(header.count * EFF_STATUS_RECORD_BYTES);
    records = (effMappedRecord_t *)sdfMemLock(allocation);
    for (; recordIndex < header.count; recordIndex++) {
        effMappedRecord_t *record = &records[recordIndex];

        memcpy(record, source, EFF_PACKED_STATUS_HEADER_BYTES);
        source += EFF_PACKED_STATUS_HEADER_BYTES;
        statusBytes = effSumRecordStatuses(records);
        if (statusBytes < record->statusBytes) {
            statusBytes = record->statusBytes;
        }
        record->status = sdfChipAllocDirect(statusBytes);
        memset(record->status, 0, statusBytes);
        memcpy(record->status, source, record->statusBytes);
        source += record->statusBytes;
        if (record->statusBytes < statusBytes) {
            record->statusBytes = statusBytes;
        }
    }
    if (headerOut != 0) {
        memcpy(headerOut, &header, sizeof(header));
    }
    return allocation;
}

INCLUDE_ASM(const s32, "code_r1_002C8188", effCreateMappedResource);

/* Build one zeroed status record and allocate the category's required status storage. */
effMappedResource_t *effCreateStatusBatch(u32 category) {
    effMappedResource_t *batch = sdfChipAllocDirect(sizeof(effMappedResource_t));
    struct sdfMemHandle_t *allocation;
    u32 recordAddress;
    u32 statusBytes;
    void *statuses;

    batch->count = 1;
    allocation = sdfMemAllocDirect(EFF_STATUS_RECORD_BYTES);
    batch->allocation = allocation;
    recordAddress = sdfMemLock(allocation);
    batch->records = (effMappedRecord_t *)recordAddress;
    memset((void *)recordAddress, 0, EFF_STATUS_RECORD_BYTES);
    {
        effMappedRecord_t *record = batch->records;
        record->category = category;
        statusBytes = effSumRecordStatuses(record);
    }
    statuses = sdfChipAllocDirect(statusBytes);
    batch->records->status = statuses;
    memset(statuses, 0, statusBytes);
    batch->records->statusBytes = statusBytes;
    return batch;
}

/* Release each record's status storage, then the record allocation and batch header. */
EffMappedResourceDestroyResult effDestroyPackedBatch(effMappedResource_t *batch) {
    s32 recordIndex;
    for (recordIndex = 0; recordIndex < batch->count; recordIndex++) {
        sdfChipFreeDirect(batch->records[recordIndex].status);
    }
    sdfMemFreeDirect(batch->allocation);
    sdfChipFreeDirect(batch);
    return EFF_MAPPED_RESOURCE_DESTROYED;
}

u32 effReleaseSlotWorkAllocation(sprHandle_t *owner) {
    sdfMemFreeDirect(owner->workAllocation);
    return 1;
}

/* Return the normal slot work unless the slot carries an alternate payload. */
void *effGetSlotWorkOrOverride(sprHandle_t *work, s32 slotIndex) {
    void *alternatePayload;
    s32 entryAddress;

    entryAddress = slotIndex * EFF_SLOT_WORK_BYTES + (s32)work->workEntries;
    alternatePayload = ((BdWork *)entryAddress)->alternate.payload;
    if (alternatePayload != NULL) {
        entryAddress = (s32)alternatePayload;
    }
    return (void *)entryAddress;
}

/* Start at zero when moving forward, otherwise at the full 16.16 endpoint. */
void effInitializeSlotPhase(effTimedState_t *effect) {
    if (effect->flags & EFF_TIMED_STATE_DIRECTION_FORWARD) {
        effect->value = EFF_SLOT_PHASE_ZERO;
    } else {
        effect->value = EFF_SLOT_PHASE_ONE;
    }
}

INCLUDE_ASM(const s32, "code_r1_002C8188", effInitializeSlotWorkFromDescription);

/* Initialize the normal slot entry at the unchanged 0xA0-byte stride. */
void effInitializeSlotWork(sprHandle_t *owner, s32 slotIndex) {
    effInitializeSlotWorkFromDescription(owner, slotIndex, &owner->workEntries[slotIndex]);
}

/* Reset every normal work slot in source order. */
void effInitializeAllSlotWork(sprHandle_t *work) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < work->SpriteNums; slotIndex++) {
        effResetSlotWork(work, slotIndex);
    }
}

/* Store the owner and slot index before invoking the work initializer. */
void effAttachSlotWorkOwner(sprHandle_t *owner, s32 slotIndex, void *payload) {
    /* Alternate records share these owner/index fields without a full work allocation. */
    BdWork *entry = (BdWork *)payload;
    entry->owner = owner;
    entry->slotIndex = slotIndex;
    effInitializeSlotWorkFromDescription(owner, slotIndex, payload);
}

/* Clear the complete normal slot, then restore owner/index and initialize it. */
void effResetSlotWork(sprHandle_t *work, u32 slotIndex) {
    BdWork *entry;

    entry = &work->workEntries[slotIndex];
    memset(entry, 0, EFF_SLOT_WORK_BYTES);
    effAttachSlotWorkOwner(work, slotIndex, entry);
}

/* Acquire missing selected handles or clear slots; clearAllSlots does not release textures.
 * Return the address following the processed resource-table entries.
 */
#ifdef VERSION_RAIDOU1
u8 *effResolveResourceSlots(sprHandle_t *set, u8 *resourceBytes, s32 clearAllSlots, s32 selectedSlot) {
    u32 slotIndex = 0;
    u8 *entryBytes = resourceBytes;

    entryBytes += ((effResourceTable_t *)entryBytes)->entryOffset;

    if (set->TexNums != 0) {
        do {
            TMX_HEADER_t *resourceData = (TMX_HEADER_t *)(resourceBytes + ((effResourceEntry_t *)entryBytes)->resourceOffset);

            entryBytes += EFF_RESOURCE_TABLE_ENTRY_BYTES;
            if (clearAllSlots == 0) {
                if (selectedSlot == -1 || selectedSlot == (s32)slotIndex) {
                    if (set->textureReferences[slotIndex] == 0) {
                        set->textureReferences[slotIndex] = sdfTexSet(resourceData);
                    }
                } else {
                    set->textureReferences[slotIndex] = 0;
                }
            } else {
                set->textureReferences[slotIndex] = 0;
            }
            slotIndex++;
        } while (slotIndex < set->TexNums);
    }
    return entryBytes;
}
#else
INCLUDE_ASM(const s32, "code_r1_002C8188", effResolveResourceSlots);
#endif

void effResolveAndReleaseResource(sprHandle_t *owner) {
    if (owner->sourceAllocation != 0) {
        u8 *data = (u8 *)sdfMemLock(owner->sourceAllocation);
        effResolveResourceSlots(owner, data, 0, -1);
        sdfMemUnlock(owner->sourceAllocation);
    }
}

void effResolveAndReleaseSelectedResource(sprHandle_t *owner, s32 slot) {
    if (owner->sourceAllocation != 0) {
        u8 *data = (u8 *)sdfMemLock(owner->sourceAllocation);
        effResolveResourceSlots(owner, data, 0, slot);
        sdfMemUnlock(owner->sourceAllocation);
    }
}

void effReleaseTextureHandlesAndResetSlots(sprHandle_t *set) {
    u32 i;

    for (i = 0; i < set->TexNums; i++) {
        if (set->textureReferences[i] != 0) {
            sdfTexReleaseReference(set->textureReferences[i]);
            set->textureReferences[i] = 0;
        }
    }
    effInitializeAllSlotWork(set);
}

u8 effHasFirstTextureHandle(const sprHandle_t *set) {
    return set->textureReferences[0] != NULL;
}

/* Allocate and clear count 0x6C-byte records; retain the existing allocation/count/address header order. */
effPayload_t *effCreatePayload(u32 recordCount) {
    u32 recordBytes = recordCount * EFF_PAYLOAD_RECORD_BYTES;
    effPayload_t *header = sdfChipAllocDirect(sizeof(effPayload_t));
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(recordBytes);
    u8 *records;

    header->count = recordCount;
    header->allocation = allocation;
    records = (u8 *)sdfMemLock(allocation);
    header->records = records;
    memset(records, 0, recordBytes);
    return header;
}

/* Release the record allocation before freeing its small header. */
u32 effDestroyPayload(effPayload_t *payload) {
    sdfMemFreeDirect(payload->allocation);
    sdfChipFreeDirect(payload);
    return 1;
}

sprHandle_t *effCreateResourceSlotSetFromAllocation(struct sdfMemHandle_t *resourceAllocation, u32 keepAllocation) {
    sprHandle_t *set;
    u8 *resource;
    u32 *entries;
    u32 index;
    u32 sourceOffset;

    set = sdfChipAllocDirect(sizeof(sprHandle_t));
    memset(set, 0, 0x30);
    set->sharesTextureReferences = EFF_SLOT_TEXTURE_REFERENCES_OWNED;
    set->sourceAllocation = keepAllocation != 0 ? resourceAllocation : 0;
    resource = (u8 *)sdfMemLock(resourceAllocation);
    set->TexNums = ((effResourceTable_t *)resource)->textureCount;
    set->textureAllocation =
        sdfMemAllocDirect(set->TexNums * 4);
    set->textureReferences = (sdfTexHandle_t **)sdfMemLock(set->textureAllocation);
    memset(set->textureReferences, 0, set->TexNums * 4);
    entries = (u32 *)effResolveResourceSlots(set, resource,
        keepAllocation, -1);

    set->SpriteNums = ((effResourceTable_t *)resource)->slotCount;
    set->descriptionAllocation =
        sdfMemAllocDirect(set->SpriteNums * 0x80);
    set->descriptions = (effSlotDescription_t *)sdfMemLock(set->descriptionAllocation);
    set->workAllocation = sdfMemAllocDirect(set->SpriteNums * 0xA0);
    set->workEntries = (BdWork *)sdfMemLock(set->workAllocation);
    for (index = 0; index < set->SpriteNums; index++) {
        sourceOffset = entries[1];
        memcpy(&set->descriptions[index],
            resource + sourceOffset, 0x80);
        effResetSlotWork(set, index);
        entries += 2;
    }
    return set;
}

sprHandle_t *effCreateResourceSlotSet(sprHandle_t *borrowedSource, u32 slot, u32 count) {
    sprHandle_t *effect = (sprHandle_t *)sdfChipAllocDirect(sizeof(sprHandle_t));
    u32 index = 0;
    effect->sharesTextureReferences = EFF_SLOT_TEXTURE_REFERENCES_BORROWED;
    {
        u32 mode = borrowedSource->TexNums;
        sdfTexHandle_t **textureReferences = borrowedSource->textureReferences;
        effect->TexNums = mode;
        effect->textureReferences = textureReferences;
    }
    effect->sourceAllocation = 0;
    effect->textureAllocation = 0;
    effect->SpriteNums = count;
    effect->descriptionAllocation = sdfMemAllocDirect(count * 0x80);
    effect->descriptions = (effSlotDescription_t *)sdfMemLock(effect->descriptionAllocation);
    effect->workAllocation = sdfMemAllocDirect(effect->SpriteNums * 0xA0);
    effect->workEntries = (BdWork *)sdfMemLock(effect->workAllocation);
    if (effect->SpriteNums != 0) {
        do {
            memcpy(&effect->descriptions[index], &borrowedSource->descriptions[slot], 0x80);
            effResetSlotWork(effect, index);
            index++;
        } while (index < effect->SpriteNums);
    }
    return effect;
}

u32 effDestroyResourceSlotSet(sprHandle_t *set) {
    if (set->sourceAllocation != 0) {
        sdfMemFreeDirect(set->sourceAllocation);
    }
    if (set->sharesTextureReferences == EFF_SLOT_TEXTURE_REFERENCES_OWNED) {
        effReleaseTextureHandlesAndResetSlots(set);
        sdfMemFreeDirect(set->textureAllocation);
    }
    sdfMemFreeDirect(set->descriptionAllocation);
    effReleaseSlotWorkAllocation(set);
    sdfChipFreeDirect(set);
    return 1;
}

u32 effSetSlotResourceAndFlags(effTimedState_t *record, effMappedRecord_t *entry, u32 flags) {
    record->flags = flags;
    record->source = entry;
    if ((flags & EFF_TIMED_STATE_INITIALIZE_PHASE_ON_BIND) != 0) {
        effInitializeSlotPhase(record);
    }
    record->delay = record->delay + 1;
    return 1;
}

u32 effSetSlotIndexedResource(effTimedState_t *target, effMappedResource_t *resources, s32 item,
                              u32 flags) {
    effSetSlotResourceAndFlags(target, &resources->records[item], flags);
    return 1;
}

u32 effSlotTransitionClearTarget(effTimedState_t *record, u32 unused) {
    record->source = 0;
    return 1;
}

u32 effClampSlotPhaseAtEnd(sprHandle_t *owner, s32 index, effTimedState_t *effect) {
    if (effect->value > EFF_SLOT_PHASE_ONE) {
        s32 flags = effect->flags;
        effect->value = EFF_SLOT_PHASE_ONE;
        if (flags & EFF_TIMED_STATE_RESTART_AT_ENDPOINT) {
            if (flags & EFF_TIMED_STATE_PING_PONG) {
                effect->flags = flags & ~EFF_TIMED_STATE_DIRECTION_FORWARD;
            } else {
                effInitializeSlotWork(owner, index);
            }
            return 0;
        }
    }
    return 1;
}

u32 effClampSlotPhaseAtStart(sprHandle_t *owner, s32 index, effTimedState_t *effect) {
    if (effect->value < EFF_SLOT_PHASE_ZERO) {
        s32 flags = effect->flags;
        effect->value = EFF_SLOT_PHASE_ZERO;
        if (flags & EFF_TIMED_STATE_RESTART_AT_ENDPOINT) {
            if (flags & EFF_TIMED_STATE_PING_PONG) {
                effect->flags = flags | EFF_TIMED_STATE_DIRECTION_FORWARD;
            } else {
                effInitializeSlotWork(owner, index);
            }
            return 0;
        }
    }
    return 1;
}

sprHandle_t *effUpdateTimedStates(sprHandle_t *effect, u32 slot, void *entryData) {
    /* Alternate payloads may be only 0x6C bytes; direct access here is limited to timed states.
     * Bucket callbacks retain their existing kind-specific pointer contract.
     */
    BdWork *entry = (BdWork *)entryData;
    effTimedState_t *states = entry->states;
    BdWork *record = &effect->workEntries[slot];
    s32 idle = 1;
    u32 i;

    for (i = 0; i < 2; i++) {
        effTimedState_t *state = &states[i];
        effMappedRecord_t *source = state->source;

        if (source != 0 && source->category != 0) {
            effRecordBucket_t *group = &effData21[source->category];
            s32 phaseStep = group->step(record, entry, state);

            if (state->delay > 0) {
                phaseStep = 0;
                state->delay -= 1;
            }
            if (phaseStep >= 0) {
                if (state->flags & EFF_TIMED_STATE_DIRECTION_FORWARD) {
                    if (state->value != EFF_SLOT_PHASE_ONE) {
                        state->value += phaseStep;
                        idle = 0;
                        if (effClampSlotPhaseAtEnd(effect, slot, state) == 0) {
                            state->delay = state->delayMax;
                            return 0;
                        }
                    }
                } else if (state->value != EFF_SLOT_PHASE_ZERO) {
                    state->value -= phaseStep;
                    idle = 0;
                    if (effClampSlotPhaseAtStart(effect, slot, state) == 0) {
                        state->delay = state->delayMax;
                        return 0;
                    }
                }
            }
        }
    }
    if (idle != 0) {
        effResetRecordRun((u8 *)effect, slot, -1);
        return 0;
    }
    return effect;
}

u32 effSetSlotOverrideWork(sprHandle_t *owner, s32 index, void *payload) {
    if (owner->workEntries[index].alternate.payload == NULL) {
        effAttachSlotWorkOwner(owner, index, payload);
    }
    owner->workEntries[index].alternate.payload = payload;
    return 1;
}

u32 effSetMaterialSlots(sprHandle_t *owner, s32 index, u32 materialFlags, void *payload) {
    u32 i;
    effTimedState_t *states;
    if (owner->workEntries[index].alternate.payload == NULL) {
        effAttachSlotWorkOwner(owner, index, payload);
    }
    owner->workEntries[index].alternate.payload = payload;
    /* Both timed states lie within the shared 0x6C-byte payload region. */
    states = ((BdWork *)payload)->states;
    for (i = 0; i < 2; i++) {
        states[i].materialFlags = materialFlags;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_002C8188", sysLeaf46);

u32 effConfigureSlotResource(s32 work, s32 index, u32 value, u32 flags) {
    BdWork *effect = &((sprHandle_t *)work)->workEntries[index];
    effSetSlotResourceAndFlags(&effect->states[0], (effMappedRecord_t *)(u32)value, flags);
    effUpdateTimedStates((sprHandle_t *)work, index, effect);
    return 1;
}

u32 effConfigureIndexedSlotResource(sprHandle_t *work, s32 index,
                                    effMappedResource_t *data, s32 item, u32 flags) {
    BdWork *effect = &work->workEntries[index];
    effSetSlotResourceAndFlags(&effect->states[0], &data->records[item], flags);
    effUpdateTimedStates(work, index, effect);
    return 1;
}

u32 effConfigureIndexedSlotMaterial(sprHandle_t *work, s32 index,
                  effMappedResource_t *data, s32 item,
                  u32 materialFlags, u32 materialValue, u32 stateFlags) {
    BdWork *effect = &work->workEntries[index];
    effSetSlotResourceAndFlags(&effect->states[0], &data->records[item], stateFlags);
    effUpdateTimedStates(work, index, effect);
    effect->states[0].materialFlags = materialFlags;
    effect->states[0].materialValue = materialValue;
    return 1;
}

u32 effConfigureWithDefaultSetting(sprHandle_t *effect, u32 slot,
                                   effMappedResource_t *resources, u32 item,
                                   u32 materialFlags, u32 stateFlags) {
    effConfigureIndexedSlotMaterial(effect, slot, resources, item,
                                    materialFlags, 0, stateFlags);
    return 1;
}

u32 effResetRecordRun(u8 *work, u32 first, u32 unused) {
    u32 i = 0;
    u32 index;

    do {
        effSlotTransitionClearTarget((effTimedState_t *)((first + i) * EFF_SLOT_WORK_BYTES +
                                    (s32)((sprHandle_t *)work)->workEntries + 0x28), unused);
        i++;
        index = first + i;
    } while (index < ((sprHandle_t *)work)->SpriteNums && (((sprHandle_t *)work)->descriptions[index].flags & EFF_SLOT_DESCRIPTION_SEQUENCE_CONTINUATION));
    return 1;
}

void effSelectPresetByKind(u32 mode, u32 value) {
    switch (mode) {
    case 0:
        sdfSubmitGsAlphaOneRegisterPacket(0x44, value);
        return;
    case 1:
        sdfSubmitGsAlphaOneRegisterPacket(0x48, value);
        return;
    case 2:
        sdfSubmitGsAlphaOneRegisterPacket(0x42, value);
        break;
    }
}

/* Rotate an offset in degrees, apply screen aspect correction, and add the supplied center. */
void itfRotateScreenOffset(s32 *outX, s32 *outY, s32 x, s32 y, s32 centerX, s32 centerY, f32 angle) {
    f32 aspect = 1.75f;
    f32 radians = angle * 0.017453293f;
    f32 rotatedX;
    f32 rotatedY;

    y = (s32)(y * aspect);
    rotatedX = sdfCos(radians) * x - sdfSin(radians) * y;
    rotatedY = sdfSin(radians) * x + sdfCos(radians) * y;
    if (rotatedX > 0.0f) {
        rotatedX += 0.1f;
    } else {
        rotatedX -= 0.1f;
    }
    if (rotatedY > 0.0f) {
        rotatedY += 0.1f;
    } else {
        rotatedY -= 0.1f;
    }
    *outX = (s32)rotatedX;
    *outY = (s32)rotatedY;
    *outX += centerX;
    *outY = (s32)(*outY / aspect + centerY);
}


/* Sensitive to this unit's declaration set: including newdata.h moves the alias class of the
 * address-taken corners, and the packet stores then schedule differently. */
void sdfDrawTexturedQuad(s32 x, s32 y, u32 z, s32 width, s32 height, const effSpriteUV_t *uv,
                         const u32 *colors, u32 flip, f32 angle, u32 alphaMode,
                         s32 filter, sdfTexHandle_t *tex, s32 ot)
{
    u32 uvs[4];
    /* Separate corner locals: itfRotateScreenOffset writes each through its own pointer. */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;
    s32 x3;
    s32 y3;
    void *packet;
    void *tag;
    u64 *prim;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 hw;
    s32 hh;
    s32 cx;
    s32 cy;
    s32 t;
    sdfGrOt_t *otp;

    if (filter == 0) {
        sdfTexSetFilter(tex, 0, 1);
    } else {
        sdfTexSetFilter(tex, 1, 1);
    }
    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 4)));
    sdf2DMakePrimPacketHeader(packet, 0x15C, 3, 0x531, 4);
    prim = (u64 *)sdf2DGetPrimPtr(packet);
    left = x + 0x7000;
    top = y + 0x7900;
    right = left + width - 8;
    bottom = top + height - 4;
    hw = (right - left) / 2;
    hh = (bottom - top) / 2;
    cx = left + hw;
    cy = top + hh;
    x0 = left;
    y0 = top;
    x3 = right;
    y3 = bottom;
    if (angle == 0.0f) {
        x1 = right;
        y1 = top;
        x2 = left;
        y2 = bottom;
    } else {
        itfRotateScreenOffset(&x0, &y0, -hw, -hh, cx, cy, angle);
        itfRotateScreenOffset(&x1, &y1, hw, -hh, cx, cy, angle);
        itfRotateScreenOffset(&x2, &y2, -hw, hh, cx, cy, angle);
        itfRotateScreenOffset(&x3, &y3, hw, hh, cx, cy, angle);
    }
    uvs[0] = uv->rect.u0 << 4;
    uvs[1] = uv->rect.v0 << 4;
    uvs[2] = uv->rect.u1 << 4;
    uvs[3] = uv->rect.v1 << 4;
    if (flip & 1) {
        t = x0;
        x0 = x1;
        x1 = t;
        t = x2;
        x2 = x3;
        x3 = t;
    }
    if (flip & 2) {
        t = y0;
        y0 = y2;
        y2 = t;
        t = y1;
        y1 = y3;
        y3 = t;
    }
    if (colors == NULL) {
        prim[0] = 0x8000000080LL;
        prim[1] = 0x8000000080LL;
        prim[6] = 0x8000000080LL;
        prim[7] = 0x8000000080LL;
        prim[12] = 0x8000000080LL;
        prim[13] = 0x8000000080LL;
        prim[18] = 0x8000000080LL;
        prim[19] = 0x8000000080LL;
    } else {
        prim[0] = (colors[0] >> 24) | ((u64)((colors[0] >> 16) & 0xFF) << 32);
        prim[1] = ((colors[0] >> 8) & 0xFF) | ((u64)(colors[0] & 0xFF) << 32);
        prim[6] = (colors[1] >> 24) | ((u64)((colors[1] >> 16) & 0xFF) << 32);
        prim[7] = ((colors[1] >> 8) & 0xFF) | ((u64)(colors[1] & 0xFF) << 32);
        prim[12] = (colors[2] >> 24) | ((u64)((colors[2] >> 16) & 0xFF) << 32);
        prim[13] = ((colors[2] >> 8) & 0xFF) | ((u64)(colors[2] & 0xFF) << 32);
        prim[18] = (colors[3] >> 24) | ((u64)((colors[3] >> 16) & 0xFF) << 32);
        prim[19] = ((colors[3] >> 8) & 0xFF) | ((u64)(colors[3] & 0xFF) << 32);
    }
    prim[2] = uvs[0] | ((u64)uvs[1] << 32);
    prim[4] = (u64)(u32)x0 | ((u64)(u32)y0 << 32);
    prim[5] = z >> 4;
    prim[8] = uvs[2] | ((u64)uvs[1] << 32);
    prim[10] = (u64)(u32)x1 | ((u64)(u32)y1 << 32);
    prim[11] = z >> 4;
    prim[14] = uvs[0] | ((u64)uvs[3] << 32);
    prim[16] = (u64)(u32)x2 | ((u64)(u32)y2 << 32);
    prim[17] = z >> 4;
    prim[20] = uvs[2] | ((u64)uvs[3] << 32);
    prim[22] = (u64)(u32)x3 | ((u64)(u32)y3 << 32);
    prim[23] = z >> 4;
    effSelectPresetByKind(alphaMode, ot);
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, tex, 0);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, ot);
}

void effSelectPresetAndDispatch(u32 x, u32 y, u32 z, u32 width,
                                    u32 height, const u32 *vertexColors, u32 presetMode, u32 presetValue) {
    effSelectPresetByKind(presetMode, presetValue);
    uiDrawGradientColorRect(x, y, z, width, height, vertexColors, presetValue);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, presetValue);
}
INCLUDE_SDATA(const s32, "code_r1_002C8188", effData43);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry15);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry16);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry17);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry18);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry19);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry20);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry21);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry22);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry23);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry24);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry25);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry26);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry27);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry28);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry29);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry30);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry31);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry32);

INCLUDE_SDATA(const s32, "code_r1_002C8188", effData21Entry33);

