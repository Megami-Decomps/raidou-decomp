#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetMFilterArea(s32 area, s32 posx, s32 posy)
{
    mfiltertarget.area[0] = area;
    mfiltertarget.area[1] = posx;
    mfiltertarget.area[2] = posy;
}
