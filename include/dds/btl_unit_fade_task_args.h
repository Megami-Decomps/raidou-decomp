#ifndef DDS_BTL_UNIT_FADE_TASK_ARGS_H

#define DDS_BTL_UNIT_FADE_TASK_ARGS_H
#include "btl_unit_fade_task_types.h"

struct BtlUnit;

/* Shared payload for the unit defeat and exit fade callbacks (12 bytes). */
typedef struct BtlUnitFadeTaskArgs {
    struct BtlUnit *unit; /* 0x00 */
    union {
        s32 duration;              /* Defeat fade tasks; -1 is a fade-in sentinel. */
        BtlUnitExitFadeMode exitMode;  /* Exit fade task, selected by task kind. */
    };
    s32 elapsed;          /* 0x08 */
} BtlUnitFadeTaskArgs;

typedef char BtlUnitFadeTaskArgsSizeCheck[sizeof(BtlUnitFadeTaskArgs) == 0x0C ? 1 : -1];

#endif
