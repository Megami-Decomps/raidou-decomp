#ifndef DDS_BTL_DEFERRED_UNIT_STATUS_ARGS_H

#define DDS_BTL_DEFERRED_UNIT_STATUS_ARGS_H
#include "common.h"

struct BtlUnit;

typedef struct BtlDeferredUnitStatusArgs {
    struct BtlUnit *unit;
    u32 minimumValue;
} BtlDeferredUnitStatusArgs;

typedef char BtlDeferredUnitStatusArgsSizeCheck[
    sizeof(BtlDeferredUnitStatusArgs) == 8 ? 1 : -1];

#endif /* DDS_BTL_DEFERRED_UNIT_STATUS_ARGS_H */
