#include "shared/kernel/dds3KernelMain/decls.h"

void dds3Stop(u32 frame)
{
    dds3PauseTime = SDF_FRAMES(frame);
    dds3PauseSign = 1;
    dds3SlowSign = 0;
}
