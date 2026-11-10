#include "shared/sdf/sdf2D/decls.h"

sdfGrPacketHeader_t *sdf2DAllocSprite(s32 aNumOfSpr) {
    sdfGrPacketHeader_t *packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, aNumOfSpr)));
    sdf2DMakePrimPacketHeader(packet, 0x156, 5, 0x53531, aNumOfSpr);
    return packet;
}
