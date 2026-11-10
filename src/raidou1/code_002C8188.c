#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_002BC8F0.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "kernel_draw.h"

#include "sdf_resource.h"

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/file_request_api.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_header.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_trig.h"

/* Return the payload allocation descriptor. For normal file loads, freeing the
 * request wrapper does not free this allocation; its resource owner must keep
 * or release it separately. */
struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *request);

/* Free a completed request wrapper; return one while it remains pending. */
FileRequestCleanupResult fileHandleFree(struct FileRequest *request);

/* fileLoadEx stores userData without copying it. When callbackAddress is
 * nonzero, the manager calls it once after the device request closes, passing
 * the completed request and userData. The context must remain valid until then;
 * callback delivery does not free the request wrapper, which callers release
 * with fileHandleFree. */
struct FileRequest *fileLoadEx(const char *requestName, u32 allocationMode,
                                              u32 callbackAddress, u32 userData);

extern void *memcpy(void *, const void *, unsigned int);

SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context);

SdfDrawPacket *sdf2DMakePrimPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags,
                                       s32 registerCount, s64 registerList,
                                       s32 loopCount);

void *sdfChipAllocDirect(s32 size);

void sdfChipFreeDirect(void *memory);

f32 sdfCos(f32 angle);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

void sdfMemUnlock(struct sdfMemHandle_t *allocation);

f32 sdfSin(f32 angle);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */
struct sdfTexHandle_t *sdfTexSet(const SdfTextureFileHeader *header);

#include "dds/sdf_gs_alpha_register_api.h"

#include "dds/common.h"

#include "dds/sdf_packet_header.h"

#include "dds/eff_object_node.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/itf_draw_grid.h"

#include "dds/eff_ref_obj.h"

#include "dds/sdf_heap_api.h"

#include "dds/ee_mmi.h"

#include "dds/pcp_vu0.h"

#include "dds/eff.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_records.h"

#include "dds/eff_record_bucket.h"

#include "dds/eff_owner_records.h"

#include "dds/sdf.h"

#include "dds/sdf_texture_file.h"

#include "dds/btl.h"

#include "dds/file_request_api.h"

/* Declared before first use (were implicit int). */
extern void * memset(void *, s32, u32);

extern u32 effColorEditRgbMenuOpen;

extern u32 effColorEditSelectedKey;

extern u32 effColorEditPage;

extern u32 D_003BD158;

extern u32 D_003BD15C;

extern u32 D_003BD160;

extern u32 effColorEditSelectedAlphaKey;

extern s32 effEditorCurrentMenu;

extern u32 effScalyTextureHandle;

extern s32 effSharedStripReferenceCount;

extern u32 effSharedScalyStripResource;

extern u32 effWindTextureHandle;

extern struct sdfMemHandle_t *D_003BC984;

extern struct sdfMemHandle_t *D_003BC994;

extern s32 effSharedRibbonReferenceCount;

extern u32 D_003BC990;

extern struct SdfTextureFileHeader *effFlashTextureHandles;

extern struct SdfTextureFileHeader *D_003BC96C;

extern u32 D_003BC950;

extern u32 D_003BC954;

typedef struct EffectObjectFlag {
    u8 pad00[0xC];
    u32 flags;
} EffectObjectFlag;

extern EffectObjectNode *effFloorModelListHead;

extern u32 effPartsListScroll;

extern s32 D_003BC970[2];

extern RefObj *D_003BC978[2];

extern EffRecordBucket D_0038FD88[];

extern u32 effSharedTextureReferenceCount;

extern u32 D_003BC9B0[2];

extern u32 D_003BC9B8[2];

extern u8 D_003BC9C0[2];

extern u8 D_003BC9C8[2];

extern u8 D_003BD088[];

extern u8 D_003BD090[];

extern u8 *D_003BD074;

extern u8 D_003BCA40[];

extern u8 D_003BD050[];

extern u8 D_003BD000[];

extern u8 D_003BD108;

extern u8 D_003BD109;

extern u8 D_003BD10A;

/* Relative offsets in an effect resource's table and its eight-byte entries. */
typedef struct EffectResourceTable {
    u8 pad00[0xC];
    u32 entryOffset;
    u8 pad10[4];
    u16 textureCount;
    u16 slotCount;
} EffectResourceTable;

typedef struct EffectResourceEntry {
    u32 unknown;
    u32 resourceOffset;
} EffectResourceEntry;

extern u16 D_003BC944;

extern u8 *D_003BC958;

extern struct sdfMemHandle_t *D_003BC960[2];

extern s8 D_003BC9AC;

extern f32 D_003BC9A8;

extern char D_003BD080[];

extern u8 D_003BD078[];

extern u8 D_003BCF30[];

extern u32 D_003BD064;

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

/* Load and instantiate a resource; only a zero keepAllocation releases the source allocation. */
EffectSlotSet *effLoadIndexedResource(const char *base, const char *name, u32 keepAllocation);

/* Publish the instance, release its source allocation, then clean up the completed file job. */
void effCompleteTransientResourceJob(struct FileRequest *job, u32 *outInstance);

/* Publish the instance without releasing its source allocation, then clean up the file job. */
void effCompleteRetainedResourceJob(struct FileRequest *job, u32 *outInstance);

/* Clear the output first; only mode one selects the retained-allocation completion path. */
void effRequestResourceByMode(const char *base, const char *name, u32 mode, u32 *outInstance);

/* Build mapped records from the retained source address, then release the original file allocation. */
EffMappedResource *effLoadMappedResource(const char *base, const char *name);

/* Publish mapped records before releasing their source allocation and completing the file job. */
void effCompleteMappedResourceJob(struct FileRequest *job,
                                  EffMappedResource **outMappedResource);

/* Initialize the output to zero and schedule mapped-record completion. */
void effRequestMappedResource(const char *base, const char *name,
                              EffMappedResource **outMappedResource);

/* Create an owner list with sixteen initially empty record buckets. */
EffectOwnerRecord *effCreateOwnerRecordList(EffectSlotSet *owner);

/* Clamp only the upper bucket bound and prepend; the former head's prev link is not rewritten. */
void effInsertSlotRecord(void *owner, EffectOwnerRecord *list, EffectSlotOwner *work, s32 slotIndex);

/* Remove the first matching slot from the raw bucket index; unlike insertion, this does not clamp it. */
s32 effRemoveSlotRecord(EffectOwnerRecord *list, EffectSlotOwner *work, s32 slotIndex);

/* Release all sixteen buckets; retain the original traversal that reads next after releasing a record. */
void effReleaseRecordBuckets();

extern void itfGridLookupValueOrDefault(void *object, s32 key);

/* Dispatch every bucket record and optionally refresh its owner/slot lookup. */
u32 effDispatchRecordBuckets(u32 refresh, EffectOwnerRecord *list, s32 drawOption);

/* Release bucket records before the owner list. */
u32 effDestroyOwnerRecordList(u32 list);

/* Convert a 4-byte effect parameter between its int and float kinds. Kind 0
 * converts (float to int, or int to float when toFloat is set); kind 2 copies
 * a float through. Returns the converted size, or 0 for other kinds. */
u32 effConvertParamValue(u32 *kind, void *out, void *in, void *toFloat);

extern u32 effConvertParamValue(u32 *, void *, void *, void *);

/* Sum the status-storage byte requirements for the selected record category. */
u32 effSumRecordStatuses(const EffMappedRecord *record);

/* Serialized file header; this is distinct from the live batch header below. */
typedef struct EffMappedHeader {
    u8 pad_00[0x14];
    u32 count;        // 0x14
    u8 pad_18[8];
} EffMappedHeader;    // 0x20

/* Copy packed headers and status data into live records, zero-filling extra status capacity.
 * The required-size calculation uses the first record, not the current row.
 * Returns the record-allocation handle, not its retained address.
 */
struct sdfMemHandle_t *effLoadMappedStatusRecords(const u8 *source, EffMappedHeader *headerOut);

/* Build the live batch header from the serialized count and owned record array. */
EffMappedResource *effCreateMappedResource(const u8 *source);

/* Build one zeroed status record and allocate the category's required status storage. */
EffMappedResource *effCreateStatusBatch(u32 category);

/* Release each record's status storage, then the record allocation and batch header. */
EffMappedResourceDestroyResult effDestroyPackedBatch(EffMappedResource *batch);

u32 effReleaseSlotWorkAllocation(EffectSlotSet *owner);

/* Return the normal slot work unless the slot carries an alternate payload. */
void *effGetSlotWorkOrOverride(EffectSlotSet *work, s32 slotIndex);

/* Start at zero when moving forward, otherwise at the full 16.16 endpoint. */
void effInitializeSlotPhase(EffTimedState *effect);

extern s8 D_0038FB70[];

extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *effect, u32 slot, void *entryData);

/* Rebuild a slot's live work from its source description, then prime the timed states. */
void effInitializeSlotWorkFromDescription(EffectSlotSet *owner, s32 slotIndex, void *payload);

/* Initialize the normal slot entry at the unchanged 0xA0-byte stride. */
void effInitializeSlotWork(EffectSlotSet *owner, s32 slotIndex);

/* Reset every normal work slot in source order. */
void effInitializeAllSlotWork(EffectSlotSet *work);

/* Store the owner and slot index before invoking the work initializer. */
void effAttachSlotWorkOwner(EffectSlotSet *owner, s32 slotIndex, void *payload);

/* Clear the complete normal slot, then restore owner/index and initialize it. */
void effResetSlotWork(EffectSlotSet *work, u32 slotIndex);

/* Acquire missing selected handles or clear slots; clearAllSlots does not release textures.
 * Return the address following the processed resource-table entries.
 */
u8 *effResolveResourceSlots(EffectSlotSet *set, u8 *resourceBytes, s32 clearAllSlots, s32 selectedSlot);

void effResolveAndReleaseResource(EffectSlotSet *owner);

void effResolveAndReleaseSelectedResource(EffectSlotSet *owner, s32 slot);

void effReleaseTextureHandlesAndResetSlots(EffectSlotSet *set);

u8 effHasFirstTextureHandle(const EffectSlotSet *set);

/* Allocate and clear count 0x6C-byte records; retain the existing allocation/count/address header order. */
EffPayload *effCreatePayload(u32 recordCount);

/* Release the record allocation before freeing its small header. */
u32 effDestroyPayload(EffPayload *payload);

#define EFF_SLOT_TEXTURE_REFERENCES_OWNED 0

#define EFF_SLOT_TEXTURE_REFERENCES_BORROWED 1

EffectSlotSet *effCreateResourceSlotSetFromAllocation(struct sdfMemHandle_t *resourceAllocation, u32 keepAllocation);

EffectSlotSet *effCreateResourceSlotSet(EffectSlotSet *borrowedSource, u32 slot, u32 count);

u32 effDestroyResourceSlotSet(EffectSlotSet *set);

u32 effSetSlotResourceAndFlags(EffTimedState *record, EffMappedRecord *entry, u32 flags);

u32 effSetSlotIndexedResource(EffTimedState *target, EffMappedResource *resources, s32 item,
                              u32 flags);

u32 effSlotTransitionClearTarget(EffTimedState *record, u32 unused);

u32 effClampSlotPhaseAtEnd(EffectSlotSet *owner, s32 index, EffTimedState *effect);

u32 effClampSlotPhaseAtStart(EffectSlotSet *owner, s32 index, EffTimedState *effect);

extern u32 effResetRecordRun(u8 *work, u32 first, u32 unused);

EffectSlotSet *effUpdateTimedStates(EffectSlotSet *effect, u32 slot, void *entryData);

u32 effSetSlotOverrideWork(EffectSlotSet *owner, s32 index, void *payload);

u32 effSetMaterialSlots(EffectSlotSet *owner, s32 index, u32 materialFlags, void *payload);

u32 effClearSlotOverrideWork(s32 work, s32 index);

u32 effConfigureSlotResource(s32 work, s32 index, u32 value, u32 flags);

u32 effConfigureIndexedSlotResource(EffectSlotSet *work, s32 index,
                                    EffMappedResource *data, s32 item, u32 flags);

u32 effConfigureIndexedSlotMaterial(EffectSlotSet *work, s32 index,
                  EffMappedResource *data, s32 item,
                  u32 materialFlags, u32 materialValue, u32 stateFlags);

u32 effConfigureWithDefaultSetting(EffectSlotSet *effect, u32 slot,
                                   EffMappedResource *resources, u32 item,
                                   u32 materialFlags, u32 stateFlags);

u32 effResetRecordRun(u8 *work, u32 first, u32 unused);

void effSelectPresetByKind(u32 mode, u32 value);

typedef struct SdfDrawPacket SdfDrawPacket;

typedef struct EffSpriteUV {
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
} EffSpriteUV;

typedef union EffSpriteColor {
    u32 rgba;
    struct {
        u32 alpha : 8;
        u32 blue : 8;
        u32 green : 8;
        u32 red : 8;
    } channels;
} EffSpriteColor;

#include "dds/sdf_packet_alloc.h"

extern void sdfTexSetFilter(sdfTexHandle_t *, s32, s32);

extern s32 sdf2DGetPrimPacketSize(s32, s32);

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

extern void effSelectPresetByKind(u32, u32);

void itfDrawTexturedSpriteRect(s32 x, s32 y, u32 depth, s32 width, s32 height,
                   const EffSpriteUV *uvRect, const EffSpriteColor *color, u32 flipFlags,
                   u32 blendKind, s32 mode, sdfTexHandle_t *texture, s32 surfaceId);

/* Rotate an offset in degrees, apply screen aspect correction, and add the supplied center. */
void itfRotateScreenOffset(s32 *outX, s32 *outY, s32 x, s32 y, s32 centerX, s32 centerY, f32 angle);

void sdfDrawTexturedQuad(s32 x, s32 y, u32 depth, s32 width, s32 height,
                   const EffSpriteUV *uvRect, const EffSpriteColor *cornerColors,
                   u32 flipFlags, f32 angle, u32 blendKind, s32 mode,
                   sdfTexHandle_t *texture, s32 surfaceId);

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

void effSelectPresetAndDispatch(u32 x, u32 y, u32 z, u32 width,
                                    u32 height, u32 vertexColors, u32 presetMode, u32 presetValue);
/* End of declarations from src/dds1/game/code_002BC8F0.c */

/* Load and instantiate a resource; only a zero keepAllocation releases the source allocation. */
EffectSlotSet *effLoadIndexedResource(const char *base, const char *name, u32 keepAllocation) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    struct sdfMemHandle_t *allocation;
    EffectSlotSet *instance;

    sprintf(path, D_003BD198, base, name);
    allocation = sdfDevLoadFileEx(path, &sourceAddress, 0);
    instance = effCreateResourceSlotSetFromAllocation(allocation, keepAllocation);
    if (keepAllocation == EFF_RESOURCE_TRANSIENT) {
        sdfMemFreeDirect(allocation);
    }
    return instance;
}

/* Publish the instance, release its source allocation, then clean up the completed file job. */
void effCompleteTransientResourceJob(struct FileRequest *job, u32 *outInstance) {
    struct sdfMemHandle_t *allocation;
    EffectSlotSet *instance;

    allocation = fileGetMemHandle(job);
    instance = effCreateResourceSlotSetFromAllocation(allocation, EFF_RESOURCE_TRANSIENT);
    *outInstance = (u32)instance;
    sdfMemFreeDirect(allocation);
    fileHandleFree(job);
}

/* Publish the instance without releasing its source allocation, then clean up the file job. */
void effCompleteRetainedResourceJob(struct FileRequest *job, u32 *outInstance) {
    struct sdfMemHandle_t *allocation;
    EffectSlotSet *instance;

    allocation = fileGetMemHandle(job);
    instance = effCreateResourceSlotSetFromAllocation(allocation, EFF_RESOURCE_KEEP_ALLOCATION);
    *outInstance = (u32)instance;
    fileHandleFree(job);
}

/* Clear the output first; only mode one selects the retained-allocation completion path. */
void effRequestResourceByMode(const char *base, const char *name, u32 mode, u32 *outInstance) {
    char path[EFF_RESOURCE_PATH_BYTES];

    sprintf(path, D_003BD198, base, name);
    *outInstance = 0;
    if (mode == EFF_RESOURCE_KEEP_ALLOCATION) {
        fileLoadEx(path, 0, (u32)effCompleteRetainedResourceJob, (u32)outInstance);
    } else {
        fileLoadEx(path, 0, (u32)effCompleteTransientResourceJob, (u32)outInstance);
    }
}

/* Build mapped records from the retained source address, then release the original file allocation. */
EffMappedResource *effLoadMappedResource(const char *base, const char *name) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    EffMappedResource *mappedResource;
    struct sdfMemHandle_t *allocation;

    sprintf(path, D_003BD198, base, name);
    allocation = sdfDevLoadFileEx(path, &sourceAddress, 0);
    mappedResource = effCreateMappedResource((const u8 *)sourceAddress);
    sdfMemFreeDirect(allocation);
    return mappedResource;
}

/* Publish mapped records before releasing their source allocation and completing the file job. */
void effCompleteMappedResourceJob(struct FileRequest *job,
                                  EffMappedResource **outMappedResource) {
    struct sdfMemHandle_t *allocation;
    u32 sourceAddress;
    EffMappedResource *mappedResource;

    allocation = fileGetMemHandle(job);
    sourceAddress = sdfMemLock(allocation);
    mappedResource = effCreateMappedResource((const u8 *)sourceAddress);
    *outMappedResource = mappedResource;
    sdfMemFreeDirect(allocation);
    fileHandleFree(job);
}

/* Initialize the output to zero and schedule mapped-record completion. */
void effRequestMappedResource(const char *base, const char *name,
                              EffMappedResource **outMappedResource) {
    char path[EFF_RESOURCE_PATH_BYTES];

    sprintf(path, D_003BD198, base, name);
    *outMappedResource = 0;
    fileLoadEx(path, 0, (u32)effCompleteMappedResourceJob, (u32)outMappedResource);
}

/* Create an owner list with sixteen initially empty record buckets. */
EffectOwnerRecord *effCreateOwnerRecordList(EffectSlotSet *owner) {
    EffectOwnerRecord *list = sdfChipAllocDirect(sizeof(EffectOwnerRecord));
    memset(list, 0, EFF_OWNER_LIST_BYTES);
    list->owner = owner;
    return list;
}

/* Clamp only the upper bucket bound and prepend; the former head's prev link is not rewritten. */
void effInsertSlotRecord(void *owner, EffectOwnerRecord *list, EffectSlotOwner *work, s32 slotIndex) {
    EffectSlot *slotData = &work->slots[slotIndex];
    s32 bucketIndex = slotData->bucketIndex;
    EffectRecord *record;

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
s32 effRemoveSlotRecord(EffectOwnerRecord *list, EffectSlotOwner *work, s32 slotIndex) {
    EffectSlot *slotData = &work->slots[slotIndex];
    s32 bucketIndex = slotData->bucketIndex;
    EffectRecord *record = list->entries[bucketIndex];

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

INCLUDE_ASM(const s32, "code_002C8188", effReleaseRecordBuckets);

/* Dispatch every bucket record and optionally refresh its owner/slot lookup. */
u32 effDispatchRecordBuckets(u32 refresh, EffectOwnerRecord *list, s32 drawOption) {
    EffectRecord **bucketHead = list->entries;
    s32 bucketCountdown = EFF_RECORD_LAST_BUCKET;

    do {
        EffectRecord *record = *bucketHead;
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
    effReleaseRecordBuckets((EffectOwnerRecord *)list);
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
u32 effSumRecordStatuses(const EffMappedRecord *record) {
    EffRecordBucket *group = &D_0038FD88[record->category];
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
struct sdfMemHandle_t *effLoadMappedStatusRecords(const u8 *source, EffMappedHeader *headerOut) {
    EffMappedHeader header;
    struct sdfMemHandle_t *allocation;
    EffMappedRecord *records;
    u32 recordIndex = 0;
    u32 statusBytes;

    memcpy(&header, source, sizeof(header));
    source += sizeof(header);
    allocation = sdfMemAllocDirect(header.count * EFF_STATUS_RECORD_BYTES);
    records = (EffMappedRecord *)sdfMemLock(allocation);
    for (; recordIndex < header.count; recordIndex++) {
        EffMappedRecord *record = &records[recordIndex];

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

INCLUDE_ASM(const s32, "code_002C8188", func_002C8A90);

/* Build one zeroed status record and allocate the category's required status storage. */
EffMappedResource *effCreateStatusBatch(u32 category) {
    EffMappedResource *batch = sdfChipAllocDirect(sizeof(EffMappedResource));
    struct sdfMemHandle_t *allocation;
    u32 recordAddress;
    u32 statusBytes;
    void *statuses;

    batch->count = 1;
    allocation = sdfMemAllocDirect(EFF_STATUS_RECORD_BYTES);
    batch->allocation = allocation;
    recordAddress = sdfMemLock(allocation);
    batch->records = (EffMappedRecord *)recordAddress;
    memset((void *)recordAddress, 0, EFF_STATUS_RECORD_BYTES);
    {
        EffMappedRecord *record = batch->records;
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
EffMappedResourceDestroyResult effDestroyPackedBatch(EffMappedResource *batch) {
    s32 recordIndex;
    for (recordIndex = 0; recordIndex < batch->count; recordIndex++) {
        sdfChipFreeDirect(batch->records[recordIndex].status);
    }
    sdfMemFreeDirect(batch->allocation);
    sdfChipFreeDirect(batch);
    return EFF_MAPPED_RESOURCE_DESTROYED;
}

u32 effReleaseSlotWorkAllocation(EffectSlotSet *owner) {
    sdfMemFreeDirect(owner->workAllocation);
    return 1;
}

/* Return the normal slot work unless the slot carries an alternate payload. */
void *effGetSlotWorkOrOverride(EffectSlotSet *work, s32 slotIndex) {
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
void effInitializeSlotPhase(EffTimedState *effect) {
    if (effect->flags & EFF_TIMED_STATE_DIRECTION_FORWARD) {
        effect->value = EFF_SLOT_PHASE_ZERO;
    } else {
        effect->value = EFF_SLOT_PHASE_ONE;
    }
}

INCLUDE_ASM(const s32, "code_002C8188", func_002C8C70);

/* Initialize the normal slot entry at the unchanged 0xA0-byte stride. */
void effInitializeSlotWork(EffectSlotSet *owner, s32 slotIndex) {
    effInitializeSlotWorkFromDescription(owner, slotIndex, &owner->workEntries[slotIndex]);
}

/* Reset every normal work slot in source order. */
void effInitializeAllSlotWork(EffectSlotSet *work) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < work->count; slotIndex++) {
        effResetSlotWork(work, slotIndex);
    }
}

/* Store the owner and slot index before invoking the work initializer. */
void effAttachSlotWorkOwner(EffectSlotSet *owner, s32 slotIndex, void *payload) {
    /* Alternate records share these owner/index fields without a full work allocation. */
    BdWork *entry = (BdWork *)payload;
    entry->owner = owner;
    entry->slotIndex = slotIndex;
    effInitializeSlotWorkFromDescription(owner, slotIndex, payload);
}

/* Clear the complete normal slot, then restore owner/index and initialize it. */
void effResetSlotWork(EffectSlotSet *work, u32 slotIndex) {
    BdWork *entry;

    entry = &work->workEntries[slotIndex];
    memset(entry, 0, EFF_SLOT_WORK_BYTES);
    effAttachSlotWorkOwner(work, slotIndex, entry);
}

/* Acquire missing selected handles or clear slots; clearAllSlots does not release textures.
 * Return the address following the processed resource-table entries.
 */
u8 *effResolveResourceSlots(EffectSlotSet *set, u8 *resourceBytes, s32 clearAllSlots, s32 selectedSlot) {
    u32 slotIndex = 0;
    u8 *entryBytes = resourceBytes;

    entryBytes += ((EffectResourceTable *)entryBytes)->entryOffset;

    if (set->textureCount != 0) {
        do {
            s32 *resourceData = (s32 *)(resourceBytes + ((EffectResourceEntry *)entryBytes)->resourceOffset);

            entryBytes += EFF_RESOURCE_TABLE_ENTRY_BYTES;
            if (clearAllSlots == 0) {
                if (selectedSlot == -1 || selectedSlot == (s32)slotIndex) {
                    if (set->textureReferences[slotIndex] == 0) {
                        set->textureReferences[slotIndex] = sdfTexSet((SdfTextureFileHeader *)(resourceData));
                    }
                } else {
                    set->textureReferences[slotIndex] = 0;
                }
            } else {
                set->textureReferences[slotIndex] = 0;
            }
            slotIndex++;
        } while (slotIndex < set->textureCount);
    }
    return entryBytes;
}

void effResolveAndReleaseResource(EffectSlotSet *owner) {
    if (owner->sourceAllocation != 0) {
        u8 *data = (u8 *)sdfMemLock(owner->sourceAllocation);
        effResolveResourceSlots(owner, data, 0, -1);
        sdfMemUnlock(owner->sourceAllocation);
    }
}

void effResolveAndReleaseSelectedResource(EffectSlotSet *owner, s32 slot) {
    if (owner->sourceAllocation != 0) {
        u8 *data = (u8 *)sdfMemLock(owner->sourceAllocation);
        effResolveResourceSlots(owner, data, 0, slot);
        sdfMemUnlock(owner->sourceAllocation);
    }
}

void effReleaseTextureHandlesAndResetSlots(EffectSlotSet *set) {
    u32 i;

    for (i = 0; i < set->textureCount; i++) {
        if (set->textureReferences[i] != 0) {
            sdfTexReleaseReference(set->textureReferences[i]);
            set->textureReferences[i] = 0;
        }
    }
    effInitializeAllSlotWork(set);
}

u8 effHasFirstTextureHandle(const EffectSlotSet *set) {
    return set->textureReferences[0] != NULL;
}

/* Allocate and clear count 0x6C-byte records; retain the existing allocation/count/address header order. */
EffPayload *effCreatePayload(u32 recordCount) {
    u32 recordBytes = recordCount * EFF_PAYLOAD_RECORD_BYTES;
    EffPayload *header = sdfChipAllocDirect(sizeof(EffPayload));
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
u32 effDestroyPayload(EffPayload *payload) {
    sdfMemFreeDirect(payload->allocation);
    sdfChipFreeDirect(payload);
    return 1;
}

EffectSlotSet *effCreateResourceSlotSetFromAllocation(struct sdfMemHandle_t *resourceAllocation, u32 keepAllocation) {
    EffectSlotSet *set;
    u8 *resource;
    u32 *entries;
    u32 index;
    u32 sourceOffset;

    set = sdfChipAllocDirect(sizeof(EffectSlotSet));
    memset(set, 0, 0x30);
    set->sharesTextureReferences = EFF_SLOT_TEXTURE_REFERENCES_OWNED;
    set->sourceAllocation = keepAllocation != 0 ? resourceAllocation : 0;
    resource = (u8 *)sdfMemLock(resourceAllocation);
    set->textureCount = ((EffectResourceTable *)resource)->textureCount;
    set->textureAllocation =
        sdfMemAllocDirect(set->textureCount * 4);
    set->textureReferences = (sdfTexHandle_t **)sdfMemLock(set->textureAllocation);
    memset(set->textureReferences, 0, set->textureCount * 4);
    entries = (u32 *)effResolveResourceSlots(set, resource,
        keepAllocation, -1);

    set->count = ((EffectResourceTable *)resource)->slotCount;
    set->descriptionAllocation =
        sdfMemAllocDirect(set->count * 0x80);
    set->descriptions = (EffectSlotDescription *)sdfMemLock(set->descriptionAllocation);
    set->workAllocation = sdfMemAllocDirect(set->count * 0xA0);
    set->workEntries = (BdWork *)sdfMemLock(set->workAllocation);
    for (index = 0; index < set->count; index++) {
        sourceOffset = entries[1];
        memcpy(&set->descriptions[index],
            resource + sourceOffset, 0x80);
        effResetSlotWork(set, index);
        entries += 2;
    }
    return set;
}

EffectSlotSet *effCreateResourceSlotSet(EffectSlotSet *borrowedSource, u32 slot, u32 count) {
    EffectSlotSet *effect = (EffectSlotSet *)sdfChipAllocDirect(sizeof(EffectSlotSet));
    u32 index = 0;
    effect->sharesTextureReferences = EFF_SLOT_TEXTURE_REFERENCES_BORROWED;
    {
        u32 mode = borrowedSource->textureCount;
        sdfTexHandle_t **textureReferences = borrowedSource->textureReferences;
        effect->textureCount = mode;
        effect->textureReferences = textureReferences;
    }
    effect->sourceAllocation = 0;
    effect->textureAllocation = 0;
    effect->count = count;
    effect->descriptionAllocation = sdfMemAllocDirect(count * 0x80);
    effect->descriptions = (EffectSlotDescription *)sdfMemLock(effect->descriptionAllocation);
    effect->workAllocation = sdfMemAllocDirect(effect->count * 0xA0);
    effect->workEntries = (BdWork *)sdfMemLock(effect->workAllocation);
    if (effect->count != 0) {
        do {
            memcpy(&effect->descriptions[index], &borrowedSource->descriptions[slot], 0x80);
            effResetSlotWork(effect, index);
            index++;
        } while (index < effect->count);
    }
    return effect;
}

u32 effDestroyResourceSlotSet(EffectSlotSet *set) {
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

u32 effSetSlotResourceAndFlags(EffTimedState *record, EffMappedRecord *entry, u32 flags) {
    record->flags = flags;
    record->source = entry;
    if ((flags & EFF_TIMED_STATE_INITIALIZE_PHASE_ON_BIND) != 0) {
        effInitializeSlotPhase(record);
    }
    record->delay = record->delay + 1;
    return 1;
}

u32 effSetSlotIndexedResource(EffTimedState *target, EffMappedResource *resources, s32 item,
                              u32 flags) {
    effSetSlotResourceAndFlags(target, &resources->records[item], flags);
    return 1;
}

u32 effSlotTransitionClearTarget(EffTimedState *record, u32 unused) {
    record->source = 0;
    return 1;
}

u32 effClampSlotPhaseAtEnd(EffectSlotSet *owner, s32 index, EffTimedState *effect) {
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

u32 effClampSlotPhaseAtStart(EffectSlotSet *owner, s32 index, EffTimedState *effect) {
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

EffectSlotSet *effUpdateTimedStates(EffectSlotSet *effect, u32 slot, void *entryData) {
    /* Alternate payloads may be only 0x6C bytes; direct access here is limited to timed states.
     * Bucket callbacks retain their existing kind-specific pointer contract.
     */
    BdWork *entry = (BdWork *)entryData;
    EffTimedState *states = entry->states;
    BdWork *record = &effect->workEntries[slot];
    s32 idle = 1;
    u32 i;

    for (i = 0; i < 2; i++) {
        EffTimedState *state = &states[i];
        EffMappedRecord *source = state->source;

        if (source != 0 && source->category != 0) {
            EffRecordBucket *group = &D_0038FD88[source->category];
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

u32 effSetSlotOverrideWork(EffectSlotSet *owner, s32 index, void *payload) {
    if (owner->workEntries[index].alternate.payload == NULL) {
        effAttachSlotWorkOwner(owner, index, payload);
    }
    owner->workEntries[index].alternate.payload = payload;
    return 1;
}

u32 effSetMaterialSlots(EffectSlotSet *owner, s32 index, u32 materialFlags, void *payload) {
    u32 i;
    EffTimedState *states;
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

INCLUDE_ASM(const s32, "code_002C8188", func_002C9A60);

u32 effConfigureSlotResource(s32 work, s32 index, u32 value, u32 flags) {
    BdWork *effect = &((EffectSlotSet *)work)->workEntries[index];
    effSetSlotResourceAndFlags(&effect->states[0], (EffMappedRecord *)(u32)value, flags);
    effUpdateTimedStates((EffectSlotSet *)work, index, effect);
    return 1;
}

u32 effConfigureIndexedSlotResource(EffectSlotSet *work, s32 index,
                                    EffMappedResource *data, s32 item, u32 flags) {
    BdWork *effect = &work->workEntries[index];
    effSetSlotResourceAndFlags(&effect->states[0], &data->records[item], flags);
    effUpdateTimedStates(work, index, effect);
    return 1;
}

u32 effConfigureIndexedSlotMaterial(EffectSlotSet *work, s32 index,
                  EffMappedResource *data, s32 item,
                  u32 materialFlags, u32 materialValue, u32 stateFlags) {
    BdWork *effect = &work->workEntries[index];
    effSetSlotResourceAndFlags(&effect->states[0], &data->records[item], stateFlags);
    effUpdateTimedStates(work, index, effect);
    effect->states[0].materialFlags = materialFlags;
    effect->states[0].materialValue = materialValue;
    return 1;
}

u32 effConfigureWithDefaultSetting(EffectSlotSet *effect, u32 slot,
                                   EffMappedResource *resources, u32 item,
                                   u32 materialFlags, u32 stateFlags) {
    effConfigureIndexedSlotMaterial(effect, slot, resources, item,
                                    materialFlags, 0, stateFlags);
    return 1;
}

u32 effResetRecordRun(u8 *work, u32 first, u32 unused) {
    u32 i = 0;
    u32 index;

    do {
        effSlotTransitionClearTarget((EffTimedState *)((first + i) * EFF_SLOT_WORK_BYTES +
                                    (s32)((EffectSlotSet *)work)->workEntries + 0x28), unused);
        i++;
        index = first + i;
    } while (index < ((EffectSlotSet *)work)->count && (((EffectSlotSet *)work)->descriptions[index].flags & EFF_SLOT_DESCRIPTION_SEQUENCE_CONTINUATION));
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

INCLUDE_ASM(const s32, "code_002C8188", sdfDrawTexturedQuad);

void effSelectPresetAndDispatch(u32 x, u32 y, u32 z, u32 width,
                                    u32 height, u32 vertexColors, u32 presetMode, u32 presetValue) {
    effSelectPresetByKind(presetMode, presetValue);
    uiDrawGradientColorRect(x, y, z, width, height, (const u32 *)vertexColors, presetValue);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, presetValue);
}
INCLUDE_SDATA(const s32, "code_002C8188", D_0046E718);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E720);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E728);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E730);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E738);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E740);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E748);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E750);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E758);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E760);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E768);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E770);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E778);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E780);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E788);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E790);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E798);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E7A0);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E7A8);

INCLUDE_SDATA(const s32, "code_002C8188", D_0046E7B0);

