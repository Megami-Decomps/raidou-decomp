#include "common.h"

#include "shared/kernel/dds3KernelCore/DeleteProcessList.c"

#include "shared/kernel/dds3KernelCore/AddProcessList.c"

#include "shared/kernel/dds3KernelCore/StartProcess.c"

#include "shared/kernel/dds3KernelCore/StartingProcess.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", RunProcess);

#include "shared/kernel/dds3KernelCore/RunningProcess.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100E68);

#include "shared/kernel/dds3KernelCore/EndProcess.c"

#include "shared/kernel/dds3KernelCore/dds3EndingProcess.c"

#include "shared/kernel/dds3KernelCore/ChangeProcessFlagChild.c"

#include "shared/kernel/dds3KernelCore/ChangeProcessFlag.c"

#include "shared/kernel/dds3KernelCore/dds3GetProcessCount.c"

#include "shared/kernel/dds3KernelCore/dds3SearchProcessNumber.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001012B0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", dds3PrintProcessList);

#include "shared/kernel/dds3KernelCore/DebugProcessTree.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101440);

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

void func_00101A78(void) {
}

#include "shared/kernel/dds3KernelCore/dds3LinkProcess.c"

#include "shared/kernel/dds3KernelCore/dds3UnlinkProcess.c"

#include "shared/kernel/dds3KernelCore/dds3SetProcessFlag.c"

#include "shared/kernel/dds3KernelCore/dds3ResetProcessFlag.c"
INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE248);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE24C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE250);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE254);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE258);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE25C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE260);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE264);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE268);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE26C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE270);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_004DE278);

