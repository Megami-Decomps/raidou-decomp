#ifndef DDS_BTL_SELECTED_UNIT_EFFECT_TASK_ARGS_H

#define DDS_BTL_SELECTED_UNIT_EFFECT_TASK_ARGS_H
#include "common.h"

struct BtlUnit;

struct SoundMixer;

struct effBTLCutHandle_t;

/* Selected-unit effect updater state, allocated and cleared as one task block. */
typedef struct BtlSelectedUnitEffectTaskArgs {
    struct BtlUnit *unit;         /* 0x00: affected unit, also used by the finish hook */
    struct SoundMixer *mixer;     /* 0x04: cloned sound mixer, released by the finish hook */
    struct effBTLCutHandle_t *effect;  /* 0x08: selected-unit effect voice */
    s32 duration;                /* 0x0C: update lifetime in frames */
    s32 counter;                 /* 0x10: elapsed update count */
} BtlSelectedUnitEffectTaskArgs;

/* btlRunTask keeps this callback registered on zero and enters its finish path
 * on nonzero. This effect yields while its voice is updating. */
typedef s32 BtlSelectedUnitEffectTaskResult;

enum {
    BTL_SELECTED_UNIT_EFFECT_TASK_CONTINUE = 0,
    BTL_SELECTED_UNIT_EFFECT_TASK_COMPLETE = 1,
};

BtlSelectedUnitEffectTaskResult btlUpdateSelectedUnitEffect(
    BtlSelectedUnitEffectTaskArgs *arguments);

typedef char BtlSelectedUnitEffectTaskArgs_size_check[
    (sizeof(BtlSelectedUnitEffectTaskArgs) == 0x14) ? 1 : -1];

#endif /* DDS_BTL_SELECTED_UNIT_EFFECT_TASK_ARGS_H */
