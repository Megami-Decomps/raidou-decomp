#ifndef SDF_PADMANAGER_H
#define SDF_PADMANAGER_H

#include "common.h"

void sdfPadInitManager(void);
void sdfPadManager(void);
void sdfPadSetAct0(s32 pad, u16 value);
void sdfPadSetAct1(s32 pad, u8 value);
void sdfPadSetAct(s32 pad, s32 value0, s32 value1);
void sdfPadWork(void);

#endif
