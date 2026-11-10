#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3MFilterSwap(s32 frame)
{
    memcpy(&dds3MFilterBuf, Eve_PartialMultiFilterGetParams(), 0x24);
    mfilterswapnow = 0;
    mfilterswapmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x400000;
        Eve_PartialMultiFilterSetParam(&mfiltertarget);
    } else {
        dds3DrawFlag |= 0x400000;
    }
}
