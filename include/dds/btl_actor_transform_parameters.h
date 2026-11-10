#ifndef DDS_BTL_ACTOR_TRANSFORM_PARAMETERS_H

#define DDS_BTL_ACTOR_TRANSFORM_PARAMETERS_H
#include "common.h"

/* Shared-or-indexed table selected by btlSelectSharedOrIndexedTransformParameters. */
typedef struct BtlActorTransformParameters {
    f32 position[3]; /* Applied to the actor body offset when the override is enabled. */
    f32 overrideEnabled;
    f32 reach;
    f32 height;
} BtlActorTransformParameters;

typedef char BtlActorTransformParameters_size_must_be_0x18[
    (sizeof(BtlActorTransformParameters) == 0x18) ? 1 : -1];

#endif /* DDS_BTL_ACTOR_TRANSFORM_PARAMETERS_H */
