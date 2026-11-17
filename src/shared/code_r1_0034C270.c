#include "common.h"
#include "sdfChip.h"

#include "newdata.h"

#include "include_asm.h"
#include "basic.h"
#include "evt_world.h"
#include "code_r1_001CC1D0.h"
#include "code_r1_0034C270.h"
#include "code_r1_002CAFD8.h"

/* Select the indexed node and report whether the index exists. */
s32 mnuSeekListNode(s32 index, struct mnuList_t *list);

#define MNU_LIST_INSERT_AFTER_ANCHOR 0x2

/* Walk the indexed entry and return its node, or NULL when it is absent. */
struct mnuListNode_t *mnuWalkNodeList(s32 index, struct mnuList_t *list);

struct mnuListNode_t;

typedef struct mnuList_t mnuList_t;

typedef struct mnuListNode_t mnuListNode_t;

/* Selected child window and page of the party skill-menu runtime. */
typedef struct mnuPartyRuntime_t {
    u8 pad00[4];
    s32 selectionList; /* 0x04: list whose cursor selects the active window */
    u8 pad08[0x1C];
    s32 selectedWindow; /* 0x24 */
    u8 pad28[4];
    s32 selectedPage;   /* 0x2C */
} mnuPartyRuntime_t;

/* The camp initializer allocates and clears this complete 0x38-byte child. */
typedef struct mnuSkillBrowseRuntime_t {
    struct sdfMemHandle_t *allocation;
    u8 pad04[4];
    s32 active;
    struct mnuList_t *categoryList;
    mnuWindowContainer_t *windows[4];
    void *panel;
    mnuWindowContainer_t *selectedWindow;
    s32 activeMark;
    s32 motionSelection;
    u32 selectionFlags;
    u32 selectedIndex;
} mnuSkillBrowseRuntime_t;

typedef char mnuSkillBrowseRuntimeSizeCheck[(sizeof(mnuSkillBrowseRuntime_t) == 0x38) ? 1 : -1];

/* Sprite slots of the active skill-menu context; intervening state is opaque. */
typedef struct mnuContextSprites_t {
    u8 pad00[0x64];
    u32 sprite64;
    s32 label68;
    u32 frame6C;
    u8 pad70[4];
    u32 sprite74;
    u32 sprite78;
    u8 pad7C[0x64];
    u32 spriteE0;
    u32 spriteE4;
} mnuContextSprites_t;

void mnuUpdateListScrollFlags(mnuList_t *list);
s32 mnuListContainsFinalNode(mnuList_t *list);
mnuListNode_t *mnuListAdvanceCursor(mnuList_t *, s32, s32);
mnuListNode_t *mnuListRetreatCursor(mnuList_t *, s32, s32);

void mnuSelectFirstListNode(mnuList_t *list);
void mnuSelectLastListNode(mnuList_t *list);
mnuListNode_t *mnuListAdvanceCursor(mnuList_t *list, s32 noScroll, s32 keepFade);
mnuListNode_t *mnuListRetreatCursor(mnuList_t *list, s32 noScroll, s32 keepFade);

mnuList_t *mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing) {
    mnuList_t *item = (mnuList_t *)sdfChipAllocZeroDirect(sizeof(mnuList_t));
    item->id = id;
    item->visibleCount = visibleCount;
    item->rowStep = rowSpacing * 8;
    item->scale = 0x100;
    item->head = NULL;
    item->first = NULL;
    item->windowOffset = 0;
    item->cursor = NULL;
    return item;
}

u32 mnuDestroyListState(mnuList_t *list) {
    mnuListNode_t *pending;

    do {
        pending = mnuRemoveListCursorNode(list);
    } while (pending != 0);
    sdfChipFreeDirect(list);
    return 1;
}

void mnuUpdateListScrollFlags(mnuList_t *list) {
    mnuListNode_t *node = list->head;
    s32 i;

    if (node == NULL) {
        list->flags = 0;
        return;
    }
    if (node->prev != NULL) {
        list->flags |= 1;
    } else {
        list->flags &= ~1;
    }
    for (i = 0; i < list->visibleCount; i++) {
        node = node->next;
        if (node == NULL) {
            list->flags &= ~2;
            return;
        }
    }
    list->flags |= 2;
}

INCLUDE_ASM(const s32, "code_r1_0034C270", mnuListAppendNode);

s32 mnuListContainsFinalNode(mnuList_t *list) {
    mnuListNode_t *node = list->head;
    s32 index = 0;
    if (node != NULL) {
        s32 count = list->visibleCount;
        do {
            if (index >= count) {
                return 0;
            }
            if (node == list->last) {
                return 1;
            }
            node = node->next;
            index++;
        } while (node != NULL);
    }
    return 0;
}

mnuListNode_t *mnuInsertListNodeRelativeToAnchor(mnuList_t *list, mnuListNode_t *anchor,
                         const void *text, s32 mode, u32 options) {
    mnuListNode_t *node;
    mnuListNode_t *walk;

    if (list->count == 0 || anchor == NULL || anchor == list->last) {
        node = mnuListAppendNode(list, text);
        if (list->windowOffset >= list->visibleCount - 1 && list->cursor != list->last) {
            list->head = list->head->next;
            list->cursor = list->cursor->next;
            if (mode == -1) {
                mnuListRetreatCursor(list, 0, 1);
            }
        } else if (mode == -2) {
            mnuListAdvanceCursor(list, 0, 1);
        }
        return node;
    }

    node = sdfChipAllocZeroDirect(sizeof(mnuListNode_t));
    node->text = text;
    if (options & MNU_LIST_INSERT_AFTER_ANCHOR) {
        node->prev = anchor;
        node->index = anchor->index;
        node->next = anchor->next;
        if (anchor->next != NULL) {
            anchor->next->prev = node;
        }
        anchor->next = node;
        walk = node;
        do {
            walk->index++;
            walk = walk->next;
        } while (walk != NULL);
    } else {
        node->prev = anchor->prev;
        node->next = anchor;
        node->index = anchor->index;
        if (anchor->prev != NULL) {
            anchor->prev->next = node;
        }
        anchor->prev = node;
        if (anchor == list->first) {
            list->first = node;
            if (list->cursor == list->head || list->count < list->visibleCount) {
                list->head = node;
            }
        }
        walk = anchor;
        while (walk != NULL) {
            walk->index++;
            walk = walk->next;
        }
    }
    list->count++;
    if (node->index >= list->head->index && node->index < list->cursor->index) {
        list->cursor = list->cursor->prev;
        if (list->first == list->head) {
            if (mode >= 0 || mode == -2) {
                if (list->count >= list->visibleCount + 1 &&
                    list->cursor->index - list->head->index == list->visibleCount - 1) {
                    list->head = list->head->next;
                    list->cursor = list->cursor->next;
                } else if (mode == -2) {
                    mnuListAdvanceCursor(list, 0, 1);
                }
            } else {
                if (list->count >= list->visibleCount + 1 &&
                    list->cursor->index - list->head->index == list->visibleCount - 1) {
                    list->head = list->head->next;
                    list->cursor = list->cursor->next;
                } else {
                    mnuListAdvanceCursor(list, 0, 1);
                }
            }
        } else {
            if (mnuListContainsFinalNode(list)) {
                if (mode < 0 && mode != -2) {
                    list->head = list->head->next;
                }
            } else {
                if (mode >= 0 || mode == -2) {
                    if (mode == -2) {
                        list->head = list->head->next;
                        list->cursor = list->cursor->next;
                    }
                } else {
                    list->head = list->head->next;
                    list->cursor = list->cursor->next;
                }
            }
        }
    }
    mnuUpdateListScrollFlags(list);
    return node;
}

mnuListNode_t *mnuRemoveListCursorNode(mnuList_t *list) {
    mnuListNode_t *node;
    mnuListNode_t *cursor;
    mnuListNode_t *previous;
    mnuListNode_t *next;

    if (list->count == 0) {
        return NULL;
    }
    node = list->cursor;
    if (node == NULL) {
        return NULL;
    }
    cursor = node;
    do {
        if (node->index > 0) {
            node->index--;
        }
        node = node->next;
    } while (node != NULL);
    node = cursor;
    previous = node->prev;
    next = node->next;
    if (list->last->index - list->head->index + 1 <= list->visibleCount) {
        if (list->head != list->first) {
            cursor = previous;
            list->head = list->head->prev;
            list->cursor = previous;
        } else if (node == list->head) {
            if (next != NULL) {
                list->head = next;
                cursor = next;
                list->cursor = next;
            } else {
                cursor = previous;
                list->head = previous;
                list->cursor = previous;
                list->windowOffset--;
            }
        } else if (next != NULL) {
            list->cursor = next;
            cursor = next;
        } else {
            cursor = previous;
            list->cursor = previous;
            list->windowOffset--;
        }
    } else if (next != NULL) {
        list->cursor = next;
        cursor = next;
    }
    if (cursor == NULL) {
        list->head = NULL;
        list->first = NULL;
        list->last = NULL;
        list->windowOffset = 0;
    }
    if (previous != NULL) {
        previous->next = next;
    }
    if (next != NULL) {
        next->prev = previous;
    }
    if (previous == NULL) {
        list->head = next;
        list->first = next;
    }
    if (next == NULL) {
        list->last = previous;
    }
    sdfChipFreeDirect(node);
    list->count--;
    mnuUpdateListScrollFlags(list);
    return list->cursor;
}

void mnuSetGridSpriteSlot(struct mnuListNode_t *node, s32 row, s32 col, s32 x, s32 y, struct sprHandle_t *sprite, s32 effect) {
    node->sprites[row * 4 + col].sprite = sprite;
    node->sprites[row * 4 + col].effect = effect;
    itfSetGridEntryQuantizedAndRefresh(sprite, effect, x, y, x, y);
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00279CC0.c:997, rework, same function 1.00 (words 17/17, calls 1.00, shape 1.00) */
mnuListNode_t *mnuWalkNodeList(s32 targetIndex, mnuList_t *list) {
    mnuListNode_t *node = list->first;
    s32 index = 0;

    if (node != NULL && targetIndex != index) {
        do {
            node = node->next;
            index++;
        } while (node != NULL && index != targetIndex);
    }
    return node;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00279CC0.c:1010, rework, same function 1.00 (words 29/29, calls 1.00, shape 1.00) */
s32 mnuSeekListNode(s32 targetIndex, mnuList_t *list) {
    s32 size = list->count;

    if (targetIndex >= size) {
        return 0;
    }
    list->windowOffset = 0;
    list->head = list->first;
    list->cursor = list->first;
    if (targetIndex > 0) {
        do {
            if (size - list->head->index <= list->visibleCount) {
                list->windowOffset += 1;
            } else {
                list->head = list->head->next;
            }
            list->cursor = list->cursor->next;
            targetIndex--;
        } while (targetIndex != 0);
    }
    return 1;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00279CC0.c:1033, rework, same function 1.00 (words 7/7, calls 1.00, shape 1.00) */
void jmsLeaf17(mnuList_t *list) {
    mnuSeekListNode(0, list);
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00279CC0.c:1037, rework, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
void jmsLeaf18(mnuList_t *list) {
    mnuSeekListNode(list->count - 1, list);
}

/* Advance the visible head when a prior window offset can be reduced. */
mnuListNode_t *mnuAdvanceListWindowStart(mnuList_t *list) {
    mnuListNode_t *cursor = list->cursor;
    mnuListNode_t *last = list->last;
    mnuListNode_t *head = list->head;

    if (cursor == last) {
        return cursor;
    }
    head = head->next;
    if (head == NULL) {
        return cursor;
    }
    list->head = head;
    list->windowOffset--;
    return cursor;
}

/* Step the visible head back one node when a full window follows it. */
mnuListNode_t *mnuRetreatListWindowStart(mnuList_t *list) {
    mnuListNode_t *cursor = list->cursor;
    mnuListNode_t *head = list->head;
    mnuListNode_t *node;
    s32 i;

    if (cursor == list->first) {
        return cursor;
    }
    node = head;
    for (i = 0; i < list->visibleCount; i++) {
        if (node == NULL) {
            return cursor;
        }
        node = node->next;
    }
    head = head->prev;
    list->head = head;
    list->windowOffset++;
    return cursor;
}

mnuListNode_t *mnuListAdvanceCursor(mnuList_t *list, s32 noScroll, s32 keepFade) {
    s32 count = list->count;
    mnuListNode_t *cursor = list->cursor;
    mnuListNode_t *last;
    mnuListNode_t *next;
    s32 offset;

    if (count < 2) {
        list->stateFlags |= 2;
    }
    if (list->stateFlags & 2) {
        list->stateFlags &= ~1;
        return NULL;
    }
    if (count == 1) {
        return cursor;
    }
    last = list->last;
    if (cursor == last) {
        mnuSelectFirstListNode(list);
        mnuUpdateListScrollFlags(list);
        cursor = list->cursor;
    } else {
        if (cursor == NULL) {
            return NULL;
        }
        next = cursor->next;
        if (next == NULL) {
            return NULL;
        }
        if (keepFade == 0) {
            cursor->animationTimer = 0x100;
        }
        offset = list->windowOffset;
        cursor = next;
        list->cursor = cursor;
        list->windowOffset = offset + 1;
        if (list->windowOffset >= list->visibleCount - 1) {
            if (noScroll == 0) {
                cursor = mnuAdvanceListWindowStart(list);
                last = list->last;
            } else if (cursor != last) {
                cursor = cursor->prev;
                list->windowOffset = offset;
                list->cursor = cursor;
            }
        }
        if (cursor == last) {
            list->stateFlags |= 3;
        }
        if (cursor->next == last && (cursor->next->flags48 & 2)) {
            list->stateFlags |= 3;
        }
        mnuUpdateListScrollFlags(list);
    }
    return cursor;
}

mnuListNode_t *mnuListRetreatCursor(mnuList_t *list, s32 noScroll, s32 keepFade) {
    s32 count = list->count;
    mnuListNode_t *cursor = list->cursor;
    mnuListNode_t *first;
    mnuListNode_t *prev;
    s32 offset;

    if (count < 2) {
        list->stateFlags |= 2;
    }
    if (list->stateFlags & 2) {
        list->stateFlags &= ~1;
        return NULL;
    }
    if (count == 1) {
        return cursor;
    }
    first = list->first;
    if (cursor == first) {
        mnuSelectLastListNode(list);
        mnuUpdateListScrollFlags(list);
        cursor = list->cursor;
    } else {
        if (cursor == NULL) {
            return NULL;
        }
        prev = cursor->prev;
        if (prev == NULL) {
            return NULL;
        }
        if (keepFade == 0) {
            cursor->animationTimer = 0x100;
        }
        offset = list->windowOffset;
        cursor = prev;
        list->cursor = cursor;
        list->windowOffset = offset - 1;
        if (list->windowOffset <= 0) {
            if (noScroll == 0) {
                cursor = mnuRetreatListWindowStart(list);
                first = list->first;
            } else if (cursor != first) {
                cursor = cursor->next;
                list->windowOffset = offset;
                list->cursor = cursor;
            }
        }
        if (cursor == first) {
            list->stateFlags |= 3;
        }
        if (cursor->prev == first && (cursor->prev->flags48 & 2)) {
            list->stateFlags |= 3;
        }
        mnuUpdateListScrollFlags(list);
    }
    return cursor;
}

mnuListNode_t *mnuAdvanceListCursorDefault(mnuList_t *list) {
    return mnuListAdvanceCursor(list, 0, 0);
}

mnuListNode_t *mnuRetreatListCursorDefault(mnuList_t *list) {
    return mnuListRetreatCursor(list, 0, 0);
}
