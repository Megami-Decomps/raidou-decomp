#ifndef DDS_FLD_BILL_CONFIG_H

#define DDS_FLD_BILL_CONFIG_H
#include "common.h"

/* Shared FLD1 type-11 billboard descriptor. The final two words have no
 * confirmed interpretation and remain opaque. */
typedef struct BillConfig {
    u32 flags;
    u32 kind;
    u32 resourceIndex;
    f32 width;
    f32 height;
    f32 depth;
    u32 projectionDistance;
    s32 mapSelector;
    union {
        u32 entry;
        u32 fadeStart;
    };
    u32 fadeEnd;
    u8 opaque28[8];
} BillConfig;

typedef char BillConfig_size_must_be_0x30[
    (sizeof(BillConfig) == 0x30) ? 1 : -1
];

#endif
