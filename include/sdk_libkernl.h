#ifndef SDK_LIBKERNL_H
#define SDK_LIBKERNL_H

#include "common.h"

/* DMA bus addresses and transfer controls occupy four 32-bit words. */
typedef struct sifDmaTransfer_t {
    u32 source;
    u32 destination;
    s32 size;
    u32 attributes;
} sifDmaTransfer_t;

SIZE_CHECK(sifDmaTransfer_size_must_be_16, sifDmaTransfer_t, 16);

struct sifRpcServerData_t;
struct sifRpcDataQueue_t;
typedef void *(*sceSifRpcFunc)(s32 function, void *request, s32 size);
s32 AddDmacHandler2(s32 channel, s32 (*handler)(s32, void *, void *),
                    s32 next, void *argument);
s32 _StartThread(s32 threadId, void *argument);
s32 sceDevctl(const char *device, s32 command, const void *arguments,
              u32 argumentSize, void *result, u32 resultSize);
void sceSifRegisterRpc(struct sifRpcServerData_t *server, u32 serviceId,
                        sceSifRpcFunc handler, void *buffer, sceSifRpcFunc callback,
                        void *callbackBuffer, struct sifRpcDataQueue_t *queue);
s32 sceWrite(s32 descriptor, const void *source, s32 size);

s32 sceOpen(const char *name, s32 flags, ...);

s32 sceSifSetDma(sifDmaTransfer_t *transfers, s32 count);

s32 sceClose(s32 descriptor);
s32 sceDclose(s32 descriptor);
s32 sceDopen(const char *path);
s32 sceDread(s32 descriptor, void *directoryEntry);
s32 sceGetstat(const char *path, void *stat);
s32 sceLseek(s32 fileHandle, s32 offset, s32 origin);
s32 sceSync(const char *path, s32 mode);

void kprintf(const char *format, ...);
void scePrintf(const char *format, ...);

s32 InitThread(void);
s32 iWakeupThread(s32 thid);

s32 DisableDmac(s32 channel);
void DisableIntc(s32 channel);
void EnableDmac(s32);
void EnableIntc(s32 channel);

s32 sceSifAllocIopHeap(s32 size);
s32 sceSifInitIopHeap(void);

s32 AddSbusIntcHandler(s32 channel, s32 handler);
s32 CancelWakeupThread(s32 thid);
void ChangeThreadPriority(s32 thid, s32 prio);
s32 Deci2Call(s32 command, void *packet);
s32 DeleteSema(s32 sema);
void ExitDeleteThread(void);
void FlushCache(s32 mode);
void GetOsdConfigParam(u32 *param);
s32 GetThreadId(void);
u64 GsGetIMR(void);
u64 GsPutIMR(u64 imr);
s32 PollSema(s32 sema);
s32 ReferThreadStatus(s32 threadId, void *status);
s32 RemoveDmacHandler(s32 channel, s32 handlerId);
void RemoveIntcHandler(s32 channel, s32 handlerId);
void RemoveSbusIntcHandler(s32 channel);
s32 RotateThreadReadyQueue(s32 priority);
s32 SetAlarm(u16 delay, void (*callback)(s32, s32, s32), void *argument);
void SetOsdConfigParam(const u32 *param);
void *SetVCommonHandler(s32 index, void *handler);
void SetVSyncFlag(volatile u32 *flag, volatile u64 *result);
void *SetVTLBRefillHandler(s32 index, void *handler);
s32 SignalSema(s32 sema);
s32 SleepThread(void);
s32 SuspendThread(s32 threadId);
void TerminateThread(s32 threadId);
s32 WaitSema(s32 sema);
s32 WakeupThread(s32 threadId);
s32 _ExecPS2(void *entry, void *gp, s32 count, char **arguments);
s32 iSignalSema(s32 sema);
s32 sceSifDmaStat(s32 transferId);
u32 sceSifGetReg(u32 registerId);
s32 sceSifSetReg(u32 registerId, u32 value);
void sceSifStopDma(void);

s32 isceSifSendCmd(s32 command, void *packet, s32 packetBytes, void *source, void *destination, s32 dataBytes);
void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);
void sceSifExitCmd(void);
u32 sceSifGetSreg(s32 index);
void sceSifRemoveCmdHandler(s32 command);
s32 sceSifSendCmd(s32 command, void *packet, s32 packetBytes, void *source, void *destination, s32 dataBytes);
void sceSifWriteBackDCache(void *address, s32 size);

s32 sceDeci2ExRecv(s32 handle, void *buffer, u16 length);
s32 sceDeci2ExSend(s32 handle, void *buffer, u16 length);
s32 sceDeci2Poll(s32 handle);
s32 sceDeci2ReqSend(s32 handle, s8 destination);

s32 sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments);
s32 sceSifLoadStartModule(const char *path, s32 argumentSize, const void *arguments, s32 *moduleResult);

void InitTLB(void);
void InitTLBFunctions(void);

void InitExecPS2(void);
void PatchIsNeeded(u32 index, void *entry);

void _kTLBException();

void sceResetttyinit(void);

void SyncDCache(void *start, void *end);

void InitAlarm(void);

#endif /* SDK_LIBKERNL_H */
