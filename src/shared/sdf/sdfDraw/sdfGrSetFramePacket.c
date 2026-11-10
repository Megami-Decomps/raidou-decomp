#include "shared/sdf/sdfDraw/decls.h"

void sdfGrSetFramePacket(sdfGrFramePacket_t *aFrame, sdfGrFrameSet_t *aFrameSet, u32 aFrameMask, s32 aZBuffMask, s32 aContext) {
    sdfGrSetRegsPacketHeader(&aFrame->packetHeader, 2);
    aFrame->context = aContext;
    aFrame->frameSet = aFrameSet;
    aFrame->frameMask = aFrameMask;
    aFrame->zbuffMask = aZBuffMask;
    aFrame->prepare = sdfGrFramePacketPredraw;
}
