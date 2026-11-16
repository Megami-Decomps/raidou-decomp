#ifndef SBTLITEM_H
#define SBTLITEM_H

#include "common.h"
#include "enums.h"

struct SBTLITEM_LISTWORK;

typedef struct SBTLITEM_TBL {
    u32 usetype;
    u32 flg;
    u16 side;
    u16 type;
    SKLID sklid;
    s32 gold;
    s32 max_num;
} SBTLITEM_TBL;

SBTLITEM_TBL *sbtlGetItemtbl(s32 id);
s32 sbtlEntryItemList_Type(struct SBTLITEM_LISTWORK *lwk, u16 type);
s32 sbtlReleaseItemList(struct SBTLITEM_LISTWORK *lwk);

#endif
