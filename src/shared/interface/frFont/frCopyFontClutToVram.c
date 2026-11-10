#include "shared/interface/frFont/decls.h"

/* Upload an image to GS memory at the given buffer, serialized by the GS semaphore. */
void frCopyFontClutToVram(s32 GS_LocalAddr, saddr_t pClut) {
    u8 loadImage[0x60];

    sceGsSetDefLoadImage(loadImage, (s16)GS_LocalAddr, 1, 0, 0, 0, 8, 2);
    WaitSema(dds3GsSema);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, pClut);
    sceGsSyncPath(0, 0);
    SignalSema(dds3GsSema);
}
