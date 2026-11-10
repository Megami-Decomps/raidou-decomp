#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdk/libipu.c */
#include "common.h"

#define IPU_CMD  (*(volatile u32 *)0x10002000)

#define IPU_CTRL (*(volatile u32 *)0x10002010)

#define IPU_BP   (*(volatile u32 *)0x10002020)

#define D3_CHCR (*(volatile u32 *)0x1000B000)

#define D3_MADR (*(volatile u32 *)0x1000B010)

#define D3_QWC  (*(volatile u32 *)0x1000B020)

#define D4_CHCR (*(volatile u32 *)0x1000B400)

#define D4_MADR (*(volatile u32 *)0x1000B410)

#define D4_QWC  (*(volatile u32 *)0x1000B420)

#define D4_TADR (*(volatile u32 *)0x1000B430)

#define D_ENABLER (*(volatile u32 *)0x1000F520)

#define D_ENABLEW (*(volatile u32 *)0x1000F590)

extern s32 DIntr(void);

extern s32 EIntr(void);

/* The sibling that drives channel 3; the IDB still carries the object's
 * gcc2_compiled label as its name. */
extern void ipuWriteFromIpuChcr(u32 chcr);

void ipuWriteFromIpuChcr(u32 chcr);

void setD4_CHCR(u32 chcr);

typedef struct sdfIpuDmaRegs_t {
    u32 inputAddress;
    u32 inputTagAddress;
    u32 inputQwords;
    u32 inputControl;
    u32 outputAddress;
    u32 outputQwords;
    u32 outputControl;
    u32 bitPointer;
    u32 ipuControl;
} sdfIpuDmaRegs_t;

void sceIpuStopDMA(sdfIpuDmaRegs_t *state);

/* The saved input count occupies $a0 only while forming the pre-call DMA
 * snapshot. See docs/matching/sdk/libipu.md for the allocation ablation. */
void sceIpuRestartDMA(sdfIpuDmaRegs_t *state);

u32 sceIpuSync(s32 mode);

void gcc2_compiled__8(u32 chcr);

extern u128 ipuDefaultQuantizerMatrices[5] __attribute__((aligned(16)));

extern u128 ipuDefaultVqClut[2] __attribute__((aligned(16)));

#define IPU_FIFO (*(volatile u128 *)0x10007010)

void sceIpuInit(void);
/* End of declarations from src/smt3/sdk/libipu.c */

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
