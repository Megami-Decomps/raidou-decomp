#include "shared/sdf/sdf3D/decls.h"

void sdf3DMakeDirectRefPacket(DmaPacketHeader *aPacket, addr_t aData, s32 aLeng) {
    s32 quadwordCount = (aLeng + 15) >> SDF_DMA_QWORD_SHIFT;
    aPacket->quadwords = quadwordCount;
    aPacket->address = EE_PHYSICAL(aData);
    aPacket->tag = SDF_VIF_FLUSHA_COMMAND;
    aPacket->command = quadwordCount | SDF_VIF_DIRECT_COMMAND;
    aPacket->unknown10 = 0;
    aPacket->unknown18 = 0;
    aPacket->unknown1C = 0;
}
