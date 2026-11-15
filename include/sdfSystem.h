#ifndef SDFSYSTEM_H
#define SDFSYSTEM_H

#include "common.h"

s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);
void FatalError(const char *format, ...);
void *sdfAllocAuto(s32 size);
s32 sdfCreateSema(s32 initCount, s32 maxCount, u32 option);
s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority);
void sdfDelayThread(u32 delay);
s32 sdfFreeAuto(void *allocation);
s32 sdfFreeAutoDelayed(void *allocation);
s32 sdfGetRunTimeCount(s32 aBaseTime);
s32 sdfGetRunTimeCountBase(void);
void sdfWaitVblank(s32 count);

#endif
