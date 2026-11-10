#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3Filter2Off(s32 frame)
{
    filter2atarget = 0;
    filter2abase = filter2target.rgba[3];
    filter2now = 0;
    filter2max = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x80000;
        dds3DrawFlag &= ~0x100000;
        Eve_FilterOFF();
    } else {
        dds3DrawFlag |= 0x80000;
    }
}
