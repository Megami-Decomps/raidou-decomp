#include "shared/kernel/dds3KernelCore/decls.h"

u32 dds3GetProcessStat(dds3ProcessID_t *id)
{
    u32 stat;

    if (!dds3ExistProcess(id)) {
        return 0;
    }
    stat = id->flag & 0xF;
    if (stat >= 4) {
        stat = 0;
    }
    return stat;
}
