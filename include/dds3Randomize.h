#ifndef DDS3RANDOMIZE_H
#define DDS3RANDOMIZE_H

#include "common.h"

u32 dds3GetRandA(void *state);
f32 dds3GetRandFloatA(void *state);
u32 dds3GetRandIntA(void *state, u32 limit);

#endif
