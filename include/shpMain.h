#ifndef SHPMAIN_H
#define SHPMAIN_H

#include "common.h"

s32 datGetItemNumEquipMinus(s32 id);
s32 fldCallGetProcessStat(void);
void shpMilkHallProcessStart(void);
void fldShowSceneModels(void);
s32 knyGetIntimateRank(void);
s32 nwdGetSelectModelID(void);
s32 shpCallProcEnd(void);
s32 shpCallRelease(void);
void shpDrawBack(void);
void shpDrawGoodPoint(void);
s32 shpGetModelHandle(void);
s32 shpGetSelectItemID(void);
void shpKonnouyaProcessStart(void);
void shpProcessStart(s32 a_shoptype);
void shpSetColorModel(u32 a_abgr, s32 a_defObjId);
s32 shpSetModelMotion(void);
void shpSetStatDraw(s32 a_bDraw);
void shpSetZoom(s32 a_objId);
void shpUpdateBack(void);
s32 shpWaitRelease(void);
void pb_draw_help_Shutdown(const char *aScriptPath);
extern const char *shpCallRagTestSubData[];

#endif
