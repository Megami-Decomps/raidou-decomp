#ifndef EFF_BATTLE_MISC_H
#define EFF_BATTLE_MISC_H

#include "common.h"

/* Packed position/basis query; the final word is a signed extent override. */
typedef struct effVectorRequest_t {
    u8 kind;
    u8 count;
    u8 directionMode;
    u8 pad_03;
    s32 extentOverride;
} effVectorRequest_t;

typedef char effVectorRequest_size_must_be_8[(sizeof(effVectorRequest_t) == 8) ? 1 : -1];

#endif
