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
u32 Init_cmpStatus(void);
void cmpUpdateFontCharPID(void);
void cmpCallGetStockDevilNums2(dds3ProcessID_t *PID);
s32 cmpCallMenuGetFlag(void);
s32 cmpGetNowSummonerRank(void);
s32 cmpGetSummonerRank2SprNo(void);

#endif
