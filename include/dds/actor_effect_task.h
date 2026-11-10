#ifndef DDS_ACTOR_EFFECT_TASK_H

#define DDS_ACTOR_EFFECT_TASK_H
#include "btl_sound.h"

#include "common.h"

typedef struct ActorEffectTaskArgs {
    SoundResourceNode *source; /* Held through the task's system-effect reference. */
    effBTLCutHandle_t *effect; /* Created and released by this task. */
    struct BtlUnit *owner; /* Borrowed actor, pinned by its effect-link reference. */
    u32 duration;
    s32 counter;
} ActorEffectTaskArgs;

typedef char ActorEffectTaskArgs_size_must_be_0x14[
    (sizeof(ActorEffectTaskArgs) == 0x14) ? 1 : -1];

#endif
