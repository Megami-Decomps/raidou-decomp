#include "shared/kernel/dds3KernelCore/decls.h"

u32 dds3GetProcessRunTime(dds3ProcessID_t *id)
{
    return id->runtime;
}
