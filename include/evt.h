#ifndef EVT_H
#define EVT_H

#include "common.h"
#include "kernel.h"

void CrossFadeShutdown(dds3ProcessID_t *PID);
s32 evtCrossFadeGetStat(void);
dds3ProcessID_t *evtStartCrossFade(dds3ProcessID_t *ParentPID, DDS3_PRIORITY Priority, s32 Length);
void evtStartRain(s32 Type);

#endif
