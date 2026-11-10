#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetMultiBlurArea(s32 area, s32 posx, s32 posy)
{
    mulblurtarget.extent = area;
    mulblurtarget.x = posx;
    mulblurtarget.y = posy;
}
