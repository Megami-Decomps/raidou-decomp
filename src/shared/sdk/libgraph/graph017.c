#include "common.h"
#include "ps2asm.h"

typedef union {
    u128 qword;
    struct {
        u64 NLOOP : 15;
        u64 EOP : 1;
        u64 pad16 : 30;
        u64 PRE : 1;
        u64 PRIM : 11;
        u64 FLG : 2;
        u64 NREG : 4;
        u64 REGS0 : 4;
        u64 REGS1 : 4;
        u64 REGS2 : 4;
        u64 REGS3 : 4;
        u64 REGS4 : 4;
        u64 REGS5 : 4;
        u64 REGS6 : 4;
        u64 REGS7 : 4;
        u64 REGS8 : 4;
        u64 REGS9 : 4;
        u64 REGS10 : 4;
        u64 REGS11 : 4;
        u64 REGS12 : 4;
        u64 REGS13 : 4;
        u64 REGS14 : 4;
        u64 REGS15 : 4;
    } f;
} sceGifTag;

typedef struct {
    s16 interlace;
    u16 mode;
    s16 fieldMode;
    s16 version;
    s32 intcHandlerEnabled;
    s32 intcHandlerId;
} GParam;

typedef struct {
    u64 pmode;
    u64 smode2;
    u64 dispfb;
    u64 display;
    u64 bgcolor;
} sceGsDispEnv;

typedef struct {
    sceGifTag tag;
    u64 bitbltbuf;
    u64 reg_bitbltbuf;
    u64 trxpos;
    u64 reg_trxpos;
    u64 trxreg;
    u64 reg_trxreg;
    u64 trxdir;
    u64 reg_trxdir;
    sceGifTag imagetag;
} sceGsLoadImage;

typedef struct {
    u32 qwc;
    u32 addr;
    u32 vifcode0;
    u32 vifcode1;
    sceGifTag tag;
    u64 bitbltbuf;
    u64 reg_bitbltbuf;
    u64 trxpos;
    u64 reg_trxpos;
    u64 trxreg;
    u64 reg_trxreg;
    u64 finish;
    u64 reg_finish;
    u64 trxdir;
    u64 reg_trxdir;
} sceGsStoreImage;

#define D2_CHCR (*(volatile u32 *)0x1000A000)

#define D2_MADR (*(volatile u32 *)0x1000A010)

#define D2_QWC (*(volatile u32 *)0x1000A020)


#include "sdk_libkernl.h"

#define VIF1_FBRST ((volatile u32 *)0x10003C10)

#define VIF1_ERR   ((volatile u32 *)0x10003C20)

#define VIF1_FIFO  ((volatile u128 *)0x10005000)

#define GIF_CTRL   ((volatile u32 *)0x10003000)

#define putVIF1_FBRST(x) (*VIF1_FBRST = (x))

#define putVIF1_ERR(x)   (*VIF1_ERR = (x))

#define putVIF1_FIFO(x)  (*VIF1_FIFO = (x))

#define putGIF_CTRL(x)   (*GIF_CTRL = (x))

#define D1_CHCR   (*(volatile u32 *)0x10009000)

#define VIF1_STAT (*(volatile u32 *)0x10003C00)

#define GIF_STAT  (*(volatile u32 *)0x10003020)

/* Native diagnostic strings are retained through DUMP_REGS macro ownership. */

extern const char sceGsDumpD1ChcrFmt[];
extern const char sceGsDumpD1TadrFmt[];
extern const char sceGsDumpD1MadrFmt[];
extern const char sceGsDumpD1QwcFmt[];
extern const char sceGsDumpD2ChcrFmt[];
extern const char sceGsDumpD2TadrFmt[];
extern const char sceGsDumpD2MadrFmt[];
extern const char sceGsDumpD2QwcFmt[];
extern const char sceGsDumpVif1StatFmt[];
extern const char sceGsDumpGifStatFmt[];

#define DUMP(fmt, addr)                      \
    scePrintf(fmt, *(volatile u32 *)(addr))

#define DUMP_REGS()                          \
    DUMP(sceGsDumpD1ChcrFmt, 0x10009000);            \
    DUMP(sceGsDumpD1TadrFmt, 0x10009030);            \
    DUMP(sceGsDumpD1MadrFmt, 0x10009010);            \
    DUMP(sceGsDumpD1QwcFmt, 0x10009020);            \
    DUMP(sceGsDumpD2ChcrFmt, 0x1000A000);            \
    DUMP(sceGsDumpD2TadrFmt, 0x1000A030);            \
    DUMP(sceGsDumpD2MadrFmt, 0x1000A010);            \
    DUMP(sceGsDumpD2QwcFmt, 0x1000A020);            \
    DUMP(sceGsDumpVif1StatFmt, 0x10003C00);            \
    DUMP(sceGsDumpGifStatFmt, 0x10003020)

extern const char sceGsDumpD1ChcrFmt[16];
extern const char sceGsDumpD1TadrFmt[16];
extern const char sceGsDumpD1MadrFmt[16];
extern const char sceGsDumpD1QwcFmt[16];
extern const char sceGsDumpD2ChcrFmt[16];
extern const char sceGsDumpD2TadrFmt[16];
extern const char sceGsDumpD2MadrFmt[16];
extern const char sceGsDumpD2QwcFmt[16];
extern const char sceGsDumpVif1StatFmt[24];
extern const char sceGsDumpGifStatFmt[24];
extern const char sceGsExecLoadImageDma2Msg[];
s32 sceGsExecLoadImage(sceGsLoadImage *pkt, void *addr);

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
