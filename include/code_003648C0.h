#ifndef CODE_003648C0_H
#define CODE_003648C0_H

#include "common.h"
#include "model.h"
#include "evtUnitManager.h"

struct tagCMP_WORK;
extern struct mdlObj_t *cmpDrawStatusModel_cmpModelData;
s32 cmpMenuFilter(struct tagCMP_WORK *wk);
void mapFunc5(void);
void mapFunc6(void);
void cmpStockListMake(dds3ProcessID_t *PID);
dds3ProcessID_t *cmpStockListProcessStart(void);
extern const char ProcessStart_cmpStatusStrCampStatusMain[];
extern const char ProcessStart_cmpStatusStrCampStatusDraw[];
s32 cmpCallGetProcessData11(dds3ProcessID_t *id);
s32 cmpCallGetProcessData12(dds3ProcessID_t *id);
s32 evtCallGetSpriteHnd(CMP_SPR a_no, s32 a_index);
/* HD cmpGetStockUnitWork: the stock unit, or NULL past the stock count. */
struct sdatUnitWork_t *cmpRefDDS3GBWK2(s32 a_stockidx);
extern s16 cmpData41;
extern u16 cmpData11; /* model MajorNo */
extern u16 cmpData12; /* model MinorNo */
/* Stock slots the camp menu checks; SBTL_PLAYER_DATA names only the first SDAT_STOCK_NUM. */
#define CMP_STOCK_NUM 20
s32 cmpChkDevilStockExist(s32 a_stockID);
s32 cmpLoadStatusModel(void);
s32 sbtlCallBtlGetSkilltbl15(SKLID a_sklId);
extern struct effMappedResource_t *evtWork[2];

/* Status record of a category 13 batch. Partial layout. */
typedef struct evtStatus13_t {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} evtStatus13_t;
extern struct sdf3DLightParallelPacket_t *cmpData18;
extern s8 cmpData10; /* model load state: 1 waits for the delayed resources, 2 loads */
extern s32 cmpData13; /* model BlockMode */
extern s32 cmpSetModelMotionData; /* motion number, -1 for none */
extern s32 cmpData16;
extern f32 cmpData17; /* motion blend length */
extern s32 cmpData19; /* light number, -1 for none */
extern struct sdfMemHandle_t *cmpWork[2]; /* the two status effect EPL files */
extern struct effHandle_t *cmpWork2[2]; /* effects loaded from cmpWork */
extern struct effHandle_t *g_hEffBst;
extern s32 g_bstID;
extern s32 cmpDeleteBstEffData;
extern s32 cmpDeleteBstEffData2;
extern const char cmpStrCampBad01Epl[];
extern const char datStrErrorItfMenuLinkSprite[];
extern struct itfSprList_t datDlcNoneCheckData[];
extern struct itfSprList_t datDlcNoneCheckData2[];
extern const char cmpStrCampBad04Epl[];
void cmpInitDstEff2(s32 a_type);
void cmpCreateBstEff(void);

/* HD arrPlayerRank: summoner ranks from highest, ended by a negative event bit. */
typedef struct cmpPlayerRank_t {
    s32 bit;
    s32 sprNo;
} cmpPlayerRank_t;
extern cmpPlayerRank_t cmpGetSummonerRank2SprNoData[];
extern s8 cmpData31[24][10];
s32 cmpMakeCateChk(void);
void cmpChkSkillSelectHd(void);
extern s8 cmpData40;
extern s8 cmpCallGetStockPID5Data;
s32 cmpChangeChkTabPageSub(void);
s32 cmpChangeChkTabPageSub2(void);
s32 cmpChangeChkTabPage(CMP_MENU_ID a_id);
s32 cmpCallGetStockPID(void);
void cmpCallGetStockDevilNums(void);
u32 Init_cmpStatus(void);
s32 cmpUpdateFontCharPID(void);
void cmpPartyDrawFunc(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_color, u8 a_alpha);
void cmpCallGetStockPID3(void);
s32 cmpMenuFilterUnit(struct sdatUnitWork_t *psUnit, struct sdatUnitWork_t *psUnitChk, ecmpKudaCMU iAttrId,
                      BSTF iBadStatus);
void cmpCallGetStockDevilNums2(dds3ProcessID_t *PID);
s32 cmpCallMenuGetFlag(void);
s32 cmpGetNowSummonerRank(void);
s32 cmpGetSummonerRank2SprNo(void);
/* HD g_menuID: the open camp menu; the high half selects the cmpLeaf16Data entry. */
extern s32 cmpCallGetCharPIDData7;
/* HD cmpBookFunc: per-page update of the book menu. */
extern s32 (*cmpLeaf16Data[4])(void);
/* Help text id of each book menu row. */
extern u16 cmpCallMakeCateChkWork[];
extern s32 cmpCallMakeCateChkData; /* entries in cmpCallMakeCateChkWork */
/* Party menu process, and the item and party ids last chosen (-1 for none). */
extern dds3ProcessID_t *cmpCallGetCharPIDData4;
extern s32 cmpCallGetCharPIDData5;
extern s32 cmpCallGetCharPIDData6;
/* Panel, select and scroll sprite lists of the category menu. */
extern struct itfSprList_t cmpSetupData[];
extern struct itfSprList_t cmpCallGetCharPIDData[];
extern struct itfSprList_t cmpCallGetCharPIDData2[];
s32 cmpCallMakeCateChk(CMP_MENU_ID a_id);
void cmpCallGetCharPID(CMP_MENU_ID a_id);
extern struct itfSprList_t cmpDrawFuncData[];

struct tagCMP_STATUS_WORK;
void cmpSequence(struct tagCMP_STATUS_WORK *wk);
void cmpSTATUSSTEP(struct tagCMP_STATUS_WORK *wk);
void cmpMemSetSeqStatusStepUpdate(struct tagCMP_STATUS_WORK *wk);
void cmpSeqGoodby(struct tagCMP_STATUS_WORK *wk);
s32 Process_cmpStatus(struct tagCMP_STATUS_WORK *wk);

/* The camp submenu built by mapMenuCreateMenuPID_itf. */
extern dds3ProcessID_t *cmpData39;
extern char cmpData43[];
extern struct itfSprList_t mapMenuCreateMenuPID_itfData[];
extern struct itfSprList_t mapMenuCreateMenuPID_itfData2[];
void DrawFuncSkl_sbtlCmndMenuNew(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_color, u8 a_alpha);
void mapCallGetStockPID(void);
void mapMenuCreateMenuPID_itf(void);

/* Skill ids of the camp skill menu rows, negative for an empty row. */
extern SKLID cmpWork9[8];
s32 cmpChkEnableUseSkill(struct sdatUnitWork_t *use, struct sdatUnitWork_t *tgt, SKLID id);
void cmpChkSkillSelect(struct sdatUnitWork_t *a_pUseUnit);

#endif
