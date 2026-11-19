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
extern struct sdf3DLightParallelPacket_t *cmpData18;

#endif
