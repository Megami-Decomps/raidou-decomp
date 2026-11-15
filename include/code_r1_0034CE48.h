#ifndef CODE_R1_0034_CE48_H
#define CODE_R1_0034_CE48_H

#include "common.h"
#include "code_r1_001CC1D0.h"

/* Scroll to the last visible page and return the resulting cursor, or NULL. */
struct MenuListNode *mnuScrollListToEnd(struct MenuList *list);

/* Retreat by one page and return the resulting cursor, or NULL if unchanged. */
struct MenuListNode *mnuScrollListToPreviousPage(struct MenuList *list);

/* Native sprite-state allocation: three resource-set owners and a state word. */
typedef struct MenuSpriteState {
    u8 pad00[0x10];
    struct EffectSlotSet *resourceSets[3];
    s32 initialValue; /* 0x1C: initialized to 0x100 */
} MenuSpriteState;

typedef char MenuSpriteState_size_must_be_0x20[(sizeof(MenuSpriteState) == 0x20) ? 1 : -1];

#endif
