#include "common.h"

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "newdata.h"
#include "sdk_libc.h"
#include "dds/btl.h"

/* Blends packed RGBA channels; weights above 0x100 are mirrored from 0x200. */
u32 fclBlendColor(u32 color0, u32 color1, u32 weight);
extern void *memcpy(void *, const void *, unsigned int);
void *memset(void *dst, s32 value, u32 size);
void qsort(void *base, u32 count, u32 size, s32 (*compare)(const void *, const void *));
void *sdfChipAllocZeroDirect(s32 size);
void sdfChipFreeDirect(void *memory);
struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);
void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);
void *sdfMemLock(struct sdfMemHandle_t *allocation);

#include "dds/itf_draw_grid.h"
#include "dds/eff_resource_slots.h"
#include "dds/mnu_list.h"
#include "dds/fpu.h"
#include "dds/eff.h"
#include "dds/menu_fade_entry.h"

struct StaffMenuRuntime;

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

typedef struct MenuList MenuList;

typedef struct MenuIconState MenuIconState;

extern void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *, s32, s32, s32, s32, s32);
extern s32 itfGridLookupValueOrDefault(EffectSlotSet *object, s32 key);
extern void mnuClearListFlagsOneAndTwo(u32 *flags);
extern void mnuDrawIconPanel(s32, s32, s32, s32, MenuIconState *, s32, s32);
extern void mnuUpdateWindowPanelHandleStatesKindFourFive(MenuIconState *);
extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);

/* Menu runtime fields shared by the party, panel and resource handlers. */
typedef struct MenuContext {
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
    MenuPageWindow partyWindow; /* 0x284: lists, page slots and selection */
    s32 partyPanelActive;  /* 0xA928 */
    s32 partyPanelLast;    /* 0xA92C */
    u8 padA930[0x104];
    MenuPanelGroup *panelGroup; /* 0xAA34 */
    s32 panelRequest;      /* 0xAA38 */
    MenuSpriteState *panelEffects; /* 0xAA3C */
    u8 padAA40[8];
    s32 party;             /* 0xAA48 */
    u8 padAA4C[0x10];
    s32 resourceList;      /* 0xAA5C */
} MenuContext;

typedef struct MenuListNode MenuListNode;

#define PARTY_BACKUP_OFFSET ((s32)&((PartyMenuData *)0)->backup)

/* Staff/skill-menu work prefix, separate from the party-selection allocation. */
typedef struct MenuPartyRuntime {
    u8 pad00[0x10];
    s32 staffMode; /* 0x10 */
    s32 staffView; /* 0x14: selects the input mask and panel view */
    s32 staffSelection; /* 0x18: chosen list entry */
    s32 staffExit; /* 0x1C: signals the close transition */
    u8 pad20[4];
    MenuWindowContainer *selectedWindow; /* 0x24 */
    s32 activeMark; /* 0x28 */
    u8 pad2C[8];
    u32 selectedIndex; /* 0x34: compared with MenuListNode.index */
} MenuPartyRuntime;

/* The idle-motion timer shares the party work area with other menu states. */
typedef struct MenuVoiceState {
    u8 pad00[0x28];
    s32 idleFrames;
    s32 motionSelection; /* -1 enables periodic motion */
} MenuVoiceState;

typedef struct MenuListDefaults {
    s32 indices[3];
} MenuListDefaults;

extern MenuListDefaults D_0042AF00;

/* First confirm stores a slot; a different second slot swaps and rebuilds.
 * Cancel clears the saved slot, opening the exit popup only if none was saved. */
typedef struct MenuPanelBlock {
    u32 word[14];
} MenuPanelBlock;

typedef struct MenuPanelWindow {
    u8 pad0[4];
    s32 field4;
    u8 pad8[0x10];
    MenuList *list; /* 0x18 */
    u8 pad1C[0x3C];
    MenuPanelBlock block; /* 0x58 */
    u8 pad90[4];
    s32 field94;
} MenuPanelWindow;

typedef struct MenuBadgePlace {
    s32 slot;
    s32 x;
    s32 y;
} MenuBadgePlace;

typedef struct MenuBadgeLayout {
    MenuBadgePlace place[2];
} MenuBadgeLayout;

#define MNU_ADVANCE_FADE(value, amount, maximum) { \
    if ((value) < (maximum)) { \
        (value) += (amount); \
    } \
    if ((value) > (maximum)) { \
        (value) = (maximum); \
    } \
}

MenuList *mnuCreateListState(u32 owner, u32 visibleCount, s32 rowSpacing);
MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade);
MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade);
void mnuDrawFourEntries(s32 x, s32 y, s32 depth, MenuList *list, MenuListNode *node, s32 drawArg);
u32 mnuBlendListNodeColorByFlags(u32 previousColor, MenuListNode *entry);
void mnuDispatchEntryWords(EffectSlotSet *menu, s32 entryIndex, MenuListNode *entry);

/* Draw visible rows, preserving each row's glyph chain and callback state. */
void mnuDrawWindowListEntries(s32 x, s32 y, s32 depth, s32 xOffset, s32 yOffset,
                   s32 fadeScale, u32 unusedFlags, MenuList *list, s32 drawArg);

void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg);

/* Destroy the owned list and optional sprite resources before freeing the window. */
void jmsDestroyCmdMenu(MenuWindowContainer *menu);




void mnuRemoveWindowListCursorNode(MenuWindowContainer *menu);
MenuListNode *mnuAdvanceListSelection(MenuWindowContainer *menu, s32 step);
MenuListNode *mnuReverseListSelection(MenuWindowContainer *menu, s32 step);
void mnuClearWindowListSelectionFlags(MenuWindowContainer *menu);

void mnuInitIconSprites(MenuIconSprites *obj, s32 w, s32 h, u32 value,
                        EffectSlotSet *resource, s32 *idx, s32 unused);




void mnuDrawWindowResourceSpriteRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, u32 option);
void mnuDrawWindowIconRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, s32 count, s32 option);
void mnuDrawVisibleWindowIconRows(u32 x, u32 y, u32 flags, MenuWindowContainer *window, u32 option);

/* Draw the window, then advance its fade scale without a post-addition clamp. */

typedef struct MenuPos {
    s32 x;
    s32 y;
} MenuPos;

typedef struct MenuPosTable {
    MenuPos pos[6];
} MenuPosTable;

typedef struct MenuPosTable3 {
    MenuPos pos[3];
} MenuPosTable3;

extern const MenuPosTable3 D_0042AF48;
void mnuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, MenuIconState *state, s32 mode, s32 surface);
extern const MenuPosTable D_0042AF60;
void mnuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 surface);

typedef struct MenuOffsets {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} MenuOffsets;

extern const MenuOffsets D_0042AF90;
void mnuDrawIconPair(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 surface);
extern void mnuDrawIconPanelFade();
extern void mnuDrawIconRow6();
extern void mnuDrawIconPair();
void mnuDrawIconPanel(s32 x, s32 y, s32 depth, s32 fade, MenuIconState *panel, s32 selectionMode, s32 drawArg);
void mnuDrawIconState(u32 x, u32 y, u32 depth, u32 fade, MenuIconState *panel, u32 selectionMode, u32 drawArg);
void mnuDrawIconPanelFullFade(u32 x, u32 y, u32 depth, MenuIconState *panel, s32 drawArg);
void mnuUpdateWindowPanelHandleStatesKindFourFive(MenuIconState *obj);
void mnuRebuildListFirstFromCursor(MenuList *list);
void mnuRebuildListLastFromCursor(MenuList *list);
void mnuLinkItemList(MenuListNode **nodeArray, s32 count);

/* Three-way comparison of unsigned primary keys, descending without subtraction. */
s32 mnuComparePrimaryKeyDescending(MenuListNode **left, MenuListNode **right);

/* Three-way comparison of unsigned primary keys, ascending without subtraction. */
s32 mnuComparePrimaryKeyAscending(MenuListNode **left, MenuListNode **right);

/* Three-way comparison of unsigned secondary keys, descending without subtraction. */
s32 mnuCompareSecondaryKeyDescending(MenuListNode **left, MenuListNode **right);

/* Three-way comparison of unsigned secondary keys, ascending without subtraction. */
s32 mnuCompareSecondaryKeyAscending(MenuListNode **left, MenuListNode **right);

/* Three-way comparison of unsigned tertiary keys, descending without subtraction. */
s32 mnuCompareTertiaryKeyDescending(MenuListNode **left, MenuListNode **right);

/* Three-way comparison of unsigned tertiary keys, ascending without subtraction. */
s32 mnuCompareTertiaryKeyAscending(MenuListNode **left, MenuListNode **right);
void mnuSortItems(MenuList *menu, s32 keyIndex, s32 ascending);
void mnuAllocateListEntries(s32 *list);
void mnuFreeListEntries(s32 *list);
void mnuRemoveFadingWindowEntry(s32 *list, u32 slotIndex);
void mnuDrawFadingWindows(s32 depth, s32 *list, s32 drawArg);
void mnuUpdateFade(s32 *list);

typedef struct MenuSpritePlacement {
    EffectSlotSet *source;
    u32 index;
    s32 x;
    s32 y;
} MenuSpritePlacement;

typedef struct MenuGatedSpritePlacement {
    EffectSlotSet *source;
    u32 index;
    s32 x;
    s32 y;
    s8 requiresFlag;
} MenuGatedSpritePlacement;

typedef struct MenuIconEntry {
    u32 id;
    s32 x;
    s32 y;
} MenuIconEntry;

typedef struct MenuIconLayout {
    MenuIconEntry entry[3];
} MenuIconLayout;

typedef struct MenuPanelSequenceSettings {
    s32 rows[5][4];
    const char *captions[2];
} MenuPanelSequenceSettings;

/* Subtract the fade step only when positive, then clamp any negative result to zero. */
void mnuDecreaseListNodeFadeCounters(MenuList *list) {
    MenuListNode *node = list->first;
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
void mnuDrawFourEntries(s32 x, s32 y, s32 depth, MenuList *list, MenuListNode *node, s32 drawArg) {
    u32 spriteIndex = 0;
    do {
        s32 selected = node == list->cursor;
        s32 index = selected * MNU_ENTRY_SPRITE_COUNT + spriteIndex;
        struct EffectSlotSet *sprite = node->sprites[index].sprite;
        if (sprite != 0) {
            itfDrawGridWithResolvedSlot(x, y, depth, 0, sprite, node->sprites[index].effect, drawArg);
        }
        spriteIndex++;
    } while (spriteIndex < MNU_ENTRY_SPRITE_COUNT);
}

/* Blend the flag-selected packed color with the caller's previous color.
 * Flag one takes precedence over DDS2's additional flag-four color choice. */
u32 mnuBlendListNodeColorByFlags(u32 previousColor, MenuListNode *entry) {
    u32 flags = entry->flags48;
    u32 color = MNU_ENTRY_MARKED_COLOR;
    if (!(flags & 1)) {
        color = (flags & 4) ? MNU_ENTRY_ALTERNATE_COLOR : MNU_ENTRY_DEFAULT_COLOR;
    }
    return fclBlendColor(color, previousColor, entry->animationTimer);
}

/* Apply the same entry-state blend to all four packed colors in one slot. */
void mnuDispatchEntryWords(EffectSlotSet *menu, s32 entryIndex, MenuListNode *entry) {
    s32 colorIndex;

    for (colorIndex = 0; colorIndex < MNU_ENTRY_COLOR_COUNT; colorIndex++) {
        u32 previousColor = menu->workEntries[entryIndex].geometry.cornerColors[colorIndex];

        menu->workEntries[entryIndex].geometry.cornerColors[colorIndex] = mnuBlendListNodeColorByFlags(previousColor, entry);
    }
}

INCLUDE_ASM(const s32, "code_0034D128", mnuDrawWindowListEntries);

/* Invoke the native window renderer at full fade with its size/flag arguments zeroed. */
void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg) {
    mnuDrawWindowListEntries(x, y, depth, 0, 0, MNU_FULL_FADE, 0, (MenuList *)(u32)menu, drawArg);
}

/* Allocate a zeroed window and its list; the last two arguments configure list rows. */
MenuWindowContainer *jmsCreateCmdMenu(s32 id, s32 width, s32 height, s32 visibleCount, s32 rowSpacing) {
    MenuWindowContainer *window = (MenuWindowContainer *)sdfChipAllocZeroDirect(sizeof(MenuWindowContainer));
    MenuList *list;
    window->width = width;
    window->height = height;
    window->id = id;
    list = mnuCreateListState(id, visibleCount, rowSpacing);
    window->fadeScale = 0;
    window->list = list;
    return window;
}

INCLUDE_ASM(const s32, "code_0034D128", jmsDestroyCmdMenu);

INCLUDE_ASM(const s32, "code_0034D128", jmsLeaf22);

INCLUDE_ASM(const s32, "code_0034D128", jmsLeaf23);

void mnuSetWindowContainerLayout(MenuWindowContainer *menu, struct EffectSlotSet *layout2C, u32 layout30, struct EffectSlotSet *layout34,
                                    u32 layout48, u32 layout38, struct EffectSlotSet *layout3C, u32 layout40,
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

void mnuInitializeBasicWindowLayout(MenuWindowContainer *menu, struct EffectSlotSet *first, u32 second) {
    mnuSetWindowContainerLayout(menu, first, second, 0, 0, 0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "code_0034D128", jmsLeaf24);

INCLUDE_ASM(const s32, "code_0034D128", mnuSetWindowPanelBounds);

void mnuCreateListWithDefaults(MenuWindowContainer *menu, u32 first, u32 second,
                               u32 third, EffectSlotSet *spriteResource) {
    MenuListDefaults defaults = D_0042AF00;
    menu->resource =
        mnuCreateWindowSpriteResources(first, second, third, spriteResource,
                                       defaults.indices, 3);
}

INCLUDE_ASM(const s32, "code_0034D128", CmdMenuCursorDrawUnlock);

MenuListNode *jmsAddCmdMenuItem(MenuWindowContainer *menu, const void *text) {
    return mnuListAppendNode(menu->list, text);
}

/* Insert `value` beside anchor in the window's list (see mnuInsertListNodeRelativeToAnchor). */
MenuListNode *mnuInsertWindowListNodeRelativeToAnchor(MenuWindowContainer *menu, MenuListNode *anchor, const void *text, s32 mode, u32 options) {
    return mnuInsertListNodeRelativeToAnchor(menu->list, anchor, text, mode, options);
}

void mnuRemoveWindowListCursorNode(MenuWindowContainer *menu) {
    mnuRemoveListCursorNode(menu->list);
}

/* Advance selection; clear its byte and panel sprite flags only when a node is returned. */
MenuListNode *mnuAdvanceListSelection(MenuWindowContainer *menu, s32 step) {
    MenuListNode *selected = mnuListAdvanceCursor(menu->list, step, 0);
    if (selected != NULL) {
        selected->selectionByte54 = 0;
        mnuClearEntryFlags(&menu->panel);
    }
    return selected;
}

/* Retreat selection with the same conditional byte/panel cleanup as advancement. */
MenuListNode *mnuReverseListSelection(MenuWindowContainer *menu, s32 step) {
    MenuListNode *selected = mnuListRetreatCursor(menu->list, step, 0);
    if (selected != NULL) {
        selected->selectionByte54 = 0;
        mnuClearEntryFlags(&menu->panel);
    }
    return selected;
}

MenuListNode *mnuAdvanceWindowListSelection(MenuWindowContainer *menu) {
    return mnuAdvanceListSelection(menu, 0);
}

MenuListNode *mnuRetreatWindowListSelection(MenuWindowContainer *menu) {
    return mnuReverseListSelection(menu, 0);
}

INCLUDE_ASM(const s32, "code_0034D128", jmsLeaf25);

void mnuClearWindowListSelectionFlags(MenuWindowContainer *menu) {
    mnuClearListFlagsOneAndTwo(&menu->list->stateFlags);
}

void mnuInitIconSprites(MenuIconSprites *obj, s32 w, s32 h, u32 value,
                        EffectSlotSet *resource, s32 *idx, s32 unused) {
    obj->value = value;
    obj->sprite[0] = effCreateResourceSlotSet(resource, idx[0], 1);
    obj->sprite[1] = effCreateResourceSlotSet(resource, idx[1], 1);
    obj->sprite[2] = effCreateResourceSlotSet(resource, idx[2], 1);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[0], 0, w, h, w, h);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[1], 0, w, h, w, h);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[2], 0, w, h, w, h);
}

/* Create an owned three-sprite bundle using the caller's slot-index array. */
MenuIconSprites *mnuCreateWindowSpriteResources(u32 width, u32 height, u32 value,
                    EffectSlotSet *resourceHandle, s32 *indices, u32 unused) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(0x18);
    MenuIconSprites *bundle = (void *)sdfMemLock(allocation);
    memset(bundle, 0, 0x18);
    bundle->allocation = allocation;
    mnuInitIconSprites(bundle, width, height, value, resourceHandle, indices, unused);
    return bundle;
}

/* Destroy every native sprite slot, then release the bundle's allocation handle. */
void mnuReleaseWindowTextures(MenuIconSprites *menu) {
    u32 spriteIndex = 0;
    do {
        effDestroyResourceSlotSet(menu->sprite[spriteIndex]);
        spriteIndex++;
    } while (spriteIndex < MNU_WINDOW_RESOURCE_SPRITES);
    sdfMemFreeDirect(menu->allocation);
}

void jmsLeaf26(void) {
}

void mnuDrawListScrollbar(s32 x, s32 y, s32 depth, u32 opacity, MenuList *list,
                   MenuIconSprites *resource, s32 drawArg) {
    s32 top;
    s32 trackExtent;
    s32 bottom;
    s32 count;
    s32 visible;
    s32 rowOffset;
    EffectSlotSet *firstSprite;
    EffectSlotDescription *firstDescription;
    BdWork *firstWork;
    BdWork *lastWork;

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

void mnuDrawWindowResourceSpriteRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, u32 option) {
    mnuDrawListScrollbar(x - 0xf0, y - 8, flags, window->fadeScale,
                  window->list, window->resource, option);
}

INCLUDE_ASM(const s32, "code_0034D128", jmsFunc45);

void mnuDrawVisibleWindowIconRows(u32 x, u32 y, u32 flags, MenuWindowContainer *window, u32 option) {
    mnuDrawWindowIconRows(x, y, flags, window, window->list->visibleCount, option);
}

INCLUDE_ASM(const s32, "code_0034D128", jmsFunc46);

INCLUDE_ASM(const s32, "code_0034D128", mnuDrawWindowSelectionPanel);

INCLUDE_ASM(const s32, "code_0034D128", jmsFunc47);

INCLUDE_ASM(const s32, "code_0034D128", mnuSpinForWindowListCount);

INCLUDE_RODATA(const s32, "code_0034D128", mnuCreateListWithDefaultsTable);

MenuIconState *mnuCreatePanelIconState(u32 mode, s32 resource, s32 material) {
    s32 keys[6] = { 0x20, 0x22, 0x1F, 0x21, 0x22, 0x1F };
    MenuIconState *panel;
    s32 index;
    s32 first;

    panel = sdfChipAllocZeroDirect(sizeof(MenuIconState));
    panel->kind = mode;
    switch (mode) {
    case 0:
    case 1:
        panel->count = 3;
        first = mode == 1 ? 3 : 0;
        for (index = 0; index < panel->count; index++) {
            panel->sprite[index] = effCreateResourceSlotSet((EffectSlotSet *)resource, keys[first + index], 1);
        }
        break;
    case 2:
    case 3:
        panel->count = 2;
        first = mode == 3 ? 3 : 0;
        for (index = 0; index < panel->count; index++) {
            panel->sprite[index] = effCreateResourceSlotSet((EffectSlotSet *)resource, keys[first + index], 1);
        }
        break;
    case 4:
        panel->count = 6;
        panel->sprite[0] = effCreateResourceSlotSet((EffectSlotSet *)resource, 0x16, 1);
        panel->sprite[1] = effCreateResourceSlotSet((EffectSlotSet *)resource, 0x15, 1);
        panel->sprite[2] = effCreateResourceSlotSet((EffectSlotSet *)resource, 0x0D, 1);
        panel->sprite[3] = effCreateResourceSlotSet((EffectSlotSet *)resource, 0x0D, 1);
        panel->sprite[4] = effCreateResourceSlotSet((EffectSlotSet *)resource, 0x0E, 1);
        panel->sprite[5] = effCreateResourceSlotSet((EffectSlotSet *)resource, 0x0E, 1);
        effConfigureIndexedSlotMaterial(panel->sprite[2], 0, (struct EffMappedResource *)material, 1, 0, 0, 0x0C);
        effConfigureIndexedSlotMaterial(panel->sprite[4], 0, (struct EffMappedResource *)material, 1, 0, 0, 0x0C);
        break;
    case 5:
        panel->count = 2;
        panel->sprite[0] = effCreateResourceSlotSet((EffectSlotSet *)resource, 0x0D, 1);
        panel->sprite[1] = effCreateResourceSlotSet((EffectSlotSet *)resource, 0x0E, 1);
        break;
    }
    return panel;
}

/* Reset low sprite flags only for a present first sprite and a supported panel kind. */
void mnuClearEntryFlags(MenuIconState *group) {
    s32 spriteIndex;
    if (group->sprite[0] != NULL && group->kind < MNU_PANEL_KIND_LIMIT) {
        for (spriteIndex = 0; spriteIndex < group->count; spriteIndex++) {
            EffectSlotSet *obj = group->sprite[spriteIndex];
            u32 *flags = &obj->workEntries->states[0].flags;
            *flags &= ~1;
            effInitializeSlotWork(obj, 0);
        }
    }
}

/* Destroy nonzero resource slots, retaining the native per-iteration count read, then free. */
void mnuReleaseResourceList(MenuIconState *list) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < list->count; slotIndex++) {
        if (list->sprite[slotIndex] != 0) {
            effDestroyResourceSlotSet(list->sprite[slotIndex]);
        }
    }
    sdfChipFreeDirect(list);
}

void mnuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, MenuIconState *state, s32 mode, s32 surface) {
    MenuPosTable3 table = D_0042AF48;
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

void mnuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 surface) {
    MenuPosTable table = D_0042AF60;
    s32 i;
    for (i = 0; i < state->count; i++) {
        if (i != 3 && i != 5) {
            effDrawTextureSlotWithBlendedCornerColors(x + table.pos[i].x, y + table.pos[i].y, z, w, 1, state->sprite[i], 0, surface);
        }
    }
}

void mnuDrawIconPair(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 surface) {
    MenuOffsets offset = D_0042AF90;
    effDrawTextureSlotWithBlendedCornerColors(x + offset.x0, y + offset.y0, z, w, 1, state->sprite[0], 0, surface);
    effDrawTextureSlotWithBlendedCornerColors(x + offset.x1, y + offset.y1, z, w, 1, state->sprite[1], 0, surface);
}

/* Dispatch six DDS2 panel kinds; only kind four forces full fade. */
void mnuDrawIconPanel(s32 x, s32 y, s32 depth, s32 fade, MenuIconState *panel, s32 selectionMode, s32 drawArg) {
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

void mnuDrawIconState(u32 x, u32 y, u32 depth, u32 fade, MenuIconState *panel, u32 selectionMode, u32 drawArg) {
    mnuDrawIconPanel(x, y, depth, fade, panel, selectionMode, drawArg);
}

void mnuDrawIconPanelFullFade(u32 x, u32 y, u32 depth, MenuIconState *panel, s32 drawArg) {
    mnuDrawIconState(x, y, depth, MNU_FULL_FADE, panel, 0, drawArg);
}

void mnuUpdateWindowPanelHandleStatesKindFourFive(MenuIconState *obj) {
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
void mnuRebuildListFirstFromCursor(MenuList *list) {
    s32 node;
    s32 first;
    s32 previous;

    previous = (s32)list->cursor;
    first = (s32)list->cursor;
    while (node = previous, node != 0) {
        first = node;
        previous = (s32)((MenuListNode *)node)->prev;
    }
    list->first = (MenuListNode *)first;
}

/* Rebuild the last-node pointer by walking forward from the cursor. */
void mnuRebuildListLastFromCursor(MenuList *list) {
    s32 node;
    s32 last;
    s32 next;

    next = (s32)list->cursor;
    last = (s32)list->cursor;
    while (node = next, node != 0) {
        last = node;
        next = (s32)((MenuListNode *)node)->next;
    }
    list->last = (MenuListNode *)last;
}

/* Reset the viewport/cursor to the first node; exactly one requests replay
 * toward the saved cursor, rather than treating every nonzero value as true. */
void mnuResetNodeLinks(MenuList *list, s32 restoreCursor) {
    MenuListNode *first;
    MenuListNode *oldCursor;

    list->windowOffset = 0;
    first = list->first;
    oldCursor = list->cursor;
    list->head = first;
    list->cursor = first;
    if (restoreCursor == 1) {
        MenuListNode *node = first;
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
void mnuLinkItemList(MenuListNode **nodeArray, s32 count) {
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

INCLUDE_ASM(const s32, "code_0034D128", mnuComparePrimaryKeyDescending);

INCLUDE_ASM(const s32, "code_0034D128", mnuComparePrimaryKeyAscending);

INCLUDE_ASM(const s32, "code_0034D128", mnuCompareSecondaryKeyDescending);

INCLUDE_ASM(const s32, "code_0034D128", mnuCompareSecondaryKeyAscending);

INCLUDE_ASM(const s32, "code_0034D128", mnuCompareTertiaryKeyDescending);

INCLUDE_ASM(const s32, "code_0034D128", mnuCompareTertiaryKeyAscending);

INCLUDE_RODATA(const s32, "code_0034D128", mnuDrawIconPanelFadeTable);

INCLUDE_RODATA(const s32, "code_0034D128", mnuDrawIconRow6Table);

INCLUDE_RODATA(const s32, "code_0034D128", mnuDrawIconPairTable);

void mnuSortItems(MenuList *menu, s32 keyIndex, s32 ascending) {
    s32 (*comparators[MNU_SORT_COMPARATOR_COUNT])(MenuListNode **, MenuListNode **) = {
        mnuComparePrimaryKeyDescending, mnuCompareSecondaryKeyDescending, mnuCompareTertiaryKeyDescending,
        mnuComparePrimaryKeyAscending, mnuCompareSecondaryKeyAscending, mnuCompareTertiaryKeyAscending
    };
    s32 nodeCount = 0;
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(menu->count * MNU_LIST_POINTER_BYTES);
    MenuListNode **items = (MenuListNode **)sdfMemLock(allocation);
    MenuListNode **writeCursor = items;
    MenuListNode *node;

    for (node = menu->first; node != NULL; node = node->next) {
        *writeCursor++ = node;
        nodeCount++;
    }
    if (ascending != 0) {
        keyIndex += MNU_SORT_KEY_COUNT;
    }
    qsort(items, nodeCount, MNU_LIST_POINTER_BYTES, comparators[keyIndex]);
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

INCLUDE_ASM(const s32, "code_0034D128", mnuAppendFadingWindowEntry);

/* Native removal only accepts indices at least five. Propagate the tail
 * record backward to the target, clearing each source's window pointer. */
void mnuRemoveFadingWindowEntry(s32 *list, u32 slotIndex) {
    s32 *entries;
    s32 *slot;
    u32 tailIndex;

    if (slotIndex >= MNU_FADING_MUTATION_THRESHOLD) {
        entries = list + 1;
        slot = entries + slotIndex;
        if (((MenuFadeEntry *)*slot)->active != 0) {
            jmsDestroyCmdMenu(((MenuFadeEntry *)*slot)->window);
        }
        tailIndex = list[0] - 1;
        ((MenuFadeEntry *)*slot)->window = 0;
        for (; slotIndex < tailIndex; tailIndex--) {
            *(MenuFadeEntry *)entries[tailIndex - 1] = *(MenuFadeEntry *)entries[tailIndex];
            ((MenuFadeEntry *)entries[tailIndex])->window = 0;
        }
        list[0]--;
    }
}

/* Use each record's stored position/window, with the caller supplying draw depth. */
void mnuDrawFadingWindows(s32 depth, s32 *list, s32 drawArg) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < (u32)list[0]; slotIndex++) {
        MenuFadeEntry *entry = (MenuFadeEntry *)list[slotIndex + 1];
        mnuDrawWindowContainer(entry->x, entry->y, depth, entry->window, drawArg);
    }
}

/* Subtract from every nonzero fade word without clamping. An already-zero
 * word calls the native remover, whose at-least-five index guard is retained. */
void mnuUpdateFade(s32 *list) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_FADING_SLOT_COUNT; slotIndex++) {
        MenuFadeEntry *entry = (MenuFadeEntry *)list[slotIndex + 1];
        if (entry->fade != 0) {
            entry->fade -= MNU_FADING_WORD_STEP;
        } else {
            mnuRemoveFadingWindowEntry(list, slotIndex);
        }
    }
}

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode12);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode11);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode10);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode9);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode8);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode7);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode6);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode5);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode4);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode3);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode2);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrEpisode1);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrPlayMahjong);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrCaseFiles);

INCLUDE_RODATA(const s32, "code_0034D128", SetCommand_jmsMenuData4StrCheckIn);

INCLUDE_RODATA(const s32, "code_0034D128", jmsStrCommandMenu);

INCLUDE_SDATA(const s32, "code_0034D128", mnuDrawWindowListEntriesData);

