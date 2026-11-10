#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3FadeSet(s32 r, s32 g, s32 b, s32 a)
{
    dds3DrawFlag &= ~3;
    fadergba[0] = r;
    fadergba[1] = g;
    fadergba[2] = b;
    fadergba[3] = a;
}
