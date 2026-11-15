#include "common.h"
#include "dds3KernelMain.h"

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", KernelDebug);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3BgSetScrollSub);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3LinkBoxZSub);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3Leaf20);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3FirstInit);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3NewGameStart);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3Leaf21);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3GetKernelCount);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3GetFrameID);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3PauseOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3PauseOff);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3Stop);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3SlowOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3SlowOff);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3ScreenCaptureOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3ScreenCaptureOff);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3SoftResetOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3SoftResetOff);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3KernelMain);
INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FrameCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3RunCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SoftResetFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SoftResetDisableFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3KernelMainData);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3KernelMainData2);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", PauseSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3Data25);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowOnData);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowOnData2);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DebugSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FrameRunTime);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3LastDrawFrameCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3VblanksPerFrame);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3KernelDebugSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FileCmdLoPrio);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FirstProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DrawFirstProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DrawProcessFlag);

