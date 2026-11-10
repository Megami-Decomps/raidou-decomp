/* Declarations of smt3-decomp src/smt3/sdk/libpad.c, for its bodies in this directory. */
#ifndef SHARED_SDK_LIBPAD_H
#define SHARED_SDK_LIBPAD_H

#include "common.h"

/* One half of a port's double-buffered pad data DMA buffer. */
typedef struct padDma {
    u8 data[0x30];     /* 0x00 button data, scePadRead copies length bytes */
    u8 actInfo[4][4];  /* 0x30 */
    u8 combInfo[4][4]; /* 0x40 */
    u16 modeTable[4];  /* 0x50 */
    s32 frame;         /* 0x58 */
    u8 pad5c[4];
    u32 length;        /* 0x60 */
    u8 ok2;            /* 0x64 */
    u8 modeCurId;      /* 0x65 */
    u8 f66;            /* 0x66 */
    u8 f67;
    u8 nrMode;         /* 0x68 */
    u8 modeCurOffs;    /* 0x69 */
    u8 nrAct;          /* 0x6A */
    u8 nrComb;         /* 0x6B */
    u8 pad6c[4];
    u8 state;          /* 0x70 */
    u8 reqState;       /* 0x71 */
    u8 ok;             /* 0x72 */
    u8 pad73[6];
    u8 mask[4];        /* 0x79 */
    u8 pad7d[3];
} padDma;

/* The 0x20-byte command block DMAd to the IOP. */
typedef struct padCmd {
    u32 seq;       /* 0x00 */
    u32 cmd;       /* 0x04 */
    u32 len;       /* 0x08 */
    u8 data[0x14]; /* 0x0C */
} padCmd;

typedef struct padState {
    padDma *dma; /* 0x00 */
    padCmd *cmd; /* 0x04 */
    u32 iopAddr; /* 0x08 */
    s32 dmaId;   /* 0x0C */
    s32 open;    /* 0x10 */
    s32 f14;
    s32 f18;
} padState;

typedef union padRpcArgs {
    s32 w[5];
    u8 b[0x14];
} padRpcArgs;

typedef struct padRpc {
    s32 cmd;      /* 0x00 */
    s32 port;     /* 0x04 */
    s32 slot;     /* 0x08 */
    padRpcArgs a; /* 0x0C */
    u8 pad20[0x60];
} padRpc;

typedef struct vrefParam {
    u8 b[12];
} vrefParam;

/* padPortState is the per-port state array, padRpcPacket the RPC packet shared by
 * every call into the IOP module, padRpcClient its sceSif client data, and
 * padInitialized the "libpad is initialised" flag. padStateNames and padRequestNames are
 * the state and request name tables, D_0054AA80 the empty string both
 * stringify functions fall back to. */
extern padState padPortState[2][4] __attribute__((section(".data")));

typedef struct padRpcClient_t {
    u32 reserved[9];
    void *server;
} padRpcClient_t;

extern padRpcClient_t padRpcClient[2] __attribute__((section(".data")));

extern padRpc padRpcPacket __attribute__((section(".data")));

extern s32 padInitialized __attribute__((section(".data")));

extern const char *padStateNames[8] __attribute__((section(".data")));

extern const char *padRequestNames[4] __attribute__((section(".data")));

extern void SyncDCache(void *start, void *end);

extern void *memcpy(void *a, const void *b, size_t c);

extern char *strcpy(char *dst, const char *src);

extern s32 sceSifCallRpc(void *cd, u32 fno, u32 mode, void *send, s32 ssize,
                         void *recv, s32 rsize, void *endfunc, void *endpara);

extern int _send_to_iop(int port, int slot);

/* padCmdBlock is the command-block array the IOP DMAs from, padDebugPrint the
 * debug-print flag used by initialization and command diagnostics. */
typedef struct sceSifDmaData {
    void *src;
    void *dest;
    s32 size;
    s32 attr;
} sceSifDmaData;

extern padCmd padCmdBlock[2][4] __attribute__((section(".data")));

extern s32 padDebugPrint __attribute__((section(".data")));

extern void *memset(void *a, int b, size_t c);

extern int scePrintf(const char *fmt, ...);

extern s32 sceSifDmaStat(u32 id);

extern u32 sceSifSetDma(sceSifDmaData *data, s32 count);

extern const char padDmaFailMsg[];

int _send_to_iop(int port, int slot);

extern s32 sceSifBindRpc(void *client, u32 service, s32 mode);

extern int scePadGetModVersion(void);

extern int scePadInit2(int mode);

int scePadInit(int mode);

int scePadInit2(int mode);

int scePadEnd(void);

int scePadPortClose(int port, int slot);

/* The IOP writes the newer frame counter into whichever half it filled last,
 * so the reader picks the half with the higher count. */
padDma *scePadGetDmaStr(int port, int slot);

u32 scePadGetFrameCount(int port, int slot);

int scePadRead(int port, int slot, u8 *data);

int scePadGetState(int port, int slot);

void scePadStateIntToStr(u32 state, char *str);

int scePadSetReqState(int port, int slot, int state);

int scePadGetReqState(int port, int slot);

void scePadReqIntToStr(u32 req, char *str);

int scePadInfoAct(int port, int slot, int actno, int term);

int scePadInfoComb(int port, int slot, int listno, int offs);

int scePadInfoMode(int port, int slot, int term, int offs);

int scePadSetMainMode(int port, int slot, int offs, int lock);

int scePadSetActDirect(int port, int slot, const u8 *data);

int scePadSetActAlign(int port, int slot, const u8 *align);

u32 scePadGetButtonMask(int port, int slot);

int scePadSetButtonInfo(int port, int slot, u32 buttonInfo);

int scePadInfoPressMode(int port, int slot);

int scePadEnterPressMode(int port, int slot);

int scePadExitPressMode(int port, int slot);

int scePadSetVrefParam(int port, int slot, const vrefParam *param);

int scePadGetPortMax(void);

int scePadGetSlotMax(int port);

int scePadGetModVersion(void);

int scePadSetWarningLevel(int level);

#endif
