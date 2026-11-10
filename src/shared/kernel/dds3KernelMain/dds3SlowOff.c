#include "shared/kernel/dds3KernelMain/decls.h"

void dds3SlowOff(void)
{
    dds3PauseSign = 0;
    dds3SlowTime = 4;
    dds3PauseTime = 0;
    dds3SlowSign = 0;
}
