#ifndef SBTLDRAW_H
#define SBTLDRAW_H

#include "common.h"
#include "kernel.h"

struct SBTLUNIT_WORK;
struct effCtexHandle_t;
struct sdatUnitWork_t;
struct sprHandle_t;

void nbStartEventPacks(struct SBTLUNIT_WORK *wk);
void sbtlAnalyzeBack(u8 a_type, u8 a_alphaRate);
void sbtlAnalyzeBackEx(u8 a_type, s32 a_x, s32 a_y, u8 a_alphaRate, struct sprHandle_t *a_hSpr);
void sbtlAnalyzeLvEx(struct sdatUnitWork_t *unit, s32 a_x, s32 a_y, u8 a_alphaRate, s32 a_bSecret, struct sprHandle_t *a_hSpr);
void sbtlGetIdling_HD(struct sdatUnitWork_t *pUnit, struct effCtexHandle_t *cTex, s32 y, s32 mode, s32 idx);
void sbtlHelpCantUse(s32 a_type, s32 a_num, u8 a_alphaRate);
void sbtlNumFont(s32 a_x, s32 a_y, u32 a_color, u8 a_alphaRate, DDS3_OT a_ot, char *a_str);

#endif
