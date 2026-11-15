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

extern void scePrintf(const char *fmt, ...);

#include "sdk_libkernl.h"

GParam *sceGsGetGParam(void);

#define VIF1_FBRST ((volatile u32 *)0x10003C10)

#define VIF1_ERR   ((volatile u32 *)0x10003C20)

#define VIF1_FIFO  ((volatile u128 *)0x10005000)

#define GIF_CTRL   ((volatile u32 *)0x10003000)

#define putVIF1_FBRST(x) (*VIF1_FBRST = (x))

#define putVIF1_ERR(x)   (*VIF1_ERR = (x))

#define putVIF1_FIFO(x)  (*VIF1_FIFO = (x))

#define putGIF_CTRL(x)   (*GIF_CTRL = (x))

void sceGsPutDispEnv(sceGsDispEnv *disp);

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

/* Reference implementation; see docs/matching/sdk/libgraph.md. */

void sceGsPutDispEnv(sceGsDispEnv *disp)
{
    GParam *gp = sceGsGetGParam();

    if (gp->version == 1) {
        *(volatile u64 *)0x12000000 = disp->pmode;
        *(volatile u64 *)0x12000070 = disp->dispfb;
        *(volatile u64 *)0x12000080 = disp->display;
        *(volatile u64 *)0x120000C0 = disp->bgcolor;
    } else {
        *(volatile u64 *)0x12000000 = disp->pmode;
        *(volatile u64 *)0x12000020 = disp->smode2;
        *(volatile u64 *)0x12000090 = disp->dispfb;
        *(volatile u64 *)0x120000A0 = disp->display;
        *(volatile u64 *)0x120000E0 = disp->bgcolor;
    }
}
