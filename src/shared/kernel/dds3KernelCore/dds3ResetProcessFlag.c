#include "shared/kernel/dds3KernelCore/decls.h"

void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type)
{
    ChangeProcessFlag(0, target, flag, type);
}
