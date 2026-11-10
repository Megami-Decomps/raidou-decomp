#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3DistBlurOn(s32 frame)
{
    distblurabase = 0;
    distblurnow = 0;
    distbluratarget = distblurtarget.rgba[3];
    distblurmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x2000;
        Eve_DistortionBlurSetParam(&distblurtarget);
        Eve_DistortionBlurON();
    } else {
        dds3DrawFlag |= 0x2000;
        Eve_DistortionBlurSetParam(&distblurtarget);
        Eve_DistortionBlurOFF();
    }
}
