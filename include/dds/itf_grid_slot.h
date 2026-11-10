#ifndef DDS_ITF_GRID_SLOT_H

#define DDS_ITF_GRID_SLOT_H
#include "common.h"

struct EffectSlotSet;

/* An indexed render slot owned by an effect resource set. */
typedef struct MenuGridSlot {
    struct EffectSlotSet *set;
    s32 index;
} MenuGridSlot;

typedef char MenuGridSlot_size_must_be_8[(sizeof(MenuGridSlot) == 8) ? 1 : -1];

#endif /* DDS_ITF_GRID_SLOT_H */
