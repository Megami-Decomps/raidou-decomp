#ifndef DDS_EVT_UNIT_WAIT_API_H

#define DDS_EVT_UNIT_WAIT_API_H
#include "common.h"

#include "evt_unit.h"

/* Whether a unit's motion has satisfied an event wait. */
typedef s32 EvtUnitWaitResult;

enum {
    EVT_UNIT_WAIT_IN_PROGRESS = 0,
    EVT_UNIT_WAIT_COMPLETE = 1
};

#endif /* DDS_EVT_UNIT_WAIT_API_H */
