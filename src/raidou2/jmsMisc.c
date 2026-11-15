#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "datCalc.h"
#include "kernel_draw.h"
#include "newdata.h"
#include "sdfDevCons.h"
#include "sdk_libc.h"
#include "dds/btl.h"

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

extern void *memcpy(void *, const void *, unsigned int);
void *memset(void *dst, s32 value, u32 size);

#include "dds/mnu.h"
#include "dds/mnu_popup_eligibility.h"
#include "dds/mnu_panel_state_prefix.h"
#include "dds/mnu_shop.h"

struct _FRQ;

#define MNU_PANEL_ITEM_COUNT 5

#define MNU_PANEL_STATE_BYTES 0x64

#define MNU_SPRITE_STATE_BYTES 0x20

#define MNU_SIMPLE_SPRITE_BYTES 0x28

#define MNU_PANEL_ITEM_BYTES 0x90

#define MNU_TRANSITION_LIMIT 0x100

#define MNU_TRANSITION_STEP 8

#define MNU_GRADIENT_FADE_STEP 0x20

#define MNU_PROFILE_PHASE_PERIOD 0x200

#define MNU_PROFILE_PHASE_STEP 12

#define MNU_PANEL_TEXTURE_COUNT 9

#define MNU_POPUP_STATE_BYTES 0x4c

#define MNU_POPUP_ENTRY_MARK_BITS 0x60000

#define MNU_PARTY_SLOT_COUNT 5

#define MNU_INPUT_PRIORITY_BIT 1

#define MNU_PAD_TRIGGER_BIT 2

#define MNU_COMMAND_RECORD_BYTES 0x38

#define MNU_COMMAND_ID_MASK 0xFFFF

#define MNU_PERCENT_SCALE 100

#define MNU_COMMAND_USE_STATUS_BOUNDARY 0x200

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

typedef struct MenuPanelItem MenuPanelItem;

typedef struct MenuPanelState {
    MenuPanelStatePrefix common;
    MenuGridSlot firstSlot; /* 0x20 */
    s32 secondOffsetX; /* 0x28: second row's position relative to the panel. */
    s32 secondOffsetY; /* 0x2C */
    MenuGridSlot secondSlot; /* 0x30 */
    MenuGridSlot thirdSlot; /* 0x38 */
    s32 thirdOffsetX; /* 0x40: third row's position relative to the panel. */
    s32 thirdOffsetY; /* 0x44 */
    MenuGridSlot fourthSlot; /* 0x48 */
    s32 fourthOffsetX; /* 0x50: fourth row's position relative to the panel. */
    s32 fourthOffsetY; /* 0x54 */
    u32 fourthValueC; /* 0x58 */
    u8 pad5C[4];
    MenuPanelHandles *resourceHandle; /* 0x60 */
} MenuPanelState;

typedef char MenuPanelStateSizeCheck[(sizeof(MenuPanelState) == MNU_PANEL_STATE_BYTES) ? 1 : -1];

struct MenuPanelItem {
    u8 pad00[0x10];
    u32 value10;
    s32 value14;
    s32 value18;
    u32 option;
    s32 selection;
    MenuGridSlot groupGridSlots[7]; /* 0x24 */
    MenuGridSlot gridSlots[5]; /* 0x5C */
    u8 pad84[4];
    u32 initialValue; /* 0x88 */
    s32 selectionRamp; /* 0x8C */
};

void mnuClearPanelTransitionState(MenuPopupState *state);
void CalcSeqHandle(s32 action, MenuPopupEntry *entry, MenuPopupState *state, u32 argument);
void jmsSeqDestroyAll(MenuPopupState *state, struct dds3ProcessID_t *task);

s32 jmsSeqChange(MenuPopupState *work, s32 *entrySlot, s32 mode,
                               void *callback);

void jmsSeqJump(s32 *entrySlot, MenuPopupEntry *entry);
void jmsSeqJumpAndFree(s32 *entrySlot, MenuPopupEntry *entry);
void jmsSeqReturn(MenuPopupState *state, s32 *entrySlot);

struct SoundHandleNode;

/* Clear the complete native popup-transition state, including saved entry addresses. */
void mnuClearPanelTransitionState(MenuPopupState *state) {
    memset(state, 0, MNU_POPUP_STATE_BYTES);
}

void CalcSeqHandle(s32 action, MenuPopupEntry *entry, MenuPopupState *state, u32 argument) {
    MenuPopupCallback callback;
    MenuPopupEntry *saved;
    s32 i;

    if (state == NULL) {
        return;
    }
    switch (action) {
        case 0:
            callback = entry->enter;
            if (callback != NULL) {
                if (entry->leave != NULL) {
                    for (i = 0; i < state->count; i++) {
                        if (entry->enter == state->entries[i]->enter) {
                            return;
                        }
                    }
                    state->entries[state->count++] = entry;
                    if ((entry->flags & MNU_POPUP_INSERT_BEFORE_TOP) && state->count >= 2) {
                        saved = state->entries[state->count - 1];
                        state->entries[state->count - 1] = state->entries[state->count - 2];
                        state->entries[state->count - 2] = saved;
                    }
                }
                callback(argument);
            }
            break;
        case 1:
        case 2:
            if (state->count != 0) {
                saved = state->entries[state->count - 1];
                if (saved->leave != NULL && action == 1) {
                    saved->leave(argument);
                }
                state->count--;
            }
            break;
    }
}

/* Pop saved entries with their leave callbacks until the state count reaches zero. */
void jmsSeqDestroyAll(MenuPopupState *state, struct dds3ProcessID_t *task) {
    s32 entryCount;

    entryCount = state->count;
    while (entryCount != 0) {
        CalcSeqHandle(1, 0, state, (u32)task);
        entryCount = state->count;
    }
}

INCLUDE_ASM(const s32, "jmsMisc", jmsChkSeqChange);

s32 jmsSeqChange(MenuPopupState *work, s32 *entrySlot, s32 mode,
                               void *callback) {
    MenuPopupState *state = work;
    MenuPopupEntry **slot = (MenuPopupEntry **)entrySlot;
    MenuPopupEntry *entry;
    s32 accepted = 1;
    s32 result = 0;
    u32 flags;

    if (callback == NULL) {
        return 0;
    }
    if (slot == NULL) {
        return 0;
    }
    entry = *slot;
    if (entry == NULL) {
        return 0;
    }
    flags = entry->flags;
    if (flags & 0x400000) {
        return 0;
    }
    switch (mode) {
    case 0:
        entry->flags = flags | 0x100000;
        if (entry->canEnter != NULL) {
            accepted = ((MenuPopupCanEnterCallback)entry->canEnter)(callback) != 0;
        }
        if (accepted || ((*slot)->flags & 1)) {
            if (!((*slot)->flags & 0x10000)) {
                CalcSeqHandle(0, *slot, state, (u32)callback);
                (*slot)->flags |= 0x10000;
            }
        }
        if (accepted) {
            result = (s32)(*slot)->start;
            (*slot)->flags |= 0x200000;
        } else {
            flags = (*slot)->flags;
            if (flags & 1) {
                (*slot)->flags = flags | 0x400000;
                (*slot)->start(callback);
                (*slot)->flags &= ~0x400000;
            }
        }
        break;
    case 1:
        if (flags & 0x100000) {
            if (flags & 0x200000) {
                result = (s32)entry->update;
            } else if (flags & 1) {
                entry->flags = flags | 0x400000;
                entry->update(callback);
                (*slot)->flags &= ~0x400000;
            }
        }
        break;
    case 2:
        if (flags & 0x100000) {
            if (flags & 0x200000) {
                if (flags & 0x20000) {
                    if (flags & 0x40000) {
                        CalcSeqHandle(2, NULL, state, (u32)callback);
                    } else {
                        CalcSeqHandle(1, NULL, state, (u32)callback);
                    }
                }
                /* Keep only the low 16 flag bits, read as a halfword. */
                (*slot)->flags = *(u16 *)*slot;
                state->entryAddress = state->lastEntryAddress;
                state->lastEntryAddress = *slot;
                result = (s32)(*slot)->finish;
                *slot = NULL;
            } else if (flags & 1) {
                entry->flags = flags | 0x400000;
                entry->finish(callback);
                (*slot)->flags &= ~0x400000;
            }
        }
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "jmsMisc", jmsSeqChkLast);

/* Bind the entry address and retain only the entry's low sixteen flag bits. */
void jmsSeqJump(s32 *entrySlot, MenuPopupEntry *entry) {
    u16 retainedFlags;

    retainedFlags = entry->flags;
    *entrySlot = (s32)entry;
    entry->flags = retainedFlags;
}

/* Bind the entry with the native insert-before-top bit after preserving low flags. */
void jmsSeqJumpAndFree(s32 *entrySlot, MenuPopupEntry *entry) {
    u16 retainedFlags;

    retainedFlags = entry->flags;
    *entrySlot = (s32)entry;
    entry->flags = retainedFlags | MNU_POPUP_INSERT_BEFORE_TOP;
}

INCLUDE_ASM(const s32, "jmsMisc", jmsSeqJumpAndPop);

/* Bind the current popup entry only when its saved address is nonzero. */
void jmsSeqReturn(MenuPopupState *state, s32 *entrySlot) {
    if (state->entryAddress != NULL) {
        jmsSeqJumpAndFree(entrySlot, state->entryAddress);
        return;
    }
}

INCLUDE_ASM(const s32, "jmsMisc", jmsChkCommonInput);
