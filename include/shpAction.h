#ifndef SHPACTION_H
#define SHPACTION_H

#include "common.h"
#include "enums.h"

s32 FadeIn(void);
s32 FadeOut(void);
void SetStatFadeOut(void);
void UpdateBuyReturnList(void);
void UpdateInit(void);
void UpdateSellReturnList(void);
s8 shpListCtrlBuySelItem(void);
s8 shpListCtrlSelNum(s32 a_type);
s8 shpListCtrlSellSelectItem(void);
s8 shpListCtrlTradeSelectDevil(void);
s32 shpListUpdateBuySelNum(void);
s32 shpListUpdateSellSelectNum(void);
void shpRefShpAction(SHP_SORT _sort);
s32 shpSelectListPage(void);
s8 shpUpdateBuy(void);
s8 shpUpdateLeaveMilkHall(void);
s8 shpUpdateNews(void);
s8 shpUpdateSell(void);
s8 shpUpdateTalk(void);
s8 shpUpdateTrade(void);

#endif
