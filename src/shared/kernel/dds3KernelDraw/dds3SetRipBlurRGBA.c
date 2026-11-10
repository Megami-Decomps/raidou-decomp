#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetRipBlurRGBA(u32 rgba)
{
    ripblurtarget.ripple.color = rgba;
}
