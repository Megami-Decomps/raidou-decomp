#ifndef DDS_BTL_CAMERA_TIMED_INSTRUCTION_H

#define DDS_BTL_CAMERA_TIMED_INSTRUCTION_H
#include "common.h"

/* Camera scripts advance serialized instructions by 0x10 bytes. The paired
 * readers consume kind, the signed parameter selector at +4, and the active
 * frame interval. The remaining halfword at +6 is opaque. */
typedef struct BtlCameraTimedInstruction {
    s32 kind;          /* 0x00 */
    s16 parameterIndex; /* 0x04 */
    u8 pad06[2];       /* 0x06 */
    f32 startFrame;    /* 0x08 */
    f32 duration;      /* 0x0C */
} BtlCameraTimedInstruction;

typedef char BtlCameraTimedInstruction_size_check[
    (sizeof(BtlCameraTimedInstruction) == 0x10) ? 1 : -1];

#endif /* DDS_BTL_CAMERA_TIMED_INSTRUCTION_H */
