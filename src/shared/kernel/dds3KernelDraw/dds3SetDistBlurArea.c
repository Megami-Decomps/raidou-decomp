#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetDistBlurArea(s32 area, s32 posx, s32 posy)
{
    distblurtarget.spread = area;
    distblurtarget.x = posx;
    distblurtarget.y = posy;
}
