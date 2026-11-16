#include "common.h"
#include "dds3KernelDraw.h"

u32 dds3NullDrawProc(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "code_00123B18", dds3Leaf29);
