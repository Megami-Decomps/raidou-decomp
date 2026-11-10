#include "shared/sdf/sdfPacketBuff/decls.h"

void sdfGrMakePacketBuff(s32 aPacketBuffSize) {
    s32 allocationAddress;
    if (sdfGrPacketBuffHandle != 0) {
        sdfMemFreeDirect(sdfGrPacketBuffHandle);
        sdfGrPacketBuffHandle = 0;
    }
    aPacketBuffSize = (aPacketBuffSize + SDF_PACKET_BUFFER_ALIGNMENT_MASK) & ~SDF_PACKET_BUFFER_ALIGNMENT_MASK;
    sdfGrPacketBuffSize = aPacketBuffSize;
    sdfGrPacketBuffHandle = sdfMemAllocDirect(aPacketBuffSize * SDF_PACKET_BUFFER_COUNT);
    allocationAddress = sdfMemLock(sdfGrPacketBuffHandle);
    sdfGrPacketBuffBase[0] = allocationAddress;
    sdfGrPacketBuffBase[1] = allocationAddress + aPacketBuffSize;
}
