#ifndef EVTSOUND_H
#define EVTSOUND_H

#include "common.h"

s32 evtCheckEventBgmLoaded(s32 eventId);
s32 evtLoadEventBgm(s32 eventId);
void evtMakeEventPackName(s32 index, char *buffer);
s32 evtPlayEventBgm(s32 eventId, s32 variation);
s32 evtPlaySe(s32 eventno, s32 seno);
s32 evtStopEventBgm(s32 eventId, s32 variation);
void evtTransEventBgm(s32 eventId, s32 variation);

s32 evtStopSe(s32 eventno, s32 seno);

#endif
