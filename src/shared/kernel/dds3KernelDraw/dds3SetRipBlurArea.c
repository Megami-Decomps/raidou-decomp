#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetRipBlurArea(s32 area, s32 posx, s32 posy)
{
    ripblurtarget.ripple.area = area;
    ripblurtarget.ripple.x = posx;
    ripblurtarget.ripple.y = posy;
}
