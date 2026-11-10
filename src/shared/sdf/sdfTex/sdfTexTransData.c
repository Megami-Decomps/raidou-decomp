#include "shared/sdf/sdfTex/decls.h"

void *sdfTexTransData(s32 aGsAddr, s32 aWidth, s32 aHeight, s32 aPixelFormat, void *aSrcData, s32 aMemMode) {
    sdfImageUploadRequest_t request;
    s32 imageBytes = sdfImageDataSize(aPixelFormat, aWidth, aHeight) * 16;

    if (aMemMode == 0) {
        if (imageBytes > 0x400) {
            request.allocation = sdfMemAllocDirect(imageBytes);
            request.pixels = (void *)sdfMemLock(request.allocation);
            request.allocationMode = 1;
        } else {
            request.pixels = sdfChipAllocDirect(imageBytes);
            request.allocationMode = 2;
        }
        memcpy(request.pixels, aSrcData, imageBytes);
    } else {
        request.pixels = aSrcData;
        request.allocationMode = 0;
    }
    request.format = aPixelFormat;
    request.destination = aGsAddr;
    request.bufferWidth = aWidth;
    request.width = aWidth;
    request.height = aHeight;
    request.x = 0;
    request.y = 0;
    sdfImageSetReq(&request);
    return (u8 *)aSrcData + imageBytes;
}
