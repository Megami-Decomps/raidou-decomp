#include "shared/kernel/dds3KernelMain/decls.h"

void dds3SoftResetOn(void)
{
    dds3SoftResetDisableFlag = 0;
}
