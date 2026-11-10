#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3Filter2Swap(s32 frame)
{
    memcpy(&dds3Filter2Buf, Eve_FilterGetParams(), 0x18);
    filter2swapnow = 0;
    filter2swapmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x100000;
        setdraw_xy(filter2target.rect);
        Eve_FilterSetParam(&filter2target);
    } else {
        dds3DrawFlag |= 0x100000;
    }
}
