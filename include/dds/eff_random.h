#ifndef DDS_EFF_RANDOM_H

#define DDS_EFF_RANDOM_H
#include "common.h"

/* Four ordered 32-bit words in the shared effect random-number state. */
typedef struct EffRandState {
    u32 words[4];
} EffRandState;

typedef char EffRandState_size_must_be_0x10[
    (sizeof(EffRandState) == 0x10) ? 1 : -1];

#endif /* DDS_EFF_RANDOM_H */
