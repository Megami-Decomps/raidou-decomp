#ifndef FCLCOMBINEINIT_H
#define FCLCOMBINEINIT_H

#include "common.h"

/* Per summon id 1-80, its skill kind: 8-9 and 16-20 set one event bit each. */
extern u8 mapData44[];

void fclCombineInitWork(void);
void mapLeaf25(void);
s32 mapNakamaGankTask(void);
s32 mapNakamaSpotTask(void);
void mapNakamaStockOnCheck(void);
void mapNakamaTakumi_FLUSH(void);

#endif
