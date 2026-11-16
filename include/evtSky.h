#ifndef EVTSKY_H
#define EVTSKY_H

#include "common.h"
#include "kernel.h"

void evtExitSky(void);
s32 evtSkyProcess1(dds3ProcessID_t *id);
void evtSkyShutdown(dds3ProcessID_t *id);
dds3ProcessID_t *evtInitSky(void);
void evtSetSkyAlpha(s32 frame, s32 a);
void evtSetBokasi(s32 sw);
u16 evtCheckSkyAlphaAnime(void);

extern s32 fldbokasi;

#endif
