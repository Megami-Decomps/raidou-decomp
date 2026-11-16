#ifndef DDS3KERNELMAIN_H
#define DDS3KERNELMAIN_H

#include "common.h"

void dds3SoftResetOff(void);
void dds3SoftResetOn(void);

void dds3NewGameStart(void);

s32 dds3BgSetScrollSub(void);
s32 dds3LinkBoxZSub(void);
u32 dds3GetFrameID(void);
u32 dds3GetKernelCount(void);

extern s32 dds3KernelDebugSign;

#ifdef VERSION_RAIDOU2
void dds3CallEventSub(u8 enable);
#endif

#endif
