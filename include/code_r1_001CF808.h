#ifndef CODE_R1_001_CF808_H
#define CODE_R1_001_CF808_H

#include "common.h"

/* The value moves from idle through file loading and then IOP commit. */
typedef s32 MnuTitleStreamLoadState;

#include "kernel.h"

void ProcessInit(void);
void mnuDestroyTitleEffectTask(dds3ProcessID_t *id);
dds3ProcessFunc_t mnuIncrementTitleEffectFrameCounter(dds3ProcessID_t *id);
void sndInit(void);

#endif
