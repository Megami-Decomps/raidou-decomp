#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _ExecPS2);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393780);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393790);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", AddSbusIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RemoveSbusIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_003937C0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetVTLBRefillHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetVCommonHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_003937F0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393800);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393810);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RemoveIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393830);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393840);

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

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393900);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393910);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", CreateThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393930);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _StartThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393950);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ExitDeleteThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", TerminateThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393980);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393990);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_003939A0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ChangeThreadPriority);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_003939C0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RotateThreadReadyQueue);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_003939E0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_003939F0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393A00);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GetThreadId);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ReferThreadStatus);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393A30);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SleepThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", WakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", CancelWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393A80);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SuspendThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iSuspendThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393AB0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393AC0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393AD0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393AE0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393AF0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", EndOfHeap);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393B10);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", CreateSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", DeleteSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SignalSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iSignalSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", WaitSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", PollSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393B80);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393B90);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393BA0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393BB0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetOsdConfigParam);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GetOsdConfigParam);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393BE0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393BF0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393C00);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393C10);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393C20);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393C30);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393C40);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393C50);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393C60);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393C70);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393C80);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393C90);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393CA0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393CB0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393CC0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393CD0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393CE0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393CF0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393D00);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393D10);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393D20);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393D30);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393D40);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393D50);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393D60);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393D70);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393D80);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393D90);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", FlushCache);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393DB0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393DC0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393DD0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393DE0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifStopDma);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393E00);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393E10);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393E20);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393E30);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GsGetIMR);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393E50);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GsPutIMR);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393E70);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393E80);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetVSyncFlag);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393EA0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393EB0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifDmaStat);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393ED0);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifSetDma);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", isceSifSetDma);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifSetDChain);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", isceSifSetDChain);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifSetReg);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifGetReg);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393F40);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", Deci2Call);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393F60);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", func_00393F70);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GetMemorySize);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _InitTLB);
