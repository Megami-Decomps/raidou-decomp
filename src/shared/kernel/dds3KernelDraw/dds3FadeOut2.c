#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3FadeOut2(s32 frame)
{
    fadergba[3] = 0;
    if (frame == 0) {
        fadenow = 0;
        fadergba[3] = -128;
        dds3DrawFlag &= ~3;
        fademax = 0;
    } else {
        fademax = SDF_FRAMES(frame);
        fadenow = 0;
        dds3DrawFlag = (dds3DrawFlag & ~1) | 2;
    }
}
