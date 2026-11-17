#include "common.h"
#include "sdfMemory.h"
#include "sdfChip.h"

#include "newdata.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "basic.h"
#include "evt_world.h"
#include "fpu.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "code_r1_002CAFD8.h"
#include "code_r1_0034D128.h"
#include "code_r1_0034CE48.h"
#include "code_r1_002C8188.h"
#include "code_r1_0034C270.h"
#include "code_r1_002CA3F0.h"
#include "frFont.h"

/* Blends packed RGBA channels; weights above 0x100 are mirrored from 0x200. */
u32 fclBlendColor(u32 color0, u32 color1, u32 weight);

void mnuClearEntryFlags(struct mnuIconState_t *group);

mnuIconSprites_t *mnuCreateWindowSpriteResources(u32 width, u32 height, u32 value,
                    struct sprHandle_t *resourceHandle, s32 *indices, u32 unused);

/* Draw a window container owned by the menu window subsystem. */
void mnuDrawWindowContainer(s32 x, s32 y, s32 depth, mnuWindowContainer_t *menu, s32 drawArg);

void mnuSetWindowPanelBounds(mnuWindowContainer_t *panel,
                             const struct mnuIconState_t *layout,
                             u32 left, u32 top, u32 right, u32 bottom);

/* Shared fade-list entry. The unobserved word at 0x04 remains opaque. */
typedef struct mnuFadeEntry_t {
    u32 active; /* 0x00 */
    u32 unknown_04;
    s32 x; /* 0x08: stored draw position. */
    s32 y; /* 0x0C: stored draw position. */
    struct mnuWindowContainer_t *window; /* 0x10 */
    s32 fade; /* 0x14 */
} mnuFadeEntry_t;

typedef char MenuFadeEntry_extent_must_be_0x18[
    (sizeof(mnuFadeEntry_t) == 0x18) ? 1 : -1];

struct staffMenuRuntime_t;

#define MNU_ENTRY_SPRITE_COUNT 4

#define MNU_ENTRY_COLOR_COUNT 4

#define MNU_ENTRY_MARKED_COLOR 0x89BDC940

#define MNU_ENTRY_DEFAULT_COLOR 0x89BDC980

#define MNU_ENTRY_ALTERNATE_COLOR 0xBBEFAB80

#define MNU_NODE_FADE_STEP 0x10

#define MNU_FULL_FADE 0x100

#define MNU_PANEL_FADE_LIMIT 0x200

#define MNU_PANEL_FADE_THRESHOLD 0x101

#define MNU_WINDOW_FADE_STEP 0x20

#define MNU_WINDOW_TRANSITION_FLAG 4

#define MNU_WINDOW_TRANSITION_CLEAR_MASK 0xFFFFFFFB

#define MNU_LIST_SELECTION_FLAG 8

#define MNU_LIST_ALTERNATE_SELECTION_FLAG 0x10

#define MNU_WINDOW_CONTAINER_BYTES 0x98

#define MNU_PANEL_LAYOUT_BYTES 0x38

#define MNU_WINDOW_RESOURCE_SPRITES 3

#define MNU_PANEL_KIND_LIMIT 6

#define MNU_SORT_KEY_COUNT 3

#define MNU_SORT_COMPARATOR_COUNT 6

#define MNU_LIST_POINTER_BYTES 4

#define MNU_FADING_SLOT_COUNT 4

#define MNU_FADING_SLOT_BYTES 0x18

#define MNU_FADING_MUTATION_THRESHOLD 5

#define MNU_FADING_WORD_STEP 0x40

#define MNU_STAFF_PARTY_SLOT_COUNT 5

#define MNU_STAFF_PARTY_LAST_SLOT 4

#define MNU_STAFF_DISPLAY_OVERFLOW 4

#define MNU_STAFF_DISPLAY_LIMIT 3

#define MNU_STAFF_PARTY_ENTRY_BYTES 0x1C4

#define MNU_STAFF_PARTY_HALFWORD_STRIDE 0xE2

#define MNU_STAFF_PARTY_BASE 0xA60

#define MNU_STAFF_BACKUP_BYTES 0x8D4

#define MNU_STAFF_PARTY_ACTIVE_BIT 1

#define MNU_STAFF_NODE_UNAVAILABLE 1

#define MNU_STAFF_NODE_SELECTED 2

#define MNU_STAFF_PARTY_PANEL_BASE 0x284

#define MNU_STAFF_PARTY_SLOTS_BASE 0xA928

#define MNU_STAFF_FADE_STEP 0x10

#define MNU_STAFF_FADE_CLOSE_THRESHOLD 0x50

#define MNU_STAFF_FADE_OPEN_THRESHOLD 0xB0

#define MNU_STAFF_INPUT_CONFIRM 1

#define MNU_STAFF_INPUT_CANCEL 2

#define MNU_STAFF_POPUP_INPUT_MASK 3

#define MNU_STAFF_VIEW_INPUT_MASK 0xC2

#define MNU_STAFF_VIEW_TOGGLE_MASK 0xC0

#define MNU_STAFF_PAGE_INPUT_MASK 0x300

#define MNU_STAFF_INPUT_PREV_PAGE 0x100

#define MNU_STAFF_INPUT_NEXT_PAGE 0x200

#define MNU_STAFF_INPUT_PREVIOUS 0x10

#define MNU_STAFF_INPUT_NEXT 0x20

#define MNU_STAFF_INPUT_NAV_STATE_MASK 0x300000

#define MNU_STAFF_INPUT_REJECTED 0x8000

#define MNU_STAFF_SKILL_INPUT_MASK 0x33

#define MNU_STAFF_REORDER_INPUT_MASK 0x37

#define MNU_STAFF_REORDER_CANCEL_MASK 6

typedef struct mnuList_t mnuList_t;

typedef struct mnuIconState_t mnuIconState_t;

extern void itfSetGridEntryQuantizedAndRefresh(sprHandle_t *, s32, s32, s32, s32, s32);
extern void mnuClearListFlagsOneAndTwo(u32 *flags);
extern void mnuDrawIconPanel(s32, s32, s32, s32, mnuIconState_t *, s32, s32);
extern void mnuUpdateWindowPanelHandleStatesKindFourFive(mnuIconState_t *);
extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, s32, sprHandle_t *, s32, s32);

/* Menu runtime fields shared by the party, panel and resource handlers. */
typedef struct mnuContext_t {
    u8 pad00[0x54];
    s32 popupState[3];     /* 0x54: passed to the popup state handlers */
    s32 displayHandle;     /* 0x60 */
    s32 resourceHandle;    /* 0x64 */
    void *displayResource; /* 0x68 */
    s32 alternateResource; /* 0x6C: used when swapping the staff panel view */
    u8 pad70[0x58];
    s32 labelHandle;       /* 0xC8 */
    u8 padCC[0x38];
    s32 imageHandle;       /* 0x104 */
    u8 pad108[4];
    s32 listHandle;        /* 0x10C */
    u8 pad110[8];
    s32 panelHandle;       /* 0x118 */
    u8 pad11C[0x168];
    mnuPageWindow_t partyWindow; /* 0x284: lists, page slots and selection */
    s32 partyPanelActive;  /* 0xA928 */
    s32 partyPanelLast;    /* 0xA92C */
    u8 padA930[0x104];
    mnuPanelGroup_t *panelGroup; /* 0xAA34 */
    s32 panelRequest;      /* 0xAA38 */
    mnuSpriteState_t *panelEffects; /* 0xAA3C */
    u8 padAA40[8];
    s32 party;             /* 0xAA48 */
    u8 padAA4C[0x10];
    s32 resourceList;      /* 0xAA5C */
} mnuContext_t;

typedef struct mnuListNode_t mnuListNode_t;

#define PARTY_BACKUP_OFFSET ((s32)&((PartyMenuData *)0)->backup)

/* Staff/skill-menu work prefix, separate from the party-selection allocation. */
typedef struct mnuPartyRuntime_t {
    u8 pad00[0x10];
    s32 staffMode; /* 0x10 */
    s32 staffView; /* 0x14: selects the input mask and panel view */
    s32 staffSelection; /* 0x18: chosen list entry */
    s32 staffExit; /* 0x1C: signals the close transition */
    u8 pad20[4];
    mnuWindowContainer_t *selectedWindow; /* 0x24 */
    s32 activeMark; /* 0x28 */
    u8 pad2C[8];
    u32 selectedIndex; /* 0x34: compared with mnuListNode_t.index */
} mnuPartyRuntime_t;

/* The idle-motion timer shares the party work area with other menu states. */
typedef struct mnuVoiceState_t {
    u8 pad00[0x28];
    s32 idleFrames;
    s32 motionSelection; /* -1 enables periodic motion */
} mnuVoiceState_t;

typedef struct mnuListDefaults_t {
    s32 indices[3];
} mnuListDefaults_t;

extern mnuListDefaults_t D_0042AF00;

/* First confirm stores a slot; a different second slot swaps and rebuilds.
 * Cancel clears the saved slot, opening the exit popup only if none was saved. */
typedef struct mnuPanelBlock_t {
    u32 word[14];
} mnuPanelBlock_t;

typedef struct mnuPanelWindow_t {
    u8 pad0[4];
    s32 field4;
    u8 pad8[0x10];
    mnuList_t *list; /* 0x18 */
    u8 pad1C[0x3C];
    mnuPanelBlock_t block; /* 0x58 */
    u8 pad90[4];
    s32 field94;
} mnuPanelWindow_t;

typedef struct mnuBadgePlace_t {
    s32 slot;
    s32 x;
    s32 y;
} mnuBadgePlace_t;

typedef struct mnuBadgeLayout_t {
    mnuBadgePlace_t place[2];
} mnuBadgeLayout_t;

#define MNU_ADVANCE_FADE(value, amount, maximum) { \
    if ((value) < (maximum)) { \
        (value) += (amount); \
    } \
    if ((value) > (maximum)) { \
        (value) = (maximum); \
    } \
}

mnuListNode_t *mnuListAdvanceCursor(mnuList_t *list, s32 noScroll, s32 keepFade);
mnuListNode_t *mnuListRetreatCursor(mnuList_t *list, s32 noScroll, s32 keepFade);
void mnuDrawFourEntries(s32 x, s32 y, s32 depth, mnuList_t *list, mnuListNode_t *node, s32 drawArg);
u32 mnuBlendListNodeColorByFlags(u32 previousColor, mnuListNode_t *entry);
void mnuDispatchEntryWords(sprHandle_t *menu, s32 entryIndex, mnuListNode_t *entry);
extern const char mnuDrawWindowListEntriesData[];

/* Draw visible rows, preserving each row's glyph chain and callback state. */
void mnuDrawWindowListEntries(s32 x, s32 y, s32 depth, s32 xOffset, s32 yOffset,
                   s32 fadeScale, u32 unusedFlags, mnuList_t *list, s32 drawArg);

void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg);

/* Destroy the owned list and optional sprite resources before freeing the window. */
void jmsDestroyCmdMenu(mnuWindowContainer_t *menu);

void mnuRemoveWindowListCursorNode(mnuWindowContainer_t *menu);
mnuListNode_t *mnuAdvanceListSelection(mnuWindowContainer_t *menu, s32 step);
mnuListNode_t *mnuReverseListSelection(mnuWindowContainer_t *menu, s32 step);
void mnuClearWindowListSelectionFlags(mnuWindowContainer_t *menu);

void mnuInitIconSprites(mnuIconSprites_t *obj, s32 w, s32 h, u32 value,
                        sprHandle_t *resource, s32 *idx, s32 unused);

void mnuDrawWindowResourceSpriteRows(s32 x, s32 y, u32 flags, mnuWindowContainer_t *window, u32 option);
void mnuDrawWindowIconRows(s32 x, s32 y, u32 flags, mnuWindowContainer_t *window, s32 count, s32 option);
void mnuDrawVisibleWindowIconRows(u32 x, u32 y, u32 flags, mnuWindowContainer_t *window, u32 option);

/* Draw the window, then advance its fade scale without a post-addition clamp. */

typedef struct mnuPos_t {
    s32 x;
    s32 y;
} mnuPos_t;

typedef struct mnuPosTable_t {
    mnuPos_t pos[6];
} mnuPosTable_t;

typedef struct mnuPosTable3_t {
    mnuPos_t pos[3];
} mnuPosTable3_t;

extern const mnuPosTable3_t D_0042AF48;
void mnuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, mnuIconState_t *state, s32 mode, s32 surface);
extern const mnuPosTable_t D_0042AF60;
void mnuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, mnuIconState_t *state, s32 surface);

typedef struct mnuOffsets_t {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} mnuOffsets_t;

extern const mnuOffsets_t D_0042AF90;
void mnuDrawIconPair(s32 x, s32 y, s32 z, s32 w, mnuIconState_t *state, s32 surface);
extern void mnuDrawIconPanelFade();
extern void mnuDrawIconRow6();
extern void mnuDrawIconPair();
void mnuDrawIconPanel(s32 x, s32 y, s32 depth, s32 fade, mnuIconState_t *panel, s32 selectionMode, s32 drawArg);
void mnuDrawIconState(u32 x, u32 y, u32 depth, u32 fade, mnuIconState_t *panel, u32 selectionMode, u32 drawArg);
void mnuDrawIconPanelFullFade(u32 x, u32 y, u32 depth, mnuIconState_t *panel, s32 drawArg);
void mnuUpdateWindowPanelHandleStatesKindFourFive(mnuIconState_t *obj);
void mnuRebuildListFirstFromCursor(mnuList_t *list);
void mnuRebuildListLastFromCursor(mnuList_t *list);
void mnuLinkItemList(mnuListNode_t **nodeArray, s32 count);

/* Three-way comparison of unsigned primary keys, descending without subtraction. */
s32 mnuComparePrimaryKeyDescending(mnuListNode_t **left, mnuListNode_t **right);

/* Three-way comparison of unsigned primary keys, ascending without subtraction. */
s32 mnuComparePrimaryKeyAscending(mnuListNode_t **left, mnuListNode_t **right);

/* Three-way comparison of unsigned secondary keys, descending without subtraction. */
s32 mnuCompareSecondaryKeyDescending(mnuListNode_t **left, mnuListNode_t **right);

/* Three-way comparison of unsigned secondary keys, ascending without subtraction. */
s32 mnuCompareSecondaryKeyAscending(mnuListNode_t **left, mnuListNode_t **right);

/* Three-way comparison of unsigned tertiary keys, descending without subtraction. */
s32 mnuCompareTertiaryKeyDescending(mnuListNode_t **left, mnuListNode_t **right);

/* Three-way comparison of unsigned tertiary keys, ascending without subtraction. */
s32 mnuCompareTertiaryKeyAscending(mnuListNode_t **left, mnuListNode_t **right);
void mnuSortItems(mnuList_t *menu, s32 keyIndex, s32 ascending);
void mnuAllocateListEntries(s32 *list);
void mnuFreeListEntries(s32 *list);
void mnuRemoveFadingWindowEntry(s32 *list, u32 slotIndex);
void mnuDrawFadingWindows(s32 depth, s32 *list, s32 drawArg);
void mnuUpdateFade(s32 *list);

typedef struct mnuSpritePlacement_t {
    sprHandle_t *source;
    u32 index;
    s32 x;
    s32 y;
} mnuSpritePlacement_t;

typedef struct mnuGatedSpritePlacement_t {
    sprHandle_t *source;
    u32 index;
    s32 x;
    s32 y;
    s8 requiresFlag;
} mnuGatedSpritePlacement_t;

typedef struct mnuIconEntry_t {
    u32 id;
    s32 x;
    s32 y;
} mnuIconEntry_t;

typedef struct mnuIconLayout_t {
    mnuIconEntry_t entry[3];
} mnuIconLayout_t;

typedef struct mnuPanelSequenceSettings_t {
    s32 rows[5][4];
    const char *captions[2];
} mnuPanelSequenceSettings_t;

/* Subtract the fade step only when positive, then clamp any negative result to zero. */
void mnuDecreaseListNodeFadeCounters(mnuList_t *list) {
    mnuListNode_t *node = list->first;
    if (node != NULL) {
        do {
            s32 timer = node->animationTimer;
            s32 reduced = timer - MNU_NODE_FADE_STEP;
            if (timer > 0) {
                node->animationTimer = reduced;
                timer = reduced;
            }
            if (timer < 0) {
                node->animationTimer = 0;
            }
            node = node->next;
        } while (node != NULL);
    }
}

/* Draw one four-sprite bank; the cursor entry selects the second bank. */
void mnuDrawFourEntries(s32 x, s32 y, s32 depth, mnuList_t *list, mnuListNode_t *node, s32 drawArg) {
    u32 spriteIndex = 0;
    do {
        s32 selected = node == list->cursor;
        s32 index = selected * MNU_ENTRY_SPRITE_COUNT + spriteIndex;
        struct sprHandle_t *sprite = node->sprites[index].sprite;
        if (sprite != 0) {
            itfDrawGridWithResolvedSlot(x, y, depth, 0, sprite, node->sprites[index].effect, drawArg);
        }
        spriteIndex++;
    } while (spriteIndex < MNU_ENTRY_SPRITE_COUNT);
}

/* Blend the flag-selected packed color with the caller's previous color.
 * Flag one takes precedence. */
u32 mnuBlendListNodeColorByFlags(u32 previousColor, mnuListNode_t *entry) {
    u32 flags = entry->flags48;
    u32 color = MNU_ENTRY_MARKED_COLOR;
    if (!(flags & 1)) {
        color = (flags & 4) ? MNU_ENTRY_ALTERNATE_COLOR : MNU_ENTRY_DEFAULT_COLOR;
    }
    return fclBlendColor(color, previousColor, entry->animationTimer);
}

/* Apply the same entry-state blend to all four packed colors in one slot. */
void mnuDispatchEntryWords(sprHandle_t *menu, s32 entryIndex, mnuListNode_t *entry) {
    s32 colorIndex;

    for (colorIndex = 0; colorIndex < MNU_ENTRY_COLOR_COUNT; colorIndex++) {
        u32 previousColor = menu->workEntries[entryIndex].geometry.cornerColors[colorIndex];

        menu->workEntries[entryIndex].geometry.cornerColors[colorIndex] = mnuBlendListNodeColorByFlags(previousColor, entry);
    }
}

/* NON_MATCHING: 18 of 148 words match.  */
#ifdef NON_MATCHING
void mnuDrawWindowListEntries(s32 x, s32 y, s32 depth, s32 xOffset, s32 yOffset,
                   s32 fadeScale, u32 unusedFlags, mnuList_t *list, s32 drawArg) {
    char text[0x10];
    mnuListNode_t *node = list->head;
    s32 textY = y;
    s32 row = 0;

    mnuDecreaseListNodeFadeCounters(list);
    while (node != NULL && row < list->visibleCount) {
        u32 color = 0xA09DC340;
        frFontHandle_t *glyph = NULL;
        u8 colorNo = 0;

        if ((node->flags48 & 1) == 0) {
            color = 0xD7CC9880;
            if ((node->flags48 & 4) == 0) {
                color = 0xA09DC380;
            }
        }
        color = mnuBlendListNodeColorByFlags(color, node);
        color = fclBlendColor(color, color & 0xFFFFFF00, fadeScale);
        if ((node == list->cursor && (list->stateFlags & 8) == 0) || (node->flags48 & 2) != 0) {
            colorNo = 4;
        }
        mnuDrawFourEntries(x, textY, depth, list, node, drawArg);
        if (node->text != NULL) {
            glyph = frLinkKanjiSub(x + xOffset, textY + yOffset, depth, color, node->text, NULL);
            frSetFontColorNo(glyph, colorNo);
            if ((list->id & 1) != 0) {
                sprintf(text, mnuDrawWindowListEntriesData, node->sortKeyPrimary);
                glyph = frwGetSmallNumFontFRQ(x + 0x1180, textY + 0x20, depth, color, (u8 *)text, glyph);
                frSetFontColorNo(glyph, colorNo);
            }
        }
        if (glyph != NULL) {
            frDrawFontOt(glyph, 1, drawArg);
            frDestroyFontRequestAfter(glyph);
        }
        if (list->drawCallback != NULL) {
            if (list->drawCallback(x, y, depth, list, node, drawArg) == 0) {
                return;
            }
        }
        row++;
        y += list->rowStep;
        textY += list->rowStep;
        node = node->next;
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0034D128", mnuDrawWindowListEntries);
#endif

/* Invoke the native window renderer at full fade with its size/flag arguments zeroed. */
void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg) {
    mnuDrawWindowListEntries(x, y, depth, 0, 0, MNU_FULL_FADE, 0, (mnuList_t *)(u32)menu, drawArg);
}

/* Allocate a zeroed window and its list; the last two arguments configure list rows. */
mnuWindowContainer_t *jmsCreateCmdMenu(s32 id, s32 width, s32 height, s32 visibleCount, s32 rowSpacing) {
    mnuWindowContainer_t *window = (mnuWindowContainer_t *)sdfChipAllocZeroDirect(sizeof(mnuWindowContainer_t));
    mnuList_t *list;
    window->width = width;
    window->height = height;
    window->id = id;
    list = mnuCreateListState(id, visibleCount, rowSpacing);
    window->fadeScale = 0;
    window->list = list;
    return window;
}

INCLUDE_ASM(const s32, "code_r1_0034D128", jmsDestroyCmdMenu);

INCLUDE_ASM(const s32, "code_r1_0034D128", jmsLeaf22);

INCLUDE_ASM(const s32, "code_r1_0034D128", jmsLeaf23);

void mnuSetWindowContainerLayout(mnuWindowContainer_t *menu, struct sprHandle_t *layout2C, u32 layout30, struct sprHandle_t *layout34,
                                    u32 layout48, u32 layout38, struct sprHandle_t *layout3C, u32 layout40,
                                    u32 layout4C) {
    menu->decorations[0].sprite = layout2C;
    menu->decorationX[2] = layout4C;
    menu->decorations[0].parameter = layout30;
    menu->decorations[1].sprite = layout34;
    menu->decorations[1].parameter = layout38;
    menu->decorationX[1] = layout48;
    menu->decorations[2].sprite = layout3C;
    menu->decorations[2].parameter = layout40;
    menu->decorationX[0] = 0;
}

void mnuInitializeBasicWindowLayout(mnuWindowContainer_t *menu, struct sprHandle_t *first, u32 second) {
    mnuSetWindowContainerLayout(menu, first, second, 0, 0, 0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "code_r1_0034D128", jmsLeaf24);

void mnuSetWindowPanelBounds(mnuWindowContainer_t *panel, const struct mnuIconState_t *layout,
                             u32 left, u32 top, u32 right, u32 bottom) {
    panel->panel = *layout;
    panel->panel.left = left;
    panel->panel.top = top;
    panel->panel.right = right;
    panel->panel.bottom = bottom;
    panel->flags |= 4;
}

void mnuCreateListWithDefaults(mnuWindowContainer_t *menu, u32 first, u32 second,
                               u32 third, sprHandle_t *spriteResource) {
    mnuListDefaults_t defaults = D_0042AF00;
    menu->resource =
        mnuCreateWindowSpriteResources(first, second, third, spriteResource,
                                       defaults.indices, 3);
}

void CmdMenuCursorDrawUnlock(mnuWindowContainer_t *window) {
    window->flags = window->flags & ~4;
}

mnuListNode_t *jmsAddCmdMenuItem(mnuWindowContainer_t *menu, const void *text) {
    return mnuListAppendNode(menu->list, text);
}

/* Insert `value` beside anchor in the window's list (see mnuInsertListNodeRelativeToAnchor). */
mnuListNode_t *mnuInsertWindowListNodeRelativeToAnchor(mnuWindowContainer_t *menu, mnuListNode_t *anchor, const void *text, s32 mode, u32 options) {
    return mnuInsertListNodeRelativeToAnchor(menu->list, anchor, text, mode, options);
}

void mnuRemoveWindowListCursorNode(mnuWindowContainer_t *menu) {
    mnuRemoveListCursorNode(menu->list);
}

/* Advance selection; clear its byte and panel sprite flags only when a node is returned. */
mnuListNode_t *mnuAdvanceListSelection(mnuWindowContainer_t *menu, s32 step) {
    mnuListNode_t *selected = mnuListAdvanceCursor(menu->list, step, 0);
    if (selected != NULL) {
        selected->selectionByte54 = 0;
        mnuClearEntryFlags(&menu->panel);
    }
    return selected;
}

/* Retreat selection with the same conditional byte/panel cleanup as advancement. */
mnuListNode_t *mnuReverseListSelection(mnuWindowContainer_t *menu, s32 step) {
    mnuListNode_t *selected = mnuListRetreatCursor(menu->list, step, 0);
    if (selected != NULL) {
        selected->selectionByte54 = 0;
        mnuClearEntryFlags(&menu->panel);
    }
    return selected;
}

mnuListNode_t *mnuAdvanceWindowListSelection(mnuWindowContainer_t *menu) {
    return mnuAdvanceListSelection(menu, 0);
}

mnuListNode_t *mnuRetreatWindowListSelection(mnuWindowContainer_t *menu) {
    return mnuReverseListSelection(menu, 0);
}

INCLUDE_ASM(const s32, "code_r1_0034D128", jmsLeaf25);

void mnuClearWindowListSelectionFlags(mnuWindowContainer_t *menu) {
    mnuClearListFlagsOneAndTwo(&menu->list->stateFlags);
}

void mnuInitIconSprites(mnuIconSprites_t *obj, s32 w, s32 h, u32 value,
                        sprHandle_t *resource, s32 *idx, s32 unused) {
    obj->value = value;
    obj->sprite[0] = effCreateResourceSlotSet(resource, idx[0], 1);
    obj->sprite[1] = effCreateResourceSlotSet(resource, idx[1], 1);
    obj->sprite[2] = effCreateResourceSlotSet(resource, idx[2], 1);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[0], 0, w, h, w, h);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[1], 0, w, h, w, h);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[2], 0, w, h, w, h);
}

/* Create an owned three-sprite bundle using the caller's slot-index array. */
mnuIconSprites_t *mnuCreateWindowSpriteResources(u32 width, u32 height, u32 value,
                    sprHandle_t *resourceHandle, s32 *indices, u32 unused) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(0x18);
    mnuIconSprites_t *bundle = (void *)sdfMemLock(allocation);
    memset(bundle, 0, 0x18);
    bundle->allocation = allocation;
    mnuInitIconSprites(bundle, width, height, value, resourceHandle, indices, unused);
    return bundle;
}

/* Destroy every native sprite slot, then release the bundle's allocation handle. */
void mnuReleaseWindowTextures(mnuIconSprites_t *menu) {
    u32 spriteIndex = 0;
    do {
        effDestroyResourceSlotSet(menu->sprite[spriteIndex]);
        spriteIndex++;
    } while (spriteIndex < MNU_WINDOW_RESOURCE_SPRITES);
    sdfMemFreeDirect(menu->allocation);
}

void jmsLeaf26(void) {
}

void mnuDrawListScrollbar(s32 x, s32 y, s32 depth, u32 opacity, mnuList_t *list,
                   mnuIconSprites_t *resource, s32 drawArg) {
    s32 top;
    s32 trackExtent;
    s32 bottom;
    s32 count;
    s32 visible;
    s32 rowOffset;
    sprHandle_t *firstSprite;
    effSlotDescription_t *firstDescription;
    itfBdWork_t *firstWork;
    itfBdWork_t *lastWork;

    if (resource == NULL) {
        return;
    }

    firstSprite = resource->sprite[0];
    firstDescription = firstSprite->descriptions;
    firstWork = firstSprite->workEntries;
    lastWork = resource->sprite[2]->workEntries;
    trackExtent = firstDescription->rect[1] * 8 - 0x28;
    top = firstWork->geometry.bounds[3];
    top += trackExtent;
    bottom = lastWork->geometry.bounds[3];
    count = list->count;
    visible = list->visibleCount;
    if (count == 0 || visible == 0 || count <= visible) {
        return;
    }

    rowOffset = (top - bottom) * list->head->index / (count - visible);
    effDrawTextureSlotWithBlendedCornerColors(x, y, depth, opacity, 1,
                  firstSprite, 0, drawArg);
    effDrawTextureSlotWithBlendedCornerColors(x, y + top, depth, opacity, 1,
                  resource->sprite[1], 0, drawArg);
    effDrawTextureSlotWithBlendedCornerColors(x, y + rowOffset + 0x28, depth, opacity, 1,
                  resource->sprite[2], 0, drawArg);
}

void mnuDrawWindowResourceSpriteRows(s32 x, s32 y, u32 flags, mnuWindowContainer_t *window, u32 option) {
    mnuDrawListScrollbar(x - 0xf0, y - 8, flags, window->fadeScale,
                  window->list, window->resource, option);
}

void jmsFunc45(u32 x, u32 y, s32 depth, mnuWindowContainer_t *window, s32 count, s32 layer) {
    s32 i;
    struct sprHandle_t *sprite = window->spriteResource;
    u32 fade = window->fadeScale;
    s32 suppressBase = window->suppressBaseSprite;

    if (sprite != NULL) {
        if (suppressBase == 0) {
            effDrawSpriteWithCornerFade(x - 0xD0, y - 0xB8, depth, fade, 1, sprite, window->baseSpriteSlot, layer);
        }
        for (i = 0; i < count; i++) {
            effDrawSpriteWithCornerFade(x + window->originX, i * window->list->rowStep + y + window->originY, depth, fade, 1, window->spriteResource, window->rowSpriteSlot, layer);
        }
    }
}

void mnuDrawVisibleWindowIconRows(u32 x, u32 y, u32 flags, mnuWindowContainer_t *window, u32 option) {
    mnuDrawWindowIconRows(x, y, flags, window, window->list->visibleCount, option);
}

void jmsFunc46(s32 x, s32 y, s32 depth, mnuWindowContainer_t *window, s32 layer) {
    u32 i;
    u32 fade = window->fadeScale;

    for (i = 0; i < 3; i++) {
        struct sprHandle_t *sprite = window->decorations[i].sprite;
        u32 slot = window->decorations[i].parameter;
        u32 offset = window->decorationX[i];

        if (sprite != NULL) {
            effDrawSpriteWithCornerFade(x + offset + 0xC0, y - 0xB8, depth, fade, 1, sprite, slot, layer);
        }
    }
}

INCLUDE_ASM(const s32, "code_r1_0034D128", mnuDrawWindowSelectionPanel);

INCLUDE_ASM(const s32, "code_r1_0034D128", jmsFunc47);

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0027BF00.c:599, rework, same function 1.00 (words 15/15, calls 1.00, shape 1.00) */
void mnuSpinForWindowListCount(mnuWindowContainer_t *window) {
    s32 remaining;

    remaining = window->list->count;
    if (0 < remaining) {
        do {
            remaining = remaining - 1;
        } while (remaining != 0);
    }
}

INCLUDE_RODATA(const s32, "code_r1_0034D128", mnuCreateListWithDefaultsTable);

mnuIconState_t *mnuCreatePanelIconState(u32 mode, s32 resource, s32 material) {
    s32 keys[6] = { 0x20, 0x22, 0x1F, 0x21, 0x22, 0x1F };
    mnuIconState_t *panel;
    s32 index;
    s32 first;

    panel = sdfChipAllocZeroDirect(sizeof(mnuIconState_t));
    panel->kind = mode;
    switch (mode) {
    case 0:
    case 1:
        panel->count = 3;
        first = mode == 1 ? 3 : 0;
        for (index = 0; index < panel->count; index++) {
            panel->sprite[index] = effCreateResourceSlotSet((sprHandle_t *)resource, keys[first + index], 1);
        }
        break;
    case 2:
    case 3:
        panel->count = 2;
        first = mode == 3 ? 3 : 0;
        for (index = 0; index < panel->count; index++) {
            panel->sprite[index] = effCreateResourceSlotSet((sprHandle_t *)resource, keys[first + index], 1);
        }
        break;
    case 4:
        panel->count = 6;
        panel->sprite[0] = effCreateResourceSlotSet((sprHandle_t *)resource, 0x16, 1);
        panel->sprite[1] = effCreateResourceSlotSet((sprHandle_t *)resource, 0x15, 1);
        panel->sprite[2] = effCreateResourceSlotSet((sprHandle_t *)resource, 0x0D, 1);
        panel->sprite[3] = effCreateResourceSlotSet((sprHandle_t *)resource, 0x0D, 1);
        panel->sprite[4] = effCreateResourceSlotSet((sprHandle_t *)resource, 0x0E, 1);
        panel->sprite[5] = effCreateResourceSlotSet((sprHandle_t *)resource, 0x0E, 1);
        effConfigureIndexedSlotMaterial(panel->sprite[2], 0, (struct effMappedResource_t *)material, 1, 0, 0, 0x0C);
        effConfigureIndexedSlotMaterial(panel->sprite[4], 0, (struct effMappedResource_t *)material, 1, 0, 0, 0x0C);
        break;
    case 5:
        panel->count = 2;
        panel->sprite[0] = effCreateResourceSlotSet((sprHandle_t *)resource, 0x0D, 1);
        panel->sprite[1] = effCreateResourceSlotSet((sprHandle_t *)resource, 0x0E, 1);
        break;
    }
    return panel;
}

/* Reset low sprite flags only for a present first sprite and a supported panel kind. */
void mnuClearEntryFlags(mnuIconState_t *group) {
    s32 spriteIndex;
    if (group->sprite[0] != NULL && group->kind < MNU_PANEL_KIND_LIMIT) {
        for (spriteIndex = 0; spriteIndex < group->count; spriteIndex++) {
            sprHandle_t *obj = group->sprite[spriteIndex];
            u32 *flags = &obj->workEntries->states[0].flags;
            *flags &= ~1;
            effInitializeSlotWork(obj, 0);
        }
    }
}

/* Destroy nonzero resource slots, retaining the native per-iteration count read, then free. */
void mnuReleaseResourceList(mnuIconState_t *list) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < list->count; slotIndex++) {
        if (list->sprite[slotIndex] != 0) {
            effDestroyResourceSlotSet(list->sprite[slotIndex]);
        }
    }
    sdfChipFreeDirect(list);
}

void mnuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, mnuIconState_t *state, s32 mode, s32 surface) {
    mnuPosTable3_t table = D_0042AF48;
    s32 width = state->sprite[0]->workEntries->sourceWidth << 4;
    s32 i;
    s32 a;
    if (state->fade > 0x100) {
        alpha = 0x200 - state->fade;
    }
    for (i = 0; i < state->count; i++) {
        if (i == 0 && mode != 1) {
            a = state->fade < 0x100 ? alpha : 0x100;
        } else {
            a = alpha;
        }
        a = i == 2 ? 0x100 : a;
        if (i == 2 && mode == 2) {
            continue;
        }
        effDrawTextureSlotWithBlendedCornerColors(x + table.pos[i].x, y + table.pos[i].y, z, a, 1, state->sprite[i], 0, surface);
    }
    state->sprite[0]->workEntries->geometry.bounds[2] = width;
    if (state->count >= 3) {
        state->sprite[2]->workEntries->geometry.bounds[2] = width;
    }
}

void mnuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, mnuIconState_t *state, s32 surface) {
    mnuPosTable_t table = D_0042AF60;
    s32 i;
    for (i = 0; i < state->count; i++) {
        if (i != 3 && i != 5) {
            effDrawTextureSlotWithBlendedCornerColors(x + table.pos[i].x, y + table.pos[i].y, z, w, 1, state->sprite[i], 0, surface);
        }
    }
}

void mnuDrawIconPair(s32 x, s32 y, s32 z, s32 w, mnuIconState_t *state, s32 surface) {
    mnuOffsets_t offset = D_0042AF90;
    effDrawTextureSlotWithBlendedCornerColors(x + offset.x0, y + offset.y0, z, w, 1, state->sprite[0], 0, surface);
    effDrawTextureSlotWithBlendedCornerColors(x + offset.x1, y + offset.y1, z, w, 1, state->sprite[1], 0, surface);
}

/* Dispatch six panel kinds; only kind four forces full fade. */
void mnuDrawIconPanel(s32 x, s32 y, s32 depth, s32 fade, mnuIconState_t *panel, s32 selectionMode, s32 drawArg) {
    switch (panel->kind) {
    case 0:
    case 1:
    case 2:
    case 3:
        mnuDrawIconPanelFade(x, y, depth, fade, panel, selectionMode, drawArg);
        return;
    case 4:
        mnuDrawIconRow6(x, y, depth, MNU_FULL_FADE, panel, drawArg);
        return;
    case 5:
        mnuDrawIconPair(x, y, depth, fade, panel, drawArg);
        break;
    }
}

void mnuDrawIconState(u32 x, u32 y, u32 depth, u32 fade, mnuIconState_t *panel, u32 selectionMode, u32 drawArg) {
    mnuDrawIconPanel(x, y, depth, fade, panel, selectionMode, drawArg);
}

void mnuDrawIconPanelFullFade(u32 x, u32 y, u32 depth, mnuIconState_t *panel, s32 drawArg) {
    mnuDrawIconState(x, y, depth, MNU_FULL_FADE, panel, 0, drawArg);
}

void mnuUpdateWindowPanelHandleStatesKindFourFive(mnuIconState_t *obj) {
    switch (obj->kind) {
    case 4:
        itfGridLookupValueOrDefault(obj->sprite[2], 0);
        itfGridLookupValueOrDefault(obj->sprite[3], 0);
        itfGridLookupValueOrDefault(obj->sprite[4], 0);
        itfGridLookupValueOrDefault(obj->sprite[5], 0);
        return;
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 5:
        itfGridLookupValueOrDefault(obj->sprite[0], 0);
        itfGridLookupValueOrDefault(obj->sprite[1], 0);
        break;
    }
}

/* Rebuild the first-node pointer by walking backward from the cursor. */
void mnuRebuildListFirstFromCursor(mnuList_t *list) {
    s32 node;
    s32 first;
    s32 previous;

    previous = (s32)list->cursor;
    first = (s32)list->cursor;
    while (node = previous, node != 0) {
        first = node;
        previous = (s32)((mnuListNode_t *)node)->prev;
    }
    list->first = (mnuListNode_t *)first;
}

/* Rebuild the last-node pointer by walking forward from the cursor. */
void mnuRebuildListLastFromCursor(mnuList_t *list) {
    s32 node;
    s32 last;
    s32 next;

    next = (s32)list->cursor;
    last = (s32)list->cursor;
    while (node = next, node != 0) {
        last = node;
        next = (s32)((mnuListNode_t *)node)->next;
    }
    list->last = (mnuListNode_t *)last;
}

/* Reset the viewport/cursor to the first node; exactly one requests replay
 * toward the saved cursor, rather than treating every nonzero value as true. */
void mnuResetNodeLinks(mnuList_t *list, s32 restoreCursor) {
    mnuListNode_t *first;
    mnuListNode_t *oldCursor;

    list->windowOffset = 0;
    first = list->first;
    oldCursor = list->cursor;
    list->head = first;
    list->cursor = first;
    if (restoreCursor == 1) {
        mnuListNode_t *node = first;
        if (node == NULL) {
            return;
        }
        do {
            if (node == oldCursor) {
                return;
            }
            mnuAdvanceListCursorDefault(list);
            node = node->next;
        } while (node != NULL);
    }
}

/* Relink and reindex the pointer array. Native endpoint writes require at
 * least two entries; zero/one-entry calls are not guarded here. */
void mnuLinkItemList(mnuListNode_t **nodeArray, s32 count) {
    s32 itemIndex;

    nodeArray[0]->prev = NULL;
    nodeArray[0]->next = nodeArray[1];
    for (itemIndex = 1; itemIndex < count - 1; itemIndex++) {
        nodeArray[itemIndex]->prev = nodeArray[itemIndex - 1];
        nodeArray[itemIndex]->next = nodeArray[itemIndex + 1];
    }
    nodeArray[count - 1]->prev = nodeArray[count - 2];
    nodeArray[count - 1]->next = NULL;
    for (itemIndex = 0; itemIndex < count; itemIndex++) {
        nodeArray[itemIndex]->index = itemIndex;
    }
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0027BF00.c:989, rework, same function 1.00 (words 10/10, calls 1.00, shape 1.00) */
s32 mnuComparePrimaryKeyDescending(mnuListNode_t **left, mnuListNode_t **right) {
    u32 leftKey = (*left)->sortKeyPrimary;
    u32 rightKey = (*right)->sortKeyPrimary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0027BF00.c:1000, rework, same function 1.00 (words 12/12, calls 1.00, shape 1.00) */
s32 mnuComparePrimaryKeyAscending(mnuListNode_t **left, mnuListNode_t **right) {
    u32 leftKey = (*left)->sortKeyPrimary;
    u32 rightKey = (*right)->sortKeyPrimary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0027BF00.c:1011, rework, same function 1.00 (words 10/10, calls 1.00, shape 1.00) */
s32 mnuCompareSecondaryKeyDescending(mnuListNode_t **left, mnuListNode_t **right) {
    u32 leftKey = (*left)->sortKeySecondary;
    u32 rightKey = (*right)->sortKeySecondary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0027BF00.c:1022, rework, same function 1.00 (words 12/12, calls 1.00, shape 1.00) */
s32 mnuCompareSecondaryKeyAscending(mnuListNode_t **left, mnuListNode_t **right) {
    u32 leftKey = (*left)->sortKeySecondary;
    u32 rightKey = (*right)->sortKeySecondary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0027BF00.c:1033, rework, same function 1.00 (words 10/10, calls 1.00, shape 1.00) */
s32 mnuCompareTertiaryKeyDescending(mnuListNode_t **left, mnuListNode_t **right) {
    u32 leftKey = (*left)->sortKeyTertiary;
    u32 rightKey = (*right)->sortKeyTertiary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0027BF00.c:1044, rework, same function 1.00 (words 12/12, calls 1.00, shape 1.00) */
s32 mnuCompareTertiaryKeyAscending(mnuListNode_t **left, mnuListNode_t **right) {
    u32 leftKey = (*left)->sortKeyTertiary;
    u32 rightKey = (*right)->sortKeyTertiary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

INCLUDE_RODATA(const s32, "code_r1_0034D128", mnuDrawIconPanelFadeTable);

INCLUDE_RODATA(const s32, "code_r1_0034D128", mnuDrawIconRow6Table);

INCLUDE_RODATA(const s32, "code_r1_0034D128", mnuDrawIconPairTable);

void mnuSortItems(mnuList_t *menu, s32 keyIndex, s32 ascending) {
    s32 (*comparators[MNU_SORT_COMPARATOR_COUNT])(mnuListNode_t **, mnuListNode_t **) = {
        mnuComparePrimaryKeyDescending, mnuCompareSecondaryKeyDescending, mnuCompareTertiaryKeyDescending,
        mnuComparePrimaryKeyAscending, mnuCompareSecondaryKeyAscending, mnuCompareTertiaryKeyAscending
    };
    s32 nodeCount = 0;
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(menu->count * MNU_LIST_POINTER_BYTES);
    mnuListNode_t **items = (mnuListNode_t **)sdfMemLock(allocation);
    mnuListNode_t **writeCursor = items;
    mnuListNode_t *node;

    for (node = menu->first; node != NULL; node = node->next) {
        *writeCursor++ = node;
        nodeCount++;
    }
    if (ascending != 0) {
        keyIndex += MNU_SORT_KEY_COUNT;
    }
    qsort(items, nodeCount, MNU_LIST_POINTER_BYTES, (s32 (*)(const void *, const void *))comparators[keyIndex]);
    mnuLinkItemList(items, nodeCount);
    mnuRebuildListFirstFromCursor(menu);
    mnuRebuildListLastFromCursor(menu);
    mnuResetNodeLinks(menu, 0);
    sdfMemFreeDirect(allocation);
}

/* Allocate four native fade records into pointer slots after the list header. */
void mnuAllocateListEntries(s32 *list) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_FADING_SLOT_COUNT; slotIndex++) {
        list[slotIndex + 1] = sdfChipAllocZeroDirect(MNU_FADING_SLOT_BYTES);
    }
}

/* Free the four record blocks, not their nested window pointers. */
void mnuFreeListEntries(s32 *list) {
    s32 *entries = list + 1;
    u32 slotIndex = 0;
    do {
        sdfChipFreeDirect(*entries++);
        slotIndex++;
    } while (slotIndex < MNU_FADING_SLOT_COUNT);
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0027BF00.c:1109, rework, same function 1.00 (words 12/12, calls 1.00, shape 1.00) */
void mnuAppendFadingWindowEntry(s32 activeValue, s32 windowAddress, s32 *list) {
    u32 slotIndex = *list;
    s32 *slot = list + slotIndex;
    mnuFadeEntry_t *entry;

    if (slotIndex < MNU_FADING_MUTATION_THRESHOLD) {
        return;
    }
    entry = (mnuFadeEntry_t *)slot[1];
    *list = slotIndex + 1;
    entry->active = activeValue;
    entry->window = (struct mnuWindowContainer_t *)windowAddress;
}

/* Native removal only accepts indices at least five. Propagate the tail
 * record backward to the target, clearing each source's window pointer. */
void mnuRemoveFadingWindowEntry(s32 *list, u32 slotIndex) {
    s32 *entries;
    s32 *slot;
    u32 tailIndex;

    if (slotIndex >= MNU_FADING_MUTATION_THRESHOLD) {
        entries = list + 1;
        slot = entries + slotIndex;
        if (((mnuFadeEntry_t *)*slot)->active != 0) {
            jmsDestroyCmdMenu(((mnuFadeEntry_t *)*slot)->window);
        }
        tailIndex = list[0] - 1;
        ((mnuFadeEntry_t *)*slot)->window = 0;
        for (; slotIndex < tailIndex; tailIndex--) {
            *(mnuFadeEntry_t *)entries[tailIndex - 1] = *(mnuFadeEntry_t *)entries[tailIndex];
            ((mnuFadeEntry_t *)entries[tailIndex])->window = 0;
        }
        list[0]--;
    }
}

/* Use each record's stored position/window, with the caller supplying draw depth. */
void mnuDrawFadingWindows(s32 depth, s32 *list, s32 drawArg) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < (u32)list[0]; slotIndex++) {
        mnuFadeEntry_t *entry = (mnuFadeEntry_t *)list[slotIndex + 1];
        mnuDrawWindowContainer(entry->x, entry->y, depth, entry->window, drawArg);
    }
}

/* Subtract from every nonzero fade word without clamping. An already-zero
 * word calls the native remover, whose at-least-five index guard is retained. */
void mnuUpdateFade(s32 *list) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_FADING_SLOT_COUNT; slotIndex++) {
        mnuFadeEntry_t *entry = (mnuFadeEntry_t *)list[slotIndex + 1];
        if (entry->fade != 0) {
            entry->fade -= MNU_FADING_WORD_STEP;
        } else {
            mnuRemoveFadingWindowEntry(list, slotIndex);
        }
    }
}

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode12);
#else
INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCompleted);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCaseFile10);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCaseFile9);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCaseFile8);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCaseFile7);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCaseFile6);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCaseFile5);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCaseFile4);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCaseFile3);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCaseFile2);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCaseFile1);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrEpisode12);
#endif

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode11);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode10);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode9);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode8);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode7);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode6);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode5);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode4);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode3);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode2);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrEpisode1);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrPlayMahjong);
#else
INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrMeetingLog);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrGoToTheRoof);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrPlayMahjong);

INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCheckCaseFiles);
#endif

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrCaseFiles);

INCLUDE_RODATA(const s32, "code_r1_0034D128", SetCommand_jmsMenuData4StrCheckIn);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "code_r1_0034D128", jmsStrCommandMenu);
#else
INCLUDE_RODATA(const s32, "code_r1_0034D128", cmpStrCommandMenu);
#endif

INCLUDE_SDATA(const s32, "code_r1_0034D128", mnuDrawWindowListEntriesData);

