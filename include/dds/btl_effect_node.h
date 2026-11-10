#ifndef DDS_BTL_EFFECT_NODE_H

#define DDS_BTL_EFFECT_NODE_H
#include "common.h"

/* Shared motion/approach parameters in a battle effect resource row. */
typedef struct BtlEffectNode {
    s16 triggerKind; /* 0x00 */
    u8 pad02[2];
    s16 rateKind; /* 0x04 */
    u8 pad06[2];
    f32 scale; /* 0x08 */
    f32 reachOffset; /* 0x0C */
    u8 pad10[2];
    u16 frameCount; /* 0x12 */
} BtlEffectNode;

/* Effect-node rows follow the 0x2C-byte variable-length table header. */
typedef struct BtlEffectResource {
    s128 baseOffset;   /* 0x00: copied into the actor body and muzzle offsets. */
    f32 actorScale;    /* 0x10: applied to the actor scale and effect vectors. */
    f32 zOffset;       /* 0x14: applied as the actor position Z offset. */
    f32 reach;         /* 0x18: copied to the actor reach field. */
    f32 height;        /* 0x1C: copied to the actor height field. */
    f32 cameraRadius;  /* 0x20: copied to the actor camera-clearance value. */
    u8 unknown24[8]; /* 0x24: opaque table-header bytes */
    BtlEffectNode nodes[1]; /* 0x2C: variable-length row array */
} BtlEffectResource;

typedef char BtlEffectNode_size_must_be_0x14[
    (sizeof(BtlEffectNode) == 0x14) ? 1 : -1];

typedef char BtlEffectResource_size_must_be_0x40[
    (sizeof(BtlEffectResource) == 0x40) ? 1 : -1];

#endif /* DDS_BTL_EFFECT_NODE_H */
