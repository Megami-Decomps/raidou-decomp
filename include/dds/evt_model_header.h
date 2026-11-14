#ifndef DDS_EVT_MODEL_HEADER_H

#define DDS_EVT_MODEL_HEADER_H
#include "common.h"

struct effEveBEDCutHandle_t;

/* Shared event-model prefix; DDS2 also consumes the opaque +0x1C..+0x2B gap
 * and the target pointer at +0x2C in the model-cut opcode. */
typedef struct EvtModelHeader {
    u8 pad00[4];
    u32 flags;          /* 0x04: bit 2 selects the header transform */
    u8 pad08[8];
    f32 positionX;      /* 0x10: script-supplied translation */
    f32 positionY;      /* 0x14 */
    f32 positionZ;      /* 0x18 */
#ifdef VERSION_DDS2
    u8 pad1C[0x10];
    struct effEveBEDCutHandle_t *target2C; /* 0x2C: model-cut scale target */
#endif
} EvtModelHeader;

#ifdef VERSION_DDS2

#endif

#endif
