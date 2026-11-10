#ifndef DDS_EFF_EVENT_H

#define DDS_EFF_EVENT_H
#include "dds3obj.h"

/* Kind-8 constructors allocate this complete 0x10-byte payload and own the
 * base-control block stored in its first word. */
typedef struct DdsSlotResourceBlock {
    dds_dds3ObjectBaseControlWorkData_t *baseControlWorkData;
    u8 pad04[0xC];
} DdsSlotResourceBlock;

typedef char DdsSlotResourceBlock_size_must_be_0x10[(sizeof(DdsSlotResourceBlock) == 0x10) ? 1 : -1];

struct SoundMixer;

#if !defined(SMT3_DEFINES_effEveBEDCutHandle_t)
typedef struct effEveBEDCutHandle_t effEveBEDCutHandle_t;
#endif

/* Event-light init block copied into the retained event owner (0x30 bytes). */
typedef struct EffEventInit {
    f32 position[4];
    f32 orientation[4];
    f32 range;
    f32 height;
    f32 param;
    u32 color;
} EffEventInit;

typedef char EffEventInit_size_must_be_0x30[(sizeof(EffEventInit) == 0x30) ? 1 : -1];

/* Identifies who releases the event handle shared by light-effect clones. */
typedef u8 EffSimpleBEDHandleOwnership;

#define EFF_SIMPLE_BED_HANDLE_BORROWED 0

#define EFF_SIMPLE_BED_HANDLE_OWNED 1

#if !defined(SMT3_DEFINES_effSimpleBED_t)
/* Per-title light-effect holder allocated by its create and clone paths. */
typedef struct effSimpleBED_t {
    struct SoundMixer *handle;
    effEveBEDCutHandle_t *owner;
    EffEventInit init;
    EffSimpleBEDHandleOwnership handleOwnership;
    u8 opaque39[3];
} effSimpleBED_t;
#endif

typedef char EffEventLight_size_must_be_0x3C[
    (sizeof(effSimpleBED_t) == 0x3C) ? 1 : -1];

/* Copied from the per-title event-node template after its vector pair is
 * replaced by caller inputs; the trailing parameters and color are preserved. */
typedef struct EffectEventVectorParameters {
    f32 firstVector[4];
    f32 secondVector[4];
    f32 parameters[3];
    u32 color;
} EffectEventVectorParameters;

typedef char EffectEventVectorParameters_size_must_be_0x30[
    (sizeof(EffectEventVectorParameters) == 0x30) ? 1 : -1];

void effEventUpdateEffectParameters(effEveBEDCutHandle_t *work);

#endif
