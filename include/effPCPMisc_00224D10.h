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
typedef struct EffEmitterType {
    void (*updateSlots)(void *record);
    void (*scaleParameters)(void *record, f32 scale);
    s32 slotBytes;
    s32 dataBytes;
} EffEmitterType;

extern EffEmitterType effEmitterTypeDescriptors[];

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

struct Standard;
struct effPartsHandle_t;
struct emdlHandle_t;

void Delete(struct effPartsHandle_t *handle);
struct emdlHandle_t *effCallChipAllocZeroDirect3(struct Standard *pParts);
struct emdlHandle_t *effPCPAnalyzeBEDCopy(struct effPartsHandle_t *handle);

#endif
