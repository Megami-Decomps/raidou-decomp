#include "shared/sdf/sdfDraw/decls.h"

void sdfGrFramePacketPredraw(sdfGrFramePacket_t *aPre, s32 aEvenOdd) {
    sdfGrFrameSet_t *view = aPre->frameSet;
    u64 frameRegister;
    u64 depthRegister;
    s16 widthPages;

    if (aPre->context == 0) {
        frameRegister = 0x4C;
        depthRegister = 0x4E;
    } else {
        frameRegister = 0x4D;
        depthRegister = 0x4F;
    }
    widthPages = (view->width + 63) >> 6;
    aPre->frame.data = ((u64)aPre->frameMask << 32) |
        (u64)(view->buffers[aEvenOdd]->addr >> 11) |
        ((s64)(view->bufferFormat << 24) | ((s64)widthPages << 16));
    aPre->frame.reg = frameRegister;
    aPre->zbuff.data = (s64)(view->auxiliaryFormat << 24) |
        (u64)(view->buffers[2]->addr >> 11);
    aPre->zbuff.reg = depthRegister;
}
