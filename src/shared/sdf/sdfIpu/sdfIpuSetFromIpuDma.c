#include "shared/sdf/sdfIpu/decls.h"

/* Start DMA from the IPU into the selected buffer, then toggle the byte index. */
void sdfIpuSetFromIpuDma(sdfIpuDecodeCb_t *cb) {
    u8 bufferIndex = cb->bufferIndex;
    HW_WRITE(D3_MADR, EE_PHYSICAL(EE_ADDR(cb->frameBuffers[bufferIndex])));
    HW_WRITE(D3_QWC, cb->buffSize / 16);
    HW_WRITE(D3_CHCR, 0x100);
    cb->bufferIndex = bufferIndex ^ 1;
}
