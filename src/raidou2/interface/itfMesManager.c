#include "common.h"
#include "interface.h"
#include "newdata.h"
#include "itfMesManager.h"
#include "kernel.h"
#include "sdfFrame.h"

typedef struct itfMesBinTypeHeader_t itfMesBinTypeHeader_t;

typedef struct itfMesLink_t itfMesLink_t;

typedef struct itfMesLinkControl_t itfMesLinkControl_t;

typedef struct itfMesManager_t itfMesManager_t;

typedef struct debug_tag debug_tag;

typedef struct itfPadState_t itfPadState_t;

/* Message tables contain relocated encoded-text addresses. */
struct itfMesBinMesHeader_t {
    u8 label[0x18];
    s16 page;
    s16 speaker;
    u32 addr[1];
};

struct itfMesBinTypeHeader_t {
    /* Relocated item-address ptr; address 1 marks a branch entry. */
    u32 *type;
    itfMesBinMesHeader_t *ptr;
};

struct itfMesResource_t {
    u8 unk0[8];
    u32 magic;
    u8 unkC[4];
    s32 fixupTableOffset;
    s32 fixupTableBytes;
    u32 entryCount;
    u8 relocated;
    u8 unk1D[3];
    itfMesBinTypeHeader_t entries[1];
};

/* The message manager owns 128 native 0x14-byte link records. */
struct itfMesLink_t {
    struct itfMesLink_t *previous;
    struct itfMesLink_t *next;
    s32 index;
    instanceMes_tag *state;
    sdfMemHandle_t *resourceHandle;
};

struct itfMesLinkControl_t {
    itfMesLink_t *activeHead;
    itfMesLink_t *activeTail;
    itfMesLink_t *firstFree;
    itfMesLink_t *lastFree;
};

struct itfMesManager_t {
    u32 activeWindowCount;
    sdfTexHandle_t *windowTexture;
    u32 ctr;
    u16 flags;
    u16 unkE;
    itfMesLinkControl_t pool;
    itfMesLink_t nodes[0x80];
};

/* State of the interactive message-layout inspector. */
struct debug_tag {
    s32 cursor;
    s32 mode;
    itfMesLink_t *pmesflow;
};

/* Named primary/secondary buttons are ports 0/1 of input bank 1. */
struct itfPadState_t {
    u8 pad00[0x20];
    union {
        s8 buttons[0x20];
        struct {
            u8 pad20;
            s8 confirm;
            s8 primaryTriangle;
            s8 primaryCancel;
            s8 alternatePrev; /* +0x24: battle panels use as an alternate previous input. */
            s8 alternateNext; /* +0x25: battle panels use as an alternate next input. */
            s8 prev;
            s8 next;
            u8 pad28[9];
            s8 secondaryCross;
            s8 secondaryTriangle;
            s8 secondaryCancel;
            s8 coarseDown;
            s8 coarseUp;
            s8 secondaryUp;
            s8 secondaryDown;
            s8 fineDown;
            u8 pad39;
            s8 fineUp;
            u8 pad3B[5];
        };
    };
};

typedef void (*itfProcessShutdown_t)(dds3ProcessID_t *task);
s32 itfMesMngMessage(void);
extern u32 scrGetMessageHandle(void);
extern itfMesInstance_t pinst[];
extern s32 scrGetIntPara(s32);
extern s32 itfMesMngRequestMessage(s32 window, s32 entryIndex, s32 itemIndex);
void itfMesMngSetPhase(s32 msgid, s8 phase);
s32 itfMesMngSelect(void);
extern void itfMesMngRequestSelect(s32 id, s32 select);
extern void scrSetIntReturnValue(s32 i);
void itfMesMngRequestSelectRelease(s32 id);
s16 itfMesMngGetSelPhase(s32 msgid);
s16 itfMesMngGetSelectNo(s32 msgid);
s32 itfMesMngMaskSelect(void);
void itfMesMngSetMaskSelect(s32 id, u32 maskpattern);
s32 itfMesMngCloseWindow(void);

#define ITF_MES_LOWER_STATUS_PAIR_MASK 0x300

#define ITF_MES_LOWER_STATUS_FIRST_BIT 0x100

#define ITF_MES_LOWER_STATUS_SECOND_BIT 0x200

#define ITF_MES_UPPER_STATUS_PAIR_MASK 0x3000

void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker);

#define ITF_MES_CLEAR_SCRIPT_PANEL_BIT 0xFFDFFFFF

u32 itfMesMngOpenSelectWindow(void);
u32 itfMesMngCloseSelectWindow(void);
s32 itfMesMngSetMesPosDefault(void);
void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);

#define ITF_MES_DEFAULT_TEXT_X 0x4B0

#define ITF_MES_DEFAULT_TEXT_Y 0xA88

extern itfMesManager_t itfMesMngManager;
itfMesResource_t *itfMesMngSetResourcePtr(s32 id, itfMesResource_t *presource);
void InitializeSpeaker(Speaker_tag *pspeaker, s32 defaultpos);
void InitializeVariable(variable_tag *pvariable);
void itfMesMngRelease(s32 id);
void DestructFlow(s32 id);
s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);

#define ITF_MES_ENTRY_STATE_MASK 7

#define ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK 0xFFFDFFFF

#define ITF_MES_OPTION_STATE_MASK 0x38

#define ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK 0xFFFBFFFF

void itfMesMngSetSpeakerPosition(s32 id, s32 x, s32 y);
void ChangeFontPosition(frFontHandle_t *pfont, s32 dist_x, s32 dist_y);
void itfMesMngSetSpeakerPositionOffset(s32 id, s32 x, s32 y);
void itfMesMngSetMesPositionOffset(s32 id, s32 x, s32 y);
void itfMesMngSetPriority(s32 id, s32 z_buf);
void ChangeZBuffer(frFontHandle_t *pfont, s32 z);
void itfMesMngSetStat(s32 id, u32 flag);

#define ITF_MES_HIGH_FLAGS_MASK 0xffff0000

s16 itfMesMngGetSelect(s32 id);
u8 *itfMesMngGetVar(s32 id, s32 index);
void itfMesMngSetVarImmediate(s32 id, u32 index, const char *pstr);
void SetVar(instanceMes_tag *pinst, s32 index, const char *pstr, s32 length);
void itfMesMngSetVarImmediate2(s32 id, s32 index, const char *pstr, s32 length);
void ResolveResourceAddress(itfMesResource_t *pdata);
itfMesBinTypeHeader_t *GetTypeHeader2(itfMesResource_t *pfileheader);

#define ITF_MES_MAGIC_MSG1 0x3147534d

u8 *itfMesMngGetDataPtrImmediate(s32 id, s32 message, s32 page);
u8 *GetMessagePagePtr(itfMesBinMesHeader_t *table, s32 index);
itfMesBinTypeHeader_t *GetTypeHeader(instanceMes_tag *pinst, s32 index);
void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha);

#define ITF_MES_CONTENT_STATUS_MASK 0x3300

s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index);
s32 itfMesMngGetWidth(s32 id, s32 message, s32 page);
void CommitVarTable(s32 *pvar);
void frSetExtDic(u8 *pExtFontData);
frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);
s32 GetMessageWidth(frFontHandle_t *pfont);
void itfMesMngSetDefaultSpeed(s32 id, u8 speed);
extern void itfMesMngSetKutiCallback(s32 id, void (*func)(void));

#define ITF_MES_WINDOW_CAPACITY 0x80

#define ITF_MES_POOL_NODE_BYTES 0x14

s32 CalcMsgMng_Process(void);
void DrawMsgMng_Shutdown(void);
void itfMesMngSetBitStatManager(u32 flags);
void itfMesMngClearBitStatManager(u32 flags);
u16 itfMesMngGetStatManager(void);
itfMesLink_t *DebugChangeInstance(itfMesLink_t *pflow);
extern itfPadState_t dds3PadButtonState;
extern char itfMesDebugItemFormat[];
extern char itfMesMarkerNone[];
extern char itfMesDebugFadeFormat[];
extern void ReleaseAllWindow(struct instanceMes_tag *pinst);
void ReleaseAllVariable(variable_tag *pvariable);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern void itfMiscLinkGive(itfMesLink_t *pelem, itfMesLinkControl_t *pstack);

#define ITF_MES_MAGIC_MSG0 0x3047534d

#define ITF_MES_STRING_COPY_ROUND_BIAS 4

#define ITF_MES_COPY_ALIGN_MASK 3

extern void *memcpy(void *a, const void *b, size_t c);

#define ITF_MES_BINARY_COPY_ROUND_BIAS 5

frFontHandle_t *UnchainFontHandleUnit(frFontHandle_t *pfont, s32 index, s32 lines);
void ChangeFontAlpha(frFontHandle_t *pfont, u32 alpha);

#define ITF_MES_COLOR_BYTE_CLEAR_MASK 0xffffff00

void CalcMesFlow(instanceMes_tag *pinst);
s32 SelectItemDefKey(select_tag *psel);
void ChangeFontColor(frFontHandle_t *pfont, s32 index, s32 lines, s32 color);
extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);
extern void itfMiscDrawSpritePrim(void *pos, void *uv, void *color, s32 z, sdfTexHandle_t *texhandle, s32 context, sdfDrawTag_t *pdrawtag);
extern u8 itfMesDebugLineVertIndex[5];
extern u8 itfMesDebugLineColorIndex[5];
void sdfAddressResolver(s32 *aDataBody, saddr_t aOffset, u8 *aAddrInfoPtr, s32 aAddrInfoLeng);
void frSetVariableRequest(s32 VarNo, s32 pStr);
void itfMesMngSetSelect(s32 id, s32 selno);
void itfMesMngMarkBitStat(s32 id, u32 flag);
void itfMesMngClearBitStat(s32 id, u32 flag);
void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type);
s8 itfMesMngGetPhase(s32 msgid);
void itfMesMngSetSelPhase(s32 msgid, s16 phase);
s32 itfMesMng_SEL_SELNO(void);
s32 itfMesMngOpenWindow(void);
s32 itfMesMngSetVarScr(void);
s32 itfMesMngIgnoreConfig(void);
s32 itfMesMngSetMesPositionScript(void);
s32 itfMesMngValidateNext(void);
s32 itfMesMngWindowType(void);
s32 itfMesMngWindowTypeAlpha(void);
s32 itfMesMngSelectDefKey(void);
u8 *GetSpeakerName(instanceMes_tag *pinst);
s32 itfMesMngRequestMessage(s32 window, s32 entryIndex, s32 itemIndex);
void itfMesMngRequestSelect(s32 id, s32 select);
u32 itfMesMngGetStat(s32 id);
extern char *pStr[];
extern char *pstr[];

/* Store the optional callback address invoked without arguments during glyph building. */
void itfMesMngSetKutiCallback(s32 id, void (*func)(void));
extern const char itfMesDebugLabelChange[16];
extern const char itfMesDebugLabelPosition[16];
extern const char itfMesDebugLabelRightBottom[16];
extern const char itfMesDebugLabelLeftTop[16];
frFontHandle_t *GetLastLinkFontHandle(frFontHandle_t *pfont);

/* Replace a retained text-slot allocation. Nonpositive byteCount copies the
 * padded string span; positive counts copy that many bytes into a zeroed block.
 * DDS1 string mode intentionally has no memset. Keep the K&R declaration form. */
void SetVar();
s32 GetNumSelectItem(s32 allitem, u32 mask);
s32 CountLineMessage(frFontHandle_t *pfont_top, frFontHandle_t *pfont);
void ChangeFontColorAll(frFontHandle_t *pfont, u8 color);
s32 GetMaskedIndex(s32 index, u32 mask);
void ReleaseAllWindow(instanceMes_tag *pinst);
extern char itfMesMarkerEdit[];
extern char itfMesMarkerSelect[];
extern char itfMesMarkerEdit[8];
extern char itfMesMarkerSelect[8];
extern char itfMesDebugItemFormat[8];
extern char itfMesMarkerNone[8];
extern char itfMesDebugFadeFormat[8];

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "dds/scr_command_timer_api.h"

/* During a command callback, return the number of earlier trace passes that
 * yielded while it was pending. The first invocation sees zero; each yielded
 * pass adds one, and a continuing or finishing handler resets the count. This
 * is callback wait state, not wall-clock time, and is valid only in the callback. */
ScrCommandYieldCount scrGetCommandTimer(void);

#include "dds/mnu.h"
#include "dds/itf_mes_panel_mode.h"
#include "dds/itf_mes_entry_start_api.h"
#include "dds/itf_panel_draw.h"
#include "dds/itf_panel_layouts.h"

#define ITF_PANEL_COLUMN_COUNT 4

#define ITF_PANEL_ROW_COUNT 2

#define ITF_PANEL_COLUMN_INSET 0x200

#define ITF_PANEL_CORNER_COLUMN_COUNT 3

#define ITF_PANEL_CORNER_STRIP_WIDTH 0x5A0

#define ITF_PANEL_FULL_COLOR_SCALE 0x80

#define ITF_PANEL_STATE_PACKET_BYTES 0x40

#define ITF_PANEL_QUAD_INDEX_COUNT 4

#define ITF_PANEL_WIDE_INDEX_COUNT 8

#define ITF_PANEL_WIDE_PASS_COUNT 3

#define ITF_PANEL_SEVEN_COLOR_COUNT 7

#define ITF_PANEL_FIVE_COLOR_COUNT 5

#define ITF_PANEL_SEVEN_SIDE_PASS_COUNT 6

#define ITF_PANEL_FIVE_SIDE_PASS_COUNT 2

#define ITF_PANEL_MESSAGE_FADE_TICKS 30

#define ITF_PANEL_MESSAGE_EXIT_TICKS 10

#define ITF_PANEL_TINT_ALPHA_NUMERATOR 0x67

#define ITF_PANEL_TINT_ALPHA_DENOMINATOR 128

#define ITF_PANEL_TINT_BORDER_WIDTH 0x240

extern void (*DrawTable[])(dds_itfpanel_t *, sdfDrawTag_t *);
extern PanelHold itfHeldPanelCursor;

extern u8 D_003B45E8[], D_003B4600[], D_003B4618[];
extern u8 D_003B4630[], D_003B4648[], D_003B4660[];
extern u8 D_004365A8[8], D_004365B0[8];
s32 itfPanelAcquireHold(void);
s32 itfPanelReleaseHold(void);
s32 itfCommandQueueHeldPanelEntry(void);

/* Configure the fade on timer zero, wait thirty ticks, then consume a negative option status.
   Keep both timer reads: setup tests a second read, while later comparisons use the cached one. */
s32 MesMngSelect(void);

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "sdk_libc.h"
#include "dds/btl_state.h"

typedef mesflow_tag ItfMesWindowRec;

#define ITF_MES_SCRIPT_PANEL_BIT 0x200000

#define ITF_MES_SKIP_CONTEXT_ENABLE_BIT 0x400000

#define ITF_MES_TEXT_X_SHIFT 4

#define ITF_MES_TEXT_Y_SHIFT 3

#define ITF_MES_OPTION_LIST_READY 0x10

#define ITF_MES_LOW_FLAGS_MASK 0xffff

#define ITF_MES_OPTION_ROW_SPACING 25

#define ITF_MES_DEFAULT_COLOR_VALUE 0x80

#define ITF_MES_TEXT_SLOT_COUNT 0x20

#define ITF_MES_MASK_BIT_COUNT ITF_MES_OPTION_BIT_INDEX_NOT_FOUND

sdfTexHandle_t *itfMesGetWindowTexture(void);

/* Install an entry's table into the window, rebuild its text glyphs and the
 * selected-item caption, then mark the entry block active. */
ItfMesEntryStartResult itfMesStartEntry(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex, s32 itemIndex);

/* Named and indexed views of the same four signed 32-bit color channels. */
typedef union UiQuadColor {
    struct {
        s32 red;
        s32 green;
        s32 blue;
        s32 alpha;
    };
    s32 channels[4];
} UiQuadColor;

typedef char UiQuadColorSizeCheck[sizeof(UiQuadColor) == 0x10 ? 1 : -1];

extern const UiQuadColor itfQueueOffsetTexturedRectTable;

void itfQueueOffsetTexturedRect(s32 *bounds, s32 *region, s32 x, s32 y,
                   s32 alpha, sdfTexHandle_t *texture, sdfDrawTag_t *command);

/* Start the window entry, then poll completion and restore its normal flags. */
s32 itfMesScriptStartEntryAndWait(void);

/* Poll/start an entry: one ends the command, zero keeps waiting.
   A negative signed status is consumed by clearing it; positive status keeps waiting. */
s32 itfMesMngMessage(void) {
    s32 window = scrGetMessageHandle();
    instanceMes_tag *record;
    message_tag *entryState;
    s32 entryIndex;
    s8 status;

    if (window < 0) {
        return 1;
    }
    record = pinst[window].mes;
    entryIndex = scrGetIntPara(0);
    entryState = &record->message;
    status = entryState->phase;
    if (status == 0) {
        if (itfMesMngRequestMessage(window, entryIndex, 0) == 0) {
            return 1;
        }
    } else if (status < 0) {
        entryState->phase = 0;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfLeaf8);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfEndMessageExSub);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfDrawMessageSub);

s8 itfMesMngGetPhase(s32 msgid) {
    return pinst[msgid].mes->message.phase;
}

void itfMesMngSetPhase(s32 msgid, s8 phase) {
    pinst[msgid].mes->message.phase = phase;
}

/* Capture the script window, reset its held queue, and switch to page three.
   Invalid windows still return the native command-complete value of one. */
s32 itfPanelAcquireHold(void) {
    PanelHold *hold;
    ItfMesWindowIndex window;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    hold = &itfHeldPanelCursor;
    hold->window = window;
    hold->unk4 = -1;
    hold->entryEndFrame = -1;
    hold->queuedCount = 0;
    itfMesMngChangeWindowType(window, ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION, 0);
    return 1;
}

/* Clean up the active held window and reset its queue; an inactive hold is a no-op. */
s32 itfPanelReleaseHold(void) {
    PanelHold *hold = &itfHeldPanelCursor;

    if (hold->window < 0) {
        return 1;
    }
    itfMesMngRequestMessageRelease(hold->window, 1);
    hold->window = -1;
    hold->unk4 = -1;
    hold->entryEndFrame = -1;
    hold->queuedCount = 0;
    return 1;
}

/* Queue script words in native second/first/third field order; first and third narrow to s16.
   Inactive/full queues are ignored with status one; negative queuedCount is not checked. */
s32 itfCommandQueueHeldPanelEntry(void) {
    PanelHold *cursor = &itfHeldPanelCursor;
    s32 first;
    s32 second;
    s32 third;

    if (cursor->window < 0) {
        return 1;
    }
    if (cursor->queuedCount >= ITF_PANEL_HOLD_CAPACITY) {
        return 1;
    }
    first = scrGetIntPara(0);
    second = scrGetIntPara(1);
    third = scrGetIntPara(2);
    cursor->items[cursor->queuedCount].second = first;
    cursor->items[cursor->queuedCount].first = second;
    cursor->items[cursor->queuedCount].third = third;
    cursor->queuedCount++;
    return 1;
}

s32 itfStepHeldPanelMessages(void) {
    s32 frame;
    s32 latest = -1;
    s32 i;

    if (itfHeldPanelCursor.window < 0) {
        return 1;
    }
    frame = scrGetCommandTimer();
    if (itfHeldPanelCursor.queuedCount == 0) {
        return 1;
    }
    for (i = 0; i < itfHeldPanelCursor.queuedCount; i++) {
        s32 start = itfHeldPanelCursor.items[i].first;

        if (start >= latest) {
            latest = start;
        }
        if (start == frame) {
            s16 entry = itfHeldPanelCursor.items[i].second;
            s32 deadline = frame + itfHeldPanelCursor.items[i].third;

            itfMesStartEntry(itfHeldPanelCursor.window, entry, 0);
            itfHeldPanelCursor.unk4 = entry;
            itfHeldPanelCursor.entryEndFrame = deadline;
        }
    }
    if (frame == itfHeldPanelCursor.entryEndFrame) {
        itfMesMngRequestMessageRelease(itfHeldPanelCursor.window, 1);
        itfHeldPanelCursor.unk4 = itfHeldPanelCursor.entryEndFrame = -1;
    }
    if (frame >= latest && frame >= itfHeldPanelCursor.entryEndFrame) {
        itfPanelReleaseHold();
        return 1;
    }
    return 0;
}

s32 itfMesMngSelect(void) {
    s32 window;
    s32 entry;
    instanceMes_tag *record;
    select_tag *options;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    record = pinst[window].mes;
    entry = scrGetIntPara(0);
    options = &record->select;
    if (options->phase == 0) {
        itfMesMngRequestSelect(window, entry);
    } else if (options->phase < 0) {
        options->phase = 0;
        scrSetIntReturnValue(options->select);
        itfMesMngRequestSelectRelease(window);
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", MesMngSelect);

s16 itfMesMngGetSelPhase(s32 msgid) {
    return pinst[msgid].mes->select.phase;
}

void itfMesMngSetSelPhase(s32 msgid, s16 phase) {
    pinst[msgid].mes->select.phase = phase;
}

/* Return the same signed word forwarded as the script result when an option completes. */
s16 itfMesMngGetSelectNo(s32 msgid) {
    return pinst[msgid].mes->select.select;
}

/* Store the script's panel mask; a negative script window is a no-op. Return 1. */
s32 itfMesMngMaskSelect(void) {
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngSetMaskSelect(window, scrGetIntPara(0));
    return 1;
}

s32 itfMesMng_SEL_SELNO(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngSetSelect(window, scrGetIntPara(0));
    return 1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngOpenWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngCloseWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngMsgWndCls);

u32 itfMesMngOpenSelectWindow(void) {
    return 1;
}

u32 itfMesMngCloseSelectWindow(void) {
    return 1;
}

s32 itfMesMngSetVarScr(void)
{
    s32 window = scrGetMessageHandle();
    s32 slot;
    s32 value;
    s32 selector;

    if (window < 0) {
        return 1;
    }
    slot = scrGetIntPara(0);
    value = scrGetIntPara(1);
    selector = scrGetIntPara(2);
    itfMesMngSetVar(window, slot, value, selector);
    return 1;
}

s32 itfMesMngIgnoreConfig(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    if (scrGetIntPara(0)) {
        itfMesMngMarkBitStat(window, 0x400000);
    } else {
        itfMesMngClearBitStat(window, 0x400000);
    }
    return 1;
}

s32 itfMesMngSetMesPositionScript(void)
{
    s32 window = scrGetMessageHandle();
    s32 x;
    s32 y;

    if (window < 0) {
        return 1;
    }
    x = scrGetIntPara(0);
    y = scrGetIntPara(1);
    itfMesMngSetMesPosition(window, x << 4, y << 3);
    return 1;
}

s32 itfMesMngSetMesPosDefault(void) {
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngSetMesPosition(window, 0x440, 0xA78);
    return 1;
}

s32 itfMesMngValidateNext(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    if (scrGetIntPara(0)) {
        itfMesMngClearBitStat(window, 0x800000);
        itfMesMngClearBitStat(window, 0x100000);
    } else {
        itfMesMngMarkBitStat(window, 0x800000);
        itfMesMngMarkBitStat(window, 0x100000);
    }
    return 1;
}

s32 itfMesMngWindowType(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngChangeWindowType(window, scrGetIntPara(0), 0);
    return 1;
}

s32 itfMesMngWindowTypeAlpha(void)
{
    s32 window = scrGetMessageHandle();
    s32 type;
    s32 alpha;

    if (window < 0) {
        return 1;
    }
    type = scrGetIntPara(0);
    alpha = scrGetIntPara(1);
    itfMesMngChangeWindowType(window, type, alpha);
    return 1;
}

s32 itfMesMngSelectDefKey(void)
{
    s32 window = scrGetMessageHandle();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = scrGetIntPara(0);
    second = scrGetIntPara(1);
    itfMesMngSetSelectDefKey(window, first, second);
    return 1;
}

/* Return the loaded message-window texture resource, not a window index. */
sdfTexHandle_t *itfMesGetWindowTexture(void) {
    return itfMesMngManager.windowTexture;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfLeaf9);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfLeaf10);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngInitialize);

/* Destroy an installed nonnegative window slot; no upper-bound validation. */
void itfMesMngRelease(s32 id) {
    if (id >= 0 && pinst[id].mes != NULL) {
        DestructFlow(id);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngClearCursor);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestMessage_itf);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesSelectTableItemText);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestMessageRelease);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestSelect_itf);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestSelectRelease);

void itfMesMngSetMaskSelect(s32 id, u32 maskpattern) {
    pinst[id].mes->select.mask = maskpattern;
}

void itfMesMngSetSelect(s32 id, s32 selno)
{
    select_tag *blk = &pinst[id].mes->select;
    u32 mask = blk->mask;
    s32 bit = mask & 1;
    s32 n = bit == 0;
    s32 i = 0;

    if (selno > 0) {
        do {
            i++;
            mask >>= 1;
            bit = mask & 1;
            if (bit == 0) {
                n++;
            }
        } while (i < selno);
    }
    n = bit == 0 ? n - 1 : 0;
    blk->select = n;
    blk->select_imm = n;
}

/* Move primary text to an absolute position; an unchanged position is a no-op. */
void itfMesMngSetSpeakerPosition(s32 id, s32 x, s32 y) {
    Speaker_tag *primaryTextBlock = &pinst[id].mes->speaker;
    s32 delta[2];

    delta[0] = x - primaryTextBlock->x;
    delta[1] = y - primaryTextBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(primaryTextBlock->pfont, delta[0], delta[1]);
    primaryTextBlock->x = x;
    primaryTextBlock->y = y;
}

/* Translate primary text and update its cached position, including zero deltas. */
void itfMesMngSetSpeakerPositionOffset(s32 id, s32 x, s32 y) {
    Speaker_tag *primaryTextBlock = &pinst[id].mes->speaker;

    ChangeFontPosition(primaryTextBlock->pfont, x, y);
    primaryTextBlock->x += x;
    primaryTextBlock->y += y;
}

/* Move selected-entry text absolutely; an unchanged position is a no-op. */
void itfMesMngSetMesPosition(s32 id, s32 x, s32 y) {
    message_tag *message = &pinst[id].mes->message;
    s32 delta[2];

    delta[0] = x - message->x;
    delta[1] = y - message->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(message->pfont, delta[0], delta[1]);
    message->x = x;
    message->y = y;
}

/* Translate selected-entry text and update its cached position. */
void itfMesMngSetMesPositionOffset(s32 id, s32 x, s32 y) {
    message_tag *message = &pinst[id].mes->message;

    ChangeFontPosition(message->pfont, x, y);
    message->x += x;
    message->y += y;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSelPosition);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSelPositionOffset);

/* Propagate a changed render value to all three glyph chains before caching it. */
void itfMesMngSetPriority(s32 id, s32 z_buf) {
    instanceMes_tag *mes = pinst[id].mes;

    if (mes->z == z_buf) {
        return;
    }
    ChangeZBuffer(mes->speaker.pfont, z_buf);
    ChangeZBuffer(mes->message.pfont, z_buf);
    ChangeZBuffer(mes->select.pfont, z_buf);
    mes->z = z_buf;
}

u32 itfMesMngGetStat(s32 id)
{
    return pinst[id].mes->stat;
}

/* Replace only the high half, preserving the existing low-half cast. */
void itfMesMngSetStat(s32 id, u32 flag) {
    instanceMes_tag *mes = pinst[id].mes;

    mes->stat = (u32)(u16)mes->stat | (flag & ITF_MES_HIGH_FLAGS_MASK);
}

void itfMesMngMarkBitStat(s32 id, u32 flag)
{
    instanceMes_tag *rec = pinst[id].mes;

    rec->stat |= flag & 0xFFFF0000;
}

void itfMesMngClearBitStat(s32 id, u32 flag)
{
    instanceMes_tag *rec = pinst[id].mes;

    rec->stat &= ~flag | 0xFFFF;
}

s16 itfMesMngGetSelect(s32 id) {
    return pinst[id].mes->select.select;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetMesPage);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetMesCurrentPage);

u8 *itfMesMngGetVar(s32 id, s32 index) {
    return (u8 *)pinst[id].mes->variable.pptr[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetVar_itf);

/* Replace a text slot with a copied NUL-terminated encoded string. */
void itfMesMngSetVarImmediate(s32 id, u32 index, const char *pstr) {
    SetVar(pinst[id].mes, index, pstr, 0);
}

/* Copy text into one of the window's message text slots. */
void itfMesMngSetVarImmediate2(s32 id, s32 index, const char *pstr, s32 length) {
    SetVar(pinst[id].mes, index, pstr, length);
}

/* Install/relocate a message resource and cache MSG1's trailing-entry word as
 * the temporary font entry. Return the previous resource without releasing it. */
itfMesResource_t *itfMesMngSetResourcePtr(s32 id, itfMesResource_t *presource) {
    instanceMes_tag *mes = pinst[id].mes;
    itfMesResource_t *previous = mes->pdata;
    itfMesBinTypeHeader_t *entry;
    u8 *temporaryFontEntry;

    mes->pdata = presource;
    ResolveResourceAddress((itfMesResource_t *)presource);
    entry = GetTypeHeader2(presource);
    temporaryFontEntry = 0;
    if (presource->magic == ITF_MES_MAGIC_MSG1) {
        /* itfMesBinTypeHeader_t is an invented type and the IDB has no counterpart,
         * so +8 past its two known words has no name to take. */
        temporaryFontEntry = *(u8 **)((u8 *)entry + 8);
    }
    mes->pext = temporaryFontEntry;
    return previous;
}

/* Look up an entry-table item; only the item index has a bounds check. */
u8 *itfMesMngGetDataPtrImmediate(s32 id, s32 message, s32 page) {
    return GetMessagePagePtr(GetTypeHeader(pinst[id].mes, message)->ptr, page);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", ResetWindowPara);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngChangeWindowType_itf);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSelectDefKey);

/* Build and release a temporary glyph chain to measure the widest grouped row.
 * Empty tables and zero/missing encoded items return zero. */
s32 itfMesMngGetWidth(s32 id, s32 message, s32 page) {
    instanceMes_tag *mes = pinst[id].mes;
    itfMesBinMesHeader_t *table;
    u8 *encodedText;
    frFontHandle_t *glyphChain;
    s32 textExtent;

    table = GetTypeHeader(mes, message)->ptr;
    if (table->page == 0) {
        return 0;
    }
    CommitVarTable((s32 *)mes->variable.pptr);
    if (mes->pext != 0) {
        frSetExtDic(mes->pext);
    }
    encodedText = GetMessagePagePtr(table, page);
    if (encodedText == 0) {
        return 0;
    }
    glyphChain = MessageRequest(0, 0, encodedText, 0);
    textExtent = GetMessageWidth(glyphChain);
    frDestroyFontRequestAfter(glyphChain);
    return textExtent;
}

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesMngSetDefaultSpeed(s32 id, u8 speed) {
    pinst[id].mes->message.color[3] = speed;
}

void itfMesMngSetKutiCallback(s32 id, void (*func)(void)) {
    pinst[id].mes->func_kuti = (addr_t)func;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngInitializeManager);

/* Set manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesMngSetBitStatManager(u32 flags) {
    itfMesMngManager.flags |= flags;
}

/* Clear manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesMngClearBitStatManager(u32 flags) {
    itfMesMngManager.flags &= ~flags;
}

/* Read the manager's 16-bit flags, distinct from itfMessageFlags. */
u16 itfMesMngGetStatManager(void) {
    return itfMesMngManager.flags;
}

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfMesDebugLabelChange);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfMesDebugLabelPosition);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfMesDebugLabelRightBottom);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfMesDebugLabelLeftTop);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngDebugProc);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfLeaf11);

/* Tear down installed content/resources, return the pool record and decrement
 * the pool-array-relative counter. An empty record is a no-op. */
void DestructFlow(s32 id) {
    itfMesLink_t *windowRecord = &itfMesMngManager.nodes[id];
    instanceMes_tag *mes = windowRecord->state;

    if (mes != NULL) {
        itfMesMngRequestMessageRelease(id, 1);
        itfMesMngRequestSelectRelease(id);
        ReleaseAllWindow(mes);
        ReleaseAllVariable(&mes->variable);
        mes->stat = 0;
        sdfMemFreeDirect(windowRecord->resourceHandle);
        windowRecord->state = NULL;
        itfMiscLinkGive(windowRecord, &itfMesMngManager.pool);
        itfMesMngManager.activeWindowCount -= 1;
    }
}

void ResolveResourceAddress(itfMesResource_t *pdata)
{
    s32 *payload = (s32 *)pdata->entries;

    if (pdata->relocated == 0) {
        sdfAddressResolver(payload, EE_ADDR_S(payload), (u8 *)pdata + pdata->fixupTableOffset, pdata->fixupTableBytes);
        pdata->relocated = 1;
    }
}

itfMesBinTypeHeader_t *GetTypeHeader(instanceMes_tag *pinst, s32 index)
{
    itfMesBinTypeHeader_t *entries = pinst->pdata->entries;

    return &entries[index];
}

itfMesBinTypeHeader_t *GetTypeHeader2(itfMesResource_t *pfileheader)
{
    itfMesBinTypeHeader_t *entries = pfileheader->entries;

    return &entries[pfileheader->entryCount];
}

ItfMesEntryItemsResult itfGetMessageType(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex) {
    return (ItfMesEntryItemsResult)GetTypeHeader(pinst[window].mes, entryIndex)->type;
}

u32 itfGetMessageNo(ItfMesWindowIndex window) {
    return pinst[window].mes->pdata->entryCount;
}

u8 *GetMessagePagePtr(itfMesBinMesHeader_t *table, s32 index)
{
    s32 count = table->page;

    if (index < 0 || index >= count) {
        return NULL;
    }
    return EE_ADDR_AS(u8 *, table->addr[index]);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", CallEventVoice);

INCLUDE_ASM(const s32, "interface/itfMesManager", SetVar);

void CommitVarTable(s32 *pvar)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        frSetVariableRequest(i, pvar[i]);
    }
}

u8 *GetSpeakerName(instanceMes_tag *pinst)
{
    u32 *items = GetTypeHeader2(pinst->pdata)->type;

    return EE_ADDR_AS(u8 *, items[pinst->speaker.num]);
}

s32 GetNumSelectItem(s32 allitem, u32 mask)
{
    s32 n = 0;

    while (allitem > 0) {
        u32 bit = mask & 1;

        mask >>= 1;
        allitem--;
        if (bit == 0) {
            n++;
        }
    }
    return n;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", GetFontHandleUnit);

/* Discard (to - from - 1) preceding rows when positive, retain the next
 * complete y-group and queue everything after it. Requires non-NULL input;
 * return NULL if discarding preceding rows exhausts the chain. */
frFontHandle_t *UnchainFontHandleUnit(frFontHandle_t *pfont, s32 index, s32 lines) {
    s32 rowsToDiscard = lines - index - 1;
    s32 rowY = pfont->y;
    frFontHandle_t *nextNode;
    frFontHandle_t *rowHead;
    frFontHandle_t *rowTail;

    while (rowsToDiscard > 0) {
        while (rowY == pfont->y) {
            nextNode = pfont->previous;
            pfont->next = NULL;
            pfont->chainHead = pfont;
            pfont->previous = NULL;
            frDestroyFontRequestAfter(pfont);
            pfont = nextNode;
            if (pfont == NULL) {
                return NULL;
            }
        }
        rowsToDiscard--;
        rowY = pfont->y;
    }
    rowHead = pfont;
    do {
        rowTail = pfont;
        pfont = pfont->previous;
    } while (pfont != NULL && rowY == pfont->y);
    while (pfont != NULL) {
        nextNode = pfont->previous;
        pfont->next = NULL;
        pfont->chainHead = pfont;
        pfont->previous = NULL;
        frDestroyFontRequestAfter(pfont);
        pfont = nextNode;
    }
    rowHead->next = NULL;
    rowTail->previous = NULL;
    for (pfont = rowHead; pfont != NULL; pfont = pfont->previous) {
        /* Despite its current name, this link points to the retained row tail. */
        pfont->chainHead = rowTail;
    }
    return rowHead;
}

frFontHandle_t *GetLastLinkFontHandle(frFontHandle_t *pfont)
{
    while (pfont->previous != NULL) {
        pfont = pfont->previous;
    }
    return pfont;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", GetLastMessagePara);

s32 CountLineMessage(frFontHandle_t *pfont_top, frFontHandle_t *pfont)
{
    s16 h = pfont->parentDimensionsOrRenderWord.parentDimensions.cellHeight;

    return ((pfont->y - pfont_top->y) >> 3) / h + 1;
}

/* Translate every linked node in text units; a NULL chain is a no-op. */
void ChangeFontPosition(frFontHandle_t *pfont, s32 dist_x, s32 dist_y) {
    if (pfont == NULL) {
        return;
    }
    do {
        pfont->x += dist_x;
        pfont->y += dist_y;
        pfont = pfont->previous;
    } while (pfont != NULL);
}

void ChangeZBuffer(frFontHandle_t *pfont, s32 z)
{
    while (pfont != NULL) {
        pfont->renderValueOrSetupOrShade.renderValue = z;
        pfont = pfont->previous;
    }
}

void ChangeFontColor(frFontHandle_t *pfont, s32 index, s32 lines, s32 color)
{
    s32 skip = lines - index - 1;
    s32 y = pfont->y;
    s32 cur = y;
    frTextHandle_t *c;
    u8 flag;

    while (skip > 0) {
        while (y == cur) {
            pfont = pfont->previous;
            if (pfont == NULL) {
                return;
            }
            cur = pfont->y;
        }
        skip--;
        y = cur;
    }
    flag = color;
    do {
        for (c = pfont->firstChild; c != NULL; c = c->next) {
            c->renderValueOrSetupOrShade.setupBytes.firstOption = flag;
        }
        pfont = pfont->previous;
    } while (pfont != NULL && y == pfont->y);
}

void ChangeFontColorAll(frFontHandle_t *pfont, u8 color)
{
    frTextHandle_t *c;

    for (; pfont != NULL; pfont = pfont->previous) {
        for (c = pfont->firstChild; c != NULL; c = c->next) {
            c->renderValueOrSetupOrShade.setupBytes.firstOption = color;
        }
    }
}

/* Clear each child's low color byte, then OR the unmasked input word.
 * High input bits can therefore also change the upper bytes. */
void ChangeFontAlpha(frFontHandle_t *pfont, u32 alpha) {
    frTextHandle_t *child;

    for (; pfont != NULL; pfont = pfont->previous) {
        for (child = pfont->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = child->parentDimensionsOrRenderWord.renderWord & ITF_MES_COLOR_BYTE_CLEAR_MASK | alpha;
        }
    }
}

s32 GetMessageWidth(frFontHandle_t *pfont)
{
    s32 max = 0;

    while (pfont != NULL) {
        s32 y = pfont->y;
        s32 w = 0;

        do {
            w += pfont->advance;
            pfont = pfont->previous;
        } while (pfont != NULL && y == pfont->y);
        if (w > max) {
            max = w;
        }
    }
    return max << 4;
}

s32 GetMaskedIndex(s32 index, u32 mask)
{
    s32 i = 0;

    while (i < 0x20) {
        if ((mask & 1) == 0) {
            if (--index < 0) {
                break;
            }
        }
        i++;
        mask >>= 1;
    }
    return i;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", RaiseMessageSpeedMax);

INCLUDE_ASM(const s32, "interface/itfMesManager", RaiseMessageSpeedMaxAll);

INCLUDE_ASM(const s32, "interface/itfMesManager", OpenMessageWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", OpenCheckSpeakerWindow);

void InitializeSpeaker(Speaker_tag *pspeaker, s32 defaultpos) {
    if (defaultpos != 0) {
        pspeaker->x = 0x3D0;
        pspeaker->y = 0x988;
    }
    pspeaker->pfont = 0;
    pspeaker->num = 0xffff;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeMessage);

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeSelect);

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeWindow);

void InitializeVariable(variable_tag *pvariable)
{
    s32 n = 0x1F;
    u32 *p = &pvariable->pptr[31];

    do {
        n--;
        *p = 0;
        p--;
    } while (n >= 0);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngInitializeSub);

INCLUDE_ASM(const s32, "interface/itfMesManager", ForceSetCurCtr);

INCLUDE_ASM(const s32, "interface/itfMesManager", ReleaseAllWindow_itf);

void ReleaseAllVariable(variable_tag *pvariable)
{
    s32 remaining = 0x1F;

    do {
        s32 index = 0x1F - remaining;

        if (pvariable->pptr[index] != 0) {
            sdfMemFreeDirect(pvariable->pmh[index]);
            pvariable->pptr[index] = 0;
        }
        remaining--;
    } while (remaining >= 0);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", ConvertZenkakuNumber);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfLeaf12);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcMesFlow_itf);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfCalcMessageSub);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcMessage);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcSelect);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfCallChangeFontPosition);

INCLUDE_ASM(const s32, "interface/itfMesManager", SelectItem);

INCLUDE_ASM(const s32, "interface/itfMesManager", SelectItemMove);

s32 SelectItemDefKey(select_tag *psel) {
    s32 i;

    for (i = 0; i < psel->numdef; i++) {
        defkey_tag *option = &psel->defkey[i];

        if (dds3PadButtonState.buttons[option->key] < 0) {
            s32 prefixLength = option->index;
            s32 rank = 0;
            u32 mask = psel->mask;

            if (prefixLength > 0) {
                s32 remaining = prefixLength;
                do {
                    if ((mask & 1) == 0) {
                        rank++;
                    }
                    mask >>= 1;
                } while (--remaining != 0);
            }
            if ((mask & 1) == 0) {
                return rank;
            }
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfCalcMesFlowItfSub);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfFunc2);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfLeaf13);

INCLUDE_ASM(const s32, "interface/itfMesManager", FadeOutWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfCallGetRandFloatA);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMesFlow);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfLeaf14);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff2);

s32 CalcMsgMng_Process(void) {
    itfMesLink_t *node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        CalcMesFlow(node->state);
        node = node->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMsgMng_Process);

void DrawMsgMng_Shutdown(void) {
    itfMesLink_t *node = itfMesMngManager.pool.activeHead;
    s32 window;
    while (node != 0) {
        window = node->index;
        node = node->next;
        DestructFlow(window);
    }
    sdfTexFreeDelayed(itfMesMngManager.windowTexture);
    itfMesMngManager.windowTexture = NULL;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", DebugDrawWindowLine);

INCLUDE_ASM(const s32, "interface/itfMesManager", DebugMoveWinVert);

INCLUDE_ASM(const s32, "interface/itfMesManager", DebugMoveWinAlpha);

itfMesLink_t *DebugChangeInstance(itfMesLink_t *pflow) {
    s32 index = 0;
    s32 count = itfMesMngManager.activeWindowCount;

    if (count <= 0) {
        return NULL;
    }
    for (;;) {
        if (pflow != NULL) {
            pflow = pflow->previous;
        }
        if (pflow == NULL) {
            pflow = itfMesMngManager.pool.activeTail;
        }
        if ((pflow->state)->window.mes != NULL) {
            break;
        }
        index++;
        if (count < index) {
            pflow = NULL;
            break;
        }
    }
    return pflow;
}

void itfQueueOffsetTexturedRect(s32 *bounds, s32 *region, s32 x, s32 y,
                   s32 alpha, sdfTexHandle_t *texture, sdfDrawTag_t *command) {
    s32 positions[4];
    s32 uv[4];
    UiQuadColor color = itfQueueOffsetTexturedRectTable;

    positions[0] = (bounds[0] + x) << 4;
    positions[1] = (bounds[1] + y) << 3;
    positions[2] = (bounds[2] + x) << 4;
    positions[3] = (bounds[3] + y) << 3;
    uv[0] = region[0] << 4;
    uv[1] = region[1] << 4;
    uv[2] = (region[0] + region[2]) << 4;
    uv[3] = (region[1] + region[3]) << 4;
    color.alpha = alpha;
    itfMiscDrawSpritePrim((DrawVertex *)positions, (DrawColorRec *)uv, (DrawColorRec *)&color, 0, texture, 0, command);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff3);

INCLUDE_ASM(const s32, "interface/itfMesManager", InputPasswdDraw);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfInputPasswd);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfCalcMesFlowItfSubTable);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfFunc2Table);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuffTable);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuffTable2);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuffTable3);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff2Table);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff2Table2);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfQueueOffsetTexturedRectTable);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff3Table);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff3Table2);

INCLUDE_RODATA(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff3Table3);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfData3);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfData4);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesDebugMenuLabelsEntry);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesMarkerEdit);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesMarkerSelect);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesDebugItemFormat);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesMarkerNone);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesDebugFadeFormat);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfCallGetRandFloatAData);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesDebugLineVertIndex);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesDebugLineColorIndex);

