#ifndef SDFBILLBOARD_H
#define SDFBILLBOARD_H

#include "common.h"

void sdfBillBoardMake1(u8 *aPacket, const f32 *aCenter, u32 aColor, const void *aUVList, const f32 *aXYOffsetList);
u32 *sdfBillBoardMakeN(const u128 *aCenter, const void *aColor, const void *aUVList, const void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));
void sdfBillBoardMakeTMPacket(u8 *aPacket, const f32 *aTM);

#endif
