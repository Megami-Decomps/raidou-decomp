#include "shared/kernel/dds3KernelCore/decls.h"

void *dds3GetProcessData(dds3ProcessID_t *id)
{
    return id->workdata;
}
