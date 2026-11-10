#include "shared/sdf/sdfIpu/decls.h"

void sdfIpuFree(sdfIpuDecodeCb_t *cb) {
    s32 interruptsEnabled;
    s32 wasActive;
    u32 dmaEnable;
    interruptsEnabled = DIntr();
    wasActive = 0;
    cb->drained = 1;
    sdfIpuDetachTask(cb);
    sdfIpuDetachQueue(cb, 1);
    if (sdfCurrIpuDecodeCb == cb) {
        HW_WRITE(IPU_CTRL, 0x40000000);
        sceIpuSync(0, 0);
        dmaEnable = HW_READ(D_ENABLER);
        HW_WRITE(D_ENABLER, 0x10000);
        HW_WRITE(D4_CHCR, 1);
        HW_WRITE(D3_CHCR, 0);
        HW_WRITE(D_ENABLER, dmaEnable);
        sdfCurrIpuDecodeCb = NULL;
        wasActive = 1;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfFreeAuto(cb->inputDmaChain);
    sdfFreeAuto(cb->frameBuffers[0]);
    sdfFreeAuto(cb->frameBuffers[1]);
    sdfImageSetChainReleaseMem(EE_ADDR_S(cb->transferPacketBuffers[0]));
    sdfImageSetChainReleaseMem(EE_ADDR_S(cb->transferPacketBuffers[1]));
    if (cb->scratchBuffer != 0) {
        sdfFreeAuto(cb->scratchBuffer - 0x100);
    }
    if (cb->textureHead != 0) {
        sdfGsMemFreeDelayed(cb->textureHead);
    }
    if (wasActive != 0) {
        sdfIpuExecQueue(0);
    }
}
