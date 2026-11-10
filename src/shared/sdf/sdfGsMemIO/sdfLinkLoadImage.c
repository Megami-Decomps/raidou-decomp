#include "shared/sdf/sdfGsMemIO/decls.h"

void sdfLinkLoadImage(sdfDrawTag_t *aDrawTag, sdfGsMemSlice_t *aGsHandle,
                   s32 aDstX, s32 aDstY, s32 aWidth, s32 aHeight,
                   saddr_t aBuffer, s32 (*aAllocator)(s32)) {
    s32 packetAddress;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packetAddress = aAllocator(SDF_DESCRIPTOR_PACKET_BYTES);
    sdfSetLoadImage(EE_ADDR_AS(sdfLoadImagePacket_t *, packetAddress), aGsHandle,
                  aDstX, aDstY, aWidth, aHeight, aBuffer);
    sdfDrawTagLink2(aDrawTag, packetAddress, packetAddress + SDF_DESCRIPTOR_PACKET_TAIL_OFFSET);
}
