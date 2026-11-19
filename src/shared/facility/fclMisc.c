#include "common.h"
#include "scr.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "sdfMemory.h"
#include "evtCommand.h"
#include "scrTraceCode.h"
#include "evtStage.h"
#include "dds3WorldBasic.h"
#include "itfMesManager.h"
#include "dds3KernelCore.h"
#include "dds3SceneBasic.h"
#include "fclMisc.h"
#include "dds3KernelDraw.h"
#include "effEvent.h"

extern u32 dds3GetRandA(void *name);
void fclClearFlag(s8 index);
extern s8 gSelMsgPos;
extern s8 gMsgInvFlag;
extern s32 gMsgID;
extern s32 gSelMsgNo;
extern s32 fclWorkValue[16];

typedef struct
{
    u8 unk_00000[0x1928];
    u8 item[0xA0];
} fclGlobalWorkItemInventory_t;

extern fclGlobalWorkItemInventory_t *fclGlobalWorkItems FAKEMATCH_ASM_NAME("DDS3_GBWK");
extern s32 fclChkMessage(void);
extern void fclScriptForcedEnd(dds3ProcessID_t *PID);
extern s8 gMsgFlag;
extern void shuffleIndices(s8 *order, s32 count, s32 rounds);
extern s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

void fclLoadStage(s32 MajorNo, s32 MinorNo);
void fclScriptForcedEnd(dds3ProcessID_t *PID);
s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

typedef struct
{
    /* 0x00 */ u8 count;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 item[0xA0];
} fclItemList_t;

void shuffleIndices(s8 *order, s32 count, s32 rounds);
extern void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha);
s8 fclInitMessage(void *pData);
extern void itfMesMngRequestMessage(s32 msg, s32 id, s32 flag);
s32 fclStartMessage(s32 MsgNo);
s32 fclDestroyMessage(void);
s32 fclChkMessage(void);
void fclUpdateMessage(void);
extern void itfMesMngSetOffsetY1(s32 msgid, s32 y1);
void fclSetMessagePos(s32 x, s32 y);

/* Sparse world-node identities recovered from the paired operation tables and
 * concrete creators. Unlisted values stay unnamed; COUNT remains the full
 * directory extent rather than an enum-completeness claim. */
enum {
    EFF_WORLD_KIND_WORLD_INFO = 0,
    EFF_WORLD_KIND_EVENT_TABLE = 1,
    EFF_WORLD_KIND_WORLD_INDEX_OWNER = 2,
    EFF_WORLD_KIND_SLOT_RING = 3,
    EFF_WORLD_KIND_CAMERA = 4,
    EFF_WORLD_KIND_FOLLOW_MODEL = 5,
    EFF_WORLD_KIND_EFFECT_TRANSFORM = 6,
    EFF_WORLD_KIND_EFFECT_OBJECT = 7,
    EFF_WORLD_KIND_RESOURCE_OWNER = 8,
    EFF_WORLD_KIND_LIGHT = 9,
    EFF_WORLD_KIND_SCRIPT_OBJECT = 10,
    EFF_WORLD_KIND_ACTION_B = 11,
    EFF_WORLD_KIND_ACTION_D = 13,
    EFF_WORLD_KIND_ACTION_10 = 16,
    EFF_WORLD_KIND_TRANSFORM_SOURCE = 17,
    EFF_WORLD_KIND_COUNT = 18
};

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#include "include_asm.h"
#include "kernel.h"
#include "draw3d.h"
#include "basic.h"
#include "evt_world.h"
#include "code_r1_001CC1D0.h"

typedef s32 EvtMessageWindowPageValueResult;

#define EVT_MESSAGE_WINDOW_PAGE_VALUE_NO_WINDOW 0

#define EVT_MESSAGE_WINDOW_PAGE_VALUE_UPDATED 1

struct sdfTexHandle_t;

struct MenuTerminalWork;

#define EVT_ACTIVE_ENTRY_LIMIT 0xC0

#define EVT_DISPLAY_VALUE_COUNT 0x10

#define EVT_LAST_DISPLAY_VALUE 0xF

#define EVT_SCRIPT_PROCESS_PRIORITY 0x7D0

#define DSP_WINDOW_ENTRY_FLAG 0x200000

#define DSP_WINDOW_DISABLE_FLAG_MAIN 0x800000

#define DSP_WINDOW_DISABLE_FLAG_SECONDARY 0x100000

#define DSP_WINDOW_CONTROL_ACTIVE 1

#define DSP_WINDOW_CONTROL_GATED 2

#define DSP_WINDOW_PANEL_BASE_Y 0x15F

#define EVT_PANEL_DRAW_COMMAND 0x53

#define EVT_PANEL_BACKGROUND_COLOR 0x30303040

#define EVT_SCROLL_TOP_FLAG 1

#define EVT_SCROLL_BOTTOM_FLAG 2

#define DSP_FLAG_EVENT_REQUIRED 0x902

#define DSP_FLAG_EVENT_HANDLED 0x907

#define DSP_FLAG_EVENT_SPECIAL_ENTRY 5

#define DSP_FLAG_EVENT_SCENE_ENTRY 3

#define DSP_FLAG_EVENT_PARTY_ENTRY 4

#define EVT_PARTY_SLOT_COUNT 5

#define DSP_AREA_TEXT_BYTES 0x19

#define DSP_NAME_TEXT_BYTES 0x13

#define DSP_DIALOG_TEXT_BYTES 0x11

extern s8 dspCapturedSoundMode;

typedef struct evtActiveFlagTable_t {
    s32 unk0;
    s32 unk4;
    s8 flags[0];
} evtActiveFlagTable_t;

typedef struct {
    u8 entryCount;
    u8 pad;
    u16 entryIndices[0];
} ActiveList;

extern void fclSetMessageVar(s32 slotIndex, const char *sourceText);

struct MenuTerminalWork;

typedef struct fclQuadRecord_t {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s32 unk10;
} fclQuadRecord_t;

void evtLoadResourcePair(const char *resourcePath, evtResourcePair_t *record);
void evtReleaseResourcePairHandle(evtResourcePair_t *record);

struct itfMesResource_t;

/* Advance the singleton window through its active, gated and cleanup phases. */
void dspAdvanceMessageWindowControl(s32 notify);
void fclSetMessageVar(s32 slotIndex, const char *sourceText);

/* Script opcode: set the supplied active-entry flag without bounds checking. */
ScrCommandResult evtOpSetActiveEntryFlag(void);

/* Script opcode: return a display-table value, clamping only its upper index.
 * It does not activate or test an active-entry flag. */
ScrCommandResult evtOpReadDisplayValue(void);

/* The outer panel owner is unresolved; its list pointer is shared by
 * the viewport flag and height consumers. */
typedef struct evtListPanelRecord_t {
    u8 pad0[0x14];
    struct mnuList_t *list;
} evtListPanelRecord_t;

s32 fclSetModelLight(char *name, dds3Basic_t *dst)
{
    dds3Basic_t *obj = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 9, name);

    if (obj == NULL) {
        return 0;
    }
    *(s32 *)(dst->workdata + 0x80) = *(s32 *)(obj->workdata + 0x78);
    return 1;
}

void fclLoadStage(s32 MajorNo, s32 MinorNo)
{
    s32 id = (MajorNo << 16) + MinorNo;

    if (dds3GetMapNo(dds3GetControlScene()) == id) {
        return;
    }
    evtLoadArea(MajorNo, MinorNo);
}

void fclFilterOFF(void)
{
    dds3SetBlur(0, 0, 0);
    dds3MultiBlurOff(0);
    dds3DistBlurOff(0);
    dds3FilterClear();
    Eve_BlurOFF();
    Eve_PartialMultiBlurOFF();
    Eve_PartialMultiFilterOFF();
    Eve_DistortionBlurOFF();
    Eve_FilterOFF();
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclFreeStage);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos)
{
    pPanelPos->X = X;
    pPanelPos->Y = Y;
    pPanelPos->Z = Z;
    pPanelPos->W = W;
    pPanelPos->H = H;
    pPanelPos->Col = Work;
}

void fclScriptForcedEnd(dds3ProcessID_t *PID)
{
    if (PID != NULL && dds3GetProcessStat(PID) != 0) {
        dds3KillProcess(PID, 0);
        PID = NULL;
    }
}

s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID)
{
    saddr_t handle;

    if (pPID != NULL)
    {
        fclScriptForcedEnd(*(dds3ProcessID_t **)pPID);
    }
    handle = EE_ADDR_S(scrStartScript2(0x7D0, EE_ADDR_AS(void *, pScriptData), StartNo));
    fclClearFlag(0);
    if (pPID != NULL)
    {
        *(s32 *)pPID = handle;
    }
    return handle;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/facility/fclMisc.c:244, match, same function 1.00 (words 10/10, calls 1.00, shape 1.00) */
s32 cmpItemOrder(u8 *a, u8 *b)
{
    u8 va = *a;
    u8 vb = *b;

    if (va > vb) {
        return 1;
    }
    if (va < vb) {
        return -1;
    }
    return 0;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/facility/fclMisc.c:258, match, same function 0.97 (words 19/20, calls 1.00, shape 0.97) */
s32 fclLeaf3(u8 *list, s32 count, u8 value)
{
    s32 kept = 0;
    s32 i;

    for (i = 0; i < count; i++) {
        if (list[i] != value) {
            u8 entry = list[i];

            list[i] = 0;
            list[kept] = entry;
            kept++;
        }
    }
    return kept;
}

void shuffleIndices(s8 *order, s32 count, s32 rounds)
{
    s32 n;

    if (rounds <= 0) {
        return;
    }
    n = rounds;
    while (n != 0) {
        s32 first = (u32)dds3GetRandA(0) % (u32)count;
        s32 second = (u32)dds3GetRandA(0) % (u32)count;
        u8 value = order[first];

        order[first] = order[second];
        order[second] = value;
        n--;
    }
}

/* Store the resource handle in word zero and the loader's data address in word one. */
void evtLoadResourcePair(const char *resourcePath, evtResourcePair_t *record) {
    record->handle = sdfDevLoadFileEx(resourcePath, (u32 *)&record->messageData, 0);
}

/* Release the handle without clearing either word of the caller's record. */
void evtReleaseResourcePairHandle(evtResourcePair_t *record) {
    sdfMemFreeDirect(record->handle);
}

s8 fclInitMessage(void *pData)
{
    s32 ret;

    if (gMsgID < 0) {
        gMsgID = itfMesMngInitialize(pData);
        itfMesMngChangeWindowType(gMsgID, 2, 0);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

/* Apply page mode zero and the caller's opaque page value to an open window. */
EvtMessageWindowPageValueResult fclChangeMessageWindowAlpha(s32 pageValue) {
    if (gMsgID < 0) {
        return EVT_MESSAGE_WINDOW_PAGE_VALUE_NO_WINDOW;
    }
    itfMesMngChangeWindowType(gMsgID, ITF_MES_PANEL_MODE_STANDARD, pageValue);
    return EVT_MESSAGE_WINDOW_PAGE_VALUE_UPDATED;
}

s32 fclStartMessage(s32 MsgNo)
{
    if (gMsgID < 0)
    {
        return 0;
    }
    itfMesMngMarkBitStat(gMsgID, 0x200000);
    itfMesMngRequestMessage(gMsgID, MsgNo, 0);
    itfMesMngSetSelPhase(gMsgID, -1);
    gMsgFlag = 1;
    return 1;
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclStartSelMessage);

INCLUDE_ASM(const s32, "facility/fclMisc", fclSetSelMessagePos);

/* body: matched in smt3 @ 9401f79e: src/smt3/facility/fclMisc.c:395, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s8 fclGetCurrentSelMessagePos(void)
{
    return gSelMsgPos;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0024CFB0.c:556, match, same function 1.00 (words 13/13, calls 1.00, shape 1.00) */
s32 fclGetSelMessagePos(void) {
    if (gMsgID < 0) {
        return -1;
    }
    return itfMesMngGetSelectNo(gMsgID);
}

/* Return the captured signed-byte panel value without querying the live window. */
s8 evtGetCapturedWindowPanelValue(void) {
    return dspCapturedSoundMode;
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclEndMessageEx);

s32 fclEndMessage(void)
{
    return fclEndMessageEx(1);
}

s32 fclDestroyMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    itfMesMngRelease(gMsgID);
    gMsgID = -1;
    gMsgFlag = 0;
    gMsgInvFlag = 0;
    return 1;
}

s32 fclChkMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    if (gMsgInvFlag != 0 && gMsgFlag == 2) {
        return 0;
    }
    return (s8)(u8)gMsgFlag;
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclChkSelMessage);

/* NON_MATCHING: 54 of 56 words match.  */
#ifdef NON_MATCHING
void fclUpdateMessageEx(s32 ClsMode) {
    if (gMsgID < 0) {
        return;
    }
    switch (gMsgFlag) {
    case 0:
        break;
    case 1:
        if (itfMesMngGetPhase(gMsgID) < 0) {
            gMsgFlag = 2;
        }
        break;
    case 2:
        if (gSelMsgNo >= 0) {
            itfMesMngSetSelectDefKey(gMsgID, 3, 1);
            itfMesMngSetSelect(gMsgID, gSelMsgPos);
            itfMesMngRequestSelect_itf(gMsgID, gSelMsgNo);
            gSelMsgNo = -1;
        }
        if (gMsgInvFlag == 0 && fclChkSelMessage() == 0) {
            gMsgFlag = 3;
        }
        break;
    case 3:
        fclEndMessageEx(ClsMode);
        break;
    }
}
#else
INCLUDE_ASM(const s32, "facility/fclMisc", fclUpdateMessageEx);
#endif

/* Request mode one from the existing message-window worker. */
void fclUpdateMessage(void) {
    dspAdvanceMessageWindowControl(1);
}

void fclSetMessageVar(s32 slotIndex, const char *sourceText) {
    itfMesMngSetVarImmediate(gMsgID, slotIndex, sourceText);
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclChkMessageInvalid);

void fclSetMessageInvalid(s32 Mode)
{
    if (Mode != 0) {
        itfMesMngClearBitStat(gMsgID, 0x800000);
        itfMesMngClearBitStat(gMsgID, 0x100000);
        gMsgInvFlag = 0;
        itfMesMngSetPhase(gMsgID, 1);
        gMsgFlag = 1;
    } else {
        itfMesMngMarkBitStat(gMsgID, 0x800000);
        itfMesMngMarkBitStat(gMsgID, 0x100000);
        gMsgInvFlag = 1;
    }
}

void fclSetMessagePos(s32 x, s32 y)
{
    itfMesMngSetMesPosition(gMsgID, x * 16, y * 8);
    itfMesMngSetOffsetY1(gMsgID, -((0x15F - y) * 8));
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclChkMessageDraw);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "facility/fclMisc", fclRefMsgIDFclMisc);

#endif
INCLUDE_ASM(const s32, "facility/fclMisc", fclScriptStartSub);

INCLUDE_ASM(const s32, "facility/fclMisc", fclLeaf4);

s32 fclSetWorkValue(s32 index, s32 value) {
    s32 result = 0;

    if (index < 16) {
        fclWorkValue[index] = value;
        result = 1;
    }
    return result;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0024CFB0.c:750, match, same function 1.00 (words 9/9, calls 1.00, shape 1.00) */
u32 fclLeaf5(s32 displayValueIndex) {
    displayValueIndex = (displayValueIndex < EVT_DISPLAY_VALUE_COUNT) ? displayValueIndex : EVT_LAST_DISPLAY_VALUE;
    return fclWorkValue[displayValueIndex];
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/facility/fclMisc.c:583, adapt, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
s32 fclCallGetIntPara(void)
{
    fclFlags[scrGetIntPara(0)] = 1;
    return 1;
}

s32 fclCallGetIntPara2(void)
{
    s32 index = scrGetIntPara(0);

    if (index >= 0x10) {
        index = 0xF;
    }
    scrSetIntReturnValue(fclWorkValue[index]);
    return 1;
}
s32 gMsgID = -1;

s8 gMsgFlag = 0;

s8 gMsgInvFlag = 0;

INCLUDE_SDATA(const s32, "facility/fclMisc", gSelMsgNo);

INCLUDE_SDATA(const s32, "facility/fclMisc", gSelMsgPos);

INCLUDE_SDATA(const s32, "facility/fclMisc", gLastSelMsgPos);

