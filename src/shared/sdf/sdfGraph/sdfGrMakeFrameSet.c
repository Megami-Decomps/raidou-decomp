#include "shared/sdf/sdfGraph/decls.h"

void sdfGrMakeFrameSet(sdfGrFrameSet_t *aFrame)
{
    s16 width;
    s16 height;
    u8 format;

    sdfGrFreeFrameSet(aFrame);
    width = aFrame->w;
    height = aFrame->buffH;
    aFrame->zbuffHandle = sdfGsAllocFrameBuff(width, height, aFrame->zbuffFormat);
    format = aFrame->pixelFormat;
    aFrame->frameHandle[0] = sdfGsAllocFrameBuff(width, height, format);
    aFrame->frameHandle[1] = sdfGsAllocFrameBuff(width, height, format);
}
