#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame)
{
    fadergba[0] = r;
    fadergba[1] = g;
    fadergba[2] = b;
    fadergba[3] = -128;
    if (frame == 0) {
        fadergba[3] = 0;
        fadenow = 0;
        fademax = 0;
        dds3FadeClear();
        return;
    }
    fademax = SDF_FRAMES(frame);
    fadenow = SDF_FRAMES(frame);
    dds3DrawFlag = (dds3DrawFlag | 1) & ~2;
}
