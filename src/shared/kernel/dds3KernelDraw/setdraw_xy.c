#include "shared/kernel/dds3KernelDraw/decls.h"

void setdraw_xy(s32 *xy)
{
    xy[2] = 512;
    xy[3] = 448;
    xy[1] = 0;
    xy[0] = 0;
}
