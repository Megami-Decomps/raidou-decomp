#ifndef CODE_00195698_H
#define CODE_00195698_H

#include "common.h"
#include "kernel.h"
struct tagMAP_MENU_WORK;
extern dds3ProcessID_t *cmpCallGetProcessStatData;
extern dds3ProcessID_t *elvCallGetProcessStatData;
extern struct sdfMemHandle_t *effQueuedResourceNameRecord5;
extern dds3ProcessID_t *mapMenuProcessCheckData;
extern dds3ProcessID_t *g_mainPID2;
extern const char itfStrItfmoneyMain[];
extern const char itfStrItfmoneyDraw[];
extern const char itfStrItfHanyoMenuSpr[];
extern const char mapStrMapMenu[];
extern const char mapStrBattlePanelBattleWakuSpr[];
s32 nmeCallGetProcessData(dds3ProcessID_t *id);
dds3ProcessID_t *savePointMenuCreateMenuPID(void);
s32 cmpCallGetProcessStat(void);
void evtCallExistProcess(void);
s32 elvCallGetProcessStat(void);
void savePointMenuChkEnableDecide(struct tagMAP_MENU_WORK *wk);
void saveLeaf(struct tagMAP_MENU_WORK *wk);
s32 saveCallMenuProcessCheck2(struct tagMAP_MENU_WORK *wk);

#endif
