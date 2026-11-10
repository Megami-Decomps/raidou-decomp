#include "shared/kernel/dds3KernelDraw/decls.h"

/* Zero amplitude stops the quake; zero duration leaves it running indefinitely. */
void dds3SetQuake(s32 frame, s32 range)
{
    if (range == 0) {
        dds3QuakeFlag = 0;
        return;
    }
    dds3QuakeFlag = 1;
    if (frame == 0) {
        dds3QuakeFrame = -1;
    } else {
        dds3QuakeFrame = SDF_FRAMES(frame);
    }
    dds3QuakeYRange = range * 10;
    dds3QuakeXRange = dds3QuakeYRange * 4 / 10;
}
