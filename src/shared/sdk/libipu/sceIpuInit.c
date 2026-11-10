#include "shared/sdk/libipu/decls.h"

void sceIpuInit(void)
{
    s32 ctrl;

    volatile u128 *fifo;

    gcc2_compiled__8(1);
    IPU_CTRL = 0x40000000;
    do {
        ctrl = IPU_CTRL;
    } while (ctrl < 0);
    IPU_CMD = 0;
    do {
        ctrl = IPU_CTRL;
    } while (ctrl < 0);
    fifo = (volatile u128 *)0x10007010;
    *fifo = ipuDefaultQuantizerMatrices[0];
    /* Keep the FIFO address in a register for the remaining transfers.
     * This conservative alias view preserves all four reads of the final
     * quantizer quadword; see docs/matching/sdk/libipu.md. */
    FAKEMATCH_ASM_LIVE(fifo);
    *fifo = ipuDefaultQuantizerMatrices[1];
    *fifo = ipuDefaultQuantizerMatrices[2];
    *fifo = ipuDefaultQuantizerMatrices[3];
    *fifo = ipuDefaultQuantizerMatrices[4];
    *fifo = ipuDefaultQuantizerMatrices[4];
    *fifo = ipuDefaultQuantizerMatrices[4];
    *fifo = ipuDefaultQuantizerMatrices[4];
    IPU_CMD = 0x50000000;
    do {
        ctrl = IPU_CTRL;
    } while (ctrl < 0);
    IPU_CMD = 0x58000000;
    do {
        ctrl = IPU_CTRL;
    } while (ctrl < 0);
    IPU_FIFO = ipuDefaultVqClut[0];
    IPU_FIFO = ipuDefaultVqClut[1];
    IPU_CMD = 0x60000000;
    do {
        ctrl = IPU_CTRL;
    } while (ctrl < 0);
    IPU_CMD = 0x90000000;
    do {
        ctrl = IPU_CTRL;
    } while (ctrl < 0);
    IPU_CTRL = 0x40000000;
    do {
        ctrl = IPU_CTRL;
    } while (ctrl < 0);
    IPU_CMD = 0;
    do {
        ctrl = IPU_CTRL;
    } while (ctrl < 0);
}
