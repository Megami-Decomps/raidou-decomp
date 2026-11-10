#include "shared/sdf/sdfIpu/decls.h"

/* Decode the frame header, allocate its buffers, then queue the bytes after that header. */
void sdfIpuSetDecodeParams(sdfIpuDecodeCb_t *cb, struct sdfIpuInitParm_t *parm, SdfStreamRead hdr, struct sdfMovieCb_t *leng) {
    saddr_t sourceSize = EE_ADDR_S(leng);
    s32 interruptsEnabled;
    u8 *frameBytes = (u8 *)hdr;
    sdfIpuInitCb(cb, parm);
    cb->width = ((sdfStreamFrameHeader_t *)frameBytes)->width;
    cb->frameCount = ((sdfStreamFrameHeader_t *)frameBytes)->cycleLength;
    cb->height = ((sdfStreamFrameHeader_t *)frameBytes)->height;
    sdfIpuAllocBuff(cb);
    sdfIpuMakeToIpuDmaTag(cb, frameBytes + 0x10, sourceSize - 0x10);
    interruptsEnabled = DIntr();
    sdfIpuAttachQueue(cb, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfIpuExecQueue(0);
}
