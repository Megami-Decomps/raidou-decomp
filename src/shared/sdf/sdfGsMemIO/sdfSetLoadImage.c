#include "shared/sdf/sdfGsMemIO/decls.h"

void sdfSetLoadImage(sdfLoadImagePacket_t *aPacket, sdfGsMemSlice_t *aGsHandle,
                   s64 aDstX, s64 aDstY, s32 aWidth,
                   s32 aHeight, u32 aBuffer) {
    s32 qwc;

    aPacket->header[0] = 0x10000006;
    aPacket->header[1] = 0x5000000611000000;
    aPacket->header[2] = 0x1000000000000004;
    aPacket->header[3] = 0xE;
    sdfPrimSetBitBltElem(&aPacket->bitblt, aGsHandle->addr, aGsHandle->w,
                  aGsHandle->pixelFormat, aDstX, aDstY, 0, 0, 0, 0, 0,
                  aWidth, aHeight, 0);

    qwc = (sdfGetTexelBitCount(aGsHandle->pixelFormat) * aWidth * aHeight) >> 7;
    aPacket->imageReferenceTag = 0x0800000000000000 | qwc;
    aPacket->imageReferencePad = 0;
    aPacket->imageGifTag0 = SDF_DMA_TAG_WORD_FIRST(
        (u32)((qwc & SDF_DMA_QWC_MASK) | 0x30000000), aBuffer);
    aPacket->imageGifTag1 = (u64)(0x51000000 | qwc) << 32;
    aPacket->flushTag0 = 0x20000002;
    aPacket->flushTag1 = 0x5000000200000000;
    aPacket->flushGifTag = 0x1000000000008001;
    aPacket->flushRegister = 0xE;
    aPacket->zero = 0;
    aPacket->finishRegister = 0x3F;
}
