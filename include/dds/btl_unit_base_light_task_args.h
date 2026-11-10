#ifndef DDS_BTL_UNIT_BASE_LIGHT_TASK_ARGS_H

#define DDS_BTL_UNIT_BASE_LIGHT_TASK_ARGS_H
#include "common.h"

struct BtlUnit;

/* Eight-byte callback arguments allocated by btlCreateUnitBaseLightTask. */
typedef struct BtlUnitBaseLightArgs {
    struct BtlUnit *unit;
    s32 delay;
} BtlUnitBaseLightArgs;

typedef char BtlUnitBaseLightArgs_size[(sizeof(BtlUnitBaseLightArgs) == 0x08) ? 1 : -1];

#endif
