#ifndef DDS_BTL_COMMAND_EFFECT_H

#define DDS_BTL_COMMAND_EFFECT_H
#include "common.h"

#include "btl_command_id.h"

/* Positive IDs select a command effect; nonpositive values suppress it. */
typedef s16 BtlCommandEffectId;

typedef struct BtlCommandEffect {
    BtlCommandId command;
    BtlCommandEffectId effect;
    u8 adjustSide;
    u8 pad07;
} BtlCommandEffect;

typedef char BtlCommandEffect_size_must_be_0x08[
    (sizeof(BtlCommandEffect) == 0x08) ? 1 : -1];

#endif
