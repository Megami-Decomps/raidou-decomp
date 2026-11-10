#ifndef DDS_SDF_PAC_STATE_H

#define DDS_SDF_PAC_STATE_H
#include "common.h"

#include "sdf_pac_packet.h"

#include "sdf_pac_work.h"

struct sdfMemHandle_t;

struct sdfTexHandle_t;

/* Complete 0x10-byte packet buffer: texture is the acquired output, while
 * resourceSlot owns the optional general-heap allocation descriptor. */
typedef struct PacBuf {
    struct sdfTexHandle_t *texture;
    struct sdfMemHandle_t *resourceSlot;
    u8 *cursor;
    s32 remainingBytes;
} PacBuf;

typedef char PacBuf_size_must_be_0x10[(sizeof(PacBuf) == 0x10) ? 1 : -1];

/* Per-queue allocation-list descriptor selected through PacState.slot. */
typedef struct PacListAllocation {
    s32 entryCount; /* 0x00 */
    s32 entryIndex; /* 0x04 */
    u8 pad08[8]; /* 0x08 */
    SdfPacStreamPacketHeader entry; /* 0x10 */
    PacBuf buffer; /* 0x20 */
} PacListAllocation;

typedef char PacListAllocation_size_must_be_0x30[
    (sizeof(PacListAllocation) == 0x30) ? 1 : -1];

/* Complete 0x38-byte PAC decoder state shared by file and model loaders. */
typedef struct PacState {
    s8 phase; /* 0x00: -1 complete, 0 boundary, 1 skip, 2 input handler */
    u8 flags; /* 0x01 */
    u16 packetCounter; /* 0x02: wraps from the initial 0xFFFF */
    s32 (*packetCallback)(); /* 0x04: event callbacks have different arities */
    void (*onInput)(struct PacState *); /* 0x08 */
    void (*onComplete)(struct PacState *); /* 0x0C */
    const u8 *inputCursor; /* 0x10 */
    s32 inputAvailable; /* 0x14 */
    s32 consumedBytes; /* 0x18 */
    u8 *outputCursor; /* 0x1C */
    s32 pendingBytes; /* 0x20 */
    u8 *decoder; /* 0x24: decoder work block, held as a byte pointer */
    PacBuf *resourceBuffer; /* 0x28 */
    union {
        PacBuf *resource;
        PacListAllocation *list;
    } slot; /* 0x2C */
    PacWork *queueHead; /* 0x30 */
    PacWork *queueTail; /* 0x34 */
} PacState;

/* The decode result follows this signed phase domain but keeps its 32-bit ABI. */
typedef s32 SdfPacPhase;

typedef char PacState_size_must_be_0x38[(sizeof(PacState) == 0x38) ? 1 : -1];

#endif /* DDS_SDF_PAC_STATE_H */
