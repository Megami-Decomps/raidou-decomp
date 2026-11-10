#ifndef DDS_BTL_APPROACH_TASK_ARGS_H

#define DDS_BTL_APPROACH_TASK_ARGS_H
#include "common.h"

struct BtlUnit;

/* Cleared task argument block for moving one unit toward a selected target. */
typedef struct BtlApproachTaskArgs {
    struct BtlUnit *unit;   /* 0x00: unit being moved */
    struct BtlUnit *target; /* 0x04: selected target unit */
    f32 motionReachOffset;  /* 0x08: scaled from the moving unit's current motion */
    f32 scale;              /* 0x0C: interpolation scale, with zero selecting 1.0 */
    u32 opaque10;            /* 0x10: cleared by allocation; not accessed by callback */
    s32 count;               /* 0x14: callback frame count */
} BtlApproachTaskArgs;

typedef char BtlApproachTaskArgsSizeCheck[
    (sizeof(BtlApproachTaskArgs) == 0x18) ? 1 : -1];

#endif /* DDS_BTL_APPROACH_TASK_ARGS_H */
