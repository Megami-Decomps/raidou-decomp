#ifndef EFFPCPMISC_00224D10_H
#define EFFPCPMISC_00224D10_H

#include "common.h"

void fileReadRecordSecondVector(void *recordBytes, void *dst);
void fileReadVectorPtr20(void *recordBytes, void *dst);
void fileSetRecordSecondVector(void *recordBytes, const void *src);
void mnuRecordSetVector(void *recordBytes, const void *src);

#include "model.h"
#include "motion.h"

/* An emitter type (0x10), effEmitterTypeDescriptors[type]: its per-frame slot
 * update, its parameter scaling, and the sizes of its slots and data. An
 * emitter record starts with its u16 type. */
typedef struct effEmitterType_t {
    void (*updateSlots)(void *record);
    void (*scaleParameters)(void *record, f32 scale);
    s32 slotBytes;
    s32 dataBytes;
} effEmitterType_t;

extern effEmitterType_t effEmitterTypeDescriptors[];

/* Scale an emitter record's parameters through its type. */
void dds3DispatchIndexedCallback(u16 *record, f32 scale);

/* The save header records whether this run was started as a New Cycle. */
typedef s16 DatNewCycleState;

/* Three opaque words copied together between the game state and save preview. */
typedef struct datSaveWords_t {
    u32 words[3];
} datSaveWords_t;

typedef char datSaveWordsSizeCheck[
    sizeof(datSaveWords_t) == 0x0C ? 1 : -1];

/* The memory-card browser copies the first 0x30 bytes of a save into a
 * compact row. Its final three words retain the shared save-word group. The
 * first two remain opaque. The last is a saved flag word. */
typedef struct fileSavePreviewRecord_t {
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
        datSaveWords_t saveWords;
        struct {
            u32 opaque24;
            u32 opaque28;
            u32 savedFlags;
        } saveState;
    };
} fileSavePreviewRecord_t;

typedef char fileSavePreviewRecordSizeCheck[
    sizeof(fileSavePreviewRecord_t) == 0x30 ? 1 : -1];

/* Next free minor id for effect models (major 5). */
extern u16 effHandleCreateData;

struct fileJobPayload_t;

/* Heap-owned model wrapper (0x14 chip block); the light buffer is a 0xE0-byte chip block. */
typedef struct effModelOwner_t {
    f32 scale;
    struct mdlObj_t *model;
    struct sdf3DLightParallelPacket_t *ownedBuffer;
    u32 flags;
    u32 unk10;
} effModelOwner_t;

SIZE_CHECK(effModelOwnerSizeCheck, effModelOwner_t, 0x14);

mdlObj_t *effHandleCreate(const u8 *data, s32 size);
mdlObj_t *effHandleCopy(mdlObj_t *sourceModel);
void Delete(effModelOwner_t *owner);
effModelOwner_t *effCreateModelOwner(struct fileJobPayload_t *source);
effModelOwner_t *effPCPAnalyzeBEDCopy(effModelOwner_t *source);

#endif
