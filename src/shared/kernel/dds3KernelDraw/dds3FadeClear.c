#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3FadeClear(void)
{
    dds3DrawFlag &= ~3;
    fadergba[0] = 0;
    fadergba[1] = 0;
    fadergba[2] = 0;
    fadergba[3] = 0;
}
