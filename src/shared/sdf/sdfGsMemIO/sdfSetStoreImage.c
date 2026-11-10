#include "shared/sdf/sdfGsMemIO/decls.h"

void sdfSetStoreImage(sdfStoreImagePacket_t *aPacket, sdfGsMemSlice_t *aGsHandle,
    s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, u32 aBuffer, u32 aCallback, u32 aCallbackParam) {
    s32 quadwordCount;

    aPacket->header[0] = 0x90000001ULL;
    aPacket->header[1] = 0x10000000ULL;
    aPacket->header[2] = 0;
    aPacket->header[3] = 0;
    aPacket->header[4] = 0x90000002ULL;
    aPacket->header[5] = 0x5000000206008000ULL;
    aPacket->header[6] = 0x1000000000000001ULL;
    aPacket->header[7] = 0xE;
    aPacket->header[8] = 0;
    aPacket->header[9] = 0x61;
    aPacket->header[10] = SDF_DMA_TAG(EE_ADDR(aPacket->tail), 0xA0000005ULL);
    aPacket->header[11] = 0x5000000500000000ULL;
    aPacket->header[12] = 0x1000000000000004ULL;
    aPacket->header[13] = 0xE;
    sdfPrimSetBitBltElem(&aPacket->bitblt, 0, 0, 0, 0, 0,
        aGsHandle->addr, aGsHandle->w, aGsHandle->pixelFormat,
        aSrcX, aSrcY, aWidth, aHeight, 1);
    quadwordCount = (sdfGetTexelBitCount(aGsHandle->pixelFormat) * aWidth * aHeight) >> 7;
    aPacket->leng = quadwordCount;
    aPacket->addr = aBuffer;
    aPacket->callback = aCallback;
    aPacket->callbackParam = aCallbackParam;
    aPacket->tail[0] = 0x20000002ULL;
    aPacket->tail[1] = 0x5000000206000000ULL;
    aPacket->tail[2] = 0x1000000000008001ULL;
    aPacket->tail[3] = 0xE;
    aPacket->tail[4] = 0x46;
    aPacket->tail[5] = 0;
}
