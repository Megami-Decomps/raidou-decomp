#ifndef EFF_BATTLE_MISC_H
#define EFF_BATTLE_MISC_H

#include "common.h"

/* Packed position/basis query; the final word is a signed extent override. */
typedef struct EffectVectorRequest {
    u8 kind;
    u8 count;
    u8 directionMode;
    u8 pad_03;
    s32 extentOverride;
} EffectVectorRequest;

typedef char EffectVectorRequest_size_must_be_8[(sizeof(EffectVectorRequest) == 8) ? 1 : -1];

#endif
