#ifndef CMPITEM_H
#define CMPITEM_H

#include "common.h"
#include "kernel.h"

struct sprHandle_t;

s32 cmpCallFuncBullet(void);
s32 cmpCallFuncUse(void);
s32 cmpChkMessageDraw(void);
s32 cmpDrawParticle(void);
void cmpStart(void);
s32 elvCallExistProcess2(void);
extern dds3ProcessID_t *hWorkArea3;
#ifdef VERSION_RAIDOU2
/* The item id of each sort position. */
extern s32 cmpData30[443];
extern struct sprHandle_t *cmpPanelEffList3;
void FreeSpriteHnd(void);
s32 cmpGetItemIDFromSortNo(s32 a_sortNo);
#endif

#endif
