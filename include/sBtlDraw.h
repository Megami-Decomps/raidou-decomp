#ifndef SBTLDRAW_H
#define SBTLDRAW_H

#include "common.h"
#include "kernel.h"
#include "itf.h"

struct SBTLUNIT_WORK;
struct effCtexHandle_t;
struct sdatUnitWork_t;
struct sprHandle_t;

void nbStartEventPacks(struct SBTLUNIT_WORK *wk);
void sbtlAnalyzeBack(u8 a_type, u8 a_alphaRate);
void sbtlAnalyzeBackEx(u8 a_type, s32 a_x, s32 a_y, u8 a_alphaRate, struct sprHandle_t *a_hSpr);
void sbtlAnalyzeLvEx(struct sdatUnitWork_t *unit, s32 a_x, s32 a_y, u8 a_alphaRate, s32 a_bSecret, struct sprHandle_t *a_hSpr);
void sbtlGetIdling_HD(s32 a_x, s32 a_y, u8 a_alphaRate, s32 a_otNo, struct sdatUnitWork_t *unit);
void sbtlHelpCantUse(s32 a_type, s32 a_num, u8 a_alphaRate);
void sbtlCallBtlAnalyzeBack3(struct SBTLUNIT_WORK *wk, u8 a_alphaRate);
char *sbtlGetPlayerName(void);
void sbtlCallCheckDying(u16 mode, s32 x, s32 y, u8 alpha, DDS3_OT ot, struct sdatUnitWork_t *unit, s8 bUnknown);
s32 sbtlCallCheckDying2(struct SBTLUNIT_WORK *wk, s32 x, s32 y);
s32 sbtlCallCheckDying3(struct SBTLUNIT_WORK *wk, s32 x, s32 y);
f32 sbtlLeaf90(s32 count);
void sbtlCallGetRandIntA6(void);
void sbtlCallBtlSearchUnitID37(void);
s32 sbtlCallBtlSearchUnitID27(void);
s32 sbtlCallCheckSkillEnable(struct SBTLUNIT_WORK *wk);
void sbtlRefSbtl59(struct SBTLUNIT_WORK *wk, s32 a_x, s32 a_y);
void sbtlRefSbtl61(struct SBTLUNIT_WORK *wk, s32 a_x, s32 a_y);
void sbtlCallBtlDrawRedGg(struct SBTLUNIT_WORK *wk);
void sbtlCallBtlGetViceLevel(s32 a_x, s32 a_y, u8 a_alphaRate, DDS3_OT a_ot, struct sdatUnitWork_t *unit, s8 bEnable);
void sbtlNakamaCursor(sdfVector4_t *pos);
void sbtlCallEventBattleStopChack2(void);
void sbtlDrawRedGg(s32 a_x, s32 a_y, u8 a_alphaRate, s32 a_sprNo, DDS3_OT a_ot);
void sbtlCallBtlCheckAisyoOld8(s32 a_x, s32 a_y, u8 a_alphaRate, DDS3_OT a_ot, struct SBTLUNIT_WORK *wk);
void sbtlNum(s32 a_x, s32 a_y, u32 a_color, u8 a_alpha, s32 a_num, DDS3_OT a_ot);
s32 sbtlCallGetSmallFontFRQ4(s32 x, s32 y, u8 bRight, u32 color, u8 alpha, s32 ot, void *str);
void sbtlCallBtlGetPlayerHitTarget(void);
void sbtlCallBtlSpriteEx(struct SBTLUNIT_WORK *wk);
void sbtlCallMoonFadeIn(void);
s32 sbtlCallMessageNumSearch4(s32 a_x, s32 a_y, s32 a_z, u32 a_color, SBTLSM a_mode);
void sbtlCallMoverInit(void);
char *sbtlLeaf112(u8 n);
void sbtlLeaf93(s32 a_x, s32 a_y, u32 a_alphaRate, s16 *a_list);
void sbtlRefSbtl55(s32 a_x, s32 a_y, u32 a_alphaRate, DDS3_OT a_ot, s16 *a_list);
void sbtlSprite(s32 a_x, s32 a_y, u8 a_alpha, s32 a_sprNo);
void sbtlSpriteEx(s32 a_x, s32 a_y, u8 a_alpha, s32 a_sprNo, DDS3_OT a_ot);
void sbtlNumFont(s32 a_x, s32 a_y, u32 a_color, u8 a_alphaRate, DDS3_OT a_ot, char *a_str);

extern char *sbtlData81[3];
extern itfMover_t sbtlWork7[128];
extern struct frTextBank_t *sbtlCallMessageNumSearch4Data;
extern u8 sbtlData119;
extern s8 sbtlData121;
extern s32 sbtlRefSbtl57Data;
extern s32 sbtlRefSbtl61Data;
extern s32 sbtlRefSbtl59Data;
extern s16 sbtlCallBtlSearchUnitID37Data[];
extern s32 sbtlCallBtlSearchUnitID37Data2;
extern s32 sbtlCallBtlSpriteData;
extern s32 sbtlCallBtlSpriteData2;
extern s32 sbtlEscapeGaugeData;
extern s32 sbtlCallCheckDying2Data;
extern s32 sbtlCallCheckDying3Data;
extern s32 sbtlCallCheckDying2Data2;
extern s32 sbtlCallCheckDying3Data2;
extern s32 sbtlCallCheckDying2Data3;
extern s32 sbtlCallCheckDying3Data3;
extern s32 sbtlCallCheckDying3Data4;
extern s16 sbtlCallEventBattleStopChack2Data[];
extern s32 sbtlCallEventBattleStopChack2Data2;
extern s32 Init_sbtlDrawData;
extern s32 sbtlData122;
extern s8 Init_sbtlDrawData2;
extern s8 sbtlData123;

#endif
