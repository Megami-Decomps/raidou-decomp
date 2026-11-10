#include "shared/kernel/dds3KernelDraw/decls.h"

s32 dds3FadeCheck(void)
{
    return (dds3DrawFlag & 3) != 0;
}
