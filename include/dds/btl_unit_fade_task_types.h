#ifndef DDS_BTL_UNIT_FADE_TASK_TYPES_H

#define DDS_BTL_UNIT_FADE_TASK_TYPES_H
#include "common.h"

/* Mode 0 uses event-unit color callbacks; mode 1 applies an eight-frame alpha fade. */
typedef enum BtlUnitExitFadeMode {
    BTL_UNIT_EXIT_FADE_MODE_EVENT_COLOR = 0,
    BTL_UNIT_EXIT_FADE_MODE_FRAME_ALPHA = 1,
} BtlUnitExitFadeMode;

#endif /* DDS_BTL_UNIT_FADE_TASK_TYPES_H */
