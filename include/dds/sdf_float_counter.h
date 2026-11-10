#ifndef DDS_SDF_FLOAT_COUNTER_H

#define DDS_SDF_FLOAT_COUNTER_H
#include "common.h"

/* The path and movie counters share this complete four-word state. */
typedef struct SdfFloatCounter {
    s32 direction; /* Zero counts up, one counts down. */
    u32 flags;
    f32 duration;
    f32 time;
} SdfFloatCounter;

typedef char SdfFloatCounter_size_must_be_0x10[
    sizeof(SdfFloatCounter) == 0x10 ? 1 : -1];

#define SDF_FLOAT_COUNTER_STEP_FINISHED 0

#define SDF_FLOAT_COUNTER_STEP_CONTINUES 1

/* Advance one counter step unless frozen. FINISHED means a non-looping
 * counter was already at its endpoint; a frozen counter still CONTINUES. */
s32 dds3PathBasicControlStep(SdfFloatCounter *counter);

#endif /* DDS_SDF_FLOAT_COUNTER_H */
