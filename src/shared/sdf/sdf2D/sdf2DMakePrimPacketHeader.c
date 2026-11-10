#include "shared/sdf/sdf2D/decls.h"

sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, s64 aGifRegs, s32 aNumOfPrim) {
    s32 packetQuadwords = aNumOfRegs * aNumOfPrim + 1;
    s64 gifTag = aNumOfPrim | ((s64)aNumOfRegs << SDF_GIF_NREG_SHIFT);

    gifTag |= (s64)aPrimReg << SDF_GIF_PRIM_SHIFT;
    gifTag |= SDF_GIF_PRE_EOP_FLAGS;
    aPacketPtr->giftag.parm = gifTag;
    aPacketPtr->dmatag.dat1 = packetQuadwords | SDF_VIF_DIRECT_COMMAND;
    aPacketPtr->giftag.regs = aGifRegs;
    aPacketPtr->dmatag.dat0 = 0;
    aPacketPtr->dmatag.qwc = packetQuadwords;
    return aPacketPtr;
}
