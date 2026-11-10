#include "shared/kernel/dds3KernelCore/decls.h"

void dds3EndProcess(dds3ProcessID_t *id)
{
    if ((id->flag & 0xF) == 2) {
        DeleteProcessList(id);
        id->flag = (id->flag & ~0xF) | 3;
        AddProcessList(id);
    }
}
