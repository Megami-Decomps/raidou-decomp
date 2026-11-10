#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _ExecPS2);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C600);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C610);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", AddSbusIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RemoveSbusIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C640);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetVTLBRefillHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetVCommonHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C670);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C680);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C690);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RemoveIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C6B0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C6C0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RemoveDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _EnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _DisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _EnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _DisableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ReleaseAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iEnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iDisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iEnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iDisableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C780);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C790);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", CreateThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C7B0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _StartThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C7D0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ExitDeleteThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", TerminateThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C800);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C810);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C820);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ChangeThreadPriority);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C840);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RotateThreadReadyQueue);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C860);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C870);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C880);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GetThreadId);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ReferThreadStatus);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C8B0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SleepThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", WakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", CancelWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C900);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SuspendThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iSuspendThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C930);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C940);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C950);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C960);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C970);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", EndOfHeap);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040C990);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", CreateSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", DeleteSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SignalSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iSignalSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", WaitSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", PollSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CA00);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CA10);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CA20);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CA30);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetOsdConfigParam);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GetOsdConfigParam);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CA60);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CA70);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CA80);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CA90);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CAA0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CAB0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CAC0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CAD0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CAE0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CAF0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CB00);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CB10);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CB20);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CB30);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CB40);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CB50);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CB60);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CB70);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CB80);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CB90);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CBA0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CBB0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CBC0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CBD0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CBE0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CBF0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CC00);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CC10);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", FlushCache);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CC30);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CC40);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CC50);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CC60);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifStopDma);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CC80);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CC90);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CCA0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CCB0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GsGetIMR);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CCD0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GsPutIMR);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CCF0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CD00);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetVSyncFlag);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CD20);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CD30);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifDmaStat);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CD50);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifSetDma);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", isceSifSetDma);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifSetDChain);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", isceSifSetDChain);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifSetReg);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifGetReg);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CDC0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", Deci2Call);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CDE0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_0040CDF0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GetMemorySize);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _InitTLB);
