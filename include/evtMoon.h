#ifndef EVTMOON_H
#define EVTMOON_H

#include "common.h"

void evtDisableMoonCalc(void);
void evtDisableMoonDraw(void);
void evtEnableMoonCalc(void);
s32 evtGetAgeOfMoon(void);
s32 evtGetAgeOfMoon16(void);
void evtSetAgeOfMoon(s32 age);

#endif
