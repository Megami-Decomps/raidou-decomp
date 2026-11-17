#ifndef EVTMOON_H
#define EVTMOON_H

#include "common.h"
#include "kernel.h"

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
s32 Process1_evtMoon(dds3ProcessID_t *id);
s32 Process2_evtMoon(dds3ProcessID_t *id);

extern struct sprHandle_t *moon_spr;
extern s32 gMoonBaseARate;
extern f32 Process2_evtMoonData7;
extern s32 gMoonCngCnt;
extern s32 gMoonAgeOld;
extern s32 gMoonAge16Old;

#ifdef VERSION_RAIDOU2
extern s32 Process2_evtMoonData8;
void evtBtlBarCtrlSub(s32 ot);
#endif

#endif
