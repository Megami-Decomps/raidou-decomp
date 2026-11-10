#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetBlur2Area(s32 posx, s32 posy)
{
    blur2target.area[0] = posx;
    blur2target.area[1] = posy;
}
