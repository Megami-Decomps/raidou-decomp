#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdk/libdma.c */
#include "common.h"

#include "libdma.h"

#define VD_CTRL ((volatile u32 *)0x1000E000)

#define VD_STAT ((volatile u32 *)0x1000E010)

#define VD_PCR  ((volatile u32 *)0x1000E020)

#define VD_SQWC ((volatile u32 *)0x1000E030)

#define VD_RBSR ((volatile u32 *)0x1000E040)

#define VD_RBOR ((volatile u32 *)0x1000E050)

#define D_STADR ((u32 *)0x1000E060)

#define CHCR(c) (*(volatile u32 *)(c))

#define MADR(c) (*(volatile u32 *)((u8 *)(c) + 0x10))

#define D_ENABLER ((volatile u32 *)0x1000F520)

#define D_ENABLEW ((volatile u32 *)0x1000F590)

/* The four byte fields select D_CTRL's channel numbers: STS (bits 4-5), STD
 * (bits 6-7), MFD (bits 2-3) and the release cycle (bit 1, bits 8-9). */
typedef struct {
    u8 sts;
    u8 std;
    u8 mfd;
    u8 rcyc;
    u16 pcr_hi;
    u16 pcr_lo;
    u16 sqwc_lo;
    u16 sqwc_hi;
    u32 rbor;
    u32 rbsr;
} sceDmaEnv;

extern sceDmaChan *sceDmaChanRegs[10];

extern sceDmaEnv sceDmaSavedEnv;

extern const int sceDmaChanResetFlag[10];

extern const u8 sceDmaStsBits[16];

extern const u8 sceDmaStdBits[16];

extern const u8 sceDmaMfdBits[16];

extern int scePrintf(const char *fmt, ...);

extern int DIntr(void);

extern int EIntr(void);

void WaitDma(sceDmaChan *chan);

void WatchDma(sceDmaChan *chan, u32 addr, int timeout);

u32 CheckAddress(addr_t addr);

void memclr(void *p, int n);

int sceDmaPause(sceDmaChan *chan);

int sceDmaPutEnv(sceDmaEnv *env);

void WaitDma(sceDmaChan *chan);

void WatchDma(sceDmaChan *chan, u32 addr, int timeout);

u32 CheckAddress(addr_t addr);

void memclr(void *p, int n);

sceDmaChan *sceDmaGetChan(u32 channel);

int sceDmaReset(int enable);

sceDmaEnv *sceDmaGetEnv(sceDmaEnv *env);

u32 sceDmaPutStallAddr(u32 addr);

void sceDmaSend(sceDmaChan *chan, void *tag);

void sceDmaSendN(sceDmaChan *chan, void *data, u32 qwc);

void sceDmaSendI(sceDmaChan *chan, void *data, u32 qwc);

/* The receive starters share this tail; 2.9 inlines it, which is what retail
 * has, so it cannot be a real function. */
static __inline__ void recv_setup(sceDmaChan *chan, int mod)
{
    chan->chcr = ((((int)chan->chcr & ~0xC) | mod) & ~1) | 0x100;
}

void sceDmaRecv(sceDmaChan *chan);

void sceDmaRecvN(sceDmaChan *chan, void *data, u32 qwc);

void sceDmaRecvI(sceDmaChan *chan, void *data, u32 qwc);

int sceDmaSync(sceDmaChan *chan, int mode, int unused);

int sceDmaWatch(sceDmaChan *chan, u32 addr, int mode, int timeout);

int sceDmaPause(sceDmaChan *chan);

int sceDmaRestart(sceDmaChan *chan, u32 chcr);

extern const int sceDmaChanResetFlag[10];

extern const u8 sceDmaStsBits[16];

extern const u8 sceDmaStdBits[16];

extern const u8 sceDmaMfdBits[16];
/* End of declarations from src/smt3/sdk/libdma.c */

void WaitDma(sceDmaChan *chan)
{
    int i = 0x1000000;

    while (CHCR(chan) & 0x100) {
        if (--i < 0) {
            scePrintf("libdma: sync timeout\n");
            sceDmaPause(chan);
        }
    }
}

void WatchDma(sceDmaChan *chan, u32 addr, int timeout)
{
    int i = timeout ? timeout : 0x1000000;

    while (MADR(chan) < addr) {
        if (--i < 0) {
            scePrintf("libdma: sync timeout\n");
            sceDmaPause(chan);
        }
    }
}

u32 CheckAddress(addr_t addr)
{
    if ((addr >> 28) == 7) {
        addr &= 0x0FFFFFFF;
        addr |= 0x80000000;
    }
    return addr;
}

void memclr(void *p, int n)
{
    u8 *q = p;

    while (n-- != 0) {
        *q++ = 0;
    }
}

sceDmaChan *sceDmaGetChan(u32 channel)
{
    if (channel < 10) {
        return sceDmaChanRegs[channel];
    }
    return NULL;
}

int sceDmaReset(int enable)
{
    sceDmaEnv env;
    u32 dmae = *VD_CTRL & 1;
    int i;

    for (i = 0; i < 10; i++) {
        if (sceDmaChanResetFlag[i]) {
            sceDmaChan *chan = sceDmaChanRegs[i];

            chan->chcr = 0;
            chan->tadr = 0;
            chan->madr = 0;
            chan->asr1 = 0;
            chan->asr0 = 0;
            chan->sadr = 0;
        }
    }
    {
        volatile u32 *stat = VD_STAT;

        *stat = 0xFF1F;
        *(u32 *)stat = *stat & 0xFF1F0000;
    }
    memclr(&env, sizeof(env));
    sceDmaPutEnv(&env);
    if (enable == 1) {
        *VD_CTRL = *VD_CTRL | 1;
    }
    return dmae;
}

INCLUDE_ASM(const s32, "sdk/libdma/libdma", sceDmaPutEnv);

sceDmaEnv *sceDmaGetEnv(sceDmaEnv *env)
{
    *env = sceDmaSavedEnv;
    return env;
}

u32 sceDmaPutStallAddr(u32 addr)
{
    u32 *reg = D_STADR;
    u32 old = *reg;

    *reg = addr;
    return old;
}

void sceDmaSend(sceDmaChan *chan, void *tag)
{
    u32 addr = CheckAddress(EE_ADDR(tag));

    WaitDma(chan);
    if (chan->tadr != 0xFFFFFFFF) {
        chan->tadr = addr;
    }
    chan->qwc = 0;
    chan->chcr = ((int)chan->chcr & ~0xC) | 0x105;
}

void sceDmaSendN(sceDmaChan *chan, void *data, u32 qwc)
{
    u32 addr = CheckAddress(EE_ADDR(data));

    WaitDma(chan);
    if (chan->madr != 0xFFFFFFFF) {
        chan->madr = addr;
    }
    chan->qwc = qwc;
    chan->chcr = ((int)chan->chcr & ~0xC) | 0x101;
}

void sceDmaSendI(sceDmaChan *chan, void *data, u32 qwc)
{
    u32 addr = CheckAddress(EE_ADDR(data));

    WaitDma(chan);
    if (chan->madr != 0xFFFFFFFF) {
        chan->madr = addr;
    }
    chan->qwc = qwc;
    chan->chcr = ((int)chan->chcr & ~0xC) | 0x109;
}

void sceDmaRecv(sceDmaChan *chan)
{
    WaitDma(chan);
    chan->qwc = 0;
    chan->chcr = ((((int)chan->chcr & ~0xC) | 0x4) & ~1) | 0x100;
}

void sceDmaRecvN(sceDmaChan *chan, void *data, u32 qwc)
{
    u32 addr = CheckAddress(EE_ADDR(data));

    WaitDma(chan);
    if (chan->madr != 0xFFFFFFFF) {
        chan->madr = addr;
    }
    chan->qwc = qwc;
    recv_setup(chan, 0);
}

void sceDmaRecvI(sceDmaChan *chan, void *data, u32 qwc)
{
    u32 addr = CheckAddress(EE_ADDR(data));

    WaitDma(chan);
    if (chan->madr != 0xFFFFFFFF) {
        chan->madr = addr;
    }
    chan->qwc = qwc;
    chan->chcr = ((((int)chan->chcr & ~0xC) | 0x8) & ~1) | 0x100;
}

int sceDmaSync(sceDmaChan *chan, int mode, int unused)
{
    if (mode == 1) {
        return (chan->chcr >> 8) & 1;
    }
    WaitDma(chan);
    return 0;
}

int sceDmaWatch(sceDmaChan *chan, u32 addr, int mode, int timeout)
{
    if (mode == 1) {
        return chan->madr < addr;
    }
    WatchDma(chan, addr, timeout);
    return 0;
}

int sceDmaPause(sceDmaChan *chan)
{
    int intr = DIntr();
    u32 enable = *D_ENABLER;
    u32 ctrl;
    u32 chcr;

    if ((enable & 0x10000) == 0) {
        *D_ENABLEW = enable | 0x10000;
    }
    ctrl = *VD_CTRL;
    chcr = CHCR(chan);
    CHCR(chan) = chcr & ~0x100;
    *D_ENABLEW = enable;
    if (intr) {
        EIntr();
    }
    return chcr;
}

int sceDmaRestart(sceDmaChan *chan, u32 chcr)
{
    u32 old = chan->chcr;

    chan->chcr = chcr;
    return (old >> 8) & 1;
}

const int sceDmaChanResetFlag[10] = { 1, 1, 1, 1, 1, 0, 0, 0, 1, 1 };

INCLUDE_RODATA(const s32, "sdk/libdma/libdma", D_00465C40);

INCLUDE_RODATA(const s32, "sdk/libdma/libdma", D_00465C50);

INCLUDE_RODATA(const s32, "sdk/libdma/libdma", D_00465C60);

