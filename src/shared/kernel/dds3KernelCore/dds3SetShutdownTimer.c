#include "shared/kernel/dds3KernelCore/decls.h"

void dds3SetShutdownTimer(dds3ProcessID_t *id, s32 newtimer)
{
    u32 stat = id->flag & 0xF;

    if (stat != 0) {
        if (stat < 4) {
            id->shutdowntimer = SDF_FRAMES_COUNTDOWN(newtimer);
        }
    }
}
