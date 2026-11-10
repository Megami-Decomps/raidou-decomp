#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdk/libgraph.c */
#include "common.h"

#include "ps2asm.h"

/* The GS interrupt mask register, through the kernel's syscall stubs. */
extern u64 GsGetIMR(void);

extern u64 GsPutIMR(u64 imr);

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

extern GParam sceGsGParam;

extern void scePrintf(const char *fmt, ...);

#include "sdk_libkernl.h"

extern s32 sceRead(s32 fd, void *buf, s32 size);

extern s32 sceClose(s32 a);

extern s32 atoi(const char *s);

extern s32 DisableIntc(s32 cause);

extern s32 RemoveIntcHandler(s32 cause, s32 handlerId);

/* Syscall 2 has the SetGsCrt void interface. */
extern void SetGsCrt(s16 interlace, s16 mode, s16 fieldMode);

GParam *sceGsGetGParam(void);

void sceGsResetGraph(s16 mode, s16 interlace, s16 outputMode, s16 fieldMode);

GParam *sceGsGetGParam(void);

#define VIF1_FBRST ((volatile u32 *)0x10003C10)

#define VIF1_ERR   ((volatile u32 *)0x10003C20)

#define VIF1_FIFO  ((volatile u128 *)0x10005000)

#define GIF_CTRL   ((volatile u32 *)0x10003000)

#define putVIF1_FBRST(x) (*VIF1_FBRST = (x))

#define putVIF1_ERR(x)   (*VIF1_ERR = (x))

#define putVIF1_FIFO(x)  (*VIF1_FIFO = (x))

#define putGIF_CTRL(x)   (*GIF_CTRL = (x))

extern const u32 sceGsResetPathPackets[8];

s32 checkModelVersion(void);

void sceGsPutDispEnv(sceGsDispEnv *disp);

#define D1_CHCR   (*(volatile u32 *)0x10009000)

#define VIF1_STAT (*(volatile u32 *)0x10003C00)

#define GIF_STAT  (*(volatile u32 *)0x10003020)

/* Native diagnostic strings are retained through DUMP_REGS macro ownership. */
/* The five timeout messages and the ten register-dump formats. */
extern const char sceGsSyncPathDma1Msg[];

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

extern const char sceGsSyncPathDma2Msg[];

extern const char sceGsSyncPathVif1Msg[];

extern const char sceGsSyncPathVu1Msg[];

extern const char sceGsSyncPathGifMsg[];

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

s32 sceGsSyncPath(s32 mode, u16 timeout);

extern const char sceGsSyncPathDma1Msg[48];

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

extern const char sceGsSyncPathDma2Msg[48];

extern const char sceGsSyncPathVif1Msg[48];

extern const char sceGsSyncPathVu1Msg[40];

extern const char sceGsSyncPathGifMsg[48];

s32 sceGsSetDefLoadImage(sceGsLoadImage *p, s16 dbp, s16 dbw, s16 dpsm, s16 dsax, s16 dsay,
                         s16 rrw, s16 rrh);

s32 sceGsSetDefStoreImage(sceGsStoreImage *p, s16 sbp, s16 sbw, s16 spsm, s16 ssax, s16 ssay,
                          s16 rrw, s16 rrh);

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

u64 sceGsPutIMR(u64 imr);

u64 sceGsGetIMR(void);
/* End of declarations from src/smt3/sdk/libgraph.c */

u64 sceGsGetIMR(void)
{
    return GsGetIMR();
}
