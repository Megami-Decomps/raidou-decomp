#ifndef EVTCUTIN_H
#define EVTCUTIN_H

#include "common.h"
#include "kernel.h"

typedef struct
{
    u32 cutin_uid[16];
    dds3ProcessID_t *cutin_pid[16];
} evtCutInData_t;

extern evtCutInData_t Data_evtCutIn;
extern char *evtCutInCreateData[];
extern char evtCutInCreateData2[];
extern char evtCutInCreateData3[];
extern char evtStrCutInDestructor[];

s32 GetIndexFromUID(evtCutInData_t *pData, u32 UID);
s32 GetRegistNums(void);
void Callback_evtCutIn(u32 UID);
s32 DestructProcess_evtCutIn(dds3ProcessID_t *PID);
void DestructDestroy_evtCutIn(dds3ProcessID_t *PID);
dds3ProcessID_t *evtGetPIDfromUID(u32 UID);
void evtEndCommand(dds3ProcessID_t *PID);
dds3ProcessID_t *evtStartCommand(dds3ProcessID_t *ParentPID, s32 X, s32 Y, s32 Type, u32 UID);

#endif
