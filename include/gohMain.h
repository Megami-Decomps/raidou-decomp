#ifndef GOHMAIN_H
#define GOHMAIN_H

#include "common.h"
#include "kernel.h"

struct gohData_t;

s32 MahjanInit(dds3ProcessID_t *PID);
dds3ProcessFunc_t MahjanUpdateProcess(dds3ProcessID_t *PID);
s32 evtBDispatchSyncD2(struct gohData_t *pData);
dds3ProcessFunc_t gohAccidentCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohBakBakDestroy(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohCallGetProcessData29(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohCallGetProcessData30(dds3ProcessID_t *PID);
s32 gohCreateRecoverList(struct gohData_t *pData);
s32 gohCreateSwordList(struct gohData_t *pData);
dds3ProcessFunc_t gohCyuuseiDrawProcess(dds3ProcessID_t *PID);
s32 gohCyuuseiStartDirection(struct gohData_t *pData);
s32 gohMiscInit(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRecoverCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRecoverDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRecoverUpdateProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootUpdateProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootWaitCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootWaitDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootWaitUpdateProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohTalkCalcProcess(dds3ProcessID_t *PID);
s32 jmsCheckScrRun(dds3ProcessID_t *PID);
s32 scrLogStart(struct gohData_t *pData);

#endif
