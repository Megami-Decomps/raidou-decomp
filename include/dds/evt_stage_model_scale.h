#ifndef DDS_EVT_STAGE_MODEL_SCALE_H

#define DDS_EVT_STAGE_MODEL_SCALE_H
#include "common.h"

/* Confirmed prefix view of a selected stage-test model table entry. */
typedef struct EvtStageModelScalePrefix {
    u8 opaque00[0x10];
    f32 scaleFactor;
} EvtStageModelScalePrefix;

#endif
