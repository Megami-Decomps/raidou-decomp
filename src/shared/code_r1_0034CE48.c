#include "common.h"

#include "newdata.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "basic.h"
#include "evt_world.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "fpu.h"
#include "code_r1_0034D128.h"
#include "code_r1_0034C048.h"
#include "code_r1_0034CE48.h"

/* Native profile-progress panel, including its cached sprite-slot pairs. */
typedef struct mnuProfilePanel_t {
    u8 pad00[0x10];
    s32 capValue;
    s32 option;
    struct sprHandle_t *sets[2]; /* 0x18: panel and particle resources. */
    u32 unk20;
    s32 trailIndex;
    s32 particleIndex;
    s32 randomOpacity[5];
    s32 phase;
    s32 opacity;
} mnuProfilePanel_t;

typedef char mnuProfilePanel_size_check[(sizeof(mnuProfilePanel_t) == 0x48) ? 1 : -1];

typedef struct mnuFadeFields_t {
    mnuWindowContainer_t previousWindow;
    s32 previousVisibleCount;
    mnuIconSprites_t savedResource;
    u32 hasResourceCopy;
    s32 previousProgress;
    mnuWindowContainer_t *currentWindow;
    s32 currentProgress;
} mnuFadeFields_t;

/* Raidou's mnuWindowContainer_t is 8 bytes longer; the fade functions read currentProgress at 0xC8. */
typedef char mnuFadeFields_size_must_be_0xCC[(sizeof(mnuFadeFields_t) == 0xCC) ? 1 : -1];

struct mnuListNode_t;

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

typedef struct mnuList_t mnuList_t;

typedef struct mnuIconState_t mnuIconState_t;

typedef struct mnuPanelDrawEntry_t {
    s32 slotIndex;
    s32 x;
    s32 y;
} mnuPanelDrawEntry_t;

/* The draw routine copies five entries; the data label has a trailing zero entry. */
typedef struct mnuPanelDrawLayout_t {
    mnuPanelDrawEntry_t entry[5];
} mnuPanelDrawLayout_t;

/* Menu runtime fields shared by the party, panel and resource handlers. */
typedef struct mnuContext_t {
    u8 pad00[8];
    jmsSeqHandle_t transitionWork; /* +0x08: native saved-entry transition state */
    s32 popupState[3];     /* 0x54: passed to the popup state handlers */
    struct sprHandle_t *displayHandle;     /* 0x60 */
    struct sprHandle_t *resourceHandle; /* 0x64: second base resource owner. */
    void *displayResource; /* 0x68 */
    struct sprHandle_t *alternateResources[10]; /* +0x6C: indexed by party unit ID */
    u8 pad94[0x1C];
    s32 selectionResources[6];
    s32 labelHandle;       /* 0xC8 */
    struct sprHandle_t *panelModels[10]; /* +0xCC: indexed by party unit ID */
    const void *partySelectionLayout; /* 0xF4: layout copied into the party window panel. */
    const void *skillCategoryLayout; /* 0xF8 */
    const void *equippedSkillLayout; /* 0xFC */
    s32 skillPanelResource; /* 0x100 */
    mnuWindowContainer_t *imageHandle; /* 0x104: passed to the window fade owner. */
    u8 pad108[4];
    s32 listHandle;        /* 0x10C */
    u8 pad110[8];
    s32 panelHandle;       /* 0x118 */
    u8 pad11C[0x168];
    mnuPageWindow_t partyWindow; /* 0x284: lists, page slots and selection */
    mnuPartyPanel_t partyPanel; /* 0xA928: counters and five native 0x34-byte entries */
    mnuPanelGroup_t *panelGroup; /* 0xAA34 */
    mnuSpriteState_t *panelRequest; /* 0xAA38 */
    mnuSpriteState_t *panelEffects; /* 0xAA3C */
    u8 padAA40[8];
    s32 party;             /* 0xAA48 */
    u8 padAA4C[0x10];
    mnuProfilePanel_t *profilePanel; /* 0xAA5C: retained profile-panel allocation */
    u16 slotOfA[0x2A0]; /* 0xAA60 */
    u16 slotOfB[0x40]; /* 0xAFA0 */
    u16 skillSlots[0x75]; /* 0xB020 */
    u8 padB10A[2];
    mnuFadeFields_t transition; /* 0xB10C: complete native fade owner. */
    s32 titleFadingOut;
    s32 titleOpacity;
    s32 titleSlide;
    s32 highlightOpacity;
} mnuContext_t;

typedef struct mnuListNode_t mnuListNode_t;

/* Native selection-state allocation is 0x30 bytes, distinct from item state. */
typedef struct StaffMenuRuntime {
    sdfMemHandle_t *allocation;
    u8 pad04[4];
    u32 resources[2];
    s32 staffMode;
    s32 staffView;
    s32 staffSelection;
    s32 staffExit;
    mnuIconState_t *iconPanel; /* 0x20: created and released with the staff panels. */
    s32 active;
    s32 idleFrames;
    s32 motionSelection;
} StaffMenuRuntime;

/* The item/skill-state constructor allocates and clears this complete 0x3C. */
typedef struct mnuSkillMenuRuntime_t {
    sdfMemHandle_t *allocation;
    u8 pad04[4];
    s32 skillMenuActive;
    mnuList_t *categoryList;
    mnuWindowContainer_t *skillWindows[4];
    struct mnuPanelState_t *skillPanel;
    mnuWindowContainer_t *selectedWindow;
    s32 activeMark;
    u32 unk2C; /* 2B6FE8 clears this after rebuilding selection. */
    u32 selectionFlags; /* 0x30: resets the selected skill entry. */
    u32 selectedIndex;
    s32 fade; /* 0x38: shared skill-category marker fade. */
} mnuSkillMenuRuntime_t;

typedef struct mnuListDefaults_t {
    s32 indices[3];
} mnuListDefaults_t;

typedef struct mnuCategoryPositions_t {
    s32 entries[4][2];
} mnuCategoryPositions_t;

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

typedef struct mnuCampPanelDrawRecord_t {
    sprHandle_t *resource;
    s32 sprite;
    s32 xOffset;
    s32 yOffset;
} mnuCampPanelDrawRecord_t;

typedef struct mnuCampPanelOrigins_t {
    mnuPoint_t primary;
    mnuPoint_t secondary;
} mnuCampPanelOrigins_t;

typedef struct mnuBadgePlace_t {
    s32 slot;
    s32 x;
    s32 y;
} mnuBadgePlace_t;

typedef struct mnuBadgeLayout_t {
    mnuBadgePlace_t place[2];
} mnuBadgeLayout_t;

typedef struct mnuCampBackdropLayout_t {
    mnuBadgeLayout_t layer[2];
} mnuCampBackdropLayout_t;

#define MNU_ADVANCE_FADE(value, amount, maximum) { \
    if ((value) < (maximum)) { \
        (value) += (amount); \
    } \
    if ((value) > (maximum)) { \
        (value) = (maximum); \
    } \
}

void mnuUpdateListScrollFlags(mnuList_t *list);
s32 mnuListContainsFinalNode(mnuList_t *list);

typedef struct mnuSpriteRef_t {
    s32 sprite;
    s32 effect;
} mnuSpriteRef_t;

/* Destroy nonzero resource slots, retaining the native per-iteration count read, then free. */
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

typedef struct mnuOffsets_t {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} mnuOffsets_t;

typedef struct mnuIconEntry_t {
    u32 id;
    s32 x;
    s32 y;
} mnuIconEntry_t;

typedef struct mnuIconLayout_t {
    mnuIconEntry_t entry[3];
} mnuIconLayout_t;

mnuListNode_t *mnuScrollListToEnd(mnuList_t *list) {
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

INCLUDE_ASM(const s32, "code_r1_0034CE48", mnuScrollListToPreviousPage);
