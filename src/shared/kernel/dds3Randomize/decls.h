/* Declarations of smt3-decomp src/smt3/kernel/dds3Randomize.c, for its bodies in this directory. */
#ifndef SHARED_KERNEL_DDS3RANDOMIZE_H
#define SHARED_KERNEL_DDS3RANDOMIZE_H

#include "common.h"

#include "kernel.h"

#include "libsdf.h"

extern u8 dds3RandCB[];

extern void sdfRandomize(void *aCB, u32 aSeed);

void dds3InitRandCB(void);

#endif
