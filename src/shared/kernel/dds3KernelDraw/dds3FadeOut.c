#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame)
{
    fadergba[0] = r;
    fadergba[1] = g;
    fadergba[2] = b;
    fadergba[3] = 0;
    if (frame == 0) {
        fadenow = 0;
        fademax = 0;
        fadergba[3] = -128;
        dds3DrawFlag &= ~3;
        return;
    }
    fademax = SDF_FRAMES(frame);
    fadenow = 0;
    dds3DrawFlag = (dds3DrawFlag & ~1) | 2;
}
