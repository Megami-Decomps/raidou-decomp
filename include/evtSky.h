#ifndef EVTSKY_H
#define EVTSKY_H

#include "common.h"
#include "kernel.h"

void evtExitSky(void);
dds3ProcessID_t *evtInitSky(void);
void evtSetSkyAlpha(s32 frame, s32 a);

#endif
