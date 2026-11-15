#ifndef CODE_R1_001_CF808_H
#define CODE_R1_001_CF808_H

#include "common.h"

/* The value moves from idle through file loading and then IOP commit. */
typedef s32 MnuTitleStreamLoadState;

#include "kernel.h"

void ProcessInit(void);
s32 mnuDestroyTitleEffectTask(dds3ProcessID_t *id);
s32 mnuIncrementTitleEffectFrameCounter(dds3ProcessID_t *id);
void sndInit(void);

#endif
