#include "shared/sdf/sdfCd/decls.h"

/* Consume up to size disc-file bytes, retaining a partial sector; return the byte count. */
s32 sdfCdImageRead(void *buff, s32 length) {
    s32 available = sdfCdImageCurrRemain;
    u8 *destination = buff;
    s32 bytes;
    s32 transferBytes;
    s32 t;
    if (available <= 0) {
        return 0;
    }
    if (length > available) {
        length = available;
    }
    if (length == 0) {
        return 0;
    }
    bytes = length;
    transferBytes = sdfCdImageCurrActive;
    if (transferBytes > 0) {
        if (transferBytes > length) {
            transferBytes = length;
        }
        memcpy(destination, sdfCdImageOffset, transferBytes);
        destination += transferBytes;
        sdfCdImageOffset += transferBytes;
        sdfCdImageCurrActive -= transferBytes;
        bytes -= transferBytes;
    }
    if (bytes > 0) {
        transferBytes = bytes;
        if ((EE_ADDR(destination) & 15) == 0) {
            t = transferBytes >> 11;
            t = transferBytes - t * 0x800;
            transferBytes -= t;
            if (transferBytes > 0) {
                sdfCdStRead(transferBytes >> 11, destination);
            }
            if (t > 0) {
                sdfCdStRead(1, sdfCdSectorBuf);
                memcpy(destination + transferBytes, sdfCdSectorBuf, t);
                sdfCdImageOffset = sdfCdSectorBuf + t;
                sdfCdImageCurrActive = 0x800 - t;
            }
        } else {
            while (transferBytes >= 0x800) {
                sdfCdStRead(1, sdfCdSectorBuf);
                memcpy(destination, sdfCdSectorBuf, 0x800);
                transferBytes -= 0x800;
                destination += 0x800;
            }
            if (transferBytes > 0) {
                sdfCdStRead(1, sdfCdSectorBuf);
                memcpy(destination, sdfCdSectorBuf, transferBytes);
                sdfCdImageOffset = sdfCdSectorBuf + transferBytes;
                sdfCdImageCurrActive = 0x800 - transferBytes;
            }
        }
    }
    sdfCdImageCurrRemain = available - length;
    if (sdfCdImageCurrRemain == 0) {
        sdfCdImageClose();
    }
    return length;
}
