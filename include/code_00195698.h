#ifndef CODE_00195698_H
#define CODE_00195698_H

#include "common.h"
#include "itf.h"
#include "kernel.h"
struct tagMAP_MENU_WORK;
extern dds3ProcessID_t *cmpCallGetProcessStatData;
extern dds3ProcessID_t *elvCallGetProcessStatData;
struct elvMenu_t;
void elvMenuRelease(struct elvMenu_t *wk);
extern struct sdfMemHandle_t *effQueuedResourceNameRecord5;
extern dds3ProcessID_t *mapMenuProcessCheckData;
extern struct itfSprList_t ListMakeListStationData[];
extern struct itfSprList_t ListMakeListStationData2[];
extern struct itfSprList_t ListMakeListStationData3[];
extern struct itfSprList_t ListMakeListStationData4[];
extern dds3ProcessID_t *g_mainPID2;
extern const char itfStrItfmoneyMain[];
extern const char itfStrItfmoneyDraw[];
extern const char itfStrItfHanyoMenuSpr[];
extern const char mapStrMapMenu[];
extern const char mapStrBattlePanelBattleWakuSpr[];
extern const char *mphCommandBUSTUPLOADSubData[];
s32 nmeCallGetProcessData(dds3ProcessID_t *id);
s32 nmeCalc(void);
s32 mapCallGetProcessData(void);
dds3ProcessID_t *savePointMenuCreateMenuPID(void);
s32 cmpCallGetProcessStat(void);
void evtCallExistProcess(void);
s32 elvCallGetProcessStat(void);
void savePointMenuChkEnableDecide(struct tagMAP_MENU_WORK *wk);
void savePointMenuCallProcEnd(struct tagMAP_MENU_WORK *wk);
s32 saveCallMenuProcessCheck2(struct tagMAP_MENU_WORK *wk);
void mphStartLoad(const char *filename);
void mphStart_itf(void);
void mphDestroy(void);
void mphStartDrawBtlRight(void);
void mphStartDrawBtlLeft(void);

#endif
