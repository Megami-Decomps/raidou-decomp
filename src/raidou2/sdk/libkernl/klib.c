#include "common.h"
#include "sdk_libkernl.h"

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _ExecPS2);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RFU008);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", AddSbusIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RemoveSbusIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", Interrupt2Iop);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetVTLBRefillHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetVCommonHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetVInterruptHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf2);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibDmacAddHandlerSub);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RemoveIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", AddDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibDmacAddHandlerSub2);

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

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iSetAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf3);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", CreateThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf4);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _StartThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ExitThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ExitDeleteThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", TerminateThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iTerminateThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf5);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf6);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ChangeThreadPriority);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iChangeThreadPriority);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RotateThreadReadyQueue);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf7);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf8);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GetThreadId);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ReferThreadStatus);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iReferThreadStatus);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SleepThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", WakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", CancelWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iCancelWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SuspendThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _iSuspendThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", ResumeThread);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf9);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf10);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf11);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf12);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", EndOfHeap);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", RFU063);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", CreateSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", DeleteSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SignalSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iSignalSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", WaitSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", PollSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iPollSema);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf13);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf14);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf15);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetOsdConfigParam);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GetOsdConfigParam);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GetGsHParam);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf16);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf17);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf18);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf19);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf20);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf21);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf22);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf23);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf24);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf25);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf26);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf27);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf28);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf29);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf30);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf31);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf32);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf33);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf34);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf35);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf36);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf37);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf38);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf39);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf40);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf41);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf42);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", FlushCache);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", CpuConfig);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf43);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf44);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf45);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifStopDma);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetCPUTimerHandler);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf46);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf47);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf48);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GsGetIMR);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iGsGetIMR);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GsPutIMR);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", iGsPutIMR);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf49);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", SetVSyncFlag);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf50);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf51);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifDmaStat);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", isceSifDmaStat);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifSetDma);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", isceSifSetDma);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifSetDChain);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", isceSifSetDChain);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifSetReg);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", sceSifGetReg);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf52);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", Deci2Call);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", PSMode);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", klibLeaf53);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", GetMemorySize);

INCLUDE_ASM(const s32, "sdk/libkernl/klib", _InitTLB);
