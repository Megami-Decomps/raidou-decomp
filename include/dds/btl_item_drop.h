#ifndef DDS_BTL_ITEM_DROP_H

#define DDS_BTL_ITEM_DROP_H
#include "common.h"

/* Battle reward slot: item identity and owned drop count are observed by clients. */
typedef struct BattleItemDrop {
    u16 id; /* 0x00 */
    u8 count; /* 0x02 */
    u8 unknown_03;
} BattleItemDrop;

typedef char BattleItemDrop_size_must_be_4[
    (sizeof(BattleItemDrop) == 4) ? 1 : -1];

#endif /* DDS_BTL_ITEM_DROP_H */
