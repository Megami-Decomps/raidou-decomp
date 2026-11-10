#include "shared/kernel/dds3KernelMain/decls.h"

void dds3PauseOn(void)
{
    dds3PauseTime = 0;
    dds3PauseSign = 1;
    dds3SlowSign = 0;
}
