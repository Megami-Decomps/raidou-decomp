#include "shared/kernel/dds3KernelCore/decls.h"

void EndProcess(dds3ProcessID_t *setp)
{
    u32 stat = setp->flag & 0xF;

    if (stat < 3) {
        if (stat == 0) {
            return;
        }

        DeleteProcessList(setp);
        setp->flag = (setp->flag & ~0xF) | 3;
        AddProcessList(setp);
        if (setp->shutdowntimer == 0) {
            KillProcess(setp);
        }
    }
}
