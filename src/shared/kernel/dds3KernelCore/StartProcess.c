#include "shared/kernel/dds3KernelCore/decls.h"

void StartProcess(dds3ProcessID_t *setp)
{
    DeleteProcessList(setp);
    setp->flag = (setp->flag & ~0xF) | 2;
    AddProcessList(setp);
    setp->starttime = 0;
    setp->runtime = 0;
}
