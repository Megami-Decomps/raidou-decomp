#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3RipBlurOff(s32 frame)
{
    ripbluratarget = 0;
    ripblurabase = ripblurtarget.ripple.rgba[3];
    ripblurnow = 0;
    ripblurmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x800000;
        dds3DrawFlag &= ~0x1000000;
        Eve_RippleBlurOFF();
    } else {
        dds3DrawFlag |= 0x800000;
    }
}
