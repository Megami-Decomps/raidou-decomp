#ifndef DDS_BTL_INDEX_LIST_SEARCH_API_H

#define DDS_BTL_INDEX_LIST_SEARCH_API_H
#include "common.h"

struct BtlIndexList;

struct BtlUnit;

typedef s32 BtlIndexListEntryIndex;

enum {
    BTL_INDEX_LIST_ENTRY_NOT_FOUND = -1,
};

/* Return the first live slot holding this unit pointer, or NOT_FOUND. */
BtlIndexListEntryIndex btlFindListIndex(
    struct BtlIndexList *list, struct BtlUnit *entry);

#endif /* DDS_BTL_INDEX_LIST_SEARCH_API_H */
