/* Declarations of smt3-decomp src/smt3/kernel/dds3KernelCore.c, for its bodies in this directory. */
#ifndef SHARED_KERNEL_DDS3KERNELCORE_H
#define SHARED_KERNEL_DDS3KERNELCORE_H

#include "common.h"

#include "kernel.h"

#include "sdfFrame.h"

/* Two ports, two sticks per port, and two axes per stick. */
extern s8 dds3PadButtonPrev[2][2][16];

extern u8 dds3PadButtonState[2][2][16];

extern u8 dds3PadStickPrev[2][2][2];

extern s8 dds3PadStickStatusPrev[2][2];

extern u8 dds3PadStickState[2][2][2];

extern u8 dds3PadStickStatusState[2][2];

extern dds3ProcessID_t *startinglistf;

extern dds3ProcessID_t *startinglistl;

extern u32 startinglistcnt;

extern dds3ProcessID_t *endinglistf;

extern dds3ProcessID_t *endinglistl;

extern u32 endinglistcnt;

extern dds3ProcessID_t *runprocess;

extern dds3ProcessID_t *runninglistf;

extern dds3ProcessID_t *runninglistl;

extern u32 runninglistcnt;

void DeleteProcessList(dds3ProcessID_t *setp);

void AddProcessList(dds3ProcessID_t *setp);

void StartProcess(dds3ProcessID_t *setp);

void StartingProcess(void);

void KillProcess(dds3ProcessID_t *setp);

void EndProcess(dds3ProcessID_t *setp);

void dds3UnlinkProcess(dds3ProcessID_t *target);

s32 RunProcess(dds3ProcessID_t *pro);

void RunningProcess(void);

void dds3EndingProcess(void);

void ChangeProcessFlag(s32 change, dds3ProcessID_t *target, u32 flag, s32 type);

s32 dds3ExistProcess(dds3ProcessID_t *id);

dds3ProcessID_t *dds3SearchProcessName(char *namestr);

s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

void ChangeProcessFlagChild(s32 change, dds3ProcessID_t *parent, u32 flag);

void *sdfChipAllocDirect(u32 aSize);

void sdfChipFreeDirect(void *aPtr);

#include "sdk_libc.h"

extern char dds3DebugMesFmt[];

extern char dds3DebugStatOpen[];

extern char dds3DebugFlagCharDot[];

extern char dds3DebugFlagCharP[];

extern char dds3DebugFlagCharS[];

extern char dds3DebugFieldClose[];

extern char dds3DebugBlockEnd[];

extern char dds3DebugTreeLineFmt[];

extern char dds3ProcessTreeDepth[];

void DebugProcess(dds3ProcessID_t *pro, const char *mes);

void DeleteProcessList(dds3ProcessID_t *setp);

void AddProcessList(dds3ProcessID_t *setp);

void StartProcess(dds3ProcessID_t *setp);

void StartingProcess(void);

s32 RunProcess(dds3ProcessID_t *pro);

void RunningProcess(void);

void KillProcess(dds3ProcessID_t *setp);

void EndProcess(dds3ProcessID_t *setp);

void dds3EndingProcess(void);

void ChangeProcessFlagChild(s32 change, dds3ProcessID_t *parent, u32 flag);

/* The 0x0FFFFFF0 mask keeps callers out of the state nibble and the top four
 * bits of flag. */
void ChangeProcessFlag(s32 change, dds3ProcessID_t *target, u32 flag, s32 type);

u32 dds3GetProcessCount(u32 type);

dds3ProcessID_t *dds3SearchProcessNumber(u32 type, s32 n);

void DebugProcess(dds3ProcessID_t *pro, const char *mes);

void dds3PrintProcessList(void);

void DebugProcessTree(dds3ProcessID_t *ps, s32 level);

void dds3PrintProcessTree(void);

s32 dds3ProcessMain(void);

/* namehash is the name's byte sum, which dds3SearchProcessName uses to reject
 * most candidates without a compare. */
dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                                  dds3ProcessFunc_t drawExit, dds3ProcessFunc_t deleteExit,
                                  void *p);

s32 dds3KillProcessName(char *name, s32 shutdowntimer);

s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

void dds3EndProcess(dds3ProcessID_t *id);

void dds3SetShutdownTimer(dds3ProcessID_t *id, s32 newtimer);

u32 dds3GetProcessStat(dds3ProcessID_t *id);

/* The name compare runs back to front, from the terminator down to byte 0. */
dds3ProcessID_t *dds3SearchProcessName(char *namestr);

dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);

s32 dds3ExistProcess(dds3ProcessID_t *id);

u32 dds3GetProcessStartTime(dds3ProcessID_t *id);

u32 dds3GetProcessRunTime(dds3ProcessID_t *id);

void dds3SetProcessData(dds3ProcessID_t *id, void *data);

void *dds3GetProcessData(dds3ProcessID_t *id);

void dds3DebugNowProcess(void);

void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

void dds3UnlinkProcess(dds3ProcessID_t *target);

void dds3SetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

#endif
