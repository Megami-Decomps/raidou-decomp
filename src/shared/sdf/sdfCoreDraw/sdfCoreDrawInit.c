#include "shared/sdf/sdfCoreDraw/decls.h"

void sdfCoreDrawInit(s32 buffSize) {
    u32 *dmaChannel = sceDmaGetChan(0);
    HW_WRITE(dmaChannel, HW_READ(dmaChannel) & ~SDF_DMA_CHCR_TTE);
    sceDmaSendN(dmaChannel, vu0MicroPacket, (vu1MicroPacket - vu0MicroPacket) >> SDF_DMA_QWORD_SHIFT);
    sceDmaSync((sceDmaChan *)dmaChannel, 0, 0);
    sdfMemFreeHandleDirect(&sdfCoreDrawBuffHandle);
    sdfCoreDrawBuffHandle = sdfMemAllocDirect(buffSize);
    corePktPointBuff = EE_ADDR(sdfMemLock(sdfCoreDrawBuffHandle));
}
