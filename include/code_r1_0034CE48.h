#ifndef CODE_R1_0034_CE48_H
#define CODE_R1_0034_CE48_H

#include "common.h"
#include "code_r1_001CC1D0.h"

/* Scroll to the last visible page and return the resulting cursor, or NULL. */
struct mnuListNode_t *mnuScrollListToEnd(struct mnuList_t *list);

/* Retreat by one page and return the resulting cursor, or NULL if unchanged. */
struct mnuListNode_t *mnuScrollListToPreviousPage(struct mnuList_t *list);

/* Native sprite-state allocation: three resource-set owners and a state word. */
typedef struct mnuSpriteState_t {
    u8 pad00[0x10];
    struct sprHandle_t *resourceSets[3];
    s32 initialValue; /* 0x1C: initialized to 0x100 */
} mnuSpriteState_t;

typedef char mnuSpriteState_size_must_be_0x20[(sizeof(mnuSpriteState_t) == 0x20) ? 1 : -1];

#endif
