#include "shared/kernel/dds3KernelCore/decls.h"

u32 dds3GetProcessStartTime(dds3ProcessID_t *id)
{
    return id->starttime;
}
