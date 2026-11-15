#ifndef SDF_PADMANAGER_H
#define SDF_PADMANAGER_H

#include "common.h"

void sdfPadInitManager(void);
void sdfPadManager(void);
void sdfPadSetAct0(s32 pad, u16 value);
void sdfPadSetAct1(s32 pad, u8 value);
void sdfPadWork(void);

#endif
