#include "common.h"

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "newdata.h"
#include "sdk_libc.h"
#include "dds/btl.h"

extern void *memcpy(void *, const void *, unsigned int);

#include "dds/mnu_list.h"
#include "dds/eff.h"

struct MenuListNode;

struct StaffMenuRuntime;

#include "dds/fpu.h"

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

#define MNU_STAFF_BACKUP_BYTES 0x8D4

#define MNU_STAFF_PARTY_ACTIVE_BIT 1

#define MNU_STAFF_NODE_SELECTED 2

#define MNU_STAFF_PARTY_PANEL_BASE 0x284

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

typedef struct MenuPanelDrawEntry {
    s32 slotIndex;
    s32 x;
    s32 y;
} MenuPanelDrawEntry;

/* The draw routine copies five entries; the data label has a trailing zero entry. */
typedef struct MenuPanelDrawLayout {
    MenuPanelDrawEntry entry[5];
} MenuPanelDrawLayout;

/* Menu runtime fields shared by the party, panel and resource handlers. */
typedef struct MenuContext {
    u8 pad00[8];
    MenuPopupState transitionWork; /* +0x08: native saved-entry transition state */
    s32 popupState[3];     /* 0x54: passed to the popup state handlers */
    struct EffectSlotSet *displayHandle;     /* 0x60 */
    struct EffectSlotSet *resourceHandle; /* 0x64: second base resource owner. */
    void *displayResource; /* 0x68 */
    struct EffectSlotSet *alternateResources[10]; /* +0x6C: indexed by party unit ID */
    u8 pad94[0x1C];
    s32 selectionResources[6];
    s32 labelHandle;       /* 0xC8 */
    struct EffectSlotSet *panelModels[10]; /* +0xCC: indexed by party unit ID */
    const void *partySelectionLayout; /* 0xF4: layout copied into the party window panel. */
    const void *skillCategoryLayout; /* 0xF8 */
    const void *equippedSkillLayout; /* 0xFC */
    s32 skillPanelResource; /* 0x100 */
    MenuWindowContainer *imageHandle; /* 0x104: passed to the window fade owner. */
    u8 pad108[4];
    s32 listHandle;        /* 0x10C */
    u8 pad110[8];
    s32 panelHandle;       /* 0x118 */
    u8 pad11C[0x168];
    MenuPageWindow partyWindow; /* 0x284: lists, page slots and selection */
    PartyPanel partyPanel; /* 0xA928: counters and five native 0x34-byte entries */
    MenuPanelGroup *panelGroup; /* 0xAA34 */
    MenuSpriteState *panelRequest; /* 0xAA38 */
    MenuSpriteState *panelEffects; /* 0xAA3C */
    u8 padAA40[8];
    s32 party;             /* 0xAA48 */
    u8 padAA4C[0x10];
    MenuProfilePanel *profilePanel; /* 0xAA5C: retained profile-panel allocation */
    u16 slotOfA[0x2A0]; /* 0xAA60 */
    u16 slotOfB[0x40]; /* 0xAFA0 */
    u16 skillSlots[0x75]; /* 0xB020 */
    u8 padB10A[2];
    MenuFadeFields transition; /* 0xB10C: complete native fade owner. */
    s32 titleFadingOut;
    s32 titleOpacity;
    s32 titleSlide;
    s32 highlightOpacity;
} MenuContext;

typedef struct MenuListNode MenuListNode;

/* Native selection-state allocation is 0x30 bytes, distinct from item state. */
typedef struct StaffMenuRuntime {
    sdfMemHandle_t *allocation;
    u8 pad04[4];
    u32 resources[2];
    s32 staffMode;
    s32 staffView;
    s32 staffSelection;
    s32 staffExit;
    MenuIconState *iconPanel; /* 0x20: created and released with the staff panels. */
    s32 active;
    s32 idleFrames;
    s32 motionSelection;
} StaffMenuRuntime;

/* The item/skill-state constructor allocates and clears this complete 0x3C. */
typedef struct SkillMenuRuntime {
    sdfMemHandle_t *allocation;
    u8 pad04[4];
    s32 skillMenuActive;
    MenuList *categoryList;
    MenuWindowContainer *skillWindows[4];
    struct MenuPanelState *skillPanel;
    MenuWindowContainer *selectedWindow;
    s32 activeMark;
    u32 unk2C; /* 2B6FE8 clears this after rebuilding selection. */
    u32 selectionFlags; /* 0x30: resets the selected skill entry. */
    u32 selectedIndex;
    s32 fade; /* 0x38: shared skill-category marker fade. */
} SkillMenuRuntime;

typedef struct MenuListDefaults {
    s32 indices[3];
} MenuListDefaults;

typedef struct MnuCategoryPositions {
    s32 entries[4][2];
} MnuCategoryPositions;

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

typedef struct MnuCampPanelDrawRecord {
    EffectSlotSet *resource;
    s32 sprite;
    s32 xOffset;
    s32 yOffset;
} MnuCampPanelDrawRecord;

typedef struct MnuCampPanelOrigins {
    MenuPoint primary;
    MenuPoint secondary;
} MnuCampPanelOrigins;

typedef struct MenuBadgePlace {
    s32 slot;
    s32 x;
    s32 y;
} MenuBadgePlace;

typedef struct MenuBadgeLayout {
    MenuBadgePlace place[2];
} MenuBadgeLayout;

typedef struct MenuCampBackdropLayout {
    MenuBadgeLayout layer[2];
} MenuCampBackdropLayout;

#define MNU_ADVANCE_FADE(value, amount, maximum) { \
    if ((value) < (maximum)) { \
        (value) += (amount); \
    } \
    if ((value) > (maximum)) { \
        (value) = (maximum); \
    } \
}

void mnuUpdateListScrollFlags(MenuList *list);
s32 mnuListContainsFinalNode(MenuList *list);

typedef struct MenuSpriteRef {
    s32 sprite;
    s32 effect;
} MenuSpriteRef;

MenuListNode *mnuAdvanceListWindowStart(MenuList *list);
MenuListNode *mnuScrollListToEnd(MenuList *list);

/* Destroy nonzero resource slots, retaining the native per-iteration count read, then free. */
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

typedef struct MenuOffsets {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} MenuOffsets;

typedef struct MenuIconEntry {
    u32 id;
    s32 x;
    s32 y;
} MenuIconEntry;

typedef struct MenuIconLayout {
    MenuIconEntry entry[3];
} MenuIconLayout;

MenuListNode *mnuScrollListToEnd(MenuList *list) {
    s32 i;

    if (list->cursor == NULL) {
        return 0;
    }
    i = 0;
    while (i < list->visibleCount) {
        if (mnuListContainsFinalNode(list)) {
            if (i == 0) {
                if (list->cursor == list->last) {
                    return 0;
                }
                list->cursor = list->last;
                list->windowOffset = list->visibleCount - 1;
                return list->last;
            }
            break;
        }
        mnuAdvanceListWindowStart(list);
        i++;
        list->cursor = list->cursor->next;
        list->windowOffset += 1;
    }
    if (mnuListContainsFinalNode(list)) {
        list->windowOffset = list->visibleCount - 1;
        list->cursor = list->last;
        return list->last;
    }
    if (list->cursor == list->head) {
        list->cursor = list->cursor->next;
        list->windowOffset = 1;
    }
    mnuUpdateListScrollFlags((u8 *)list);
    return list->cursor;
}

INCLUDE_ASM(const s32, "code_0034CE48", mnuScrollListToPreviousPage);
