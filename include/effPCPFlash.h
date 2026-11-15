#ifndef EFFPCPFLASH_H
#define EFFPCPFLASH_H

#include "common.h"

typedef struct dds3FlashPolyList_t dds3FlashPolyList_t;

dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 face);
void dds3FlashPolyListDelete(dds3FlashPolyList_t *hPoly);
void dds3FlashPolyListDraw(dds3FlashPolyList_t *work);
void *dds3FlashPolyListGetVertex(dds3FlashPolyList_t *group, s32 groupIndex);
void *dds3FlashPolyListGetVC(dds3FlashPolyList_t *group, s32 groupIndex);
void dds3FlashPolyListSetBlend(dds3FlashPolyList_t *hPoly, s32 blend);
void dds3FlashPolyListSetABGR(dds3FlashPolyList_t *hPoly, u32 color);

#endif
