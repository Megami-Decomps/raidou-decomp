#ifndef DDS_EFF_COPIED_PAYLOAD_H

#define DDS_EFF_COPIED_PAYLOAD_H
#include "common.h"

#include "dds3_path.h"

#include "eff_transform.h"

struct sdfMemHandle_t;

/* Payload owner shared by the paired effect units. +0x3C remains opaque. */
typedef struct EffCopiedPayload {
    u8 *body;                       /* 0x00 */
    s32 size;                       /* 0x04 */
    u32 slotCount;                  /* 0x08: populated effect/target pairs */
    u32 selectedTargetIndex;        /* 0x0C: chosen target path slot */
    dds3Basic_t *effects[5];      /* 0x10 */
    dds3PathBasicControl_t *targets[5]; /* 0x24 */
    struct sdfMemHandle_t *allocation; /* 0x38 */
    u8 opaque3C[4];
} EffCopiedPayload;

typedef struct EffCopiedPayloadWork {
    u8 opaque00[0x30];
    EffCopiedPayload *payload; /* 0x30 */
    u8 opaque34[4];
    u32 parameter; /* 0x38 */
} EffCopiedPayloadWork;

typedef char EffCopiedPayload_size_check[
    (sizeof(EffCopiedPayload) == 0x40) ? 1 : -1];

typedef char EffCopiedPayloadWork_size_check[
    (sizeof(EffCopiedPayloadWork) == 0x3C) ? 1 : -1];

#endif /* DDS_EFF_COPIED_PAYLOAD_H */
