#include "common.h"

#include "shared/kernel/dds3KernelCore/DeleteProcessList.c"

#include "shared/kernel/dds3KernelCore/AddProcessList.c"

#include "shared/kernel/dds3KernelCore/StartProcess.c"

#include "shared/kernel/dds3KernelCore/StartingProcess.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", RunProcess);

#include "shared/kernel/dds3KernelCore/RunningProcess.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100E38);

#include "shared/kernel/dds3KernelCore/EndProcess.c"

#include "shared/kernel/dds3KernelCore/dds3EndingProcess.c"

#include "shared/kernel/dds3KernelCore/ChangeProcessFlagChild.c"

#include "shared/kernel/dds3KernelCore/ChangeProcessFlag.c"

#include "shared/kernel/dds3KernelCore/dds3GetProcessCount.c"

#include "shared/kernel/dds3KernelCore/dds3SearchProcessNumber.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101280);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", dds3PrintProcessList);

#include "shared/kernel/dds3KernelCore/DebugProcessTree.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101410);

#include "shared/kernel/dds3KernelCore/dds3ProcessMain.c"

#include "shared/kernel/dds3KernelCore/dds3StartProcess.c"

#include "shared/kernel/dds3KernelCore/dds3KillProcessName.c"

#include "shared/kernel/dds3KernelCore/dds3KillProcess.c"

#include "shared/kernel/dds3KernelCore/dds3EndProcess.c"

#include "shared/kernel/dds3KernelCore/dds3SetShutdownTimer.c"

#include "shared/kernel/dds3KernelCore/dds3GetProcessStat.c"

#include "shared/kernel/dds3KernelCore/dds3SearchProcessName.c"

#include "shared/kernel/dds3KernelCore/dds3SearchProcessPriority.c"

#include "shared/kernel/dds3KernelCore/dds3ExistProcess.c"

#include "shared/kernel/dds3KernelCore/dds3GetProcessStartTime.c"

#include "shared/kernel/dds3KernelCore/dds3GetProcessRunTime.c"

#include "shared/kernel/dds3KernelCore/dds3SetProcessData.c"

#include "shared/kernel/dds3KernelCore/dds3GetProcessData.c"

void func_00101A48(void) {
}

#include "shared/kernel/dds3KernelCore/dds3LinkProcess.c"

#include "shared/kernel/dds3KernelCore/dds3UnlinkProcess.c"

#include "shared/kernel/dds3KernelCore/dds3SetProcessFlag.c"

#include "shared/kernel/dds3KernelCore/dds3ResetProcessFlag.c"
INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B150);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B154);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B158);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B15C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B160);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B164);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B168);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B16C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B170);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B174);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B178);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0046B180);

