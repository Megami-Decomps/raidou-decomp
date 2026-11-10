#include "shared/sdf/sdfGraph/decls.h"

void sdfGrCapture(void) {
    s32 width;
    s32 height;
    s32 format;
    s32 pixelBytes;
    s32 rowBytes;
    s32 stride;
    sdfMemHandle_t *allocation;
    s32 rows;
    u8 *pixels;
    u8 *source;
    u8 *destination;

    if (*(u8 *)&sdfGrCaptureBuff) {
        width = sdfGrFrameSet.w;
        height = sdfGrFrameSet.buffH;
        format = sdfGrFrameSet.pixelFormat;
        pixelBytes = 4;
        if (format == 2) {
            pixelBytes = 2;
        }
        rowBytes = pixelBytes * width;
        stride = rowBytes * 2;
        sdfDispThreadFlag = 5;
        allocation = sdfMemAllocDirect(rowBytes * height);
        pixels = (u8 *)sdfMemLock(allocation);
        sceGsSetDefStoreImage(
            (sceGsStoreImage *)EE_UNCACHED(EE_PHYSICAL(EE_ADDR(&sdfGrStoreImage))),
            sdfGrFrameSet.frameHandle[1]->addr >> 6,
            width / 64, format, 0, 0, width, height);
        sceGsExecStoreImage(&sdfGrStoreImage, pixels);
        sceGsSyncPath(0, 0);
        destination = EE_ADDR_AS(u8 *, sdfGrCaptureReq);
        source = pixels;
        rows = height;
        do {
            memcpy(destination, source, rowBytes);
            destination += stride;
            source += rowBytes;
        } while (--rows != 0);
        sceGsSetDefStoreImage(
            (sceGsStoreImage *)EE_UNCACHED(EE_PHYSICAL(EE_ADDR(&sdfGrStoreImage))),
            sdfGrFrameSet.frameHandle[0]->addr >> 6,
            width / 64, format, 0, 0, width, height);
        sceGsExecStoreImage(&sdfGrStoreImage, pixels);
        sceGsSyncPath(0, 0);
        destination = EE_ADDR_AS(u8 *, sdfGrCaptureReq) + rowBytes;
        source = pixels;
        rows = height;
        do {
            memcpy(destination, source, rowBytes);
            destination += stride;
            source += rowBytes;
        } while (--rows != 0);
        sdfMemFreeDirect(allocation);
        sdfGrCaptureBuff = 0;
        if (sdfGrCaptureTarget) {
            EE_ADDR_AS(sdfGrCaptureFunc_t, sdfGrCaptureTarget)(EE_ADDR_AS(u8 *, sdfGrCaptureReq));
        }
    }
}
