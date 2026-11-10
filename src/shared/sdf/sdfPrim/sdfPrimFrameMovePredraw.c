#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimFrameMovePredraw(sdfPrimFrameMovePacket_t *aPre, s32 aEvenOdd) {
    aPre->packet.bitblt.bitbltbuf.data =
        (aPre->packet.bitblt.bitbltbuf.data & ~0x3FFF) |
        (u64)(u32)(sdfGrFrameSet.buffers[aEvenOdd ^ aPre->worker.option0]->addr >> 6);
}
