#ifndef DDS_EVT_UNIT_MOTION_SETUP_H

#define DDS_EVT_UNIT_MOTION_SETUP_H
#include "common.h"

#include "evt_unit.h"

/* Typed view of the three stored motion setup halfwords at evtUnitHandle_t +0xC4. */
typedef struct EvtUnitMotionSetup {
    s16 motionId;
    s16 startFrame;
    s16 endFrame;
} EvtUnitMotionSetup;

typedef char EvtUnitMotionSetupSizeCheck[(sizeof(EvtUnitMotionSetup) == 6) ? 1 : -1];

/* Overlay the six-byte record on evtUnitHandle_t bytes +0xC4..+0xC9. */

#define EVT_UNIT_MOTION_SETUP(unit) \
    ((EvtUnitMotionSetup *)((u8 *)(unit) + 0xC4))

#endif /* DDS_EVT_UNIT_MOTION_SETUP_H */
