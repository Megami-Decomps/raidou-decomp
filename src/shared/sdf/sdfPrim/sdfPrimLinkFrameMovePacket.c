#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimLinkFrameMovePacket(sdfDrawTag_t *aDrawTag, sdfPreDrawTag_t *aPreDrawTag,
                   sdfGsMemSlice_t *aBuffHandle, s32 aDstX, s32 aDstY,
                   s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight,
                   s32 aFrameSw, s32 (*aAllocator)(s32)) {
    sdfPrimFrameMovePacket_t *packet;
    sdfPrimBitBltPacket_t *drawPacket;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimFrameMovePacket_t *, aAllocator(0x70));
    packet->worker.option0 = aFrameSw;
    packet->worker.predrawFunc = sdfPrimFrameMovePredraw;
    drawPacket = &packet->packet;

    sdfPrimSetBitBltPacket(
        drawPacket, aBuffHandle->addr, aBuffHandle->w,
        aBuffHandle->pixelFormat, aDstX, aDstY, sdfGrFrameSet.buffers[0]->addr,
        sdfGrFrameSet.width, sdfGrFrameSet.bufferFormat, aSrcX, aSrcY, aWidth,
        aHeight, 2);
    sdfPreDrawTagLink(aPreDrawTag, &packet->worker);
    sdfDrawTagLink(aDrawTag, EE_ADDR_S(drawPacket));
}
