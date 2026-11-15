#ifndef EVTMOON_H
#define EVTMOON_H

#include "common.h"

void evtDisableMoonCalc(void);
void evtDisableMoonDraw(void);
void evtEnableMoonCalc(void);
s32 evtGetAgeOfMoon(void);
s32 evtGetAgeOfMoon16(void);
void evtSetAgeOfMoon(s32 age);

void CalcBlendColor(u32 *pDstCol, u32 *pSrcCol, s32 AlphaRate);
void PreCalcFade(s32 Frame);
void evtDisableMoonDraw2(void);
void evtEnableMoonDraw(void);
void evtEnableMoonDraw2(void);
void evtMoonAddMove(f32 leng);
s32 evtMoonFadeCheck(void);
void evtMoonFadeIn(s32 Frame);
void evtMoonFadeOut(s32 Frame);
void evtMoonInitProcess(void);

#endif
