#include "shared/sdf/sdf3D/decls.h"

void sdf3DLinkInitPacket(sdfDrawTag_t *aTag, DmaPacketHeader *aPacket) {
    aPacket->address = EE_PHYSICAL(EE_ADDR(vu1MicroPacket));
    aPacket->quadwords = (vu1MicroPacketEnd - vu1MicroPacket) >> SDF_DMA_QWORD_SHIFT;
    aPacket->tag = 0;
    aPacket->command = 0;
    aPacket->unknown10 = 0;
    aPacket->unknown18 = 0;
    aPacket->unknown1C = 0;
    sdfDrawTagLinkRef(aTag, EE_ADDR(aPacket));
}
