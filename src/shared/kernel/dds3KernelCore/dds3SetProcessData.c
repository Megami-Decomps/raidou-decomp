#include "shared/kernel/dds3KernelCore/decls.h"

void dds3SetProcessData(dds3ProcessID_t *id, void *data)
{
    id->workdata = data;
}
