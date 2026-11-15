#include "common.h"

#include "datCalc.h"
#include "kernel_draw.h"
#include "newdata.h"
#include "sdfDevCons.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "fpu.h"
#include "basic.h"
#include "evt_world.h"
#include "code_r1_0034D128.h"
#include "code_r1_0034C048.h"
#include "code_r1_0034CE48.h"

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * DDS1 and DDS2 use the same observed layout. Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

enum SdfMotionStateValue {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
};

/* Model-context lifetime, per-frame update, and broadcast accessors. */

typedef u32 MnuInputActionFlags;

#define MNU_INPUT_ACTION_PREVIOUS_PAGE 0x00000400u

#define MNU_INPUT_ACTION_LAST_PAGE 0x00000800u

/* Clear only the window's panel-transition bit. */
void mnuClearWindowPanelTransitionFlag(struct MenuWindowContainer *window);

#define MNU_PANEL_ITEM_COUNT 5

#define MNU_PANEL_STATE_BYTES 0x8C

#define MNU_PANEL_GROUP_BYTES 0x2C

#define MNU_SPRITE_STATE_BYTES 0x20

#define MNU_SIMPLE_SPRITE_BYTES 0x20

#define MNU_PANEL_ITEM_BYTES 0xAC

#define MNU_PROFILE_PANEL_BYTES 0x48

#define MNU_TRANSITION_LIMIT 0x100

#define MNU_TRANSITION_STEP 8

#define MNU_GRADIENT_FADE_STEP 0x20

#define MNU_PROFILE_PHASE_PERIOD 0x200

#define MNU_PROFILE_PHASE_STEP 1

#define MNU_PANEL_TEXTURE_COUNT 7

#define MNU_POPUP_STATE_BYTES 0x4c

#define MNU_POPUP_ENTRY_MARK_BITS 0x60000

#define MNU_PARTY_SLOT_COUNT 5

#define MNU_INPUT_PRIORITY_BIT 1

#define MNU_PAD_TRIGGER_BIT 2

#define MNU_COMMAND_RECORD_BYTES 0x38

#define MNU_COMMAND_ID_MASK 0xFFFF

#define MNU_PERCENT_SCALE 100

#define MNU_COMMAND_USE_STATUS_BOUNDARY 0x220

#define MNU_RATIO_QUARTER_PERCENT 25

#define MNU_RATIO_HALF_PERCENT 50

#define EVT_STAGE_ENTRY_INDEX_MASK 0xFFFF

#define EVT_STAGE_ENTRY_BYTES 60

#define EVT_STAGE_MODEL_RECORD_BYTES 624

#define EVT_STAGE_COLUMN_VALUE_COUNT 8

#define EVT_STAGE_MOTION_SUPPRESS_FALLBACK 1

#define EVT_STAGE_DEFAULT_BLEND_FRAMES 15.0f

#define EVT_STAGE_PROJECTION_BIAS 2048.0f

#define EVT_STAGE_DEFAULT_TARGET_Z -400.0f

#define EVT_STAGE_SCREEN_WORK_BYTES 0x20

#define EVT_STAGE_USE_ENTRY_MOTION 0xffffffffffffffff

typedef struct MenuList MenuList;

typedef struct MenuQueuedRenderContext {
    s32 panelIndex;
    MenuPageWindow *menu;
    MenuPoint origin;
    MenuPoint extent;
} MenuQueuedRenderContext;

typedef char MenuQueuedRenderContext_size_check[
    sizeof(MenuQueuedRenderContext) == 0x18 ? 1 : -1];

typedef struct MenuCommandScatterLayout {
    MenuPoint points[7];
} MenuCommandScatterLayout;

typedef char MenuCommandScatterLayout_size_check[
    sizeof(MenuCommandScatterLayout) == 0x38 ? 1 : -1];

typedef struct _FRQ _FRQ;

typedef struct MenuSpacing {
    s32 step;
    s32 gap;
    s32 tail;
} MenuSpacing;

typedef struct MenuPanelState {
    MenuPanelStatePrefix common;
    MenuGridSlot backgroundSlot; /* 0x20 */
    s32 corners[5][2]; /* 0x28 */
    MenuGridSlot selectedCenterSlot; /* 0x50 */
    MenuGridSlot selectedCornerSlot; /* 0x58 */
    MenuGridSlot rowSlot; /* 0x60 */
    s32 secondOffsetX; /* 0x68: row position relative to the panel. */
    s32 secondOffsetY; /* 0x6C */
    MenuGridSlot headingSlot; /* 0x70 */
    s32 thirdOffsetX; /* 0x78: heading position relative to the panel. */
    s32 thirdOffsetY; /* 0x7C */
    const s32 *headingIndices; /* 0x80: caller supplies a static sprite-index table. */
    u8 pad84[4];
    struct MenuIconState *resourceHandle; /* 0x88 */
} MenuPanelState;

typedef char MenuPanelStateSizeCheck[(sizeof(MenuPanelState) == MNU_PANEL_STATE_BYTES) ? 1 : -1];

typedef struct MenuWorkCamera {
    s32 x;
    s32 y;
    f32 depth; /* third component uses float in evtStageTestViewTargetVector */
} MenuWorkCamera;

typedef struct MenuWorkPosition {
    s32 x;
    s32 y;
    s32 z;
} MenuWorkPosition;

struct SoundHandleNode;

void mnuHandleListPageJumpInput(const void *resource, MenuWindowContainer *window,
                                MnuInputActionFlags *actionFlags) {
    struct MenuListNode *top = NULL;
    struct MenuListNode *bottom = NULL;
    MenuList *list = window->list;

    if (resource != NULL) {
        if (list->count >= list->visibleCount) {
            if (*actionFlags & MNU_INPUT_ACTION_PREVIOUS_PAGE) {
                top = mnuScrollListToPreviousPage(list);
            }
            if (*actionFlags & MNU_INPUT_ACTION_LAST_PAGE) {
                bottom = mnuScrollListToEnd(window->list);
            }
            if (top == NULL) {
                *actionFlags &= ~MNU_INPUT_ACTION_PREVIOUS_PAGE;
            }
            if (bottom == NULL) {
                *actionFlags &= ~MNU_INPUT_ACTION_LAST_PAGE;
            }
            mnuClearWindowPanelTransitionFlag(window);
            return;
        }
    }
    *actionFlags &= ~MNU_INPUT_ACTION_PREVIOUS_PAGE;
    *actionFlags &= ~MNU_INPUT_ACTION_LAST_PAGE;
}

void mnuHandlePanelListPageJumpInput(MenuWindowContainer *window,
                                     MnuInputActionFlags *actionFlags) {
    mnuHandleListPageJumpInput(window->resource, window, actionFlags);
}
