#ifndef CODE_003BF2E0_H
#define CODE_003BF2E0_H

#include "common.h"
#include "gohMain.h"
#include "libsdf.h"
#include "jmsMisc.h"
#include "itf.h"
#include "enums.h"

/* ee-g++ vtable slot: this adjustment, then the method. */
typedef struct jmsVtblEntry_t {
    s16 delta;
    s16 index;
    s32 (*func)();
} jmsVtblEntry_t;

/* The object jmsFirstInitSub allocates (0xBC bytes); jmsLeaf12 and jmsRefDDS3GBWK construct it. */
typedef struct jmsWork_t {
    /* 0x00 */ jmsVtblEntry_t *vtbl;
    /* 0x04 */ sdfMemHandle_t *hFile;
    /* 0x08 */ u32 fileData;
    /* 0x0C */ sdfMemHandle_t *hFile2;
    /* 0x10 */ u32 fileData2;
    /* 0x14 */ jmsMemsetWork_t *gbwk;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C[0x90];
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ s32 unkB0;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ s32 unkB8;
} jmsWork_t;

/* HD MenuFlag: the event bit each jmsMenu entry needs, 0 for none. */
/* "%2d", the level format shared by rstResultPlayerLevel and rstCallBlendColor3/4. */
extern const char rstCallBlendColor4Data[];
/* Phase of the result screen gauge glow, fed to sdfSin. */
extern f32 rstData14;
extern u16 SetRootMenuData[];
/* HD strJmsMenu: each jmsMenu entry's text. */
extern char *cmpData34[];
s32 CheckFlagTGS(s32 MenuID);
void SetFlagMenu(dds3ProcessID_t *PID, s32 MenuID, ITFMENU_CB_GF Flag, s8 Bool);
void SetRootMenu(dds3ProcessID_t *PID, s32 MenuID, itfMenuFunc_t func);
void SetMenu(dds3ProcessID_t *PID, s32 MenuID, s32 ParentMenuID, itfMenuFunc_t func);
void jmsStart(void);
/* jmsMenu item callback and title draw function. */
s32 cmpCallGetProcessData27(void);
void cmpCallGetProcessData32(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_fontColor, u8 a_alphaRate);
void cmpCallGetProcessData28(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_fontColor, u8 a_alphaRate);
extern char cmpData50[];
void cmpCallGetProcessData31(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_fontColor, u8 a_alphaRate);
void jmsDisableSelect(s32 MenuID);
/* Menu flags copied into SetRootMenuData[14..25]. */
extern u32 cmpCallMenuGetMenuNumData[4];
extern u32 cmpCallMenuGetMenuNumData2[4];
extern u32 cmpCallMenuGetMenuNumData3[4];
extern struct itfSprList_t cmpCallMenuGetMenuNumData4[];
extern struct itfSprList_t cmpCallMenuGetMenuNumData5[];

extern jmsVtblEntry_t jmsData12[34];
extern jmsVtblEntry_t jmsData13[33];
extern jmsWork_t *jmsLeaf11Data;
extern const char jmsStrJimusyoJmsQuestTbl[];
extern const char jmsStrJimusyoJmsQuestStrBmd[];
void jmsLeaf12(jmsWork_t *wk);
s32 jmsRefDDS3GBWK(jmsWork_t *wk);
void cmpCallBitCheck10(jmsWork_t *wk);
void rstCallGetStockDevilNums(gohData_t *work);
extern s32 rstCallBitCheckWork;

extern const char jmsStrJmsMenuMain[];
extern const char jmsStrJmsMenuDraw[];
s32 ProcessCheck_jmsMenu(void);
s32 Process_jmsMenu(dds3ProcessID_t *PID);
void jmsMenuShutdown(dds3ProcessID_t *PID);
s32 cmpCallGetProcessData33(dds3ProcessID_t *PID);
void cmpCallMenuGetMenuNum(struct jmsInputPanelWork_t *work);
void jmsMenuMain(void);

/* Result screen state cmpLoadLocalizeText sets up: the two player data copies, the stock pair being read and the
   per-stock flags and skills reset for the 20 stock slots. */
extern struct SBTL_PLAYER_DATA *rstData8;
extern struct SBTL_PLAYER_DATA *rstData9;
extern struct sdatUnitWork_t *g_pUnitSrc;
extern struct sdatUnitWork_t *g_pUnitDst;
extern s32 evtCallCheckRankUpData2;
extern s32 evtCallCheckRankUpData3;
extern s32 g_dvlLevelUpNum[20];
extern u8 rstWork2[20];
extern s32 rstOmoideGaIppaiWork4[20];
extern s32 rstOmoideGaIppaiWork5[20];
extern u8 s_bRegistMemSkill[20][2];
/* A stock devil's level-up skills for the growth screen (HD _LvUpSkl). */
typedef struct _LvUpSkl {
    /* 0x00 */ s32 nmlSkl[3];
    /* 0x0C */ s32 nmlSklPos[3];
    /* 0x18 */ s32 memSkl[2];
    /* 0x20 */ u8 bGetMemSkl[2];
    /* 0x22 */ u8 ofcMemSkl[2];
} _LvUpSkl;

extern _LvUpSkl g_aLvUpSkl[20];
s32 rstSetLvUpSkl(s32 index, s32 *count);

#endif
