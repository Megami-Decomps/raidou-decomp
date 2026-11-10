/* Declarations of smt3-decomp src/smt3/sdk/libipu.c, for its bodies in this directory. */
#ifndef SHARED_SDK_LIBIPU_H
#define SHARED_SDK_LIBIPU_H

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

#endif
