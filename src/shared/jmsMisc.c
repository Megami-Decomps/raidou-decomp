#include "common.h"

#include "datCalc.h"
#include "kernel_draw.h"
#include "newdata.h"
#include "sdfDevCons.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "basic.h"
#include "evt_world.h"
#include "code_r1_001CC1D0.h"
#include "jmsMisc.h"
#include "dds3PadManager.h"

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * Node payloads depend on the slot. */
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

#define MNU_POPUP_INSERT_BEFORE_TOP 0x20000

/* The generic window panel is embedded at +0x4C and copied as 0x38 bytes. */
typedef struct mnuPanelHandles_t {
    u32 panelKind;
    u8 pad04[4];
    s32 count;
    struct sprHandle_t *handles[6];
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
    s32 transition;
} mnuPanelHandles_t;

typedef char mnuPanelHandles_size_must_be_0x38[(sizeof(mnuPanelHandles_t) == 0x38) ? 1 : -1];

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

typedef struct mnuPanelItem_t mnuPanelItem_t;

typedef struct mnuPanelState_t {
    mnuPanelStatePrefix_t common;
    mnuGridSlot_t firstSlot; /* 0x20 */
    s32 secondOffsetX; /* 0x28: second row's position relative to the panel. */
    s32 secondOffsetY; /* 0x2C */
    mnuGridSlot_t secondSlot; /* 0x30 */
    mnuGridSlot_t thirdSlot; /* 0x38 */
    s32 thirdOffsetX; /* 0x40: third row's position relative to the panel. */
    s32 thirdOffsetY; /* 0x44 */
    mnuGridSlot_t fourthSlot; /* 0x48 */
    s32 fourthOffsetX; /* 0x50: fourth row's position relative to the panel. */
    s32 fourthOffsetY; /* 0x54 */
    u32 fourthValueC; /* 0x58 */
    u8 pad5C[4];
    mnuPanelHandles_t *resourceHandle; /* 0x60 */
} mnuPanelState_t;

typedef char mnuPanelStateSizeCheck[(sizeof(mnuPanelState_t) == MNU_PANEL_STATE_BYTES) ? 1 : -1];

struct mnuPanelItem_t {
    u8 pad00[0x10];
    u32 value10;
    s32 value14;
    s32 value18;
    u32 option;
    s32 selection;
    mnuGridSlot_t groupGridSlots[7]; /* 0x24 */
    mnuGridSlot_t gridSlots[5]; /* 0x5C */
    u8 pad84[4];
    u32 initialValue; /* 0x88 */
    s32 selectionRamp; /* 0x8C */
};

void CalcSeqHandle(s32 action, jmsSeqUnit_t *entry, jmsSeqHandle_t *state, dds3ProcessID_t *ID);

struct SoundHandleNode;

/* Clear the complete native popup-transition state, including saved entry addresses. */
void mnuClearPanelTransitionState(jmsSeqHandle_t *state) {
    memset(state, 0, MNU_POPUP_STATE_BYTES);
}

void CalcSeqHandle(s32 action, jmsSeqUnit_t *entry, jmsSeqHandle_t *state, dds3ProcessID_t *ID) {
    jmsInt32Delegate callback;
    jmsSeqUnit_t *saved;
    s32 i;

    if (state == NULL) {
        return;
    }
    switch (action) {
        case 0:
            callback = entry->pInit;
            if (callback != NULL) {
                if (entry->pDestroy != NULL) {
                    for (i = 0; i < state->count; i++) {
                        if (entry->pInit == state->entries[i]->pInit) {
                            return;
                        }
                    }
                    state->entries[state->count++] = entry;
                    if ((entry->Stat & MNU_POPUP_INSERT_BEFORE_TOP) && state->count >= 2) {
                        saved = state->entries[state->count - 1];
                        state->entries[state->count - 1] = state->entries[state->count - 2];
                        state->entries[state->count - 2] = saved;
                    }
                }
                callback(ID);
            }
            break;
        case 1:
        case 2:
            if (state->count != 0) {
                saved = state->entries[state->count - 1];
                if (saved->pDestroy != NULL && action == 1) {
                    saved->pDestroy(ID);
                }
                state->count--;
            }
            break;
    }
}

/* Pop saved entries with their leave callbacks until the state count reaches zero. */
void jmsSeqDestroyAll(jmsSeqHandle_t *state, struct dds3ProcessID_t *task) {
    s32 entryCount;

    entryCount = state->count;
    while (entryCount != 0) {
        CalcSeqHandle(1, 0, state, task);
        entryCount = state->count;
    }
}

INCLUDE_ASM(const s32, "jmsMisc", jmsChkSeqChange);

s32 jmsSeqChange(jmsSeqHandle_t *work, s32 *entrySlot, s32 mode, dds3ProcessID_t *ID) {
    jmsSeqHandle_t *state = work;
    jmsSeqUnit_t **slot = (jmsSeqUnit_t **)entrySlot;
    jmsSeqUnit_t *entry;
    s32 accepted = 1;
    s32 result = 0;
    u32 flags;

    if (ID == NULL) {
        return 0;
    }
    if (slot == NULL) {
        return 0;
    }
    entry = *slot;
    if (entry == NULL) {
        return 0;
    }
    flags = entry->Stat;
    if (flags & 0x400000) {
        return 0;
    }
    switch (mode) {
    case 0:
        entry->Stat = flags | 0x100000;
        if (entry->pCheck != NULL) {
            accepted = entry->pCheck(ID) != 0;
        }
        if (accepted || ((*slot)->Stat & 1)) {
            if (!((*slot)->Stat & 0x10000)) {
                CalcSeqHandle(0, *slot, state, ID);
                (*slot)->Stat |= 0x10000;
            }
        }
        if (accepted) {
            result = (s32)(*slot)->pCalc;
            (*slot)->Stat |= 0x200000;
        } else {
            flags = (*slot)->Stat;
            if (flags & 1) {
                (*slot)->Stat = flags | 0x400000;
                (*slot)->pCalc(ID);
                (*slot)->Stat &= ~0x400000;
            }
        }
        break;
    case 1:
        if (flags & 0x100000) {
            if (flags & 0x200000) {
                result = (s32)entry->pDraw;
            } else if (flags & 1) {
                entry->Stat = flags | 0x400000;
                entry->pDraw(ID);
                (*slot)->Stat &= ~0x400000;
            }
        }
        break;
    case 2:
        if (flags & 0x100000) {
            if (flags & 0x200000) {
                if (flags & 0x20000) {
                    if (flags & 0x40000) {
                        CalcSeqHandle(2, NULL, state, ID);
                    } else {
                        CalcSeqHandle(1, NULL, state, ID);
                    }
                }
                /* Keep only the low 16 flag bits, read as a halfword. */
                (*slot)->Stat = *(u16 *)*slot;
                state->entryAddress = state->lastEntryAddress;
                state->lastEntryAddress = *slot;
                result = (s32)(*slot)->pUpdate;
                *slot = NULL;
            } else if (flags & 1) {
                entry->Stat = flags | 0x400000;
                entry->pUpdate(ID);
                (*slot)->Stat &= ~0x400000;
            }
        }
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "jmsMisc", jmsSeqChkLast);

/* Bind the entry address and retain only the entry's low sixteen flag bits. */
void jmsSeqJump(s32 *entrySlot, jmsSeqUnit_t *entry) {
    u16 retainedFlags;

    retainedFlags = entry->Stat;
    *entrySlot = (s32)entry;
    entry->Stat = retainedFlags;
}

/* Bind the entry with the native insert-before-top bit after preserving low flags. */
void jmsSeqJumpAndFree(s32 *entrySlot, jmsSeqUnit_t *entry) {
    u16 retainedFlags;

    retainedFlags = entry->Stat;
    *entrySlot = (s32)entry;
    entry->Stat = retainedFlags | MNU_POPUP_INSERT_BEFORE_TOP;
}

void jmsSeqJumpAndPop(struct jmsSeqUnit_t **pCurrentSeqUnit, struct jmsSeqUnit_t *pSeqUnit) {
    u16 retainedFlags;

    retainedFlags = pSeqUnit->Stat;
    *pCurrentSeqUnit = pSeqUnit;
    pSeqUnit->Stat = retainedFlags | 0x60000;
}

/* Bind the current popup entry only when its saved address is nonzero. */
void jmsSeqReturn(jmsSeqHandle_t *state, s32 *entrySlot) {
    if (state->entryAddress != NULL) {
        jmsSeqJumpAndFree(entrySlot, state->entryAddress);
        return;
    }
}

/* NON_MATCHING: 182 of 183 words match.  */
#ifdef NON_MATCHING
u32 jmsChkCommonInput(u32 ControlBit) {
    s32 flags = 0;

    if (ControlBit & 1) {
        flags = dds3PadButtonState[0x23] < 0;
    }
    if (ControlBit & 0x2) {
        if (dds3PadButtonState[0x21] < 0) {
            flags |= 0x2;
        }
    }
    if (ControlBit & 0x10) {
        if (dds3PadButtonState[0x26] & 2) {
            flags |= 0x10;
        }
    }
    if (ControlBit & 0x20) {
        if (dds3PadButtonState[0x27] & 2) {
            flags |= 0x20;
        }
    }
    if (ControlBit & 0x40) {
        if (dds3PadButtonState[0x24] & 2) {
            flags |= 0x40;
        }
    }
    if (ControlBit & 0x80) {
        if (dds3PadButtonState[0x25] & 2) {
            flags |= 0x80;
        }
    }
    if (ControlBit & 0x4) {
        if (dds3PadButtonState[0x22] < 0) {
            flags |= 0x4;
        }
    }
    if (ControlBit & 0x8) {
        if (dds3PadButtonState[0x20] < 0) {
            flags |= 0x8;
        }
    }
    if (ControlBit & 0x200) {
        if (dds3PadButtonState[0x2a] & 2) {
            flags |= 0x200;
        }
    }
    if (ControlBit & 0x100) {
        if (dds3PadButtonState[0x28] & 2) {
            flags |= 0x100;
        }
    }
    if (ControlBit & 0x800) {
        if (dds3PadButtonState[0x2b] & 2) {
            flags |= 0x800;
        }
    }
    if (ControlBit & 0x400) {
        if (dds3PadButtonState[0x29] & 2) {
            flags |= 0x400;
        }
    }
    if (ControlBit & 0x1000) {
        if (dds3PadButtonState[0x2c] < 0) {
            flags |= 0x1000;
        }
    }
    if (ControlBit & 0x2000) {
        if (dds3PadButtonState[0x2d] < 0) {
            flags |= 0x2000;
        }
    }
    if (ControlBit & 0x20000) {
        if (dds3PadButtonState[0x2a] < 0) {
            flags |= 0x20000;
        }
    }
    if (ControlBit & 0x10000) {
        if (dds3PadButtonState[0x28] < 0) {
            flags |= 0x10000;
        }
    }
    if (ControlBit & 0x80000) {
        if (dds3PadButtonState[0x2b] < 0) {
            flags |= 0x80000;
        }
    }
    if (ControlBit & 0x40000) {
        if (dds3PadButtonState[0x29] < 0) {
            flags |= 0x40000;
        }
    }
    if (ControlBit & 0x10) {
        if (dds3PadButtonState[0x26] != 0) {
            flags |= 0x100000;
        }
    }
    if (ControlBit & 0x20) {
        if (dds3PadButtonState[0x27] != 0) {
            flags |= 0x200000;
        }
    }
    if (ControlBit & 0x40) {
        if (dds3PadButtonState[0x24] != 0) {
            flags |= 0x400000;
        }
    }
    if (ControlBit & 0x80) {
        if (dds3PadButtonState[0x25] != 0) {
            flags |= 0x800000;
        }
        if (dds3PadButtonState[0x2A] != 0) {
            flags |= 0x1000000;
        }
    }
    if (ControlBit & 0x40) {
        if (dds3PadButtonState[0x28] != 0) {
            flags |= 0x2000000;
        }
    }
    if (ControlBit & 0x80) {
        if (dds3PadButtonState[0x2b] != 0) {
            flags |= 0x4000000;
        }
    }
    if (ControlBit & 0x40) {
        if (dds3PadButtonState[0x29] != 0) {
            flags |= 0x8000000;
        }
    }
    if (flags & 1) {
        flags = flags & 1;
    }
    return flags;
}
#else
INCLUDE_ASM(const s32, "jmsMisc", jmsChkCommonInput);
#endif

#ifdef VERSION_RAIDOU1
/* Detective menu sprite layout: parts are placed relative to their parent part (-1 for none). */
typedef struct jmsSprParts_t {
    s16 parentNo;
    s16 spriteNo;
    s16 x;
    s16 y;
} jmsSprParts_t;

typedef struct jmsSprTable_t {
    s32 nums;
    jmsSprParts_t *parts;
} jmsSprTable_t;

jmsSprParts_t jmsSprTable01[17] = {
    { -1, 17, 44, 63 },
    { 0, 18, 2, 3 },
    { 0, 28, 7, 6 },
    { 0, 28, 10, 336 },
    { 0, 29, 7, 6 },
    { 0, 31, 126, 6 },
    { 0, 32, 10, 336 },
    { 0, 30, 298, 336 },
    { 0, 16, 2, 1 },
    { 0, 19, 392, 311 },
    { 0, 34, 416, 4 },
    { 0, 33, 5, 134 },
    { -1, 83, 83, 19 },
    { 12, 84, 317, 0 },
    { 12, 82, 29, 0 },
    { 12, 13, 19, 3 },
    { 12, 15, 301, 3 },
};

jmsSprParts_t jmsSprTable02[4] = {
    { -1, 27, 20, 225 },
    { -1, 25, 20, 225 },
    { -1, 26, 473, 225 },
    { -1, 24, 473, 225 },
};

/* Objects of 8 bytes or less default to .sdata; retail keeps these two in .data. */
jmsSprParts_t jmsSprTable03[1] __attribute__((section(".data"))) = {
    { -1, 95, 444, 414 },
};

jmsSprParts_t jmsSprTable04[1] __attribute__((section(".data"))) = {
    { -1, 78, 444, 414 },
};

jmsSprParts_t jmsSprTable05[4] = {
    { -1, 61, -36, -24 },
    { -1, 62, 3, -29 },
    { -1, 71, -11, -14 },
    { -1, 70, -11, -14 },
};

jmsSprTable_t jmsSprTableRoot[5] = {
    { 17, jmsSprTable01 },
    { 4, jmsSprTable02 },
    { 1, jmsSprTable03 },
    { 1, jmsSprTable04 },
    { 4, jmsSprTable05 },
};
#endif
