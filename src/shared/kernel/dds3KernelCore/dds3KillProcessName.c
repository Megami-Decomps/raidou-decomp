#include "shared/kernel/dds3KernelCore/decls.h"

s32 dds3KillProcessName(char *name, s32 shutdowntimer)
{
    dds3ProcessID_t *id = dds3SearchProcessName(name);

    if (id == NULL) {
        return 0;
    }
    return dds3KillProcess(id, shutdowntimer);
}
