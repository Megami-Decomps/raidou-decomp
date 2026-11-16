#ifndef EVTEVENTPACK_H
#define EVTEVENTPACK_H

#include "common.h"
#include "kernel.h"

typedef struct evtPackEntry_t {
    s32 kind;
    s32 dataSize;
    s32 cueCount;
    u32 dataOffset;
    s32 secondaryResourceId;
    s32 resourceId;
    u8 pad18[4];
    s32 motionId;
} evtPackEntry_t;

typedef struct evtPackHeader_t {
    u8 pad00[0x10];
    s32 entryCount;
    u8 pad14[0xC];
    evtPackEntry_t entries[0];
} evtPackHeader_t;

s32 evtGetEventPackStat(u32 eventno);
void *evtGetResorceData(u32 eventno, s32 rid);
s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);
void evtKillEventPack(s32 eventno);
dds3ProcessID_t *evtLoadEventPack(s32 eventno);
dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);
s32 evtRegistBEField(s32 eventno, s32 rid);
dds3ProcessID_t *evtSearchEventPack(u32 eventno);

#endif
