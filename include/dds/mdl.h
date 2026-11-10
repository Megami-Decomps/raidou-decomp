#ifndef DDS_MDL_H

#define DDS_MDL_H
#include "../model.h"
#include "common.h"

#include "sdf_draw.h"

#if !defined(SMT3_DEFINES_dds3ModelHandle_t)
typedef struct dds3ModelHandle_t dds3ModelHandle_t;
#endif

/* Model-context update gates; these do not share the inner dds_sdfModelRootNode_t flag byte. */

#define MDL_SKIP_TRANSFORMS 1

#define MDL_SKIP_ANCHORS 2

#define MDL_REQUIRE_ANCHOR_ENABLE 4

struct MdlPartEntry;

struct MdlObj;

struct sdfGrOt_t;

/* Relative-linked viewer records share packed payload and parameter words.
 * Kinds 1/2 use the count and part-index halfwords at 0x0C/0x0E; kind 3
 * reads the whole parameter word. Kind 4 reads both selector halfwords at 0x08/0x0A. */
typedef struct MdlRecord {
    s32 kind;       /* 0x00: 0xFFFF terminates the record chain */
    s32 nextOffset; /* 0x04: relative byte offset to next record */
    union {
        u32 word;               /* 0x08 */
        s32 id;                 /* Kind 1/2/5 resource-record ID. */
        struct {
            u16 selectorA;
            u16 selectorB;
        } stream;
    } payload;
    union {
        u32 word;               /* 0x0C */
        s32 minimumTime;        /* Kind 5 deferred initialization threshold. */
        struct {
            u16 count;
            u16 partIndex;
        } part;
    } parameter;
    union {
        u32 word10; /* 0x10: kind-four stream parameter */
        f32 anchorScale; /* Optional kind 1/2 extension when nextOffset >= 0x11. */
        struct {
            u8 playbackCadenceStep;
            u8 loopMode;
            u16 partIndex;
        } object;
        struct {
            u16 slotCount; /* Kind 1/2 group records: following entity-slot packets. */
            u16 unk12;
        } half;
    } tail;
} MdlRecord;

typedef char MdlRecord_size_must_be_0x14[(sizeof(MdlRecord) == 0x14) ? 1 : -1];

struct SdfMapPositionRecord;

struct sdfModelDef_t;

struct sdfTexHandle_t;

struct EffTrackPolyWork;

/* Known resource-item kinds; the owner stores this domain in a halfword. */
typedef enum MdlResourceKind {
    MDL_RESOURCE_BILLBOARD = 0,
    MDL_RESOURCE_EFFECT = 1,
    MDL_RESOURCE_TRACK_POLY = 2,
    MDL_RESOURCE_OBJECT = 3
} MdlResourceKind;

typedef struct MdlObjectAttachment {
    dds3ModelHandle_t *owner;
    struct MdlObj *object; /* Borrowed from the owner's heterogeneous part list. */
    struct sdfTexHandle_t *texture; /* Borrowed texture-list node used for stream creation. */
    s32 minimumTime;
    u8 attributes[8];
} MdlObjectAttachment;

#define MDL_MOTION_SLOT_CREATE_ON_MODEL_LOAD 1

typedef struct BattleGroupSlot {
    s32 flags; /* 0x00: bit 0 creates a motion owner for new model contexts. */
    s16 motionSlotIndex;
    s16 motionIndex;
    MotionTable *motionTable;
    struct sdfMemHandle_t *resourceHandle;
} BattleGroupSlot;

typedef struct MdlLoadPayload {
    DevRequest *resourceList;
    struct sdfModelDef_t *itemList;
    struct sdfMemHandle_t *requestAllocation;
    MotionTable *motionTable;
    struct sdfMemHandle_t *motionResource;
    void *partInfo; /* Borrowed relative-linked record-list transport. */
    struct sdfMemHandle_t *resourceHandle;
    DevRequest *partList;
} MdlLoadPayload;

/* mdlPrepare allocates this 0x2C-byte callback work record. */
typedef struct MdlLoadRequest {
    u16 group;
    u16 id;
    u8 resourceListRequested;
    u8 itemsRequested;
    u8 awaitingMotionLoad;
    u8 pad7;
    u32 options; /* MdlRegistrationMode consumed when this request completes. */
    MdlLoadPayload payload;
} MdlLoadRequest;

typedef struct MdlDevSlot {
    struct MdlDevSlot *next;
    dds_sdfModelRootNode_t *model;
    s32 remaining;
} MdlDevSlot;

/* This owner has inline color interpolation data, not a request buffer at +0xC. */
typedef struct MdlDevList {
    MdlDevSlot *first;
    s16 usedCount;
    u16 capacity;
    s16 stride;
    s16 growStep;
    s32 byteCount;
    f32 color[4];
    f32 colorStep[4];
} MdlDevList;

#if !defined(SMT3_DEFINES_dds3ModelHandle_t)
/* Context constructors allocate 0x38 bytes; their SDK model slot lives at +0x18. */
struct dds3ModelHandle_t {
    u32 flags;
    dds3ModelHandle_t *next;
    dds3ModelHandle_t *previous;
    dds3ModelResrc_t *sub;
    union {
        u32 word;
        struct {
            s16 id; /* Current MdlMotionSearchId, stored in its native halfword. */
            s16 motionIndex; /* Entry index passed to sdfMotionChangeBlend. */
        } h;
    } current;
    dds3ModelEffectCtrlr_t *resourceItems;
    dds_sdfModelRootNode_t *inner;
    sdfMotionCtrlr_t *first;
    sdfMotionCtrlr_t *slots[4];
    MdlDevList *devList;
    f32 objectRadius; /* Propagated to the rebuilt object's inner radius. */
};
#endif

typedef char BattleGroupSlot_size_must_be_0x10[(sizeof(BattleGroupSlot) == 0x10) ? 1 : -1];

typedef char MdlDevSlot_size_must_be_0xC[(sizeof(MdlDevSlot) == 0xC) ? 1 : -1];

typedef char MdlDevList_size_must_be_0x30[(sizeof(MdlDevList) == 0x30) ? 1 : -1];

typedef char MdlCtx_size_must_be_0x38[(sizeof(dds3ModelHandle_t) == 0x38) ? 1 : -1];

typedef char MdlLoadPayload_size_must_be_0x20[(sizeof(MdlLoadPayload) == 0x20) ? 1 : -1];

typedef char MdlLoadRequest_size_must_be_0x2C[(sizeof(MdlLoadRequest) == 0x2C) ? 1 : -1];

typedef char MdlObjectAttachment_size_must_be_0x18[(sizeof(MdlObjectAttachment) == 0x18) ? 1 : -1];

#endif /* DDS_MDL_H */
