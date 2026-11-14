#include "common.h"

/* Declarations from smt3-decomp src/smt3/facility/fclMisc.c */
#include "common.h"

#include "scr.h"

#include "facility.h"

#include "fpu.h"

#include "libsdf.h"

#include "newdata.h"

#include "ps2asm.h"

#include "event.h"

void fclFreeStage(void);

extern s32 dds3GetProcessStat(void *id);

extern u32 dds3GetRandA(void *name);

extern s32 fclSetWorkValue(s8 index, s32 value);

s32 fclGetFlag(s8 index);

void fclClearFlag(s8 index);

extern s8 gSelMsgPos;

extern s8 gMsgInvFlag;

extern s8 fclFlags[8];

extern s32 gMsgID;

extern s32 gSelMsgNo;

extern s8 fclEndMessageEx(s8 Mode);

extern void evtFreeStage(void);

extern s32 sdfDelayedResrcCheck(void);

extern void fclFilterOFF(void);

extern void sdfFileCmdReadFile(const char *aFileName, s32 aBuffer, s32 aLength, void *aCallback, void *aUserData);

extern void itfMesMngSetVarImmediate(s32 id, u32 index, s32 pstr);

extern s16 itfMesMngGetSelectNo(s32 msgid);

extern s16 itfMesMngGetSelPhase(s32 msgid);

extern s32 fclWorkValue[16];

extern s32 scrGetIntPara(s32 n);

extern void scrSetIntReturnValue(s32 i);

typedef struct
{
    u8 unk_00000[0x1928];
    u8 item[0xA0];
} fclGlobalWorkItemInventory_t;


extern s32 datAddItem(s32 id, s32 num);

extern s32 dds3KillProcess(s32 id, s32 shutdowntimer);

extern s32 fclChkMessage(void);

extern void fclScriptForcedEnd(u8 *PID);

extern u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);

extern s8 gMsgFlag;

extern void itfMesMngRelease(s32 id);

extern void *dds3GetControlScene(void);

extern void *dds3SceneBasicSearchNameAndType(void *scene, s32 type, char *name);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

extern void shuffleIndices(s8 *order, s32 count, s32 rounds);

extern s32 cmpItemOrder(u8 *a, u8 *b);

extern s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

extern s32 datGetItemSlotIndex(s32 id);

extern void datAddHearts(s32 id);

s32 fclAddItem(u16 ItemID, s32 Nums);

typedef struct
{
    /* 0x00 */ u8 unk_00[0x18];
    /* 0x18 */ u8 *unk_18;
} fclSceneObj_t;

s32 fcl_213998(char *name, fclSceneObj_t *dst);

extern s32 dds3GetMapNo(void *scene);

extern void evtLoadArea(s32 fld, s32 area);

void fclLoadStage(s32 MajorNo, s32 MinorNo);

extern void dds3SetBlur(s32 frame, s32 alpha, s32 scale);

extern void dds3MultiBlurOff(s32 frame);

extern void dds3DistBlurOff(s32 frame);

extern void Eve_BlurOFF(void);

extern void Eve_PartialMultiBlurOFF(void);

extern void Eve_PartialMultiFilterOFF(void);

extern void Eve_DistortionBlurOFF(void);

extern void Eve_FilterOFF(void);

void fclFilterOFF(void);

void fclFreeStage(void);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

void fclScriptForcedEnd(u8 *PID);

s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

typedef struct
{
    /* 0x00 */ u8 count;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 item[0xA0];
} fclItemList_t;

void func_00213BB8(fclItemList_t *list);

extern s32 cmpItemHasFlag(u16 item, s8 which);

void fclCreateItemListEx(fclItemList_t *list, s8 which);

s32 cmpItemOrder(u8 *a, u8 *b);

s32 func_00213CD8(u8 *list, s32 count, u8 value);

void shuffleIndices(s8 *order, s32 count, s32 rounds);

extern char *text2[];

char *func_00213DC0(datUnitWork_t *unit);

u32 func_00213DF0(u32 color, u32 scaleFactor);

u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);

extern s32 itfMesMngInitialize(void *data);

extern void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha);

s8 fclInitMessage(void *pData);

extern void itfMesMngMarkBitStat(s32 id, s32 flag);

extern void itfMesMngRequestMessage(s32 msg, s32 id, s32 flag);

extern void itfMesMngSetSelPhase(s32 msgid, s32 phase);

s32 fclStartMessage(s32 MsgNo);

s32 fclStartSelMessage(s32 SelMsgNo);

void fclSetSelMessagePos(s8 SelPos);

s8 fclGetCurrentSelMessagePos(void);

s8 fclGetSelMessagePos(void);

extern void itfMesMngSetPhase(s32 msgid, s32 phase);

extern void itfMesMngMsgWndCls(s32 msgid);

extern void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker);

extern void fclSetMessageInvalid(s8 Mode);

s8 fclEndMessageEx(s8 Mode);

s8 fclEndMessage(void);

s32 fclDestroyMessage(void);

s32 fclChkMessage(void);

s32 fclChkSelMessage(void);

extern s8 itfMesMngGetPhase(s32 msgid);

extern s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index);

extern void itfMesMngSetSelect(s32 id, s32 selno);

extern void itfMesMngRequestSelect(s32 id, s32 select);

void fclUpdateMessage(void);

void fclSetMessageVar_20743088(s8 Index, saddr_t pStr);

s8 fcl_214270(void);

extern void itfMesMngClearBitStat(s32 id, s32 flag);

void fclSetMessageInvalid(s8 Mode);

extern void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);

extern void itfMesMngSetOffsetY1(s32 msgid, s32 y1);

void fcl_2142F0(s32 x, s32 y);

s32 fclChkMessageDraw(void *PID);

void fclClearFlag(s8 index);

s32 fclGetFlag(s8 index);

s32 fclSetWorkValue(s8 index, s32 value);

s8 fclGetWorkValue(s8 index);

s32 fcl_214428(void);

s32 fcl_214458(void);

s32 fclLoadFileDone(s32 unused, s32 cmd, s32 value, s32 unused2, s32 *out);

void fclLoadFileAsync(const char *name, void *buf);

extern s32 gMsgID;

extern s8 gMsgFlag;

extern s8 gMsgInvFlag;

extern s32 gSelMsgNo;

extern s8 gSelMsgPos;

extern char fclStrFee[4];

extern u32 D_0054FC68[2];

extern char fclStrOwn[4];

extern u32 D_0054FC78[2];

extern u32 D_0054FC80[2];

extern char fclStrTotal[6];

extern char fclStrMoney[6];

extern char fclStrNo[3];

extern char fclStrYes[4];
/* End of declarations from src/smt3/facility/fclMisc.c */

/* Declarations from dds-decomp src/dds1/game/code_0024CFB0.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_fclGlobalWorkItemInventory_t 1

#define SMT3_DEFINES_fclItemList_t 1

#define SMT3_DEFINES_fclSceneObj_t 1

#include "dds/dds3_fade.h"

#include "dds/eff_transform.h"

#include "dds/kwln.h"

#include "dds/mdl_flag_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_texture_file.h"

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

/* Return nonzero while either foreground fade direction is enabled. */
s32 dds3FadeCheck(void);

void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration);

/* Foreground alpha transitions, stepped by dds3FadeUpdate. */
void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);

u32 dds3GetProcessData(const dds3ProcessID_t *task);

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

/* Set and clear one flag using the providers' signed-index adjustment. */
void evtBitOn(s32 flag);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */
struct sdfTexHandle_t *sdfTexSet(const SdfTextureFileHeader *header);

#include "dds/dsp_entry_api.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr.h"

#include "dds/scr.h"

#include "dds/dsp_message_window_api.h"

#include "dds/dsp_channel.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/dsp_active_api.h"

#include "dds/evt_message_window_api.h"

#include "dds/evt_world_object_api.h"

#include "dds/evt_active_entry_flag_api.h"

#include "dds/dds3_task_state.h"

#include "dds/dds3_task_state_query.h"

#include "dds/itf_mes_window.h"

#include "dds/itf_mes_panel_mode.h"

#include "dds/itf_mes_entry_start_api.h"

#include "dds/itf_mes_select_def_key_result.h"

#include "dds/mnu.h"

#include "dds/mnu_list.h"

#include "dds/mnu_terminal_work.h"

#include "dds/sdf_heap_api.h"

#include "dds/kwln.h"

#include "dds/dds3_fade.h"

#include "dds/dat_state.h"

#include "dds/evt_world.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3obj.h"

#include "dds/sdf_texture_file.h"

#include "dds/eff_random.h"

#include "dds/eff_transform.h"

#include "dds/eff_light.h"

#include "dds/mdl_flag_api.h"

#include "dds/mnu_event_flag_entries.h"

#include "dds/eff_event_draw.h"

struct sdfTexHandle_t;

struct MenuTerminalWork;

extern void mnuTerminalSelectResourceBank(struct MenuTerminalWork *);

extern void mnuApplyFadeTrackMode(s32, struct MenuTerminalWork *);

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

extern s32 D_003BC410;

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

extern EvtActiveFlagTable evtActiveEntryFlags;

extern void mnuDrawTerminalEffectIfAvailable(s32 state);

#include "dds/dds3_world_secondary.h"

extern void evtCopyEntryStringToActiveWindow(s32 slotIndex, const char *sourceText);

extern MenuPopupEntry mnuTerminalPopupEntries[];

extern u8 *D_003BAA70;

extern u8 *D_003BAA74;

extern u8 *D_003BAA78;

extern u8 *D_003BAA84;

extern MnuSceneFlagEntry mnuSceneFlagEventEntries[MNU_SCENE_FLAG_ENTRY_COUNT];

extern MnuPartyFlagEntry mnuPartyFlagEventEntries[MNU_PARTY_FLAG_ENTRY_COUNT];

/* Recreate the message window from the terminal's loaded message resource. */
void evtCloseDisplayChannelAndEnsureMessageWindow(struct MenuTerminalWork *work);

/* Start the first eligible special, scene, or active-party flag event.
 * Only panel kinds zero/one are accepted. Mark the handled flag immediately;
 * this is not a wait for dialogue completion, and dispatch results are ignored. */
s32 dspStartFlagEvent(struct MenuTerminalWork *work);

/* Initialize the current terminal task's resource bank and mode-zero fade track. */
s32 mnuPrepareTerminalPanelState(dds3ProcessID_t *task);

/* Registered terminal panel leave hook: no side effects; returns one. */
u32 mnuTerminalPanelLeaveNoOp(void);

/* Propagate the panel worker's nonzero result before considering a flag-event popup.
 * When its popup slot and message control are idle, fall back if no event starts. */
s32 dspUpdateFlagEvent(dds3ProcessID_t *request);

/* Prepare the current terminal work and dispatch panel mode one. */
s32 mnuDispatchTerminalPanel(dds3ProcessID_t *request);

/* Request the message-window transition before dispatching terminal panel mode two. */
s32 mnuDispatchTerminalPanelExit(dds3ProcessID_t *request);

void evtClearActiveFlag(s32 flagIndex);

/* Reset display slots for a zero exit marker, otherwise start the existing fade-in.
 * The active-flag call's index is the zero the fade's arguments leave in a0. */
u32 mnuApplyTerminalExitFadeOrReset(dds3ProcessID_t *task);

/* Select the terminal's mode-one fade track before starting the black fade-out. */
s32 mnuStartTerminalPanelFadeOut(dds3ProcessID_t *task);

extern char D_003AF710[];

extern char D_0036AE48[];

/* Propagate the panel worker result; create a ready popup only for an empty slot.
 * Kind zero waits for the named task unless active flag zero overrides the wait;
 * other kinds wait for the fade to finish. */
s32 mnuUpdateTerminalReadyPopup(dds3ProcessID_t *request);

/* Draw the available terminal effect, then dispatch panel mode one. */
s32 mnuDispatchTerminalPanelModeOne(dds3ProcessID_t *request);

/* Dispatch the current work in terminal panel mode two. */
s32 mnuDispatchTerminalPanelModeTwo(dds3ProcessID_t *request);

/* Clear the terminal task's stored selection word and report completion. */
u32 mnuClearTerminalPanelSelection(dds3ProcessID_t *task);

struct MenuTerminalWork;

extern void mnuReleaseWorkResources(struct MenuTerminalWork *work);

extern void mnuTerminalBuildMenus(struct MenuTerminalWork *host);

/* Release the current task's menu resources before rebuilding its terminal menus. */
s32 mnuRebuildTerminalMenuResources(dds3ProcessID_t *task);

/* Registered terminal rebuild start hook: no side effects; returns zero. */
u32 mnuTerminalRebuildStartNoOp(void);

/* Registered terminal rebuild update hook: no side effects; returns zero. */
u32 mnuTerminalRebuildUpdateNoOp(void);

/* Registered terminal rebuild finish hook: no side effects; returns zero. */
u32 mnuTerminalRebuildFinishNoOp(void);

u32 func_0024D608(void);

/* Copy the source node's nested value only on a successful lookup.
 * The destination and both nested data pointers are caller-owned. */
s32 evtCopyWorldObjectEntryValue(const char *objectName, s32 destination);

extern s32 evtCommand_FIELD_CLS(void);

/* Shut down the stage before clearing its draw transitions. */
void evtShutdownStageAndResetDrawTransitions(void);

typedef struct QuadRecord {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s32 unk10;
} QuadRecord;

/* Emit active game-entry indices in ascending order, excluding entry zero.
 * DDS1 scans 1..191; the caller must provide enough trailing halfwords. */
void evtCollectActiveGameIndices(ActiveList *list);

/* Keep non-excluded bytes in order and return the retained count.
 * Only retained source positions are cleared; this does not zero the tail. */
s32 evtCompactFilteredBytes(u8 *buffer, s32 length, u8 excluded);

/* Store the resource handle in word zero and the loader's data address in word one. */
void evtLoadResourcePair(const char *resourcePath, EvtResourcePair *record);

/* Release the handle without clearing either word of the caller's record. */
void evtReleaseResourcePairHandle(EvtResourcePair *record);

struct ItfMesSub;

/* Apply page mode zero and the caller's opaque page value to an open window. */
EvtMessageWindowPageValueResult evtSetMessageWindowPageValue(s32 pageValue);

extern s32 dspReadWindowSecondPanelValue(void);

/* Store an option-list entry for deferred construction and snapshot the
 * panel's second halfword. */
s32 evtStoreValueAndCaptureWindowPanelValue(ItfMesEntryIndex entryIndex);

/* Update the byte-sized option only while the singleton window is open. */
void evtSetMessageWindowOptionWhenOpen(ItfMesOptionBitIndex selectedBitIndex);

/* Read the second panel halfword, or -1 when no message window exists.
 * Despite the inherited name, this is not a sound-driver query. */
s32 dspReadWindowSecondPanelValue(void);

/* Return the captured signed-byte panel value without querying the live window. */
s8 evtGetCapturedWindowPanelValue(void);

/* Reset an open window, optionally notify it first, and report whether it existed.
 * Cleanup leaves the singleton handle intact; fclDestroyMessage destroys it. */
u32 evtCleanupMessageWindow(s32 notify);

/* Finish/reset the singleton window with notification and return whether it existed. */
u32 evtFinishMessageWindowAndNotify(void);

/* Snapshot the second panel halfword only after the first halfword is nonnegative. */
s32 dspCaptureWindowSecondPanelValue(void);

extern void itfMesBuildOptionList(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex);

/* Advance the singleton window through its active, gated and cleanup phases. */
void dspAdvanceMessageWindowControl(s32 notify);

/* Request mode one from the existing message-window worker. */
void dspRequestMessageWindowModeOne(void);

void evtCopyEntryStringToActiveWindow(s32 slotIndex, const char *sourceText);

/* Return the gate byte independently of the singleton window's existence. */
s8 dspGetWindowStateGate(void);

/* Enable clears both disable flags and restores panel/control state one.
 * Disable sets the gate and flags, but deliberately leaves the control byte alone.
 * Callers must already have a valid window. */
void dspSetActive(s32 enabled);

/* Convert x/y to the window's 16-by-8 coordinate units and set its panel offset.
 * The original signed shifts are retained, including their input assumptions. */
void evtMoveMessageWindowWithPanelOffset(s32 x, s32 y);

/* Clear one active-entry flag; the caller supplies a valid index. */
void evtClearActiveFlag(s32 flagIndex);

/* Test one active-entry flag; the caller supplies a valid index. */
EvtActiveEntryFlagState evtIsActiveFlagSet(s32 flagIndex);

/* Reject only indices at/above the upper bound; negative indices are not checked. */
s32 evtSetBoundedDisplayValue(s32 displayValueIndex, s32 displayValue);

/* Clamp only the upper side to the last display slot; negative indices pass through. */
u32 evtGetBoundedDisplayValue(s32 displayValueIndex);

/* Script opcode: set the supplied active-entry flag without bounds checking. */
ScrCommandResult evtOpSetActiveEntryFlag(void);

/* Script opcode: return a display-table value, clamping only its upper index.
 * It does not activate or test an active-entry flag. */
ScrCommandResult evtOpReadDisplayValue(void);

/* Acquire the loaded data's texture reference, then release its temporary resource.
 * DDS1 passes the size output as a pointer, unlike DDS2's address-valued slot. */
struct sdfTexHandle_t *fclLoadTexture(const char *path);

/* The outer panel owner is unresolved; its list pointer is shared by
 * the viewport flag and height consumers. */
typedef struct EvtListPanelRecord {
    u8 pad0[0x14];
    struct MenuList *list;
} EvtListPanelRecord;

extern void uiDrawUniformRgbRange(s32 *, s32 *, s32, u32, s32);

void evtDrawListViewportIndicators(s32 x, s32 topY, s32 bottomY, s32 size, EvtListPanelRecord *record);

extern void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);

void evtDrawListViewportPanel(s32 x, s32 y, s32 width, EvtListPanelRecord *record);

/* Draw a plain background rectangle with the shared panel color and command. */
void evtDrawPlainPanel(u32 x, u32 y, u32 width, u32 height);
/* End of declarations from src/dds1/game/code_0024CFB0.c */

s32 fcl_213998(char *name, fclSceneObj_t *dst)
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

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00123BF8);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00123C58);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00123C78);

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

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00123D48);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00123D70);

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
EvtMessageWindowPageValueResult evtSetMessageWindowPageValue(s32 pageValue) {
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

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00123F80);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00123FB0);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00123FC8);

INCLUDE_ASM(const s32, "facility/fclMisc", dspReadWindowSecondPanelValue);

/* Return the captured signed-byte panel value without querying the live window. */
s8 evtGetCapturedWindowPanelValue(void) {
    return dspCapturedSoundMode;
}

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00124010);

/* Finish/reset the singleton window with notification and return whether it existed. */
u32 evtFinishMessageWindowAndNotify(void) {
    return evtCleanupMessageWindow(1);
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

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00124130);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00124178);

/* Request mode one from the existing message-window worker. */
void dspRequestMessageWindowModeOne(void) {
    dspAdvanceMessageWindowControl(1);
}

void evtCopyEntryStringToActiveWindow(s32 slotIndex, const char *sourceText) {
    itfMesMngSetVarImmediate(gMsgID, slotIndex, sourceText);
}

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00124298);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_001242A0);

void fcl_2142F0(s32 x, s32 y)
{
    itfMesMngSetMesPosition(gMsgID, x * 16, y * 8);
    itfMesMngSetOffsetY1(gMsgID, -((0x15F - y) * 8));
}

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00124358);

INCLUDE_ASM(const s32, "facility/fclMisc", func_001287A0);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00124380);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_00124390);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_001243A0);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_001243C8);

INCLUDE_ASM(const s32, "facility/fclMisc", func_r1_001243F0);

s32 fcl_214458(void)
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

INCLUDE_SDATA(const s32, "facility/fclMisc", D_r1_0046B5C0);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_r1_0046B5C4);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_r1_0046B5C5);

