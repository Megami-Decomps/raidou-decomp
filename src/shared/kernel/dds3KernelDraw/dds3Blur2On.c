#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3Blur2On(s32 frame)
{
    blur2abase = 0;
    blur2now = 0;
    blur2atarget = blur2target.rgba[3];
    blur2max = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x20000;
        setdraw_xy(blur2target.rect);
        Eve_BlurSetParam(&blur2target);
        Eve_BlurON();
    } else {
        dds3DrawFlag |= 0x20000;
    }
}
