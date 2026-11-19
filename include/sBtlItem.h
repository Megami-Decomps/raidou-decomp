#ifndef SBTLITEM_H
#define SBTLITEM_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"

struct sdfMemHandle_t;
struct SBTLITEM_WORK;

typedef struct SBTLITEM_DATA {
    s32 itemid;
    s32 num;
} SBTLITEM_DATA;

typedef struct SBTLITEM_LISTWORK {
    struct sdfMemHandle_t *handle;
    s32 num;
    SBTLITEM_DATA *data;
} SBTLITEM_LISTWORK;

typedef struct SBTLITEM_TBL {
    u32 usetype;
    u32 flg;
    u16 side;
    u16 type;
    SKLID sklid;
    s32 gold;
    s32 max_num;
} SBTLITEM_TBL;

extern SBTLITEM_TBL *tbldata;

SBTLITEM_TBL *sbtlGetItemtbl(s32 id);
s32 sbtlEntryItemList_Type(struct SBTLITEM_LISTWORK *lwk, u16 type);
s32 sbtlReleaseItemList(struct SBTLITEM_LISTWORK *lwk);
s32 sbtlDeleteItemList(struct SBTLITEM_LISTWORK *lwk);
s32 sbtlItem_CheckSetItem(struct SBTLITEM_WORK *item, const sdfVector4_t *pos);
#ifdef VERSION_RAIDOU2
/* raidou1 names this function sbtlItem_CheckSetItem. */
s32 sbtlItemIDDrop(struct SBTLITEM_WORK *item, const sdfVector4_t *pos);
#endif
s32 sbtlSetDropItem(s32 itemid, s32 num, s16 per);

#endif
