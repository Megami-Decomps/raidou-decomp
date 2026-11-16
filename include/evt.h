#ifndef EVT_H
#define EVT_H

#include "common.h"
#include "kernel.h"

void CrossFadeShutdown(dds3ProcessID_t *PID);
s32 evtCrossFadeGetStat(void);
dds3ProcessID_t *evtStartCrossFade(dds3ProcessID_t *ParentPID, DDS3_PRIORITY Priority, s32 Length);
void evtStartRain(s32 Type);
s32 evtChkUID(s16 no);
s32 evtCutInCreate(s32 *pFrame, dds3ProcessID_t *ParentPID, s32 x, s32 y, s32 z, s32 frame, s32 length, s32 scale, s32 no);
s32 evtScreenBreakCreate(dds3ProcessID_t *ParentPID, DDS3_PRIORITY Priority, s32 Type);

#endif
