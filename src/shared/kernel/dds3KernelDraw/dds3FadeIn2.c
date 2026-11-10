#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3FadeIn2(s32 frame)
{
    fadergba[3] = -128;
    if (frame == 0) {
        fadergba[3] = 0;
        dds3DrawFlag &= ~3;
        fadenow = 0;
        fademax = 0;
    } else {
        fademax = SDF_FRAMES(frame);
        fadenow = SDF_FRAMES(frame);
        dds3DrawFlag = (dds3DrawFlag | 1) & ~2;
    }
}
