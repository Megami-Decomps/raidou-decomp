/* Declarations of smt3-decomp src/smt3/sdk/libmc.c, for its bodies in this directory. */
#ifndef SHARED_SDK_LIBMC_H
#define SHARED_SDK_LIBMC_H

#include "common.h"

#include "ps2asm.h"

/* The sif client data of the mcserv module; +0x24 is non-zero once bind
 * succeeded, which every call tests before using the RPC. */
typedef struct mcRpcCd {
    u32 pad00[9];
    s32 bound;
} mcRpcCd;

extern mcRpcCd mcRpcClient __attribute__((section(".data")));

/* The 0x30-byte request shared by short commands. Their argument words vary;
 * the write view names the consumed fields and inline transfer prefix. */
typedef union mcReq {
    s32 w[8];
    struct {
        s32 descriptor;
        u8 opaque04[8];
        s32 bulkSize;
        u8 opaque10[4];
        u32 inlineSize;
        const void *bulkBuffer;
        u8 opaque1C[4];
        u8 inlineData[16];
    } write;
} mcReq;

extern mcReq mcRequest __attribute__((section(".data")));

/* The 0x40-byte entry block that sits just before the request: four words of
 * directory entry info and a 0x20-byte name. */
typedef struct mcEntInfo {
    u32 info[8];
    char name[0x20];
} mcEntInfo;

extern mcEntInfo mcEntInfoBuf __attribute__((section(".data")));

/* The name-carrying request: the same header, then a 0x400-byte path. */
typedef struct mcNameReq {
    s32 port;
    s32 slot;
    s32 flag;
    s32 maxent;
    void *pwd;
    char name[0x400];
} mcNameReq;

extern mcNameReq mcNameRequest __attribute__((section(".data")));

/* Read-specific view of the shared callback buffer. The producer supplies
 * the second payload extent; other commands use different reply layouts. */
typedef struct {
    s32 firstLength;
    s32 secondLength;
    u8 *firstDestination;
    u8 *secondDestination;
    u8 firstData[0x40];
    u8 secondData[0];
} mcReadReplyPrefix_t;

/* The 0xC0-byte block the RPC end functions read back, and the 0x400-byte
 * working directory sceMcChdir hands to mceStorePwd. */
extern u8 mcEndBuf[0xC0] __attribute__((section(".data")));

extern u8 mcPwdBuf[0x400] __attribute__((section(".data")));

extern saddr_t mcReplyDst0 __attribute__((section(".data")));

extern saddr_t mcReplyDst1 __attribute__((section(".data")));

extern saddr_t mcReplyDst2 __attribute__((section(".data")));

/* The shared reply begins with result/version words. The client accessor
 * also publishes the command semaphore at the end of its 0x40-byte buffer. */
typedef struct mcReply {
    s32 result;
    s32 serverVersion;
    s32 managerVersion;
    u8 reserved0C[0x30];
    s32 sema;
} mcReply;

extern mcReply mcReplyBuf __attribute__((section(".data")));

extern s32 mcLastCmd __attribute__((section(".data")));

extern s32 mcSema __attribute__((section(".data")));

extern s32 PollSema(s32 sema);

extern s32 SignalSema(s32 sema);

extern s32 DeleteSema(s32 sema);

extern s32 sceSifCallRpc(void *cd, u32 fno, u32 mode, void *send, s32 ssize,
                         void *recv, s32 rsize, void *endfunc, void *endpara);

extern s32 sceMcOpen(s32 a, s32 b, char *c, s32 d);

extern s32 sceSifCheckStatRpc(void *cd);

extern void mcDelayThread(u16 ticks);

extern char *strncpy(char *dst, const char *src, size_t n);

extern int scePrintf(const char *fmt, ...);

/* The chunk this string opens also carries the float constants the next
 * unit needs, so it stays one INCLUDE_RODATA rather than an inline literal. */
extern const char D_0054AEE8[];

extern void sceSifWriteBackDCache(void *addr, s32 size);

extern void FlushCache(s32 mode);

extern void mceIntrReadFixAlign(void *argument);

extern void mceGetInfoApdx(void *reply);

extern void mceStorePwd(char *dest);

typedef struct mcSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} mcSemaParam_t;

extern s32 CreateSema(mcSemaParam_t *param);

extern s32 WaitSema(s32 sema);

extern s32 sceMcSync(s32 mode, s32 *command, s32 *result);

extern void sceSifInitRpc(s32 mode);

extern s32 sceSifBindRpc(mcRpcCd *client, u32 service, s32 mode);

s32 sceMcInit(void);

s32 sceMcEnd(void);

/* Both outputs are generic pointer slots and may alias. */
mcRpcCd *_lmcGetClientPtr(void **reply, void **command);

s32 sceMcChangeThreadPriority(s32 prio);

s32 sceMcGetSlotMax(s32 port);

s32 sceMcOpen(s32 a, s32 b, char *c, s32 d);

s32 sceMcMkdir(s32 a, s32 b, char *c);

s32 sceMcClose(s32 a);

s32 sceMcSeek(s32 fd, s32 offset, s32 origin);

void mceIntrReadFixAlign(void *argument);

s32 sceMcRead(s32 a, void *b, s32 c);

s32 sceMcWrite(s32 a, const void *b, s32 c);

extern s32 iWakeupThread(s32 thread);

extern s32 GetThreadId(void);

extern s32 SetAlarm(u16 ticks, void (*callback)(s32, s32, s32), void *argument);

extern s32 SleepThread(void);

/* SetAlarm delivers the common argument in its third callback register. */
void mcHearAlarm(s32 cause, s32 address, s32 thread);

void mcDelayThread(u16 ticks);

void mceGetInfoApdx(void *response);

s32 sceMcGetInfo(s32 a, s32 b, s32 *c, s32 *d, s32 *e);

s32 sceMcUdCheckNewCard(void);

s32 sceMcGetDir(s32 a, s32 b, char *c, s32 d, s32 e, void *f);

extern u32 strlen(const char *str);

extern void *memcpy(void *dest, const void *src, size_t size);

void mceStorePwd(char *dest);

s32 sceMcChdir(s32 a, s32 b, char *c, char *d);

s32 sceMcFormat(s32 port, s32 slot);

s32 sceMcDelete(s32 port, s32 slot, char *name);

s32 sceMcFlush(s32 port);

s32 sceMcSetFileInfo(s32 port, s32 slot, char *name, mcEntInfo *info, s32 flags);

s32 sceMcRename(s32 port, s32 slot, char *name, char *newname);

s32 sceMcUnformat(s32 port, s32 slot);

s32 sceMcGetEntSpace(s32 port, s32 slot, char *name);

#endif
