#ifndef DDS_SDK_LIBKERNL_H

#define DDS_SDK_LIBKERNL_H
/* SDK kernel: threads, semaphores, interrupts, alarms, file io and SIF (libkernl). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

#include "sce_io.h"

#include "sdf_kernel_thread_param.h"

#ifndef SDK_SKIP_AddSbusIntcHandler

#endif

#ifndef SDK_SKIP_CancelWakeupThread

#endif

#ifndef SDK_SKIP_ChangeThreadPriority

#endif

#ifndef SDK_SKIP_CreateSema

#endif

#ifndef SDK_SKIP_CreateThread

#endif

#ifndef SDK_SKIP_DIntr

#endif

#ifndef SDK_SKIP_DeleteThread

#endif

#ifndef SDK_SKIP_EIntr

#endif

#ifndef SDK_SKIP_EnableIntc

#endif

#ifndef SDK_SKIP_Exit

#endif

#ifndef SDK_SKIP_ExitDeleteThread

#endif

#ifndef SDK_SKIP_FlushCache

#endif

#ifndef SDK_SKIP_GetThreadId

#endif

#ifndef SDK_SKIP_LoadExecPS2

#endif

#ifndef SDK_SKIP_RemoveDmacHandler

#endif

#ifndef SDK_SKIP_RemoveIntcHandler

#endif

#ifndef SDK_SKIP_RemoveSbusIntcHandler

#endif

#ifndef SDK_SKIP_SetAlarm

#endif

#ifndef SDK_SKIP_SignalSema

#endif

#ifndef SDK_SKIP_SleepThread

#endif

#ifndef SDK_SKIP_TerminateThread

#endif

#ifndef SDK_SKIP_WaitSema

#endif

#ifndef SDK_SKIP_WakeupThread

#endif

#ifndef SDK_SKIP__StartThread

#endif

#ifndef SDK_SKIP_iSignalSema

#endif

#ifndef SDK_SKIP_iWakeupThread

#endif

#ifndef SDK_SKIP_sceClose

#endif

#ifndef SDK_SKIP_sceDclose

#endif

#ifndef SDK_SKIP_sceDopen

#endif

#ifndef SDK_SKIP_sceDread

#endif

#ifndef SDK_SKIP_sceFsReset

#endif

#ifndef SDK_SKIP_sceGetstat

#endif

#ifndef SDK_SKIP_sceOpen

#endif

#ifndef SDK_SKIP_sceSifAllocIopHeap

#endif

#ifndef SDK_SKIP_sceSifDmaStat

#endif

#ifndef SDK_SKIP_sceSifGetSreg

#endif

#ifndef SDK_SKIP_sceSifInitIopHeap

#endif

#ifndef SDK_SKIP_sceSifInitRpc

#endif

#ifndef SDK_SKIP_sceSifLoadFileReset

#endif

#ifndef SDK_SKIP_sceSifLoadModule

#endif

#ifndef SDK_SKIP_sceSifRebootIop

s32 sceSifRebootIop(const char *iopImagePath);

#endif

#ifndef SDK_SKIP_sceSifRegisterRpc

#endif

#ifndef SDK_SKIP_sceSifRpcLoop

#endif

#ifndef SDK_SKIP_sceSifSendCmd

#endif

#ifndef SDK_SKIP_sceSifSetDma

#endif

#ifndef SDK_SKIP_sceSifSetRpcQueue

#endif

#ifndef SDK_SKIP_sceSync

#endif

#ifndef SDK_SKIP_sceWrite

#endif

#ifndef SDK_SKIP_sdfDevSeekFile

#endif

#ifndef SDK_SKIP_DisableDmac

#endif

#ifndef SDK_SKIP_EnableDmac

#endif

#ifdef VERSION_DDS2

#ifndef SDK_SKIP_sdfDevReadFileBytes

#endif

#endif

#endif
