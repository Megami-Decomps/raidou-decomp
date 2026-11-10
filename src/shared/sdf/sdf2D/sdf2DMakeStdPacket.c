#include "shared/sdf/sdf2D/decls.h"

u32 sdf2DMakeStdPacket(u32 aPacket, s32 aLeng) {
    sdfGrSetRegsPacketHeader(EE_ADDR_AS(sdfGrPacketHeader_t *, aPacket), (aLeng >> SDF_DMA_QWORD_SHIFT) - 2);
    return aPacket;
}
