#include "shared/kernel/dds3KernelCore/decls.h"

s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer)
{
    if (id == NULL || id == (dds3ProcessID_t *)-1) {
        return 0;
    }
    switch (id->flag & 0xF) {
    case 1:
    case 2:
        id->shutdowntimer = SDF_FRAMES_COUNTDOWN(shutdowntimer);
        EndProcess(id);
        return 1;
    case 3:
        id->shutdowntimer = SDF_FRAMES_COUNTDOWN(shutdowntimer);
        if (id->shutdowntimer == 0) {
            KillProcess(id);
            return 1;
        }
        break;
    case 0:
    default:
        return 0;
    }
    return 1;
}
