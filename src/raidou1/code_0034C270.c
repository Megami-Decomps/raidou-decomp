#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"
#include "dds/dds3_process.h"
#include "dds/fr_font.h"
#include "dds/sdf_chip.h"

void *sdfChipAllocZeroDirect(s32 size);
void sdfChipFreeDirect(void *memory);

#include "dds/camp_menu_context.h"
#include "dds/itf_draw_grid.h"
#include "dds/eff_resource_slots.h"
#include "dds/eff_resource_source_lifetime.h"
#include "dds/mnu_input.h"
#include "dds/mnu.h"
#include "dds/mnu_list.h"
#include "dds/mnu_shop.h"
#include "dds/mnu_scroll_panel.h"
#include "dds/dat_state.h"
#include "dds/pty_roster_api.h"
#include "dds/mnu_standard_partner_requirements_api.h"
#include "dds/eff.h"
#include "dds/eff_resource_records.h"
#include "dds/itf.h"
#include "dds/mnu_staff.h"

struct MenuListNode;

typedef struct MenuList MenuList;

typedef struct MenuListNode MenuListNode;

/* Selected child window and page of the party skill-menu runtime. */
typedef struct MenuPartyRuntime {
    u8 pad00[4];
    s32 selectionList; /* 0x04: list whose cursor selects the active window */
    u8 pad08[0x1C];
    s32 selectedWindow; /* 0x24 */
    u8 pad28[4];
    s32 selectedPage;   /* 0x2C */
} MenuPartyRuntime;

/* The camp initializer allocates and clears this complete 0x38-byte child. */
typedef struct SkillBrowseRuntime {
    struct sdfMemHandle_t *allocation;
    u8 pad04[4];
    s32 active;
    struct MenuList *categoryList;
    MenuWindowContainer *windows[4];
    void *panel;
    MenuWindowContainer *selectedWindow;
    s32 activeMark;
    s32 motionSelection;
    u32 selectionFlags;
    u32 selectedIndex;
} SkillBrowseRuntime;

typedef char SkillBrowseRuntimeSizeCheck[(sizeof(SkillBrowseRuntime) == 0x38) ? 1 : -1];

/* Sprite slots of the active skill-menu context; intervening state is opaque. */
typedef struct MenuContextSprites {
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
} MenuContextSprites;

MenuList *mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing);
u32 mnuDestroyListState(MenuList *list);
void mnuUpdateListScrollFlags(MenuList *list);
MenuListNode *mnuListAppendNode();
s32 mnuListContainsFinalNode(MenuList *list);
MenuListNode *mnuListAdvanceCursor(MenuList *, s32, s32);
MenuListNode *mnuListRetreatCursor(MenuList *, s32, s32);

MenuListNode *mnuInsertListNodeRelativeToAnchor(MenuList *list, MenuListNode *anchor,
                         const void *text, s32 mode, u32 options);

MenuListNode *mnuRemoveListCursorNode(MenuList *list);
void mnuSelectFirstListNode(MenuList *list);
void mnuSelectLastListNode(MenuList *list);
MenuListNode *mnuAdvanceListWindowStart(MenuList *list);
MenuListNode *mnuRetreatListWindowStart(MenuList *list);
MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade);
MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade);
MenuListNode *mnuAdvanceListCursorDefault(MenuList *list);
MenuListNode *mnuRetreatListCursorDefault(MenuList *list);

MenuList *mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing) {
    MenuList *item = (MenuList *)sdfChipAllocZeroDirect(sizeof(MenuList));
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

u32 mnuDestroyListState(MenuList *list) {
    MenuListNode *pending;

    do {
        pending = mnuRemoveListCursorNode(list);
    } while (pending != 0);
    sdfChipFreeDirect(list);
    return 1;
}

void mnuUpdateListScrollFlags(MenuList *list) {
    MenuListNode *node = list->head;
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

INCLUDE_ASM(const s32, "code_0034C270", mnuListAppendNode);

s32 mnuListContainsFinalNode(MenuList *list) {
    MenuListNode *node = list->head;
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

MenuListNode *mnuInsertListNodeRelativeToAnchor(MenuList *list, MenuListNode *anchor,
                         const void *text, s32 mode, u32 options) {
    MenuListNode *node;
    MenuListNode *walk;

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

    node = sdfChipAllocZeroDirect(sizeof(MenuListNode));
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

MenuListNode *mnuRemoveListCursorNode(MenuList *list) {
    MenuListNode *node;
    MenuListNode *cursor;
    MenuListNode *previous;
    MenuListNode *next;

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

INCLUDE_ASM(const s32, "code_0034C270", mnuSetGridSpriteSlot);

INCLUDE_ASM(const s32, "code_0034C270", mnuWalkNodeList);

INCLUDE_ASM(const s32, "code_0034C270", mnuSeekListNode);

INCLUDE_ASM(const s32, "code_0034C270", jmsLeaf17);

INCLUDE_ASM(const s32, "code_0034C270", jmsLeaf18);

/* Advance the visible head when a prior window offset can be reduced. */
MenuListNode *mnuAdvanceListWindowStart(MenuList *list) {
    MenuListNode *cursor = list->cursor;
    MenuListNode *last = list->last;
    MenuListNode *head = list->head;

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
MenuListNode *mnuRetreatListWindowStart(MenuList *list) {
    MenuListNode *cursor = list->cursor;
    MenuListNode *head = list->head;
    MenuListNode *node;
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

MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade) {
    s32 count = list->count;
    MenuListNode *cursor = list->cursor;
    MenuListNode *last;
    MenuListNode *next;
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

MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade) {
    s32 count = list->count;
    MenuListNode *cursor = list->cursor;
    MenuListNode *first;
    MenuListNode *prev;
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

MenuListNode *mnuAdvanceListCursorDefault(MenuList *list) {
    return mnuListAdvanceCursor(list, 0, 0);
}

MenuListNode *mnuRetreatListCursorDefault(MenuList *list) {
    return mnuListRetreatCursor(list, 0, 0);
}
