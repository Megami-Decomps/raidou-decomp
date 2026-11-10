#include "shared/kernel/dds3KernelMain/decls.h"

void dds3SoftResetOff(void)
{
    dds3SoftResetDisableFlag = 1;
}
