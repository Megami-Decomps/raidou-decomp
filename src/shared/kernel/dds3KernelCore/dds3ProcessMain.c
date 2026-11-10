#include "shared/kernel/dds3KernelCore/decls.h"

s32 dds3ProcessMain(void)
{
    StartingProcess();
    RunningProcess();
    dds3EndingProcess();
    return 1;
}
