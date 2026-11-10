#include "shared/kernel/dds3KernelCore/decls.h"

void dds3SetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type)
{
    ChangeProcessFlag(1, target, flag, type);
}
