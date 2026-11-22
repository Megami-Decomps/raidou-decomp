#ifndef CMPITEM_H
#define CMPITEM_H

#include "common.h"
#include "kernel.h"
#include "enums.h"

struct sprHandle_t;

s32 cmpCallFuncBullet(void);
s32 cmpCallFuncUse(void);
s32 cmpChkMessageDraw(void);
s32 cmpDrawParticle(void);
void cmpStart(void);
s32 elvCallExistProcess2(void);
extern dds3ProcessID_t *hWorkArea3;
/* Item ids collected by MakeList, and their count. */
extern s32 cmpCallBitCheck2Data;
extern u16 cmpCallBitCheck2Work[];
#ifdef VERSION_RAIDOU2
/* The item id of each sort position. */
extern s32 cmpData30[443];
extern struct sprHandle_t *cmpPanelEffList3;
void FreeSpriteHnd(void);
s32 cmpGetItemIDFromSortNo(s32 a_sortNo);
s32 MakeList(CMP_MENU_ID a_id);
#endif

#endif
