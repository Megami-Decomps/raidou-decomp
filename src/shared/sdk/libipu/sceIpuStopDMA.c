#include "shared/sdk/libipu/decls.h"

void sceIpuStopDMA(sdfIpuDmaRegs_t *state)
{
    u32 ctrl;

    setD4_CHCR(1);
    state->inputAddress = D4_MADR;
    state->inputTagAddress = D4_TADR;
    state->inputQwords = D4_QWC;
    state->inputControl = D4_CHCR;
    do {
        ctrl = IPU_CTRL & 0xF0;
    } while (ctrl != 0);
    ipuWriteFromIpuChcr(0);
    state->outputAddress = D3_MADR;
    state->outputQwords = D3_QWC;
    state->outputControl = D3_CHCR;
    state->bitPointer = IPU_BP;
    state->ipuControl = IPU_CTRL;
}
