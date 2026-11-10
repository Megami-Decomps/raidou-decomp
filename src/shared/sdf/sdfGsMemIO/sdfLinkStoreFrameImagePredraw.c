#include "shared/sdf/sdfGsMemIO/decls.h"

void sdfLinkStoreFrameImagePredraw(sdfStoreFrameImagePacket_t *aPre, s32 aEvenOdd) {
    aPre->packet.bitblt.bitbltbuf.data =
        (aPre->packet.bitblt.bitbltbuf.data & ~0x3FFF) |
        (u64)(u32)(sdfGrFrameSet.buffers[aEvenOdd]->addr >> 6);
}
