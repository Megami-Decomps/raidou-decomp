#ifndef SHPMISC_H
#define SHPMISC_H

#include "common.h"

s32 knyGetGoodRank(void);
s32 knyGetPriceBuy(s32 a_itemNo);
s32 knyGetPriceSell(s32 a_itemNo);
void knySetGoodPoint(s32 a_point);
void nwdAddGoodPoint(s32 a_add);
void nwdCtrlGoodRank(s8 a_mode);
s32 nwdGetGoodRank(void);
s32 nwdGetMagTrade(s32 a_stockNo);
s32 nwdGetPriceOrder(s32 a_itemNo);
void shpAddGoodPointBuy(s32 a_price);
void shpAddGoodPointOrder(s32 a_price);
void shpAddGoodPointSell(s32 a_price);
void shpAddGoodPointTrade(s32 a_price);

#endif
