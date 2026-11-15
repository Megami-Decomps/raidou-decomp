#ifndef EFFPCPMISC_00224D10_H
#define EFFPCPMISC_00224D10_H

#include "common.h"

void fileReadRecordSecondVector(void *recordBytes, void *dst);
void fileReadVectorPtr20(void *recordBytes, void *dst);
void fileSetRecordSecondVector(void *recordBytes, const void *src);
void mnuRecordSetVector(void *recordBytes, const void *src);

#include "model.h"
#include "motion.h"

/* Scale an emitter record's parameters through its type. */
void dds3DispatchIndexedCallback(u16 *record, f32 scale);

/* The save header records whether this run was started as a New Cycle. */
typedef s16 DatNewCycleState;

/* Three opaque words copied together between the game state and save preview. */
typedef struct DatSaveWords {
    u32 words[3];
} DatSaveWords;

typedef char DatSaveWordsSizeCheck[
    sizeof(DatSaveWords) == 0x0C ? 1 : -1];

/* The memory-card browser copies the first 0x30 bytes of a save into a
 * compact row. Its final three words retain the shared save-word group. The
 * first two remain opaque. The last is a saved flag word. */
typedef struct FileSavePreviewRecord {
    char signature[3];
    s8 version;
    s8 mapGroup;
    s8 mapIndex;
    u8 pad06[2];
    s32 playTicks;
    s16 status;
    DatNewCycleState newCycle;
    s8 party[8];
    s8 levels[8];
    u32 money;
    union {
        DatSaveWords saveWords;
        struct {
            u32 opaque24;
            u32 opaque28;
            u32 savedFlags;
        } saveState;
    };
} FileSavePreviewRecord;

typedef char FileSavePreviewRecordSizeCheck[
    sizeof(FileSavePreviewRecord) == 0x30 ? 1 : -1];

typedef struct MdlDevSlot {
    struct MdlDevSlot *next;
    sdfModelRootNode_t *model;
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

typedef char MdlDevSlot_size_must_be_0xC[(sizeof(MdlDevSlot) == 0xC) ? 1 : -1];

typedef char MdlDevList_size_must_be_0x30[(sizeof(MdlDevList) == 0x30) ? 1 : -1];

typedef struct dds3ModelHandle_t dds3ModelHandle_t;

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
    sdfModelRootNode_t *inner;
    sdfMotionCtrlr_t *first;
    sdfMotionCtrlr_t *slots[4];
    MdlDevList *devList;
    f32 objectRadius; /* Propagated to the rebuilt object's inner radius. */
};

typedef char MdlCtx_size_must_be_0x38[(sizeof(dds3ModelHandle_t) == 0x38) ? 1 : -1];

#endif
