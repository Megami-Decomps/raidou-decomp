#ifndef EVTEVENTPACK_H
#define EVTEVENTPACK_H

#include "common.h"
#include "kernel.h"

s32 evtGetEventPackStat(u32 eventno);
void *evtGetResorceData(u32 eventno, s32 rid);
s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);
void evtKillEventPack(s32 eventno);
dds3ProcessID_t *evtLoadEventPack(s32 eventno);
dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);
s32 evtRegistBEField(s32 eventno, s32 rid);
dds3ProcessID_t *evtSearchEventPack(u32 eventno);

#endif
