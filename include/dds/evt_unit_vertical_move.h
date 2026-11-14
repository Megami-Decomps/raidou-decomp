#ifndef DDS_EVT_UNIT_VERTICAL_MOVE_H

#define DDS_EVT_UNIT_VERTICAL_MOVE_H
#include "evt_unit.h"

/* Simulate the motion and return the step count stored in unit->stepCount. */
s32 evtUnitPrepareVerticalMoveSteps(evtUnitHandle_t *unit);

#endif /* DDS_EVT_UNIT_VERTICAL_MOVE_H */
