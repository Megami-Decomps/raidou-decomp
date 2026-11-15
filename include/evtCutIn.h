#ifndef EVTCUTIN_H
#define EVTCUTIN_H

#include "common.h"
#include "kernel.h"

void evtEndCommand(dds3ProcessID_t *PID);
dds3ProcessID_t *evtStartCommand(dds3ProcessID_t *ParentPID, s32 X, s32 Y, s32 Type, u32 UID);

#endif
