/* Declarations of smt3-decomp src/smt3/sdk/libdma.c, for its bodies in this directory. */
#ifndef SHARED_SDK_LIBDMA_H
#define SHARED_SDK_LIBDMA_H

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

#endif
