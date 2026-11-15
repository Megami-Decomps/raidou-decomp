#ifndef SHPDRAW_H
#define SHPDRAW_H

#include "common.h"

void PartsMoney(s32 a_money, s32 a_trade, f32 a_alphaRate);
void knyDrawKeyHelp(s32 a_type, f32 a_alphaRate);
void nwdDrawKeyHelp(s32 a_type, f32 a_alphaRate);
void scrCallClearProfileFlagsTable3(s32 a_money, f32 a_alphaRate);
void shpBackSpr(s32 a_shopType, f32 a_alphaRate1, f32 a_alphaRate2);
s32 shpCheckRootLineMax(void);
void shpCursorLine(s32 a_shopType, s32 a_menuNo);
void shpResetRootLine(void);
void shpRootCursorInit(void);
void shpRootCursorSelect(s32 a_x, s32 a_y);
void shpRootLine(s32 a_x, s32 a_y);

#endif
