#ifndef DDS_EFF_MATH_H

#define DDS_EFF_MATH_H
#include "common.h"

struct EffCubicBezierArray;

/* One cubic segment: four XYZ controls, evaluation position and frame step.
 * The allocation header follows the slots; it is a distinct pointer. */
typedef struct EffCubicBezierSlot {
    f32 controlPoints[4][3];
    f32 t;
    f32 step;
} EffCubicBezierSlot;

typedef s32 EffCubicBezierSlotIndex;

typedef s32 EffCubicBezierStepResult;

enum {
    EFF_CUBIC_BEZIER_STEP_CLAMPED_TO_END = 0,
    EFF_CUBIC_BEZIER_STEP_WITHIN_RANGE = 1
};

#endif
