#ifndef DDS_BTL_AFFINITY_ROW_H

#define DDS_BTL_AFFINITY_ROW_H
#include "common.h"

typedef struct BtlAffinityRow {
    s32 value[19];
} BtlAffinityRow;

typedef char BtlAffinityRow_size_must_be_0x4C[
    (sizeof(BtlAffinityRow) == 0x4C) ? 1 : -1];

#endif
