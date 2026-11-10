#include "shared/sdf/sdfGsMemIO/decls.h"

void sdfLinkStoreFrameImage(sdfDrawTag_t *aDrawTag, sdfPreDrawTag_t *aPreDrawTag, s32 aSrcX, s32 aSrcY,
                   s32 aWidth, s32 aHeight, s32 aBuffer, s32 aCallback, s32 aCallbackParam,
                   s32 (*aAllocator)(s32)) {
    sdfStoreFrameImagePacket_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfStoreFrameImagePacket_t *, aAllocator(SDF_PATCHABLE_PACKET_BYTES));
    packet->worker.predrawFunc = sdfLinkStoreFrameImagePredraw;
    sdfSetStoreImage(&packet->packet,
        sdfGrFrameSet.buffers[0], aSrcX, aSrcY, aWidth, aHeight,
        aBuffer, aCallback, aCallbackParam);
    sdfPreDrawTagLink(aPreDrawTag, &packet->worker);
    sdfDrawTagLink2(aDrawTag, EE_ADDR(&packet->packet),
        EE_ADDR(packet) + SDF_PATCHABLE_PACKET_TAIL_OFFSET);
}
