#ifndef FLDFILERESOLVER_H
#define FLDFILERESOLVER_H

#include "common.h"

s32 PlayerCalc_Nml(void);
void PlayerMotionSe(s32 hisMotion, s32 upMotion);
s32 fldCatGetNakamaSub(void);
void fldDrawFrushOt(s32 r, s32 g, s32 b, s32 alp, s32 scl, s32 ot);
s32 fldPlayerCalc(void);
void fldPlayer_MoveCalc(void);
void fldPlayer_MoveStart(f32 sx, f32 sy, f32 sz, f32 sr, f32 ex, f32 ey, f32 ez, f32 er);
void mapAiboAdd(f32 x, f32 y, f32 z);
void mapAiboFoceSet(f32 x, f32 y, f32 z);
void mapAiboFoceSet3(f32 x, f32 y, f32 z);
void mapAiboGet(f32 *x, f32 *y, f32 *z);
void mapAiboMinAlpDown(s32 rgba);
void mapAiboTask(void);
void mapAiboWakeUp(s32 mode);
void mapSummonDvAdd(f32 x, f32 y, f32 z);
void mapSummonDvFoceSet(f32 x, f32 y, f32 z);
void mapSummonDvFoceSet2(f32 x, f32 y, f32 z);
void mapSummonDvGet(f32 *x, f32 *y, f32 *z);
void mapSummonDvTask(void);
void rstDrawEvoBgFade(s32 Motion, f32 spd);
s32 sndSeStatus(void);
void mapAiboInit(void);
void mapSummonDvInit(void);

#endif
