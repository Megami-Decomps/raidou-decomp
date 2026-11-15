#ifndef SDK_LIBGRAPH_H
#define SDK_LIBGRAPH_H

#include "common.h"

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

typedef struct sceGsLoadImage {
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

typedef struct sceGsStoreImage {
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

extern GParam sceGsGParam;

void sceGsResetGraph(s16 mode, s16 interlace, s16 outputMode, s16 fieldMode);
GParam *sceGsGetGParam(void);
void sceGsResetPath(void);
s32 checkModelVersion(void);
s32 sceGsSyncPath(s32 mode, s32 timeout);
s32 sceGsSetDefLoadImage(sceGsLoadImage *p, s16 dbp, s16 dbw, s16 dpsm, s16 dsax, s16 dsay,
                         s16 rrw, s16 rrh);
s32 sceGsSetDefStoreImage(sceGsStoreImage *p, s16 sbp, s16 sbw, s16 spsm, s16 ssax, s16 ssay,
                          s16 rrw, s16 rrh);
s32 sceGsExecLoadImage(sceGsLoadImage *pkt, void *addr);
s32 sceGsExecStoreImage(sceGsStoreImage *pkt, void *addr);
u64 sceGsGetIMR(void);
u64 sceGsPutIMR(u64 imr);

#endif
