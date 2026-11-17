#include "common.h"
#include "sdfAddrResolve.h"
#include "interface.h"
#include "newdata.h"
#include "itfMesManager.h"
#include "kernel.h"
#include "sdfFrame.h"
#include "sdfMemory.h"
#include "dds3KernelDraw.h"
#include "dds3KernelCore.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "itfMisc.h"
#include "sdfTex.h"
#include "scrTraceCode.h"
#include "frFont.h"
#include "itfMisc.h"
#include "itfPanel.h"
#include "kernel_draw.h"
#include "evtCommand.h"
#include "itf.h"
#include "dds3Randomize.h"
#include "sdk_libm.h"
#include "sdfSound.h"
#include "dds3KernelMisc.h"
#include "sdfDevCons.h"

extern itfMesShake_t itfData;
extern u32 itfData3;

/* Per-axis share of the amplitude above which the shake pulls back to zero. */
typedef struct itfMesShakeGain_t {
    f32 axis[2];
} itfMesShakeGain_t;

extern itfMesShakeGain_t itfCallGetRandFloatAData[];
void itfCallGetRandFloatA(void);
s32 itfLeaf14(instanceMes_tag *pinst);
s32 itfCallGrAllocPacketBuff(instanceMes_tag *pinst);
void itfCallGrAllocPacketBuff2(instanceMes_tag *pinst);

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
    itfMesLink_t nodes[0x40];
};

/* State of the interactive message-layout inspector. */
struct debug_tag {
    s32 cursor;
    s32 mode;
    itfMesLink_t *pmesflow;
};
extern debug_tag itfMesDebug;

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
extern s32 itfMesMngRequestMessage(s32 window, s32 entryIndex, s32 itemIndex);
void itfMesMngSetPhase(s32 msgid, s8 phase);
s32 itfMesMngSelect(void);
extern void itfMesMngRequestSelect(s32 id, s32 select);
void itfMesMngRequestSelectRelease(s32 id);
s16 itfMesMngGetSelPhase(s32 msgid);
s32 itfMesMngGetSelectNo(s32 msgid);
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
void InitializeMessage(message_tag *pmessage, s32 defaultpos);
void InitializeVariable(variable_tag *pvariable);
void InitializeSelect(select_tag *pselect);
void InitializeWindow(window_tag *pwindow);
void itfMesMngRelease(s32 id);
void DestructFlow(s32 id);

#define ITF_MES_ENTRY_STATE_MASK 7

#define ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK 0xFFFDFFFF

#define ITF_MES_OPTION_STATE_MASK 0x38

#define ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK 0xFFFBFFFF

void itfMesMngSetSpeakerPosition(s32 id, s32 x, s32 y);
void ChangeFontPosition(frFontHandle_t *pfont, s32 dist_x, s32 dist_y);
void itfMesMngSetSpeakerPositionOffset(s32 id, s32 x, s32 y);
void itfMesMngSetMesPositionOffset(s32 id, s32 x, s32 y);
void itfMesMngSetSelPosition(s32 id, s32 x, s32 y);
void itfMesMngSetSelPositionOffset(s32 id, s32 x, s32 y);
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

s32 itfMesMngGetWidth(s32 id, s32 message, s32 page);
void CommitVarTable(s32 *pvar);
s32 GetMessageWidth(frFontHandle_t *pfont);
void itfMesMngSetDefaultSpeed(s32 id, u8 speed);
extern void itfMesMngSetKutiCallback(s32 id, void (*func)(void));

#define ITF_MES_WINDOW_CAPACITY 0x40

#define ITF_MES_POOL_NODE_BYTES 0x14

s32 CalcMsgMng_Process(dds3ProcessID_t *id);
void DrawMsgMng_Shutdown(dds3ProcessID_t *id);
s32 DrawMsgMng_Process(dds3ProcessID_t *id);
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
extern void itfMiscLinkGive(itfMesLink_t *pelem, itfMesLinkControl_t *pstack);

#define ITF_MES_MAGIC_MSG0 0x3047534d

#define ITF_MES_STRING_COPY_ROUND_BIAS 4

#define ITF_MES_COPY_ALIGN_MASK 3

#define ITF_MES_BINARY_COPY_ROUND_BIAS 5

/* Keyframes of the second cursor controller: itfFunc2 interpolates ctr0 and
 * the two PS2-only floats between consecutive entries. */
typedef struct itfMesMngCurKey_t {
    s32 frame;
    s32 ctr0;
    f32 unk14;
    f32 unk18;
} itfMesMngCurKey_t;

typedef struct itfMesMngCurKeyTable_t {
    itfMesMngCurKey_t key[3];
} itfMesMngCurKeyTable_t;

extern itfMesMngCurKeyTable_t itfFunc2Table;

/* Keyframes of the first cursor controller, interpolated by itfCalcMesFlowItfSub. */
typedef struct itfMesMngFlowKey_t {
    s32 frame;
    s32 ctr0;
    s32 ctr1;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
} itfMesMngFlowKey_t;

typedef struct itfMesMngFlowKeyTable_t {
    itfMesMngFlowKey_t key[5];
} itfMesMngFlowKeyTable_t;

extern itfMesMngFlowKeyTable_t itfCalcMesFlowItfSubTable;

/* Cursor sprite sheet for itfCallGrAllocPacketBuff: five uv rects, four
 * sprite sizes and the base colour whose alpha is replaced per sprite. */
typedef struct itfMesMngCursorUvTable_t {
    s32 uv[5][4];
} itfMesMngCursorUvTable_t;

typedef struct itfMesMngCursorSizeTable_t {
    s32 size[4][2];
} itfMesMngCursorSizeTable_t;

typedef struct itfMesMngCursorColor_t {
    u32 rgba[4];
} itfMesMngCursorColor_t;

extern itfMesMngCursorUvTable_t itfCallGrAllocPacketBuffTable;
extern itfMesMngCursorSizeTable_t itfCallGrAllocPacketBuffTable2;
extern itfMesMngCursorColor_t itfCallGrAllocPacketBuffTable3;

frFontHandle_t *UnchainFontHandleUnit(frFontHandle_t *pfont, s32 index, s32 lines);
void ChangeFontAlpha(frFontHandle_t *pfont, u32 alpha);
void ForceSetCurCtr(curctr_tag *pcurctr, s32 phase);
void InitializeCurCtr(curctr_tag *pcurctr, s32 isdisp);
void ResetWindowPara(window_tag *pwin, s32 type, s32 alpha);
void itfCalcMesFlowItfSub(instanceMes_tag *pinst);
void itfFunc2(instanceMes_tag *pinst);

#define ITF_MES_COLOR_BYTE_CLEAR_MASK 0xffffff00

void CalcMesFlow(instanceMes_tag *pinst);
s32 SelectItemDefKey(select_tag *psel);
void ChangeFontColor(frFontHandle_t *pfont, s32 index, s32 lines, s32 color);
extern u8 itfMesDebugLineVertIndex[5];
extern u8 itfMesDebugLineColorIndex[5];
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
 * String mode has no memset. Keep the K&R declaration form. */
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
extern char *itfMesSelectionMarkers[2];
extern char *itfMesDebugMenuLabels[5];

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
#include "font.h"
#include "code_r1_001C4B78.h"
#include "code_r1_001CC1D0.h"

/* Prior interpreter trace passes that yielded while a script command remained
 * pending; callbacks observe the count before the current pass is recorded. */
typedef u32 ScrCommandYieldCount;

/* During a command callback, return the number of earlier trace passes that
 * yielded while it was pending. The first invocation sees zero; each yielded
 * pass adds one, and a continuing or finishing handler resets the count. This
 * is callback wait state, not wall-clock time, and is valid only in the callback. */
ScrCommandYieldCount scrGetCommandTimer(void);

/* Signed index selecting an entry record in an installed message resource. */
typedef s32 ItfMesEntryIndex;

/* Outcome reported by starting an entry in a message window. */
typedef s32 ItfMesEntryStartResult;

ItfMesEntryStartResult itfMesStartEntry(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex, s32 itemIndex);

/* The message manager owns a fixed pool of 64 windows in this 0x520-byte bank. */
typedef struct mesflow_tag {
    struct mesflow_tag *previous;
    struct mesflow_tag *next;
    ItfMesWindowIndex index;
    instanceMes_tag *state;
    sdfMemHandle_t *resourceHandle;
} mesflow_tag;

typedef char ItfMesPoolNode_size_must_be_0x14[(sizeof(mesflow_tag) == 0x14) ? 1 : -1];

#define ITF_PANEL_HOLD_CAPACITY 0x40

typedef struct itfPanelHoldItem_t {
    s32 first;
    s16 second;
    s16 third;
} itfPanelHoldItem_t;

/* Bounded held-panel queue. Unknown scalar slots stay unnamed. */
typedef struct itfPanelHold_t {
    s32 window;
    s16 unk4;
    s16 queuedCount;
    s32 entryEndFrame; /* Script timer deadline of the active entry. */
    itfPanelHoldItem_t items[ITF_PANEL_HOLD_CAPACITY];
} itfPanelHold_t;

typedef char itfPanelHoldItem_size_must_be_8[sizeof(itfPanelHoldItem_t) == 8 ? 1 : -1];

typedef char itfPanelHold_size_must_be_0x20C[
    sizeof(itfPanelHold_t) == 0x20C ? 1 : -1];

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

extern void (*DrawTable[])(itfpanel_t *, sdfDrawTag_t *);
extern itfPanelHold_t itfHeldPanelCursor;

extern u8 D_003B45E8[], D_003B4600[], D_003B4618[];
extern u8 D_003B4630[], D_003B4648[], D_003B4660[];
extern u8 D_004365A8[8], D_004365B0[8];
s32 itfPanelAcquireHold(void);
s32 itfPanelReleaseHold(void);
s32 itfCommandQueueHeldPanelEntry(void);

#ifdef VERSION_RAIDOU2
/* Configure the fade on timer zero, wait thirty ticks, then consume a negative option status.
   Keep both timer reads: setup tests a second read, while later comparisons use the cached one. */
s32 MesMngSelect(void);

#endif
#include "sdk_libc.h"

/* The selected text scan reports whether it contains the F413 voice cue. */
typedef s32 ItfMesVoiceCueResult;

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
extern s32 itfInitTextDrawArgs(const u8 *encodedText, _FRQ *sub);

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

/* body: matched in smt3 @ 9401f79e: src/smt3/interface/itfMesManager.c:480, match, same function 0.97 (words 17/18, calls 1.00, shape 0.97) */
void itfLeaf8(s32 msgid, s32 y1)
{
    itfPnlSetPositionOffset(pinst[msgid].mes->window.mes, 0, y1, 0, 0, 0);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfEndMessageExSub);

#ifdef VERSION_RAIDOU1
s32 itfMesMngChkWaitTag(s32 msgid) {
    return pinst[msgid].mes->message.textInterfaceMask & 1;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngChkWaitTag);
#endif

#ifdef VERSION_RAIDOU1
s8 itfMesMngGetPhase(s32 msgid)
{
    return pinst[msgid].mes->message.phase;
}

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 msgid, s8 phase) {
    pinst[msgid].mes->message.phase = phase;
}

/* Capture the script window, reset its held queue, and switch to page three.
   Invalid windows still return the native command-complete value of one. */
s32 itfPanelAcquireHold(void) {
    itfPanelHold_t *hold;
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
    itfPanelHold_t *hold = &itfHeldPanelCursor;

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
    itfPanelHold_t *cursor = &itfHeldPanelCursor;
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
#else
s8 itfMesMngGetPhase(s32 msgid) {
    return pinst[msgid].mes->message.phase;
}

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 msgid, s8 phase) {
    pinst[msgid].mes->message.phase = phase;
}

/* Capture the script window, reset its held queue, and switch to page three.
   Invalid windows still return the native command-complete value of one. */
s32 itfPanelAcquireHold(void) {
    itfPanelHold_t *hold;
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
    itfPanelHold_t *hold = &itfHeldPanelCursor;

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
    itfPanelHold_t *cursor = &itfHeldPanelCursor;
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
#endif

#ifdef VERSION_RAIDOU1
extern s32 MesMngSelectWork;
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", MesMngSelect);
#endif

#ifdef VERSION_RAIDOU1
s32 MesMngSelect(void) {
    s32 window;
    s32 elapsedTicks;
    s32 entry;
    instanceMes_tag *record;
    select_tag *options;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    elapsedTicks = scrGetCommandTimer();
    if (scrGetCommandTimer() == 0) {
        dds3SetFilter2Para(0x44);
        dds3SetFilter2RGBA(0x5E1C1C1C);
        dds3Filter2On(ITF_PANEL_MESSAGE_FADE_TICKS);
        MesMngSelectWork = -1;
        return 0;
    }
    if (elapsedTicks < ITF_PANEL_MESSAGE_FADE_TICKS) {
        return 0;
    }
    if (elapsedTicks == ITF_PANEL_MESSAGE_FADE_TICKS) {
        itfMesMngChangeWindowType(window, ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION, 0);
    } else if (MesMngSelectWork == -1) {
        record = pinst[window].mes;
        entry = scrGetIntPara(0);
        options = &record->select;
        if (options->phase == 0) {
            itfMesMngRequestSelect(window, entry);
        } else if (options->phase < 0) {
            options->phase = 0;
            scrSetIntReturnValue(options->select);
            itfMesMngRequestSelectRelease(window);
            dds3Filter2Off(ITF_PANEL_MESSAGE_EXIT_TICKS);
            MesMngSelectWork = 0;
            return 1;
        }
    }
    return 0;
}

/* Return the option-block signed status word, not the separate entry status byte. */
#endif
s16 itfMesMngGetSelPhase(s32 msgid) {
    return pinst[msgid].mes->select.phase;
}

#ifdef VERSION_RAIDOU1
void itfMesMngSetSelPhase(s32 msgid, s16 phase)
{
    pinst[msgid].mes->select.phase = phase;
}

/* Return the same signed word forwarded as the script result when an option completes. */
s32 itfMesMngGetSelectNo(s32 msgid) {
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
#else
void itfMesMngSetSelPhase(s32 msgid, s16 phase) {
    pinst[msgid].mes->select.phase = phase;
}

/* Return the same signed word forwarded as the script result when an option completes. */
s32 itfMesMngGetSelectNo(s32 msgid) {
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
#endif

#ifdef VERSION_RAIDOU1
s32 itfMesMngOpenWindow(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngMarkBitStat(window, 0x200000);
    itfSetMesDispPriorityData[0] = 0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngOpenWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngCloseWindow);
#endif

#ifdef VERSION_RAIDOU1
s32 itfMesMngCloseWindow(void) {
    s32 window = scrGetMessageHandle();
    instanceMes_tag *mes;
    u32 scriptStatus;

    if (window < 0) {
        return 1;
    }
    mes = pinst[window].mes;
    scriptStatus = mes->stat & ITF_MES_LOWER_STATUS_PAIR_MASK;
    if (scriptStatus == ITF_MES_LOWER_STATUS_FIRST_BIT || scriptStatus == ITF_MES_LOWER_STATUS_SECOND_BIT) {
        itfMesMngMsgWndCls(window);
    }
    return scriptStatus < 1;
}

void itfMesMngMsgWndCls(s32 msgid) {
    instanceMes_tag *mes = pinst[msgid].mes;
    u32 previousFlags = mes->stat;

    if (previousFlags & ITF_MES_LOWER_STATUS_PAIR_MASK) {
        mes->stat = previousFlags | ITF_MES_LOWER_STATUS_PAIR_MASK;
    }
    if (previousFlags & ITF_MES_UPPER_STATUS_PAIR_MASK) {
        mes->stat = mes->stat | ITF_MES_UPPER_STATUS_PAIR_MASK;
    }
    if (previousFlags & 0xC00) {
        mes->stat = mes->stat | 0xC00;
    }
    itfMesMngRequestMessageRelease(msgid, 1);
    itfMesMngRequestSelectRelease(msgid);
    FadeOutWindow();
    mes->stat &= ITF_MES_CLEAR_SCRIPT_PANEL_BIT;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngMsgWndCls);
#endif

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

#ifdef VERSION_RAIDOU1
s32 itfMesMngInitialize(void *data) {
    itfMesLink_t *node = (itfMesLink_t *)itfMiscLinkTake((itfmisclinkstack_t *)&itfMesMngManager.pool);
    s32 window = node->index;
    instanceMes_tag *mes;
    sdfMemHandle_t *handle;

    handle = sdfMemAllocDirect(sizeof(instanceMes_tag));
    node->resourceHandle = handle;
    mes = EE_ADDR_AS(instanceMes_tag *, sdfMemLock(handle));
    node->state = mes;
    mes->pdata = NULL;
    itfMesMngSetResourcePtr(window, data);
    mes->stat = 0;
    mes->otindex = 0x67;
    mes->z = 0xFFFFF0;
    mes->type = 0;
    mes->func_kuti = 0;
    InitializeSpeaker(&mes->speaker, 1);
    InitializeMessage(&mes->message, 1);
    InitializeSelect(&mes->select);
    InitializeWindow(&mes->window);
    InitializeVariable(&mes->variable);
    InitializeCurCtr(&mes->curctr[0], 0);
    InitializeCurCtr(&mes->curctr[1], 0);
    itfMesMngManager.activeWindowCount++;
    return window;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngInitialize);
#endif

/* Destroy an installed nonnegative window slot; no upper-bound validation. */
void itfMesMngRelease(s32 id) {
    if (id >= 0 && pinst[id].mes != NULL) {
        DestructFlow(id);
    }
}

#ifdef VERSION_RAIDOU1
void itfMesMngClearCursor(s32 window) {
    instanceMes_tag *mes;

    if (window >= 0) {
        mes = pinst[window].mes;
        InitializeCurCtr(&mes->curctr[0], 0);
        InitializeCurCtr(&mes->curctr[1], 0);
    }
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngClearCursor);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestMessage_itf);
#endif

#ifdef VERSION_RAIDOU1
s32 itfMesMngRequestMessage_itf(s32 window, s32 entryIndex, s32 itemIndex) {
    instanceMes_tag *panel = pinst[window].mes;
    message_tag *mes = &panel->message;
    Speaker_tag *spk;
    itfMesBinMesHeader_t *table;
    u32 flags;
    u32 mode;

    table = GetTypeHeader(panel, entryIndex)->ptr;
    mes->page = itemIndex;
    mes->table = table;
    mes->maxpage = table->page;
    if (table->page == 0) {
        return 0;
    }
    CommitVarTable((s32 *)panel->variable.pptr);
    if (panel->pext != NULL) {
        frSetExtDic(panel->pext);
    }
    ReadytoMessage(panel);
    spk = &panel->speaker;
    if (spk->num != (u16)table->speaker) {
        u16 nameId = table->speaker;

        if (spk->pfont != NULL) {
            frDestroyFontRequestAfter(spk->pfont);
            spk->pfont = NULL;
        }
        spk->num = nameId;
        if (nameId != 0xFFFF) {
            if (panel->window.spk != NULL) {
                itfPnlRelease(panel->window.spk);
                panel->window.spk = NULL;
            }
            if (nameId & 0x8000) {
                spk->pfont = MessageRequestEx(spk->x, spk->y, 0, 0, 0, 0x80, itfMesMngGetVar(window, nameId & 0x7FFF), NULL);
            } else {
                spk->pfont = MessageRequestEx(spk->x, spk->y, 0, 0, 0, 0x80, GetSpeakerName(panel), NULL);
            }
            frSetFontColor(spk->pfont, 0x6464C880);
            ChangeZBuffer(spk->pfont, panel->z);
        }
    }
    OpenCheckSpeakerWindow(panel);
    flags = panel->stat;
    if (flags & 0x200000) {
        mode = flags & 0x300;
        if (mode == 0 || mode == 0x300) {
            OpenMessageWindow(panel);
            panel->stat = (panel->stat & ~7) | 2;
        } else {
            panel->stat = (flags & ~7) | 3;
        }
    } else {
        panel->stat = (flags & ~7) | 3;
    }
    mes->phase = 1;
    panel->stat |= 0x1000000;
    return 1;
}

ItfMesVoiceCueResult itfMesSelectTableItemText(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex, s32 itemIndex) {
    instanceMes_tag *mes = pinst[window].mes;
    itfMesBinMesHeader_t *table = GetTypeHeader(mes, entryIndex)->ptr;
    message_tag *message = &mes->message;

    message->page = itemIndex;
    message->table = table;
    message->maxpage = table->page;
    if (table->page != 0) {
        return itfInitTextDrawArgs(GetMessagePagePtr(table, (s16)itemIndex), 0);
    }
    return 0;
}

/* Release/reset selected-entry glyphs and their status; optionally do the same
 * for primary text. Negative window is a no-op; no other slot validation. */
void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker) {
    instanceMes_tag *mes;
    message_tag *message;
    Speaker_tag *primaryTextBlock;

    if (id < 0) {
        return;
    }
    mes = pinst[id].mes;
    message = &mes->message;
    primaryTextBlock = &mes->speaker;
    if (message->pfont != NULL) {
        frDestroyFontRequestAfter(message->pfont);
        message->pfont = NULL;
    }
    InitializeMessage(message, 0);
    mes->stat &= ~ITF_MES_ENTRY_STATE_MASK;
    mes->stat &= ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK;
    if (cls_speaker == 0) {
        return;
    }
    if (primaryTextBlock->pfont != NULL) {
        frDestroyFontRequestAfter(primaryTextBlock->pfont);
        primaryTextBlock->pfont = NULL;
    }
    InitializeSpeaker(primaryTextBlock, 0);
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesSelectTableItemText);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestMessageRelease);
#endif

/* NON_MATCHING: 73 of 123 words match.  */
#ifdef NON_MATCHING
void itfMesMngRequestSelect_itf(s32 id, s32 select) {
    instanceMes_tag *panel = pinst[id].mes;
    select_tag *sel = &panel->select;
    itfMesBinTypeHeader_t *entry;
    itfMesBinMesHeader_t *table;
    s32 count;
    s32 y;
    frFontHandle_t *chain;

    entry = GetTypeHeader(panel, select);
    if (sel->pfont != NULL) {
        frDestroyFontRequestAfter(sel->pfont);
        sel->pfont = NULL;
    }
    table = entry->ptr;
    count = GetNumSelectItem(table->speaker, sel->mask);
    y = sel->y - ((count * 25 - 25) << 3);
    CommitVarTable((s32 *)panel->variable.pptr);
    if (panel->pext != NULL) {
        frSetExtDic(panel->pext);
    }
    chain = GetFontHandleUnit(&table->addr[1], table->speaker, sel->mask, sel->x, y, panel->z);
    sel->lines = count;
    sel->pfont = chain;
    sel->selState[1] = 4;
    sel->phase = 1;
    panel->stat = (panel->stat & ~0x38) | 0x10;
    sel->selState[0] = 0;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestSelect_itf);
#endif

void itfMesMngRequestSelectRelease(s32 id) {
    instanceMes_tag *mes = pinst[id].mes;
    select_tag *optionBlock = &mes->select;

    if (optionBlock->pfont != NULL) {
        frDestroyFontRequestAfter(optionBlock->pfont);
        optionBlock->pfont = NULL;
    }
    optionBlock->mask = 0;
    optionBlock->phase = 0;
    optionBlock->lines = 0;
    optionBlock->select = -1;
    optionBlock->selState[0] = 0;
    optionBlock->selState[1] = 0;
    mes->stat &= ~ITF_MES_OPTION_STATE_MASK;
    mes->stat &= ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK;
}

#ifdef VERSION_RAIDOU1
void itfMesMngSetMaskSelect(s32 id, u32 maskpattern)
{
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
#else
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
#endif

#ifdef VERSION_RAIDOU1
/* Move option text absolutely; an unchanged position is a no-op. */
void itfMesMngSetSelPosition(s32 id, s32 x, s32 y) {
    select_tag *optionBlock = &pinst[id].mes->select;
    s32 delta[2];

    delta[0] = x - optionBlock->x;
    delta[1] = y - optionBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(optionBlock->pfont, delta[0], delta[1]);
    optionBlock->x = x;
    optionBlock->y = y;
}

/* Translate option text and update its cached position. */
void itfMesMngSetSelPositionOffset(s32 id, s32 x, s32 y) {
    select_tag *optionBlock = &pinst[id].mes->select;

    ChangeFontPosition(optionBlock->pfont, x, y);
    optionBlock->x += x;
    optionBlock->y += y;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSelPosition);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSelPositionOffset);
#endif

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

s32 itfMesMngGetMesPage(s32 id) {
    return pinst[id].mes->message.maxpage;
}

s32 itfMesMngGetMesCurrentPage(s32 id) {
    return pinst[id].mes->message.page;
}

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

void itfMesMngChangeWindowTypeSub(s32 id, s32 type, s32 alpha)
{
    instanceMes_tag *mes = pinst[id].mes;
    window_tag *window = &mes->window;

    if (mes->type == type && window->fadeLimit == alpha) {
        return;
    }
    mes->type = type;
    ResetWindowPara(window, type, alpha);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngChangeWindowType_itf);

#ifdef VERSION_RAIDOU1
/* body: matched in smt3 @ 9401f79e: src/smt3/interface/itfMesManager.c:1274, near, same function 1.00 (words 50/50, calls 1.00, shape 1.00) */
s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index) {
    select_tag *optionBlock = &pinst[id].mes->select;
    defkey_tag *option = NULL;
    s32 optionIndex;

    if ((u32)key >= 0x10) {
        return 0;
    }
    if (key == 3 || key == 6 || key == 7) {
        return 0;
    }
    for (optionIndex = 0; optionIndex < optionBlock->numdef; optionIndex++) {
        if (optionBlock->defkey[optionIndex].key == key) {
            option = &optionBlock->defkey[optionIndex];
            break;
        }
    }
    if (option == NULL) {
        if (optionBlock->numdef >= 15) {
            return 0;
        }
        option = &optionBlock->defkey[optionBlock->numdef];
        optionBlock->numdef++;
    }
    option->key = key;
    option->index = index;
    return 1;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSelectDefKey);
#endif

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

void itfMesMngInitializeManager(void) {
    itfMesLink_t *poolNode;

    itfMesMngManager.activeWindowCount = 0;
    itfMesMngManager.ctr = 0;
    itfMesMngManager.flags = 0;
    itfMesMngManager.windowTexture = itfMiscLoadTexture("/itf/MESWIN.TMX");
    itfMiscLinkInitialize((itfmisclinkstack_t *)&itfMesMngManager.pool, (itfLink_t *)itfMesMngManager.nodes, ITF_MES_WINDOW_CAPACITY, ITF_MES_POOL_NODE_BYTES);
    for (poolNode = itfMesMngManager.pool.firstFree; poolNode != NULL; poolNode = poolNode->next) {
        poolNode->state = NULL;
    }
    dds3StartProcess("CalcMsgMng", 0x407, 0, 0, CalcMsgMng_Process, (dds3ProcessFunc_t)DrawMsgMng_Shutdown, NULL);
    dds3StartProcess("DrawMsgMng", 0x2B18, 0, 0, DrawMsgMng_Process, (dds3ProcessFunc_t)DrawMsgMng_Shutdown, NULL);
}

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

s32 itfMesMngDebugProc(void) {
    window_tag *panel;
    itfpanel_t *panelSprite;
    s32 *bounds;
    sdfDrawTag_t *packetList;
    s32 menuItemIndex;
    s32 menuItemY;
    const char *menuItemFormat;
    const char *selectionMarker;
    const char *coordinateFormat;

    if (itfMesDebug.pmesflow == NULL) {
        itfMesLink_t *window = DebugChangeInstance(NULL);

        itfMesDebug.pmesflow = window;
        if (window == NULL) {
            return 0;
        }
    }
    {
        debug_tag *debug = &itfMesDebug;

        panel = &debug->pmesflow->state->window;
        switch (debug->mode) {
        case 0:
            if (dds3PadButtonState.secondaryUp & 2) {
                if (--debug->cursor < 0) {
                    debug->cursor = 2;
                }
            } else if (dds3PadButtonState.secondaryDown & 2) {
                if (++debug->cursor >= 5) {
                    debug->cursor = 0;
                }
            }
            if (dds3PadButtonState.secondaryCancel < 0) {
                debug_tag *confirmDebug = &itfMesDebug;

                switch (confirmDebug->cursor) {
                case 0:
                case 1:
                case 2:
                    confirmDebug->mode = 1;
                    break;
                case 3:
                    confirmDebug->mode = 2;
                    break;
                case 4:
                    confirmDebug->pmesflow = DebugChangeInstance(confirmDebug->pmesflow);
                    break;
                }
            } else if (dds3PadButtonState.secondaryCross < 0) {
                return -1;
            }
            break;
        case 1:
            DebugMoveWinVert(panel, debug->cursor);
            if (dds3PadButtonState.secondaryCross < 0) {
                debug->mode = 0;
            }
            break;
        case 2:
            DebugMoveWinAlpha(panel);
            if (dds3PadButtonState.secondaryCross < 0) {
                itfMesDebug.mode = 0;
            }
            break;
        }
    }

    packetList = dds3CallGrAllocPacketBuff9();
    dds3ConsPanelTagLink(packetList, 0x10, 0x10, 0x1E, 9);
    menuItemFormat = itfMesDebugItemFormat;
    menuItemY = 0x7A00;
    for (menuItemIndex = 0; menuItemIndex < 5; menuItemIndex++, menuItemY += 0x60) {
        if (itfMesDebug.cursor == menuItemIndex) {
            selectionMarker = itfMesSelectionMarkers[itfMesDebug.mode != 0];
        } else {
            selectionMarker = itfMesMarkerNone;
        }
        sdfDrawTagLink(packetList,
                        EE_ADDR(sdfDevPrintf(0x7180, menuItemY, 0xFFFFF0, 0,
                                                     menuItemFormat, selectionMarker, itfMesDebugMenuLabels[menuItemIndex])));
    }

    coordinateFormat = "( %3d,%3d )";
    panelSprite = panel->mes;
    bounds = &panelSprite->left;
    sdfDrawTagLink(packetList,
                    EE_ADDR(sdfDevPrintf(0x7E00, 0x7A00, 0xFFFFF0, 0,
                                                 coordinateFormat, bounds[0] >> 4,
                                                 bounds[1] >> 3)));
    sdfDrawTagLink(packetList,
                    EE_ADDR(sdfDevPrintf(0x7E00, 0x7A60, 0xFFFFF0, 0,
                                                 coordinateFormat, bounds[2] >> 4,
                                                 bounds[3] >> 3)));
    sdfDrawTagLink(packetList,
                    EE_ADDR(sdfDevPrintf(0x7E00, 0x7B20, 0xFFFFF0, 0,
                                                 itfMesDebugFadeFormat, panel->fadeLimit)));
    bounds = itfMesDebug.pmesflow->state->window.mes_offset;
    sdfDrawTagLink(packetList,
                    EE_ADDR(sdfDevPrintf(0x7180, 0x7C40, 0xFFFFF0, 0,
                                                 "OFFSET : %3d,%3d - %3d,%3d",
                                                 bounds[0] >> 4,
                                                 bounds[1] >> 3,
                                                 bounds[2] >> 4,
                                                 bounds[3] >> 3)));
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packetList);
    return 0;
}

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

void ReadytoMessage(instanceMes_tag *pinst) {
    message_tag *mes = &pinst->message;
    frFontHandle_t *chain;
    u32 result;

    if (mes->pfont != NULL) {
#ifdef VERSION_RAIDOU1
        frDestroyFontRequestAfter(mes->pfont);
#else
        mes->pfontPrev = mes->pfont;
        mes->limit = 128;
#endif
        mes->pfont = NULL;
    }
    chain = MessageRequestEx(mes->x, mes->y, mes->color[0], mes->color[1], mes->color[2], mes->color[3],
                             GetMessagePagePtr(mes->table, mes->page), NULL);
    if (pinst->type == 3) {
        if (frCallEventVoiceSub(chain) == 1) {
            frFontCenterGlyphChainAt(0x1000, 0xC60, chain);
        } else {
            frFontCenterGlyphChainAt(0x1000, 0xBF8, chain);
        }
    }
    if (!(pinst->stat & 0x400000)) {
        if ((itfMesMngManager.flags & 1) != 0) {
            RaiseMessageSpeedMax(chain);
        }
    }
    GetLastMessagePara(chain, mes);
#ifdef VERSION_RAIDOU1
    result = frGetMessageResult(3);
#else
    result = frGetMessageResult(0x13);
#endif
    mes->textInterfaceMask = result;
    if ((result & 2) != 0) {
        pinst->stat |= 0x10000;
    } else {
        pinst->stat &= 0xFFFEFFFF;
    }
    if (frGetMessageResult(4) != 0) {
        if (pinst->func_kuti != 0) {
            ((void (*)(void))pinst->func_kuti)();
        }
    }
    if (frGetMessageResult(8) != 0) {
        evtLipsExecFunction(itfGetTextLipsUnitId(), itfGetTextLipsMotionId());
    }
    ChangeZBuffer(chain, pinst->z);
    mes->lines = CountLineMessage(GetLastLinkFontHandle(chain), chain);
    mes->pfont = chain;
}

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

frFontHandle_t *GetFontHandleUnit(u32 *list, s32 count, u32 mask, s32 x, s32 y, s32 depth)
{
    frFontHandle_t *chain = NULL;
    s32 i;

    for (i = 0; i < count; i++, list++) {
        if ((mask & 1) != 0) {
            mask >>= 1;
            continue;
        }
        chain = MessageRequestEx(x, y, 0, 0, 0, 0x80, EE_ADDR_AS(u8 *, *list), chain);
        y += (s16)chain->parentDimensionsOrRenderWord.parentDimensions.cellHeight * 8;
        mask >>= 1;
    }
    if (chain != NULL) {
        ChangeZBuffer(chain, depth);
        ChangeFontAlpha(chain, 0x80);
    }
    return chain;
}

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

void GetLastMessagePara(frFontHandle_t *pfont, message_tag *pmessage)
{
    frTextHandle_t *shade = pfont->lastChild;

    if (shade != NULL) {
        u8 ctx = pfont->glyphCodeOrContext.byteRoles.encodedContextByte;

        pmessage->color[3] = ctx >> 1;
        pmessage->color[0] = shade->renderValueOrSetupOrShade.setupBytes.fontIndex;
        pmessage->color[1] = shade->renderValueOrSetupOrShade.setupBytes.firstOption;
        pmessage->color[2] = shade->renderValueOrSetupOrShade.setupBytes.secondOption;
    }
}

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

void RaiseMessageSpeedMaxAll(frFontHandle_t *pfont) {
    while (pfont != NULL) {
        SwitchOK(pfont);
        pfont = pfont->previous;
    }
}

void OpenMessageWindow(instanceMes_tag *pinst) {
    window_tag *place = &pinst->window;
    message_tag *mes = &pinst->message;
    s32 top;

    if (place->mes == NULL) {
        place->mes = itfPnlRegist2(6, EE_ADDR(itfMesMngManager.windowTexture));
        if (place->spk != NULL) {
            place->mes->textRightX = pinst->speaker.pfont->x + frCallGetStrWidth3(0, pinst->speaker.pfont);
        }
    }
    top = mes->y + place->mes_offset[1];
    itfPnlSetPosition(place->mes, mes->x + place->mes_offset[0], top,
                      mes->x + place->mes_offset[2], mes->y + place->mes_offset[3], pinst->z);
    place->mes->screenY = top;
    itfPnlSetColor(place->mes, place->red, place->green, place->blue, 0);
    pinst->stat = (pinst->stat & ~0x300) | 0x100;
}

void OpenCheckSpeakerWindow(instanceMes_tag *pinst) {
    Speaker_tag *origin = &pinst->speaker;
    window_tag *place = &pinst->window;
    if (origin->pfont != NULL && !(pinst->stat & 0x10000)) {
        if (place->spk == NULL) {
            s32 width = origin->pfont->advance * 16;
            place->spk = itfPnlRegist2(7, EE_ADDR(itfMesMngManager.windowTexture));
            itfPnlSetPosition(place->spk, origin->x - 0x120, origin->y - 0x20, origin->x + width - 0x90, origin->y + 0xD0, pinst->z);
            itfPnlSetColor(place->spk, 0x7F, 0x7F, 0x7F, 0);
        }
        pinst->stat = (pinst->stat & ~0x3000) | 0x1000;
    } else if (place->spk != 0) {
        pinst->stat |= 0x3000;
    }
}

void InitializeSpeaker(Speaker_tag *pspeaker, s32 defaultpos) {
    if (defaultpos != 0) {
        pspeaker->x = 0x3D0;
        pspeaker->y = 0x988;
    }
    pspeaker->pfont = 0;
    pspeaker->num = 0xffff;
}

#ifdef VERSION_RAIDOU1
void InitializeMessage(message_tag *pmessage, s32 defaultpos) {
    if (defaultpos != 0) {
        pmessage->x = 0x440;
        pmessage->y = 0xA78;
    }
    pmessage->pfont = NULL;
    pmessage->phase = 0;
    pmessage->textInterfaceMask = 0;
    pmessage->lines = 0;
    pmessage->page = 0;
    pmessage->maxpage = 0;
    pmessage->color[0] = 0;
    pmessage->color[1] = 0;
    pmessage->color[2] = 0;
    pmessage->color[3] = 0x80;
    pmessage->table = NULL;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeMessage);
#endif

void InitializeSelect(select_tag *pselect)
{
    pselect->x = 0x610;
    pselect->y = 0xC08;
    pselect->pfont = NULL;
    pselect->mask = 0;
    pselect->phase = 0;
    pselect->select = -1;
    pselect->select_imm = -1;
    pselect->lines = 0;
    pselect->widestRowX = 0;
    pselect->widestRowWidth = 0;
    pselect->glyphColorIntensity = 0;
    pselect->numdef = 0;
}

void InitializeWindow(window_tag *pwindow)
{
    pwindow->spk = NULL;
    pwindow->mes = NULL;
    pwindow->overlay = NULL;
    ResetWindowPara(pwindow, 0, 0);
}

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

void InitializeCurCtr(curctr_tag *pcurctr, s32 isdisp) {
    if (isdisp == 0) {
        pcurctr->isdraw = 0;
    }
    pcurctr->ctr1 = 0x80;
    pcurctr->phase = 0;
    pcurctr->ctr0 = 0;
    pcurctr->frame = 0;
    if (isdisp == 0) {
        pcurctr->lf_wait = 0;
        pcurctr->lf_after_wait = 0;
    }
    pcurctr->cut_wait = 0;
    pcurctr->ctr_key = 0;
    pcurctr->unk14 = 1.0f;
    pcurctr->unk18 = 1.0f;
    pcurctr->unk1C = 1.0f;
    pcurctr->unk20 = 1.0f;
}

void ForceSetCurCtr(curctr_tag *pcurctr, s32 phase) {
    pcurctr->phase = phase;
}

void ReleaseAllWindow_itf(instanceMes_tag *pinst) {
    window_tag *place = &pinst->window;

    if (place->spk != NULL) {
        itfPnlRelease(place->spk);
        place->spk = NULL;
    }
    if (place->mes != NULL) {
        itfPnlRelease(place->mes);
        place->mes = NULL;
    }
    if (place->overlay != NULL) {
        itfPnlRelease(place->overlay);
        place->overlay = NULL;
    }
    pinst->stat &= ~0xF00;
}

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

u16 *ConvertZenkakuNumber(s32 buf, u16 *num) {
    s32 digits[10];
    s32 count = 0;
    s32 radix = 10;
    s32 *p = digits;
    s32 i;

    do {
        *p = buf % radix;
        buf = buf / radix;
        count++;
        p++;
    } while (buf > 0 && count < 10);

    for (i = count - 1; i >= 0; i--) {
        *num = (u16)digits[i] * 0x100 + 0x9080;
        num++;
    }
    *num = 0;
    return num;
}

void ResetWindowPara(window_tag *pwin, s32 type, s32 alpha)
{
    pwin->mes_offset[0] = -0x230;
    pwin->mes_offset[1] = -0xC8;
    pwin->mes_offset[2] = 0x19B0;
    pwin->mes_offset[3] = 0x308;
    pwin->red = 0x80;
    pwin->green = 0x80;
    pwin->blue = 0x80;
    pwin->fadeLimit = (alpha > 0) ? alpha : 0x80;
}

void CalcMesFlow_itf(instanceMes_tag *pinst) {
    CalcWindow(pinst);
    CalcMessage(pinst);
    CalcSelect(pinst);
    itfCalcMesFlowItfSub(pinst);
    itfFunc2(pinst);
}

/* NON_MATCHING: 47 of 124 words match.  */
#ifdef NON_MATCHING
/* body: matched in smt3 @ 9401f79e: src/smt3/interface/itfMesManager.c:2096, near, same function 0.67 (words 124/81, calls 0.67, shape 0.70) */
void CalcWindow(instanceMes_tag *pinst) {
    window_tag *place = &pinst->window;
    itfpanel_t *sprite = place->mes;
    itfpanel_t *frame;
    itfpanel_t *overlay;
    u32 mode;

    mode = pinst->stat & 0x300;
    switch (mode) {
    case 0x100:
        sprite->alpha += SDF_FRAME_STEP(0x18);
        if (sprite->alpha >= place->fadeLimit || pinst->type == 3) {
            sprite->alpha = place->fadeLimit;
            pinst->stat = (pinst->stat & ~0x307) | 0x203;
        }
        break;
    case 0x300:
        sprite->alpha -= SDF_FRAME_STEP(8);
        if (sprite->alpha <= 0 || pinst->type == 3) {
            sprite->alpha = 0;
            pinst->stat &= ~0x300;
        }
        break;
    }
    frame = place->spk;
    mode = pinst->stat & 0x3000;
    switch (mode) {
    case 0x1000:
        frame->alpha += SDF_FRAME_STEP(0x20);
        if (frame->alpha >= 0xC8) {
            frame->alpha = 0xC8;
            pinst->stat = (pinst->stat & ~0x3000) | 0x2000;
        }
        break;
    case 0x3000:
        frame->alpha -= SDF_FRAME_STEP(0x20);
        if (frame->alpha <= 0) {
            frame->alpha = 0;
            pinst->stat &= ~0x3000;
            itfPnlRelease(frame);
            place->spk = NULL;
        }
        break;
    }
    overlay = place->overlay;
    mode = pinst->stat & 0xC00;
    switch (mode) {
    case 0x400:
        overlay->alpha += SDF_FRAME_STEP(0x18);
        if (overlay->alpha >= place->fadeLimit) {
            overlay->alpha = place->fadeLimit;
            pinst->stat = (pinst->stat & ~0xC07) | 0x803;
        }
        break;
    case 0xC00:
        overlay->alpha -= SDF_FRAME_STEP(8);
        if (overlay->alpha <= 0) {
            overlay->alpha = 0;
            pinst->stat &= ~0xC00;
            itfPnlRelease(overlay);
            place->overlay = NULL;
        }
        break;
    }
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", CalcWindow);
#endif

void itfCalcMessageSub(instanceMes_tag *pinst) {
    pinst->curctr[0].isdraw = 1;
}

/* NON_MATCHING: 45 of 163 words match.  */
#ifdef NON_MATCHING
void CalcMessage(instanceMes_tag *pinst) {
    message_tag *mes = &pinst->message;
    u32 flags;

    if (mes->phase != 1) {
        return;
    }
    flags = pinst->stat;
    if ((flags & 7) == 4) {
        if (pinst->curctr[0].ctr_key < 100) {
            pinst->curctr[0].ctr_key++;
        }
        if (pinst->curctr[0].lf_wait < 14) {
            pinst->curctr[0].lf_wait++;
        }
        if (flags & 0x800000) {
            mes->phase = -1;
            return;
        }
        if (pinst->curctr[0].lf_wait < 8) {
            return;
        }
        if ((flags & 0x100000) == 0) {
            itfCalcMessageSub(pinst);
        }
        if (pinst->curctr[0].lf_after_wait == 0 && mes->textInterfaceMask != 0) {
            if ((dds3PadButtonState.primaryCancel >= 0 && dds3PadButtonState.confirm >= 0)
                || pinst->curctr[0].lf_wait < 14) {
                if (dds3PadButtonState.primaryTriangle == 0 || pinst->curctr[0].ctr_key < 5 || (itfData3 & 1)) {
                    return;
                }
            }
            if (mes->textInterfaceMask != 0) {
                p2sdPlaySE(9, 0x7F, 0x3F);
            }
            pinst->curctr[0].lf_after_wait = 1;
            return;
        }
        pinst->curctr[0].lf_after_wait++;
        if (pinst->curctr[0].lf_after_wait < 6 && mes->textInterfaceMask != 0) {
            return;
        }
        InitializeCurCtr(&pinst->curctr[0], 0);
        mes->page++;
        if (mes->page >= mes->maxpage) {
            mes->phase = -1;
            return;
        }
        CommitVarTable((s32 *)pinst->variable.pptr);
        if (pinst->pext != NULL) {
            frSetExtDic(pinst->pext);
        }
        ReadytoMessage(pinst);
        OpenCheckSpeakerWindow(pinst);
        pinst->stat = (pinst->stat & ~7) | 3;
    } else {
        if (pinst->curctr[0].cut_wait < 8) {
            pinst->curctr[0].cut_wait++;
            if (pinst->curctr[0].cut_wait < 8) {
                return;
            }
        }
        if (dds3PadButtonState.primaryCancel < 0 || dds3PadButtonState.confirm < 0
            || (dds3PadButtonState.primaryTriangle != 0 && (itfData3 & 1) == 0)) {
            if ((pinst->stat & 0x1400000) == 0) {
                RaiseMessageSpeedMaxAll(mes->pfont);
                if (pinst->speaker.pfont != NULL) {
                    SwitchOK(pinst->speaker.pfont);
                }
                flags = pinst->stat;
            }
        }
        pinst->stat = flags & ~0x1000000;
    }
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", CalcMessage);
#endif

/* NON_MATCHING: 185 of 187 words match.  */
#ifdef NON_MATCHING
void CalcSelect(instanceMes_tag *panel) {
    select_tag *selection = &panel->select;
    message_tag *entry = &panel->message;
    u32 flags = panel->stat;
    s32 top;
    s32 bottom;
    s32 entryY;

    switch (selection->phase) {
    case 1:
        if (panel->type == 3) {
            panel->stat = (flags & ~0x38) | 0x18;
            selection->selState[1] = 4;
            selection->phase = 2;
            selection->selState[0] = 0;
            break;
        }
        entryY = entry->y;
        bottom = entryY + entry->lines * (25 << 3);
        top = selection->y - ((selection->lines * 25U - 25U) << 3);
        if (entryY == 0xA78 && panel->window.mes != NULL) {
            panel->window.mes->scrollSpan = ((bottom - top) / 32) * 32 + 32;
        }
        if (entry->pfont != NULL && top < bottom) {
            itfCallChangeFontPosition(panel, -32);
            return;
        }
        if ((flags & 0xC00) != 0x400) {
            if (entry->pfont != NULL) {
                ChangeFontColorAll(entry->pfont, 3);
            }
            selection->phase = 12;
        }
        break;
    case 12:
        if (selection->selState[0] != 0) {
            selection->selState[0]--;
        } else {
            selection->phase = 2;
            panel->stat = (flags & ~0x38) | 0x18;
        }
        break;
    case 2:
        if ((panel->stat & 0x38) == 0x20) {
            if (selection->selState[1] != 0) {
                selection->selState[1]--;
            }
            if (SelectItem(panel) == 1) {
                selection->pfont = UnchainFontHandleUnit(selection->pfont, selection->select, selection->lines);
                if ((flags & 0xC00) == 0x800) {
                    panel->stat |= 0xC00;
                }
                selection->select = GetMaskedIndex(selection->select, selection->mask);
                selection->numdef = 0;
                ForceSetCurCtr(&panel->curctr[1], -1);
                panel->stat = (panel->stat & ~0x38) | 0x28;
                selection->glyphColorIntensity = 0x80;
                selection->phase = 3;
            }
        }
        break;
    case 3:
        selection->glyphColorIntensity -= 16;
        if (selection->glyphColorIntensity <= 0) {
            selection->glyphColorIntensity = 0;
            selection->phase = 4;
            InitializeCurCtr(&panel->curctr[1], 0);
        }
        ChangeFontAlpha(selection->pfont, selection->glyphColorIntensity);
        return;
    case 4:
        if (entry->pfont != NULL && entry->y < 0xA78) {
            itfCallChangeFontPosition(panel, 32);
            return;
        }
        selection->phase = -1;
        break;
    }
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", CalcSelect);
#endif

/* body: matched in smt3 @ 9401f79eb: src/smt3/interface/itfMesManager.c:2256, match, same function 1.00 (words 54/54, calls 1.00, shape 1.00) */
void itfCallChangeFontPosition(instanceMes_tag *pinst, s32 y)
{
    Speaker_tag *speaker = &pinst->speaker;
    message_tag *message = &pinst->message;
    window_tag *window = &pinst->window;

    message->y += y;
    ChangeFontPosition(message->pfont, 0, y);
    if (window->mes != NULL) {
        itfPnlSetPositionOffset(window->mes, 0, y, 0, 0, 0);
    }
    if (window->spk != NULL) {
        itfPnlSetPositionOffset(window->spk, 0, y, 0, y, 0);
    }
    if (speaker->pfont != NULL) {
        speaker->y += y;
        ChangeFontPosition(speaker->pfont, 0, y);
    }
}

s32 SelectItem(instanceMes_tag *pinst) {
    select_tag *sel = &pinst->select;
    s32 dir = 0;
    s32 index;
    if (dds3PadButtonState.prev & 2) {
        if (sel->select != 0) {
            dir = -1;
        } else if (dds3PadButtonState.prev < 0) {
            dir = -1;
        }
    } else if (dds3PadButtonState.next & 2) {
        if (sel->select != sel->lines - 1 || dds3PadButtonState.next < 0) {
            dir = 1;
        }
    }
    if (dir != 0) {
        SelectItemMove(sel, dir);
        InitializeCurCtr(&pinst->curctr[1], 1);
    }
    if (sel->selState[1] != 0) {
        return 0;
    }
    if (dds3PadButtonState.primaryCancel < 0) {
        p2sdPlaySE(0, 0x7F, 0x3F);
        return 1;
    }
#ifdef VERSION_RAIDOU2
    if ((pinst->stat & 0x2000000) && dds3PadButtonState.confirm < 0) {
        return 0;
    }
#endif
    if (sel->numdef > 0 && (index = SelectItemDefKey(sel)) >= 0) {
        p2sdPlaySE(0, 0x7F, 0x3F);
        if (index != sel->select) {
            ChangeFontColor(sel->pfont, sel->select, sel->lines, 0);
            ChangeFontColor(sel->pfont, index, sel->lines, 1);
            sel->select = index;
            sel->select_imm = index;
        }
        return 1;
    }
    return 0;
}

void SelectItemMove(select_tag *psel, s32 direct) {
    s32 cur = psel->select;
    u16 maxv;
    ChangeFontColor(psel->pfont, cur, psel->lines, 0);
    if (direct < 0) {
        cur = cur - 1;
        if (cur < 0) {
            cur = psel->lines - 1;
        }
        maxv = psel->lines;
    } else {
        cur = cur + 1;
        if (cur >= psel->lines) {
            cur = 0;
        }
        maxv = psel->lines;
    }
    ChangeFontColor(psel->pfont, cur, (s16)maxv, 1);
    psel->select = cur;
    psel->select_imm = cur;
    p2sdPlaySE(4, 0x7F, 0x3F);
}

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

/* NON_MATCHING: 45 of 200 words match.  */
#ifdef NON_MATCHING
void itfCalcMesFlowItfSub(instanceMes_tag *pinst) {
    itfMesMngFlowKeyTable_t table;
    curctr_tag *pcurctr = &pinst->curctr[0];
    s32 phase;
    s32 frame;
    s32 elapsed;
    s32 span;

    table = itfCalcMesFlowItfSubTable;
    if (pcurctr->isdraw != 0) {
        phase = pcurctr->phase;
        if (phase >= 0) {
            frame = pcurctr->frame;
            elapsed = frame - table.key[phase].frame;
            span = table.key[phase + 1].frame - table.key[phase].frame;
            pcurctr->unk14 = table.key[phase].unk14
                + (table.key[phase + 1].unk14 - table.key[phase].unk14) * elapsed / span;
            pcurctr->unk18 = table.key[phase].unk18
                + (table.key[phase + 1].unk18 - table.key[phase].unk18) * elapsed / span;
            pcurctr->unk1C = table.key[phase].unk1C
                + (table.key[phase + 1].unk1C - table.key[phase].unk1C) * elapsed / span;
            pcurctr->unk20 = table.key[phase].unk20
                + (table.key[phase + 1].unk20 - table.key[phase].unk20) * elapsed / span;
            pcurctr->ctr0 = table.key[phase].ctr0
                + (table.key[phase + 1].ctr0 - table.key[phase].ctr0) * elapsed / span;
            pcurctr->ctr1 = table.key[phase].ctr1
                + (table.key[phase + 1].ctr1 - table.key[phase].ctr1) * elapsed / span;
            if (frame >= table.key[phase + 1].frame) {
                pcurctr->phase = phase + 1;
                if ((u16)(phase + 1) >= 4) {
                    InitializeCurCtr(pcurctr, 1);
                    pcurctr->unk14 = table.key[0].unk14;
                    pcurctr->ctr0 = table.key[0].ctr0;
                    pcurctr->unk18 = table.key[0].unk18;
                    pcurctr->ctr1 = table.key[0].ctr1;
                    pcurctr->unk1C = table.key[0].unk1C;
                    pcurctr->unk20 = table.key[0].unk20;
                }
            }
            pcurctr->frame++;
        } else {
            if (pcurctr->ctr0 > 0) {
                pcurctr->ctr0 -= 8;
            }
            if (pcurctr->ctr0 < 0) {
                pcurctr->ctr0 = 0;
            }
            if (pcurctr->ctr1 > 0) {
                pcurctr->ctr1 -= 8;
            }
            if (pcurctr->ctr1 < 0) {
                pcurctr->ctr1 = 0;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfCalcMesFlowItfSub);
#endif

/* NON_MATCHING: 56 of 123 words match.  */
#ifdef NON_MATCHING
void itfFunc2(instanceMes_tag *pinst) {
    itfMesMngCurKeyTable_t table;
    curctr_tag *pcurctr = &pinst->curctr[1];
    s32 phase;
    s32 frame;
    s32 elapsed;
    s32 span;

    table = itfFunc2Table;
    if (pcurctr->isdraw != 0) {
        phase = pcurctr->phase;
        if (phase >= 0) {
            frame = pcurctr->frame;
            elapsed = frame - table.key[phase].frame;
            span = table.key[phase + 1].frame - table.key[phase].frame;
            pcurctr->unk18 = table.key[phase].unk18
                + (table.key[phase + 1].unk18 - table.key[phase].unk18) * elapsed / span;
            pcurctr->unk14 = table.key[phase].unk14
                + (table.key[phase + 1].unk14 - table.key[phase].unk14) * elapsed / span;
            pcurctr->ctr0 = table.key[phase].ctr0
                + (table.key[phase + 1].ctr0 - table.key[phase].ctr0) * elapsed / span;
            if (frame >= table.key[phase + 1].frame) {
                pcurctr->phase = phase + 1;
                if ((u16)(phase + 1) >= 2) {
                    InitializeCurCtr(pcurctr, 1);
                    return;
                }
            }
            pcurctr->frame++;
        } else {
            if (pcurctr->ctr0 > 0) {
                pcurctr->ctr0 -= 8;
            }
            if (pcurctr->ctr0 < 0) {
                pcurctr->ctr0 = 0;
            }
            if (pcurctr->ctr1 > 0) {
                pcurctr->ctr1 -= 8;
            }
            if (pcurctr->ctr1 < 0) {
                pcurctr->ctr1 = 0;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfFunc2);
#endif

INCLUDE_ASM(const s32, "interface/itfMesManager", itfLeaf13);

void FadeOutWindow(void) {
    itfData.active = 0;
    itfData.timer = 0;
}

void itfCallGetRandFloatA(void) {
    s32 range[2];
    itfMesShakeGain_t gain = itfCallGetRandFloatAData[0];
    s32 amplitude;
    f32 kick;
    s32 i;

    if (itfData.active == 0) {
        itfData.dist[0] = 0;
        itfData.dist[1] = 0;
        return;
    }
    if (itfData.timer == 0) {
        itfData.active = 0;
        return;
    }
    range[0] = itfData.power[0] * 0.1f;
    range[1] = itfData.power[1] * 0.1f;
    for (i = 0; i < 2; i++) {
        if (range[i] != 0.0f) {
            amplitude = dds3GetRandFloatA(0) * range[i] * 0.5f + range[i] * 0.5f;
            if (range[i] * gain.axis[i] <= fabsf(itfData.dist[i])) {
                if (itfData.dist[i] <= 0.0f) {
                    itfData.dist[i] = (f32)itfData.dist[i] + amplitude;
                } else {
                    itfData.dist[i] = (f32)itfData.dist[i] - amplitude;
                }
            } else {
                kick = dds3GetRandFloatA(0) - 0.5f;
                itfData.dist[i] = itfData.dist[i] + (amplitude * kick + amplitude * kick);
            }
            if (itfData.dist[i] < -range[i]) {
                itfData.dist[i] = -range[i];
            }
            if (range[i] < itfData.dist[i]) {
                itfData.dist[i] = range[i];
            }
        }
    }
    if (itfData.timer != 0) {
        u32 divisor = itfData.timer + 5;

        itfData.power[0] -= itfData.power[0] / divisor;
        itfData.power[1] -= itfData.power[1] / divisor;
    }
    if (itfData.timer - 1 < 0xFFFE) {
        itfData.timer--;
    }
}

#ifdef VERSION_RAIDOU1
s32 DrawMesFlow(instanceMes_tag *pinst) {
    u32 stat = pinst->stat;
    frFontHandle_t *messageFont;
    frFontHandle_t *selectFont;

    itfCallGetRandFloatA();
    if (pinst->select.pfont != NULL) {
        itfData.dist[0] = 0;
        itfData.dist[1] = 0;
    }
    if (pinst->message.pfont != NULL) {
        ChangeFontPosition(pinst->message.pfont, itfData.dist[0], itfData.dist[1]);
    }
    if (pinst->speaker.pfont != NULL) {
        ChangeFontPosition(pinst->speaker.pfont, itfData.dist[0], itfData.dist[1]);
    }
    itfMesMngManager.flags &= ~2;
    DrawWindow(pinst);
    {
        frFontHandle_t *speakerFont = pinst->speaker.pfont;

        if ((stat & 0x10000) == 0 && speakerFont != NULL) {
            frDrawFont(speakerFont);
        }
    }
    if (pinst->message.pfont != NULL) {
        ChangeFontPosition(pinst->message.pfont, -itfData.dist[0], -itfData.dist[1]);
    }
    if (pinst->speaker.pfont != NULL) {
        ChangeFontPosition(pinst->speaker.pfont, -itfData.dist[0], -itfData.dist[1]);
    }
    messageFont = pinst->message.pfont;
    if ((stat & 0x20000) == 0 && (stat & 7) >= 3 && frDrawFont(messageFont) > 0) {
        u32 cur = pinst->stat;

        if ((cur & 7) != 4) {
            pinst->curctr[0].ctr_key = 0;
            pinst->curctr[0].lf_wait = 0;
            pinst->curctr[0].lf_after_wait = 0;
            pinst->stat = (cur & ~7) | 4;
            pinst->curctr[0].cut_wait = 0;
        }
    }
    selectFont = pinst->select.pfont;
    if ((stat & 0x40000) == 0) {
        u32 selStat = stat & 0x38;

        if (selStat >= 0x18 && frDrawFontEx(selectFont, 1) > 0 && selStat == 0x18) {
            select_tag *select = &pinst->select;

            if (select->select == -1) {
                select->select = 0;
                select->select_imm = 0;
            }
            ChangeFontColor(select->pfont, select->select, select->lines, 1);
            pinst->stat = (pinst->stat & ~0x38) | 0x20;
            pinst->curctr[1].isdraw = 2;
        }
    }
    if (pinst->curctr[0].isdraw & 1) {
        itfCallGrAllocPacketBuff2(pinst);
    }
    if (pinst->curctr[1].isdraw & 2) {
        if (pinst->type == 3) {
            return itfLeaf14(pinst);
        }
        return itfCallGrAllocPacketBuff(pinst);
    }
    return 3;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMesFlow);
#endif

void DrawWindow(instanceMes_tag *pinst) {
    window_tag *place;

    if (pinst->stat & 0x80000) {
        return;
    }
    place = &pinst->window;
    if ((pinst->stat & 0x300) >= 0x100) {
        if (pinst->type != 3) {
            itfPnlDraw(place->mes, &itfData, &sdfGrOtTable[pinst->otindex]);
        }
        itfMesMngManager.flags |= 2;
        if (place->spk != NULL) {
            itfPnlDraw(place->spk, &itfData, &sdfGrOtTable[pinst->otindex]);
        }
    }
    if (place->overlay != NULL) {
        itfPnlDraw(place->overlay, &itfData, &sdfGrOtTable[pinst->otindex]);
    }
    if (itfMesDebug.pmesflow != NULL && itfMesDebug.pmesflow->state == pinst && place != NULL) {
        DebugDrawWindowLine(place->mes);
    }
}

/* NON_MATCHING: 65 of 274 words match.  */
#ifdef NON_MATCHING
s32 itfCallGrAllocPacketBuff(instanceMes_tag *pinst) {
    itfMesMngCursorUvTable_t uv = itfCallGrAllocPacketBuffTable;
    itfMesMngCursorSizeTable_t size = itfCallGrAllocPacketBuffTable2;
    itfMesMngCursorColor_t color = itfCallGrAllocPacketBuffTable3;
    s32 vertex[4];
    frFontPosition_t position;
    select_tag *psel = &pinst->select;
    s32 index = psel->select_imm;
    curctr_tag *pcurctr = &pinst->curctr[1];
    sdfDrawTag_t *tag;
    s32 width;
    s32 x;
    s32 y;
    s32 cx;
    s32 cy;
    s32 halfWidth;
    s32 halfHeight;
    sdfGrOt_t *otp;

    if (index == -1) {
        return -1;
    }
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    width = size.size[1][0] * 2;
    frFontGetGlyphPositionAtIndex(&position, index, psel->pfont);
    psel->widestRowWidth = width;
    psel->widestRowX = position.x;
    x = position.x - 304;
    y = position.y - 16;

    color.rgba[3] = pcurctr->ctr1;
    vertex[0] = x;
    vertex[1] = y;
    vertex[2] = x + size.size[0][0];
    vertex[3] = y + size.size[0][1];
    itfMiscDrawSpritePrim(vertex, uv.uv[0], &color, pinst->z, itfMesMngManager.windowTexture, 0, tag);

    color.rgba[3] = pcurctr->ctr1;
    vertex[0] = x + size.size[0][0];
    vertex[1] = y;
    vertex[2] = x + width;
    vertex[3] = y + size.size[1][1];
    itfMiscDrawSpritePrim(vertex, uv.uv[1], &color, pinst->z, itfMesMngManager.windowTexture, 0, tag);

    color.rgba[3] = pcurctr->ctr1;
    vertex[0] = x + size.size[0][0];
    vertex[1] = y + size.size[1][1];
    vertex[2] = x + width;
    vertex[3] = y + size.size[1][1] + size.size[1][1];
    itfMiscDrawSpritePrim(vertex, uv.uv[2], &color, pinst->z, itfMesMngManager.windowTexture, 0, tag);

    color.rgba[3] = pcurctr->ctr1;
    vertex[0] = position.x - 256;
    vertex[1] = y + 40;
    vertex[2] = position.x - 256 + size.size[2][0];
    vertex[3] = y + 40 + size.size[2][1];
    itfMiscDrawSpritePrim(vertex, uv.uv[3], &color, pinst->z, itfMesMngManager.windowTexture, 0, tag);

    cx = x + size.size[3][0] / 2;
    halfWidth = size.size[3][0] * pcurctr->unk14 * 0.5f;
    halfHeight = size.size[3][1] * pcurctr->unk18 * 0.5f;
    color.rgba[3] = pcurctr->ctr0;
    cy = y + size.size[3][1] / 2;
    vertex[0] = cx - halfWidth - 96;
    vertex[1] = cy - halfHeight + 48;
    vertex[2] = cx + halfWidth - 96;
    vertex[3] = cy + halfHeight + 48;
    itfMiscDrawAlphaPrim(tag, 1, 0);
    itfMiscDrawSpritePrim(vertex, uv.uv[4], &color, pinst->z, itfMesMngManager.windowTexture, 0, tag);
    itfMiscDrawAlphaPrim(tag, 0, 0);
    otp = &sdfGrOtTable[pinst->otindex];
    otp->regFunc(otp, tag);
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff);
#endif

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0018B5C0.c:1, match */
s32 itfLeaf14(instanceMes_tag *pinst) {
    return itfCallGrAllocPacketBuff(pinst);
}

/* NON_MATCHING: 165 of 310 words match.  */
#ifdef NON_MATCHING
/* Page-wait cursor sheet for itfCallGrAllocPacketBuff2: three uv rects in
 * 1/16 texels, drawn at fixed rects below the message panel. */
typedef struct itfMesMngWaitUvTable_t {
    s32 uv[3][4];
} itfMesMngWaitUvTable_t;

typedef struct itfMesMngWaitSize_t {
    s32 size[3][2];
} itfMesMngWaitSize_t;

typedef struct itfMesMngWaitRect_t {
    s32 rect[3][4];
} itfMesMngWaitRect_t;

extern itfMesMngWaitUvTable_t itfCallGrAllocPacketBuff2Table;
extern itfMesMngCursorColor_t itfCallGrAllocPacketBuff2Table2;

void itfCallGrAllocPacketBuff2(instanceMes_tag *pinst) {
    itfMesMngWaitUvTable_t uv = itfCallGrAllocPacketBuff2Table;
    itfMesMngWaitSize_t size = { {
        { (uv.uv[0][2] - uv.uv[0][0]) >> 4, (uv.uv[0][3] - uv.uv[0][1]) >> 4 },
        { (uv.uv[1][2] - uv.uv[2][0]) >> 4, (uv.uv[1][3] - uv.uv[2][1]) >> 4 },
        { (uv.uv[2][2] - uv.uv[2][0]) >> 4, (uv.uv[2][3] - uv.uv[2][1]) >> 4 },
    } };
    itfpanel_t *mes = pinst->window.mes;
    s32 top = mes != NULL ? mes->top : 0;
    itfMesMngWaitRect_t vertex = { {
        { 435 << 4, top + (76 << 3), (size.size[0][0] + 435) << 4, top + ((size.size[0][1] + 76) << 3) },
        { 433 << 4, top + (70 << 3), (size.size[1][0] + 433) << 4, top + ((size.size[1][1] + 70) << 3) },
        { 433 << 4, top + (81 << 3), (size.size[2][0] + 433) << 4, top + ((size.size[2][1] + 81) << 3) },
    } };
    itfMesMngCursorColor_t color = itfCallGrAllocPacketBuff2Table2;
    s32 rect[4];
    curctr_tag *pcurctr = &pinst->curctr[0];
    sdfDrawTag_t *tag;
    s32 fade;
    s32 alpha;
    s32 cx;
    s32 cy;
    s32 halfWidth;
    s32 halfHeight;
    sdfGrOt_t *otp;

    if (mes == NULL) {
        return;
    }
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    fade = pinst->curctr[0].lf_after_wait * 32 < 129 ? pinst->curctr[0].lf_after_wait * 32 : 128;
    alpha = 128 - fade;
    if (alpha < 0) {
        alpha = 0;
    }
    color.rgba[3] = alpha;
    itfMiscDrawSpritePrim(vertex.rect[0], uv.uv[0], &color, pinst->z, itfMesMngManager.windowTexture, 0, tag);

    halfWidth = (f32)(size.size[1][0] << 4) * pcurctr->unk14 * 0.5f;
    halfHeight = (f32)(size.size[1][1] << 3) * pcurctr->unk18 * 0.5f;
    alpha = pcurctr->ctr0 - fade;
    if (alpha < 0) {
        alpha = 0;
    }
    color.rgba[3] = alpha;
    cx = vertex.rect[1][0] + (size.size[1][0] << 3);
    cy = vertex.rect[1][1] + (size.size[1][1] << 2);
    rect[0] = cx - halfWidth;
    rect[1] = cy - halfHeight;
    rect[2] = cx + halfWidth;
    rect[3] = cy + halfHeight;
    itfMiscDrawAlphaPrim(tag, 1, 0);
    itfMiscDrawSpritePrim(rect, uv.uv[1], &color, pinst->z, itfMesMngManager.windowTexture, 0, tag);
    itfMiscDrawAlphaPrim(tag, 0, 0);

    halfWidth = (f32)(size.size[2][0] << 4) * pcurctr->unk1C * 0.5f;
    halfHeight = (f32)(size.size[2][1] << 3) * pcurctr->unk20 * 0.5f;
    alpha = pcurctr->ctr1 - fade;
    if (alpha < 0) {
        alpha = 0;
    }
    color.rgba[3] = alpha;
    cx = vertex.rect[2][0] + (size.size[2][0] << 3);
    cy = vertex.rect[2][1] + (size.size[2][1] << 2);
    rect[0] = cx - halfWidth;
    rect[1] = cy - halfHeight;
    rect[2] = cx + halfWidth;
    rect[3] = cy + halfHeight;
    itfMiscDrawAlphaPrim(tag, 1, 0);
    itfMiscDrawSpritePrim(rect, uv.uv[2], &color, pinst->z, itfMesMngManager.windowTexture, 0, tag);
    itfMiscDrawAlphaPrim(tag, 0, 0);
    otp = &sdfGrOtTable[pinst->otindex];
    otp->regFunc(otp, tag);
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff2);
#endif

s32 CalcMsgMng_Process(dds3ProcessID_t *id) {
    itfMesLink_t *node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        CalcMesFlow(node->state);
        node = node->next;
    }
    return 0;
}

#ifdef VERSION_RAIDOU1
/* body: matched in smt3 @ 9401f79e: src/smt3/interface/itfMesManager.c:2592, match, same function 1.00 (words 28/28, calls 1.00, shape 1.00) */
s32 DrawMsgMng_Process(dds3ProcessID_t *id) {
    itfMesLink_t *node;

    if (dds3NullDrawProc() != 0) {
        return 0;
    }
    node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        DrawMesFlow(node->state);
        node = node->next;
    }
    itfMesMngManager.ctr++;
    return 0;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMsgMng_Process);
#endif

void DrawMsgMng_Shutdown(dds3ProcessID_t *id) {
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

void DebugDrawWindowLine(itfpanel_t *meswin) {
    sdfIVector2_t vertices[4] = {
        {meswin->left, meswin->top},
        {meswin->right, meswin->top},
        {meswin->right, meswin->bottom},
        {meswin->left, meswin->bottom}
    };
    sdfDrawTag_t *list;

    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    itfMiscDrawLineStripPrim(vertices, &DebugDrawWindowLineData, itfMesDebugLineVertIndex, itfMesDebugLineColorIndex, 5, 0xFFFFFF, list);
    dds3Data10.regFunc(&dds3Data10, list);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/interface/itfMesManager.c:2636, near, same function 1.00 (words 100/100, calls 1.00, shape 1.00) */
void DebugMoveWinVert(window_tag *pwin, s32 flag) {
    itfpanel_t *sprite = pwin->mes;
    s32 *bounds;
    s32 dx;
    s32 dy;

    if (sprite != NULL) {
        bounds = pwin->mes_offset;
        if (dds3PadButtonState.coarseDown & 2) {
            dx = -16;
        } else {
            dx = ((u8)dds3PadButtonState.coarseUp << 3) & 0x10;
        }
        if (dds3PadButtonState.secondaryUp & 2) {
            dy = -8;
        } else {
            dy = ((u8)dds3PadButtonState.secondaryDown << 2) & 8;
        }
        if (dds3PadButtonState.secondaryCancel != 0) {
            dx *= 8;
            dy *= 8;
        }
        if (dx != 0 || dy != 0) {
            switch (flag) {
            case 0:
                itfPnlSetPositionOffset(sprite, dx, dy, 0, 0, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                break;
            case 1:
                itfPnlSetPositionOffset(sprite, 0, 0, dx, dy, 0);
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            case 2:
                itfPnlSetPositionOffset(sprite, dx, dy, dx, dy, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            }
        }
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/interface/itfMesManager.c:2683, near, same function 1.00 (words 36/36, calls 1.00, shape 1.00) */
void DebugMoveWinAlpha(window_tag *pwin) {
    s32 step;

    if (dds3PadButtonState.coarseDown & 2) {
        step = -1;
    } else {
        step = (dds3PadButtonState.coarseUp & 2) > 0;
    }
    if (dds3PadButtonState.secondaryUp & 2) {
        step = -1;
    } else if (dds3PadButtonState.secondaryDown & 2) {
        step = 1;
    }
    if (dds3PadButtonState.secondaryCancel != 0) {
        step *= 10;
    }
    if (step != 0) {
        s32 index = (pwin->fadeLimit + step) & 0xFF;

        pwin->fadeLimit = index;
        if (pwin->mes != NULL) {
            pwin->mes->alpha = index;
        }
    }
}

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
    itfMiscDrawSpritePrim(positions, uv, &color, 0, texture, 0, command);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfCallGrAllocPacketBuff3);

INCLUDE_ASM(const s32, "interface/itfMesManager", InputPasswdDraw);

s32 itfInputPasswd(void) {
    s32 window = scrGetMessageHandle();
    instanceMes_tag *state;
    s32 entry;

    if (window < 0) {
        return 1;
    }
    state = pinst[window].mes;
    entry = scrGetIntPara(0);
    if (state->message.phase == 0) {
        InputPasswdDrawWork = 0;
        InputPasswdDrawWork2 = 0;
        itfCallGrAllocPacketBuff3Work = 0;
        itfInputPasswdWork = 0;
        itfCallGrAllocPacketBuff3Work2 = 0;
        itfMesMngMarkBitStat(window, 0x800000);
        itfMesMngMarkBitStat(window, 0x100000);
        if (itfMesMngRequestMessage_itf(window, entry, 0) == 0) {
            return 1;
        }
    } else {
        InputPasswdDraw();
        if (itfInputPasswdWork == 0 && dds3PadButtonState.primaryCancel < 0) {
            itfInputPasswdWork = 1;
            p2sdPlaySE(0, 0x7F, 0x3F);
        }
        if (itfInputPasswdWork != 0 && itfCallGrAllocPacketBuff3Work == 0) {
            itfMesMngClearBitStat(window, 0x800000);
            itfMesMngClearBitStat(window, 0x100000);
            itfMesMngSetPhase(window, 0);
            itfMesMngRequestMessageRelease(window, 0);
            scrSetIntReturnValue(InputPasswdDrawWork);
            return 1;
        }
    }
    return 0;
}

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

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesSelectionMarkers);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesMarkerEdit);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesMarkerSelect);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesDebugItemFormat);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesMarkerNone);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesDebugFadeFormat);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfCallGetRandFloatAData);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesDebugLineVertIndex);

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMesDebugLineColorIndex);

