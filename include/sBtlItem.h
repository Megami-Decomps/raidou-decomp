#ifndef SBTLITEM_H
#define SBTLITEM_H

#include "common.h"

struct SBTLITEM_LISTWORK;

s32 sbtlEntryItemList_Type(struct SBTLITEM_LISTWORK *lwk, u16 type);
s32 sbtlReleaseItemList(struct SBTLITEM_LISTWORK *lwk);

#endif
