#include "common.h"
#include "scr.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern s32 dds3GetProcessStat(void *id);
extern u32 dds3GetRandA(void *name);
void fclClearFlag(s8 index);
extern s8 gSelMsgPos;
extern s8 gMsgInvFlag;
extern s32 gMsgID;
extern s32 gSelMsgNo;
extern void itfMesMngSetVarImmediate(s32 id, u32 index, s32 pstr);
extern s32 fclWorkValue[16];
extern s32 scrGetIntPara(s32 n);
extern void scrSetIntReturnValue(s32 i);

typedef struct
{
    u8 unk_00000[0x1928];
    u8 item[0xA0];
} fclGlobalWorkItemInventory_t;

extern fclGlobalWorkItemInventory_t *fclGlobalWorkItems FAKEMATCH_ASM_NAME("DDS3_GBWK");
extern s32 dds3KillProcess(s32 id, s32 shutdowntimer);
extern s32 fclChkMessage(void);
extern void fclScriptForcedEnd(u8 *PID);
extern s8 gMsgFlag;
extern void itfMesMngRelease(s32 id);
extern void *dds3GetControlScene(void);
extern void *dds3SceneBasicSearchNameAndType(void *scene, s32 type, char *name);
extern void shuffleIndices(s8 *order, s32 count, s32 rounds);
extern s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

typedef struct
{
    /* 0x00 */ u8 unk_00[0x18];
    /* 0x18 */ u8 *unk_18;
} fclSceneObj_t;

extern s32 dds3GetMapNo(void *scene);
extern void evtLoadArea(s32 fld, s32 area);
void fclLoadStage(s32 MajorNo, s32 MinorNo);
void fclScriptForcedEnd(u8 *PID);
s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

typedef struct
{
    /* 0x00 */ u8 count;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 item[0xA0];
} fclItemList_t;

void shuffleIndices(s8 *order, s32 count, s32 rounds);
extern s32 itfMesMngInitialize(void *data);
extern void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha);
s8 fclInitMessage(void *pData);
extern void itfMesMngMarkBitStat(s32 id, s32 flag);
extern void itfMesMngRequestMessage(s32 msg, s32 id, s32 flag);
extern void itfMesMngSetSelPhase(s32 msgid, s32 phase);
s32 fclStartMessage(s32 MsgNo);
s32 fclDestroyMessage(void);
s32 fclChkMessage(void);
void fclUpdateMessage(void);
extern void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);
extern void itfMesMngSetOffsetY1(s32 msgid, s32 y1);
void fclSetMessagePos(s32 x, s32 y);

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

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

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#include "dds/scr.h"
#include "dds/evt_message_window_api.h"
#include "dds/itf_mes_panel_mode.h"
#include "dds/mnu.h"

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

typedef struct EvtActiveFlagTable {
    s32 unk0;
    s32 unk4;
    s8 flags[0];
} EvtActiveFlagTable;

typedef struct {
    u8 entryCount;
    u8 pad;
    u16 entryIndices[0];
} ActiveList;

extern void fclSetMessageVar(s32 slotIndex, const char *sourceText);

struct MenuTerminalWork;

extern s32 evtCommand_FIELD_CLS(void);

typedef struct QuadRecord {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s32 unk10;
} QuadRecord;

void evtLoadResourcePair(const char *resourcePath, EvtResourcePair *record);
void evtReleaseResourcePairHandle(EvtResourcePair *record);

struct ItfMesSub;

EvtMessageWindowPageValueResult fclChangeMessageWindowAlpha(s32 pageValue);
s8 evtGetCapturedWindowPanelValue(void);

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
typedef struct EvtListPanelRecord {
    u8 pad0[0x14];
    struct MenuList *list;
} EvtListPanelRecord;

s32 fclSetModelLight(char *name, fclSceneObj_t *dst)
{
    fclSceneObj_t *obj = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 9, name);

    if (obj == NULL) {
        return 0;
    }
    *(s32 *)(dst->unk_18 + 0x80) = *(s32 *)(obj->unk_18 + 0x78);
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

INCLUDE_ASM(const s32, "facility/fclMisc", fclFilterOFF);

INCLUDE_ASM(const s32, "facility/fclMisc", fclFreeStage);

INCLUDE_ASM(const s32, "facility/fclMisc", fclInitPanelPos);

void fclScriptForcedEnd(u8 *PID)
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
        fclScriptForcedEnd(*(u8 **)pPID);
    }
    handle = EE_ADDR_S(scrStartScript2(0x7D0, EE_ADDR_AS(void *, pScriptData), StartNo));
    fclClearFlag(0);
    if (pPID != NULL)
    {
        *(s32 *)pPID = handle;
    }
    return handle;
}

INCLUDE_ASM(const s32, "facility/fclMisc", cmpItemOrder);

INCLUDE_ASM(const s32, "facility/fclMisc", fclLeaf3);

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
void evtLoadResourcePair(const char *resourcePath, EvtResourcePair *record) {
    record->handle = sdfDevLoadFileEx(resourcePath, (u32 *)&record->messageData, 0);
}

/* Release the handle without clearing either word of the caller's record. */
void evtReleaseResourcePairHandle(EvtResourcePair *record) {
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

INCLUDE_ASM(const s32, "facility/fclMisc", fclGetCurrentSelMessagePos);

INCLUDE_ASM(const s32, "facility/fclMisc", fclGetSelMessagePos);

/* Return the captured signed-byte panel value without querying the live window. */
s8 evtGetCapturedWindowPanelValue(void) {
    return dspCapturedSoundMode;
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclEndMessageEx);

INCLUDE_ASM(const s32, "facility/fclMisc", fclEndMessage);

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

INCLUDE_ASM(const s32, "facility/fclMisc", fclUpdateMessageEx);

/* Request mode one from the existing message-window worker. */
void fclUpdateMessage(void) {
    dspAdvanceMessageWindowControl(1);
}

void fclSetMessageVar(s32 slotIndex, const char *sourceText) {
    itfMesMngSetVarImmediate(gMsgID, slotIndex, sourceText);
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclChkMessageInvalid);

INCLUDE_ASM(const s32, "facility/fclMisc", fclSetMessageInvalid);

void fclSetMessagePos(s32 x, s32 y)
{
    itfMesMngSetMesPosition(gMsgID, x * 16, y * 8);
    itfMesMngSetOffsetY1(gMsgID, -((0x15F - y) * 8));
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclChkMessageDraw);

INCLUDE_ASM(const s32, "facility/fclMisc", fclScriptStartSub);

INCLUDE_ASM(const s32, "facility/fclMisc", fclLeaf4);

INCLUDE_ASM(const s32, "facility/fclMisc", fclSetWorkValue);

INCLUDE_ASM(const s32, "facility/fclMisc", fclLeaf5);

INCLUDE_ASM(const s32, "facility/fclMisc", fclCallGetIntPara);

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

