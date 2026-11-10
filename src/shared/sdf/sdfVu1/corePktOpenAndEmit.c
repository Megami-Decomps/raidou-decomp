#include "shared/sdf/sdfVu1/decls.h"

void corePktOpenAndEmit(sdfCorePacketCb_t *cb) {
    u32 packetStart = sdfGrOpenPacketBuff();
    u32 alignedStart = (packetStart + 0x3F) & ~0x3F;
    u32 ringAddress = EE_UNCACHED_RING(EE_PHYSICAL(alignedStart + 0x40));
    cb->packetOpenPtr = packetStart;
    cb->packetTopPtr = alignedStart + 0x30;
    cb->packetStartPtr = EE_ADDR_AS(u32 *, ringAddress);
    cb->packetPtr = EE_ADDR_AS(u32 *, ringAddress);
    if ((cb->attrBits & 0x4000) != 0) {
        u8 savedVectors[0x30];
        VU0_STORE_VF24_VF25_VF26(savedVectors);
        corePktBuildFaceList(cb);
        corePktEmitAllPasses(cb);
        corePktSetupPointBuff(cb, cb->pointCount);
        VU0_LOAD_VF24_VF25_VF26(savedVectors);
        corePktRelightBackFaces(cb->pointBuff, cb->pointCount);
        corePktBuildBackFaceList(cb);
        corePktEmitAllPasses(cb);
    } else {
        corePktBuildFaceList(cb);
        corePktEmitAllPasses(cb);
    }
}
