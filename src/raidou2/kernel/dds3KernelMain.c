#include "common.h"
#include "kernel.h"
#include "sdfFrame.h"
#include "dds3KernelMain.h"

extern u32 dds3RunCount;
extern u8 dds3SoftResetDisableFlag;
extern u32 dds3PauseSign;
extern u32 dds3PauseTime;
extern u32 dds3SlowSign;
extern u32 dds3SlowTime;
extern u32 dds3SlowRunCount;
extern u32 dds3SlowEndCount;
extern u16 dds3VblanksPerFrame;
extern s16 dds3FrameRunTime;
extern s16 dds3LastDrawFrameCount;
extern s8 dds3FileCmdLoPrio;
extern char dds3FirstProcessName[];
extern char dds3DrawFirstProcessName[];

#include "sdk_libc.h"

void dds3PauseOn(void);
void dds3PauseOff(void);
void dds3Stop(u32 frame);
void dds3SlowOn(u32 aSlowTime, u32 slowcnt);
void dds3SlowOff(void);
void dds3ScreenCaptureOn(void);
void dds3ScreenCaptureOff(void);
void dds3SoftResetOn(void);
void dds3SoftResetOff(void);
extern u32 dds3DebugSign;
extern char dds3FirstProcessName[8];
extern char dds3DrawFirstProcessName[8];
extern u8 dds3DrawProcessFlag;

void dds3PauseOn(void)
{
    dds3PauseTime = 0;
    dds3PauseSign = 1;
    dds3SlowSign = 0;
}

void dds3PauseOff(void)
{
    dds3PauseSign = 0;
    dds3PauseTime = 0;
    dds3SlowSign = 0;
}

void dds3Stop(u32 frame)
{
    dds3PauseTime = SDF_FRAMES(frame);
    dds3PauseSign = 1;
    dds3SlowSign = 0;
}

void dds3SlowOn(u32 aSlowTime, u32 slowcnt)
{
    u32 runCount = dds3RunCount;

    dds3SlowSign = 1;
    dds3SlowTime = aSlowTime;
    dds3SlowRunCount = runCount;
    dds3SlowEndCount = slowcnt;
    dds3PauseSign = 0;
    dds3PauseTime = 0;
}

void dds3SlowOff(void)
{
    dds3PauseSign = 0;
    dds3SlowTime = 4;
    dds3PauseTime = 0;
    dds3SlowSign = 0;
}

void dds3ScreenCaptureOn(void) {
}

void dds3ScreenCaptureOff(void) {
}

void dds3SoftResetOn(void)
{
    dds3SoftResetDisableFlag = 0;
}

void dds3SoftResetOff(void)
{
    dds3SoftResetDisableFlag = 1;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3CallEventSub);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3KernelMain);
u32 dds3PauseSign = 0;

u32 dds3PauseTime = 0;

u32 dds3SlowSign = 0;

u32 dds3SlowTime = 4;

u32 dds3SlowRunCount = 0;

u32 dds3SlowEndCount = 0;

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DebugSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FrameRunTime);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3LastDrawFrameCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3VblanksPerFrame);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3CallEventSubData);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FileCmdLoPrio);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FirstProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DrawFirstProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DrawProcessFlag);

