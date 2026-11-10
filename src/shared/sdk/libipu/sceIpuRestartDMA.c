#include "shared/sdk/libipu/decls.h"

/* The saved input count occupies $a0 only while forming the pre-call DMA
 * snapshot. See docs/matching/sdk/libipu.md for the allocation ablation. */
void sceIpuRestartDMA(sdfIpuDmaRegs_t *state)
{
    u32 bp = state->bitPointer;
    u32 bit = bp & 127;
    u32 buffered = ((bp >> 16) & 3) + ((bp >> 8) & 15);
    FAKEMATCH_ASM_REG(u32, inputQwords, "$4") = state->inputQwords;
    u32 qwc = inputQwords + buffered;
    u32 madr = state->inputAddress - (buffered << 4);
    s32 ctrl;

    if (state->outputAddress != 0 && state->outputQwords != 0) {
        D3_MADR = state->outputAddress;
        D3_QWC = state->outputQwords;
        ipuWriteFromIpuChcr(state->outputControl | 0x100);
    }
    do {
        ctrl = IPU_CTRL;
    } while (ctrl < 0);
    IPU_CMD = bit;
    do {
        ctrl = IPU_CTRL;
    } while (ctrl < 0);
    if (madr != 0 && qwc != 0) {
        D4_MADR = madr;
        D4_TADR = state->inputTagAddress;
        D4_QWC = qwc;
        setD4_CHCR(state->inputControl | 0x100);
    }
}
