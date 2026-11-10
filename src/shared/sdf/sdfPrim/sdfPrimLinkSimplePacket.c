#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimLinkSimplePacket(s32 aTag, void (*aFunc)(s32), s32 aPacketSize, s32 (*aAllocator)(s32)) {
    s32 packetAddress;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packetAddress = aAllocator(aPacketSize);
    aFunc(packetAddress);
    sdfDrawTagLink(EE_ADDR_PTR(aTag), packetAddress);
}
