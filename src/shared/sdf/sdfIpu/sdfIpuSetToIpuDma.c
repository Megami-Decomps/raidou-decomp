#include "shared/sdf/sdfIpu/decls.h"

/* Start IPU input from the initial chain or a feed slot; defer an empty ring. */
void sdfIpuSetToIpuDma(sdfIpuDecodeCb_t *cb) {
    if (cb->active == 0) {
        sdfIpuPendingCb = cb;
        HW_WRITE(D4_TADR, EE_PHYSICAL(EE_ADDR(cb->inputDmaChain)));
        HW_WRITE(D4_QWC, 0);
        HW_WRITE(D4_CHCR, 0x105);
    } else {
        if (cb->filledSlots == 0) {
            cb->inputDmaStartPending = 1;
            return;
        }
        sdfIpuPendingCb = cb;
        cb->inputFeedDmaInFlight = 1;
        HW_WRITE(D4_MADR, EE_PHYSICAL(EE_ADDR(cb->scratchBuffer) + (cb->firstSlot << 13)));
        HW_WRITE(D4_QWC, 0x2000 / 16);
        HW_WRITE(D4_CHCR, 0x101);
    }
    cb->inputDmaStartPending = 0;
}
