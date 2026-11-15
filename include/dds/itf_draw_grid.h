#ifndef DDS_ITF_DRAW_GRID_H

#define DDS_ITF_DRAW_GRID_H
#include "common.h"

#include "itf_grid_slot.h"

struct EffectSlotSet;

/* Bind a cached resource set and signed slot index. */
void itfGridStorePosition(MenuGridSlot *slot,
                          struct EffectSlotSet *resource, s32 resourceIndex);

void itfDrawGridWithResolvedSlot(s32 offsetX, s32 offsetY, s32 z,
                                 s32 drawFlags, struct EffectSlotSet *object,
                                 s32 index, s32 surfaceIndex);

#endif
