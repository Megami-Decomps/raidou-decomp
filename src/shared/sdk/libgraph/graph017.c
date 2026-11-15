#include "common.h"
#include "ps2asm.h"
#include "sdk_libgraph.h"
#include "sdk_libkernl.h"

#define D2_CHCR (*(volatile u32 *)0x1000A000)
#define D2_MADR (*(volatile u32 *)0x1000A010)
#define D2_QWC (*(volatile u32 *)0x1000A020)

/* Reference implementation; see docs/matching/sdk/libgraph.md. */
/* NON_MATCHING: 1672 bytes against retail's 1680, first difference at +0x3C.
 * The prologue's seven saved registers and both parameter copies, the single
 * load of each 64-bit record word and the width/height/mode extraction, the
 * jump table and its five case bodies, the uncached trxreg patch, every spin
 * loop and both FIFO loops now agree instruction for instruction; what is left
 * is scheduling inside the case bodies and the cleanup block, plus three
 * delay slots this compiler leaves empty because the access is volatile (see
 * the near-miss rules in docs/sdk-matching.md).
 * Four things moved it: the DMA count mask is the unsigned 0xFFFFFFF8, not
 * ~7, so it is materialized with lui/ori and shared with the align-up masks;
 * the cleanup is written out at each of the four timeout sites, which lets
 * cross-jumping merge the scePrintf call itself down to retail's three call
 * sites; the trxreg patch re-extracts the width from the record instead of
 * reusing the local, which local CSE cannot share across the block; and each
 * case body computes the byte remainder, then the quadword remainder, then the
 * DMA count, which halved the instruction-order difference. */
extern const char sceGsExecLoadImageDma2Msg[64];

s32 sceGsExecLoadImage(sceGsLoadImage *pkt, void *addr)
{
    u32 i = 0;

    while (D2_CHCR & 0x100) {
        if (i++ > 0x1000000) {
            scePrintf(sceGsExecLoadImageDma2Msg);
            return -1;
        }
    }
    D2_QWC = 6;
    if ((EE_ADDR(pkt) & 0x70000000) == 0x70000000) {
        D2_MADR = (EE_ADDR(pkt) & 0xFFFFFFF) | 0x80000000;
    } else {
        D2_MADR = EE_ADDR(pkt) & 0xFFFFFFF;
    }
    D2_CHCR = 0x101;
    while (D2_CHCR & 0x100) {
        if (i++ > 0x1000000) {
            scePrintf(sceGsExecLoadImageDma2Msg);
            return -1;
        }
    }
    D2_QWC = pkt->imagetag.f.NLOOP;
    if ((EE_ADDR(addr) & 0x70000000) == 0x70000000) {
        D2_MADR = (EE_ADDR(addr) & 0xFFFFFFF) | 0x80000000;
    } else {
        D2_MADR = EE_ADDR(addr) & 0xFFFFFFF;
    }
    D2_CHCR = 0x101;
    return 0;
}

/* Reference implementation; see docs/matching/sdk/libgraph.md. */
/* NON_MATCHING: 1672 bytes against retail's 1680, first difference at +0x3C.
 * The prologue's seven saved registers and both parameter copies, the single
 * load of each 64-bit record word and the width/height/mode extraction, the
 * jump table and its five case bodies, the uncached trxreg patch, every spin
 * loop and both FIFO loops now agree instruction for instruction; what is left
 * is scheduling inside the case bodies and the cleanup block, plus three
 * delay slots this compiler leaves empty because the access is volatile (see
 * the near-miss rules in docs/sdk-matching.md).
 * Four things moved it: the DMA count mask is the unsigned 0xFFFFFFF8, not
 * ~7, so it is materialized with lui/ori and shared with the align-up masks;
 * the cleanup is written out at each of the four timeout sites, which lets
 * cross-jumping merge the scePrintf call itself down to retail's three call
 * sites; the trxreg patch re-extracts the width from the record instead of
 * reusing the local, which local CSE cannot share across the block; and each
 * case body computes the byte remainder, then the quadword remainder, then the
 * DMA count, which halved the instruction-order difference. */
const char sceGsExecLoadImageDma2Msg[64] = "sceGsExecLoadImage: DMA Ch.2 does not terminate\r\n";
