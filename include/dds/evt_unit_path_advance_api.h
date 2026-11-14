#ifndef DDS_EVT_UNIT_PATH_ADVANCE_API_H

#define DDS_EVT_UNIT_PATH_ADVANCE_API_H
#include "common.h"

#include "evt_unit.h"

/* Whether evtGetNextMovePos advanced a path-backed unit position. */
typedef s32 EvtUnitPathAdvanceResult;

enum {
    EVT_UNIT_PATH_NOT_ADVANCED = 0,
    EVT_UNIT_PATH_ADVANCED = 1
};

#endif /* DDS_EVT_UNIT_PATH_ADVANCE_API_H */
