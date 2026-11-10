#ifndef DDS_EVT_UNIT_MOTION_API_H

#define DDS_EVT_UNIT_MOTION_API_H
#include "evt_unit.h"

/* Preserve the signed script-selector width at the evtSetUnitMotion boundary. */
typedef s32 EvtUnitMotionMode;

enum {
    EVT_UNIT_MOTION_MODE_RESTART_PENDING_MOTION = 0,
    EVT_UNIT_MOTION_MODE_LOOP = 1,
    EVT_UNIT_MOTION_MODE_SINGLE = 2,
    EVT_UNIT_MOTION_MODE_WOBBLE = 3
};

/* Values outside the named cases add no mode flag in evtSetUnitMotion. */

#endif /* DDS_EVT_UNIT_MOTION_API_H */
