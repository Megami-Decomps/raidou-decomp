#ifndef DDS_BTL_UNIT_DEFEAT_BY_DELTA_RESULT_H

#define DDS_BTL_UNIT_DEFEAT_BY_DELTA_RESULT_H
#include "common.h"

struct BtlUnit;

/* The result includes battle status and record exemptions as well as HP delta. */
typedef s32 BtlUnitDefeatByDeltaResult;

enum {
    BTL_UNIT_DEFEAT_BY_DELTA_RESULT_NOT_DEFEATED = 0,
    BTL_UNIT_DEFEAT_BY_DELTA_RESULT_DEFEATED = 1
};

BtlUnitDefeatByDeltaResult btlIsUnitDefeatTriggeredByValueDelta(
    struct BtlUnit *unit, s32 delta);

#endif /* DDS_BTL_UNIT_DEFEAT_BY_DELTA_RESULT_H */
