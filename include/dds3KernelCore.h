#ifndef DDS3KERNELCORE_H
#define DDS3KERNELCORE_H

#include "common.h"
#include "kernel.h"

s32 dds3ExistProcess(dds3ProcessID_t *id);
u32 dds3GetProcessCount(u32 type);
void *dds3GetProcessData(dds3ProcessID_t *id);
u32 dds3GetProcessRunTime(dds3ProcessID_t *id);
u32 dds3GetProcessStat(dds3ProcessID_t *id);
s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
s32 dds3KillProcessName(char *name, s32 shutdowntimer);
void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
dds3ProcessID_t *dds3SearchProcessName(const char *namestr);
dds3ProcessID_t *dds3SearchProcessNumber(u32 type, s32 n);
void dds3SetProcessData(dds3ProcessID_t *id, void *data);
dds3ProcessID_t *dds3StartProcess(const char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3, dds3ProcessFunc_t drawExit, dds3ProcessFunc_t deleteExit, void *p);

#endif
