#include "shared/sdf/sdfGsMemIO/decls.h"

void sdfLinkStoreImage(sdfDrawTag_t *aDrawTag, sdfGsMemSlice_t *aGsHandle, s32 aSrcX,
                   s32 aSrcY, s32 aWidth, s32 aHeight, saddr_t aBuffer, s32 aCallback,
                   s32 aCallbackParam, s32 (*aAllocator)(s32)) {
    s32 packetAddress;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packetAddress = aAllocator(0xf0);
    sdfSetStoreImage(EE_ADDR_AS(sdfStoreImagePacket_t *, packetAddress), aGsHandle,
        aSrcX, aSrcY, aWidth, aHeight, aBuffer, aCallback, aCallbackParam);
    sdfDrawTagLink2(aDrawTag, packetAddress, packetAddress + 0xc0);
}
