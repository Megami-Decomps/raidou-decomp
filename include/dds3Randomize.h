#ifndef DDS3RANDOMIZE_H
#define DDS3RANDOMIZE_H

#include "common.h"
#include "sdfRand.h"

/* Unsized so ee-gcc addresses it absolutely, not through $gp. */
extern sdfRandCB_t dds3RandCB[];

u32 dds3GetRandA(void *state);
f32 dds3GetRandFloatA(void *state);
u32 dds3GetRandIntA(void *state, u32 limit);

#endif
