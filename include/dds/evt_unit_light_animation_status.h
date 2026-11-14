#ifndef DDS_EVT_UNIT_LIGHT_ANIMATION_STATUS_H

#define DDS_EVT_UNIT_LIGHT_ANIMATION_STATUS_H
#include "common.h"

#include "evt_unit.h"

typedef u8 EvtUnitLightAnimationStatus;

enum {
    EVT_UNIT_LIGHT_ANIMATION_INACTIVE = 0,
    EVT_UNIT_LIGHT_ANIMATION_ACTIVE = 1
};

#endif /* DDS_EVT_UNIT_LIGHT_ANIMATION_STATUS_H */
