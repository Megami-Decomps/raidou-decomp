#include "common.h"

/* Declarations from smt3-decomp src/smt3/interface/itfMesManager.c */
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

void itfMesMngMsgWndCls(s32 msgid);

extern void fldPanelSet(s32 mode);

#define ITF_MES_UPPER_STATUS_PAIR_MASK 0x3000

void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker);

#define ITF_MES_CLEAR_SCRIPT_PANEL_BIT 0xFFDFFFFF

u32 itfMesMngOpenSelectWindow(void);

u32 itfMesMngCloseSelectWindow(void);

s32 itfMesMngSetMesPosDefault(void);

void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);

#define ITF_MES_DEFAULT_TEXT_X 0x4B0

#define ITF_MES_DEFAULT_TEXT_Y 0xA88

s32 itfMesMngInitialize(itfMesResource_t *sub);

extern itfMesLink_t *itfMiscLinkTake(itfMesLinkControl_t *pstack);

extern itfMesManager_t itfMesMngManager;

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern u32 sdfMemLock(sdfMemHandle_t *aHandle);

itfMesResource_t *itfMesMngSetResourcePtr(s32 id, itfMesResource_t *presource);

void InitializeSpeaker(Speaker_tag *pspeaker, s32 defaultpos);

void InitializeMessage(message_tag *pmessage, s32 defaultpos);

extern void InitializeSelect(struct select_tag *pselect);

void InitializeWindow(window_tag *pwindow);

void InitializeVariable(variable_tag *pvariable);

void InitializeCurCtr(curctr_tag *pcurctr, s32 isdisp);

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

void itfMesMngSetSelPosition(s32 id, s32 x, s32 y);

void itfMesMngSetSelPositionOffset(s32 id, s32 x, s32 y);

void itfMesMngSetPriority(s32 id, s32 z_buf);

void ChangeZBuffer(frFontHandle_t *pfont, s32 z);

void itfMesMngSetStat(s32 id, u32 flag);

#define ITF_MES_HIGH_FLAGS_MASK 0xffff0000

s16 itfMesMngGetSelect(s32 id);

s16 itfMesMngGetMesPage(s32 id);

s16 itfMesMngGetMesCurrentPage(s32 id);

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

extern void ResetWindowPara(window_tag *pwin, s32 type, s32 alpha);

void itfPnlRelease(itfpanel_t *handle);

s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index);

s32 itfMesMngGetWidth(s32 id, s32 message, s32 page);

void CommitVarTable(s32 *pvar);

void frSetExtDic(u8 *pExtFontData);

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);

s32 GetMessageWidth(frFontHandle_t *pfont);

void itfMesMngSetDefaultSpeed(s32 id, u8 speed);

extern void itfMesMngSetKutiCallback(s32 id, void (*func)(void));

void itfMesMngInitializeManager(void);

extern sdfTexHandle_t *itfMiscLoadTexture(const char *filename);

extern void itfMiscLinkInitialize(itfMesLinkControl_t *pool, itfMesLink_t *nodes, s32 count, s32 stride);

#define ITF_MES_WINDOW_CAPACITY 0x80

#define ITF_MES_POOL_NODE_BYTES 0x14

extern dds3ProcessID_t *dds3StartProcess(const char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                         s32 v3, dds3ProcessFunc_t drawExit,
                         itfProcessShutdown_t deleteExit, u32 p);

s32 CalcMsgMng_Process(void);

void DrawMsgMng_Shutdown(void);

s32 DrawMsgMng_Process(dds3ProcessID_t *id);

void itfMesMngSetBitStatManager(u32 flags);

void itfMesMngClearBitStatManager(u32 flags);

u16 itfMesMngGetStatManager(void);

s32 itfMesMngDebugProc(void);

extern debug_tag itfMesDebug;

itfMesLink_t *DebugChangeInstance(itfMesLink_t *pflow);

extern itfPadState_t dds3PadButtonState;

void DebugMoveWinVert(window_tag *pwin, s32 flag);

void DebugMoveWinAlpha(window_tag *pwin);

sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

extern void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y,
                        s32 sx, s32 sy);

extern char itfMesDebugItemFormat[];

extern char *itfMesSelectionMarkers[2];

extern char itfMesMarkerNone[];

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr,
                                  const char *aStr, ...);

extern char *itfMesDebugMenuLabels[];

extern char itfMesDebugFadeFormat[];

extern sdfGrOt_t sdfGrOtTable[];

void itfMesMngDebugShutdown(void);

extern void ReleaseAllWindow(struct instanceMes_tag *pinst);

void ReleaseAllVariable(variable_tag *pvariable);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void itfMiscLinkGive(itfMesLink_t *pelem, itfMesLinkControl_t *pstack);

u32 CheckMessageBinary(itfMesResource_t *pdata);

#define ITF_MES_MAGIC_MSG0 0x3047534d

extern u32 strlen(const char *text);

#define ITF_MES_STRING_COPY_ROUND_BIAS 4

#define ITF_MES_COPY_ALIGN_MASK 3

extern void *memcpy(void *a, const void *b, size_t c);

#define ITF_MES_BINARY_COPY_ROUND_BIAS 5

extern void *memset(void *a, s32 b, size_t c);

frFontHandle_t *UnchainFontHandleUnit(frFontHandle_t *pfont, s32 index, s32 lines);

void ChangeFontAlpha(frFontHandle_t *pfont, u32 alpha);

#define ITF_MES_COLOR_BYTE_CLEAR_MASK 0xffffff00

void OpenCheckSpeakerWindow(instanceMes_tag *pinst);

void OpenMessageWindow(instanceMes_tag *pinst);

void ReadytoMessage(instanceMes_tag *panel);

extern itfpanel_t *itfPnlRegist2(s32 type, addr_t texhandle);

void itfPnlSetPosition(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);

void itfPnlSetColor(itfpanel_t *handle, s32 r, s32 g, s32 b, s32 a);

void CalcMesFlow(instanceMes_tag *pinst);

extern void CalcWindow(instanceMes_tag *pinst);

extern void CalcMessage(instanceMes_tag *pinst);

extern void CalcSelect(instanceMes_tag *);

void CalcCurCtr(instanceMes_tag *pinst);

s32 SelectItem(instanceMes_tag *pinst);

void SelectItemMove(select_tag *psel, s32 direct);

extern void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

s32 SelectItemDefKey(select_tag *psel);

void ChangeFontColor(frFontHandle_t *pfont, s32 index, s32 lines, s32 color);

extern u32 dds3NullDrawProc(void);

void DrawMesFlow(instanceMes_tag *pinst);

void DrawWindow(instanceMes_tag *pinst);

void DrawRetPageCursor(instanceMes_tag *pinst);

void DrawSelectCursor(instanceMes_tag *pinst);

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

void DebugDrawWindowLine(itfpanel_t *meswin);

extern s32 sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void itfMiscDrawLineStripPrim(void *, void *, u8 *, u8 *, s32, u32, sdfDrawTag_t *);

extern s32 itfMesSelectRectVerts[8];

extern itfColor_t itfMesSelectRectColors[2];

extern u8 itfMesSelectRectIndex[];

extern u8 itfMesSelectRectColorIndex[];

extern void itfMiscDrawSpritePlanePrim(sdfIVector2_t *pos, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag);

extern void itfMiscDrawRectanglePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command);

extern void itfMiscDrawSpritePrim2(sdfIVector2_t *pos, itfColor_t *uv, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag);

extern s32 itfMesRetPageUv[4];

extern s32 itfMesRetPageColor[4];

extern void itfMiscDrawSpritePrim(void *pos, void *uv, void *color, s32 z, sdfTexHandle_t *texhandle, s32 context, sdfDrawTag_t *pdrawtag);

extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context);

extern s32 D_00326798[];

extern u8 itfMesDebugLineVertIndex[5];

extern u8 itfMesDebugLineColorIndex[5];

void itfPnlSetPositionOffset(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);

void sdfAddressResolver(s32 *aDataBody, saddr_t aOffset, u8 *aAddrInfoPtr, s32 aAddrInfoLeng);

void frSetVariableRequest(s32 VarNo, s32 pStr);

void itfMesMngSetSelect(s32 id, s32 selno);

void itfMesMngMarkBitStat(s32 id, u32 flag);

void itfMesMngClearBitStat(s32 id, u32 flag);

void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type);

/* Poll/start an entry: one ends the command, zero keeps waiting.
   A negative signed status is consumed by clearing it; positive status keeps waiting. */
s32 itfMesMngMessage(void);

void itfMesMngSetOffsetY1(s32 msgid, s32 y1);

s8 itfMesMngGetPhase(s32 msgid);

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 msgid, s8 phase);

s32 itfMesMngSelect(void);

/* Return the option-block signed status word, not the separate entry status byte. */
s16 itfMesMngGetSelPhase(s32 msgid);

void itfMesMngSetSelPhase(s32 msgid, s16 phase);

/* Return the same signed word forwarded as the script result when an option completes. */
s16 itfMesMngGetSelectNo(s32 msgid);

/* Store the script's panel mask; a negative script window is a no-op. Return 1. */
s32 itfMesMngMaskSelect(void);

s32 itfMesMng_SEL_SELNO(void);

s32 itfMesMngOpenWindow(void);

/* Finish single-bit states of the lower status pair; return whether its original
 * value was zero. Finishing a nonzero state therefore still returns 0. */
s32 itfMesMngCloseWindow(void);

/* Mark each originally nonzero status pair with both bits, then clean/reset
 * content and clear the script panel bit. The status pairs are set, not cleared. */
void itfMesMngMsgWndCls(s32 msgid);

u32 itfMesMngOpenSelectWindow(void);

u32 itfMesMngCloseSelectWindow(void);

s32 itfMesMngSetVarScr(void);

s32 itfMesMngIgnoreConfig(void);

s32 itfMesMngSetMesPositionScript(void);

/* Move selected-entry text to the fixed default position; return 1. */
s32 itfMesMngSetMesPosDefault(void);

s32 itfMesMngValidateNext(void);

s32 itfMesMngWindowType(void);

s32 itfMesMngWindowTypeAlpha(void);

s32 itfMesMngSelectDefKey(void);

s32 itfMesMngInitialize(itfMesResource_t *sub);

/* Destroy an installed nonnegative window slot; no upper-bound validation. */
void itfMesMngRelease(s32 id);

/* Called above its definition, where ee-gcc assumed int. */
u8 *GetSpeakerName(instanceMes_tag *pinst);

s32 itfMesMngRequestMessage(s32 window, s32 entryIndex, s32 itemIndex);

/* Release/reset selected-entry glyphs and their status; optionally do the same
 * for primary text. Negative window is a no-op; no other slot validation. */
void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker);

/* Called above its definition, where ee-gcc assumed int. */
frFontHandle_t *GetFontHandleUnit(u32 *list, s32 count, u32 mask, s32 x, s32 y, s32 depth);

void itfMesMngRequestSelect(s32 id, s32 select);

/* Release option glyphs, clear option selection/state and invalidate its rank.
 * Other content and the window allocation remain installed. */
void itfMesMngRequestSelectRelease(s32 id);

void itfMesMngSetMaskSelect(s32 id, u32 maskpattern);

void itfMesMngSetSelect(s32 id, s32 selno);

/* Move primary text to an absolute position; an unchanged position is a no-op. */
void itfMesMngSetSpeakerPosition(s32 id, s32 x, s32 y);

/* Translate primary text and update its cached position, including zero deltas. */
void itfMesMngSetSpeakerPositionOffset(s32 id, s32 x, s32 y);

/* Move selected-entry text absolutely; an unchanged position is a no-op. */
void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);

/* Translate selected-entry text and update its cached position. */
void itfMesMngSetMesPositionOffset(s32 id, s32 x, s32 y);

/* Move option text absolutely; an unchanged position is a no-op. */
void itfMesMngSetSelPosition(s32 id, s32 x, s32 y);

/* Translate option text and update its cached position. */
void itfMesMngSetSelPositionOffset(s32 id, s32 x, s32 y);

/* Propagate a changed render value to all three glyph chains before caching it. */
void itfMesMngSetPriority(s32 id, s32 z_buf);

u32 itfMesMngGetStat(s32 id);

/* Replace only the high half, preserving the existing low-half cast. */
void itfMesMngSetStat(s32 id, u32 flag);

void itfMesMngMarkBitStat(s32 id, u32 flag);

void itfMesMngClearBitStat(s32 id, u32 flag);

/* Return the cached clear-bit rank; reset state uses -1. */
s16 itfMesMngGetSelect(s32 id);

/* Return the selected table's cached item count, without another table lookup. */
s16 itfMesMngGetMesPage(s32 id);

/* Return the stored signed-halfword item index, without another table lookup. */
s16 itfMesMngGetMesCurrentPage(s32 id);

/* Return the replacement text stored in a window slot. */
u8 *itfMesMngGetVar(s32 id, s32 index);

/* Localised name text, 0x40 bytes after the manager. */
extern char itfMesMngVarText[];

extern char itfMesFmtInt[];

extern char *text1[];

extern char *text2[];

extern char *pStr[];

extern char *pstr[];

extern char *datExplainTable[];

extern char D_00311450[];

extern char D_00311850[];

void itfMesMngGetVarLocalize(s32 id);

/* Called above its definition, where ee-gcc assumed int. */
char *ConvertZenkakuNumber(s32 buf, char *num);

void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type);

/* Replace a text slot with a copied NUL-terminated encoded string. */
void itfMesMngSetVarImmediate(s32 id, u32 index, const char *pstr);

/* Copy text into one of the window's message text slots. */
void itfMesMngSetVarImmediate2(s32 id, s32 index, const char *pstr, s32 length);

/* Install/relocate a message resource and cache MSG1's trailing-entry word as
 * the temporary font entry. Return the previous resource without releasing it. */
itfMesResource_t *itfMesMngSetResourcePtr(s32 id, itfMesResource_t *presource);

/* Look up an entry-table item; only the item index has a bounds check. */
u8 *itfMesMngGetDataPtrImmediate(s32 id, s32 message, s32 page);

/* Refresh changed panel values only when both content-status pairs are clear.
 * Release the old primitive after applying the panel mode and fade limit. */
void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha);

/* Set or append the option with `id` in the window's option list; ids 1, 6, 7 and >= 16 are rejected. */
s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index);

/* Build and release a temporary glyph chain to measure the widest grouped row.
 * Empty tables and zero/missing encoded items return zero. */
s32 itfMesMngGetWidth(s32 id, s32 message, s32 page);

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesMngSetDefaultSpeed(s32 id, u8 speed);

/* Store the optional callback address invoked without arguments during glyph building. */
void itfMesMngSetKutiCallback(s32 id, void (*func)(void));

/* Initialize the fixed window pool, load its texture and register the named
 * calculation/drawing tasks. Keep the existing task flags and callback casts. */
void itfMesMngInitializeManager(void);

/* Set manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesMngSetBitStatManager(u32 flags);

/* Clear manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesMngClearBitStatManager(u32 flags);

/* Read the manager's 16-bit flags, distinct from itfMessageFlags. */
u16 itfMesMngGetStatManager(void);

extern const char itfMesDebugLabelChange[16];

extern const char itfMesDebugLabelPosition[16];

extern const char itfMesDebugLabelRightBottom[16];

extern const char itfMesDebugLabelLeftTop[16];

s32 itfMesMngDebugProc(void);

/* Reset the three otherwise unnamed message-state words. */
void itfMesMngDebugShutdown(void);

/* Tear down installed content/resources, return the pool record and decrement
 * the pool-array-relative counter. An empty record is a no-op. */
void DestructFlow(s32 id);

void ResolveResourceAddress(itfMesResource_t *pdata);

/* Recognize MSG0/MSG1 magic only; this is not complete format validation. */
u32 CheckMessageBinary(itfMesResource_t *pdata);

itfMesBinTypeHeader_t *GetTypeHeader(instanceMes_tag *pinst, s32 index);

itfMesBinTypeHeader_t *GetTypeHeader2(itfMesResource_t *pfileheader);

u8 *GetMessagePagePtr(itfMesBinMesHeader_t *table, s32 index);

/* Called above its definition, where ee-gcc assumed int. */
frFontHandle_t *GetLastLinkFontHandle(frFontHandle_t *pfont);

void GetLastMessagePara(frFontHandle_t *pfont, message_tag *pmessage);

void RaiseMessageSpeedMax(frFontHandle_t *pfont);

void ReadytoMessage(instanceMes_tag *panel);

/* Replace a retained text-slot allocation. Nonpositive byteCount copies the
 * padded string span; positive counts copy that many bytes into a zeroed block.
 * DDS1 string mode intentionally has no memset. Keep the K&R declaration form. */
void SetVar();

void CommitVarTable(s32 *pvar);

u8 *GetSpeakerName(instanceMes_tag *pinst);

s32 GetNumSelectItem(s32 allitem, u32 mask);

frFontHandle_t *GetFontHandleUnit(u32 *list, s32 count, u32 mask, s32 x, s32 y, s32 depth);

/* Discard (to - from - 1) preceding rows when positive, retain the next
 * complete y-group and queue everything after it. Requires non-NULL input;
 * return NULL if discarding preceding rows exhausts the chain. */
frFontHandle_t *UnchainFontHandleUnit(frFontHandle_t *pfont, s32 index, s32 lines);

frFontHandle_t *GetLastLinkFontHandle(frFontHandle_t *pfont);

void GetLastMessagePara(frFontHandle_t *pfont, message_tag *pmessage);

s32 CountLineMessage(frFontHandle_t *pfont_top, frFontHandle_t *pfont);

/* Translate every linked node in text units; a NULL chain is a no-op. */
void ChangeFontPosition(frFontHandle_t *pfont, s32 dist_x, s32 dist_y);

void ChangeZBuffer(frFontHandle_t *pfont, s32 z);

void ChangeFontColor(frFontHandle_t *pfont, s32 index, s32 lines, s32 color);

void ChangeFontColorAll(frFontHandle_t *pfont, u8 color);

/* Clear each child's low color byte, then OR the unmasked input word.
 * High input bits can therefore also change the upper bytes. */
void ChangeFontAlpha(frFontHandle_t *pfont, u32 alpha);

s32 GetMessageWidth(frFontHandle_t *pfont);

s32 GetMaskedIndex(s32 index, u32 mask);

void RaiseMessageSpeedMax(frFontHandle_t *pfont);

void RaiseMessageSpeedMaxAll(frFontHandle_t *pfont);

void OpenMessageWindow(instanceMes_tag *pinst);

void OpenCheckSpeakerWindow(instanceMes_tag *pinst);

void InitializeSpeaker(Speaker_tag *pspeaker, s32 defaultpos);

void InitializeMessage(message_tag *pmessage, s32 defaultpos);

void InitializeSelect(select_tag *pselect);

void InitializeWindow(window_tag *pwindow);

void InitializeVariable(variable_tag *pvariable);

void InitializeCurCtr(curctr_tag *pcurctr, s32 isdisp);

void ForceSetCurCtr(curctr_tag *pcurctr, s16 phase, s16 ctr0, s16 ctr1);

void ReleaseAllWindow(instanceMes_tag *pinst);

void ReleaseAllVariable(variable_tag *pvariable);

char *ConvertZenkakuNumber(s32 buf, char *num);

void ResetWindowPara(window_tag *pwin, s32 type, s32 alpha);

void CalcMesFlow(instanceMes_tag *pinst);

void CalcWindow(instanceMes_tag *pinst);

void CalcMessage(instanceMes_tag *pinst);

void func_001D0518(instanceMes_tag *pinst, s32 y);

/* DDS itfUpdateSoundSelectionTransition (redthing1), adapted to SMT3's
 * 20-pixel rows and absence of the donor's sprite-transition overrides. */
void CalcSelect(instanceMes_tag *panel);

void func_001D0518(instanceMes_tag *pinst, s32 y);

s32 SelectItem(instanceMes_tag *pinst);

void SelectItemMove(select_tag *psel, s32 direct);

s32 SelectItemDefKey(select_tag *psel);

void CalcCurCtr(instanceMes_tag *pinst);

void DrawMesFlow(instanceMes_tag *pinst);

void DrawWindow(instanceMes_tag *pinst);

void DrawRetPageCursor(instanceMes_tag *pinst);

s32 CalcMsgMng_Process(void);

s32 DrawMsgMng_Process(dds3ProcessID_t *id);

void DrawMsgMng_Shutdown(void);

void DebugDrawWindowLine(itfpanel_t *meswin);

void DebugMoveWinVert(window_tag *pwin, s32 flag);

/* Step the stepper's index by pad input: one per press, ten with the fast modifier held; mirror it into the target. */
void DebugMoveWinAlpha(window_tag *pwin);

itfMesLink_t *DebugChangeInstance(itfMesLink_t *pflow);

extern char itfMesFmtInt[8];

extern char itfMesDebugLabelAlpha[8];

extern char itfMesMarkerEdit[];

extern char itfMesMarkerSelect[];

extern char *itfMesSelectionMarkers[2];

extern char itfMesMarkerEdit[8];

extern char itfMesMarkerSelect[8];

extern char itfMesDebugItemFormat[8];

extern char itfMesMarkerNone[8];

extern char itfMesDebugFadeFormat[8];

extern u8 itfMesDebugLineVertIndex[5];

extern u8 itfMesDebugLineColorIndex[5];
/* End of declarations from src/smt3/interface/itfMesManager.c */

/* Declarations from dds-decomp src/dds2/game/code_001A1B08.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#define SMT3_DEFINES_debug_tag 1

#define SMT3_DEFINES_itfMesBinMesHeader_t 1

#define SMT3_DEFINES_itfMesBinTypeHeader_t 1

#define SMT3_DEFINES_itfMesLinkControl_t 1

#define SMT3_DEFINES_itfMesLink_t 1

#define SMT3_DEFINES_itfMesManager_t 1

#define SMT3_DEFINES_itfMesResource_t 1

#define SMT3_DEFINES_itfPadState_t 1

#define SMT3_DEFINES_itfProcessShutdown_t 1

#include "dds/scr_command_timer_api.h"

/* During a command callback, return the number of earlier trace passes that
 * yielded while it was pending. The first invocation sees zero; each yielded
 * pass adds one, and a continuing or finishing handler resets the count. This
 * is callback wait state, not wall-clock time, and is valid only in the callback. */
ScrCommandYieldCount scrGetCommandTimer(void);

#include "dds/mnu.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr_command_parameter_index.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/scr_command_timer_api.h"

#include "dds/sdf_packet_list.h"

#include "dds/itf.h"

#include "dds/itf_mes_window.h"

#include "dds/itf_mes_panel_mode.h"

#include "dds/itf_mes_entry_start_api.h"

#include "dds/sdf.h"

#include "dds/itf_panel_draw.h"

#include "dds/itf_text_vector.h"

#include "dds/itf_panel_api.h"

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

extern void itfMesBuildOptionList(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex);

extern void itfMiscSetRatioRGB(DrawColorRec *output, s32 scaleX, s32 scaleY, s32 scaleZ, s32 w, const TextVector *input, s32 count);

extern TextVector itfPanelColorTemplates[];

extern TextVector D_003B4548[];

extern TextVector itfPanelGradientColorTemplate[];

extern TextVector itfPanelGradientColorPair[];

extern u8 D_004365C8[8];

extern u8 D_004365D0[8];

extern void itfMiscDrawTriangleStripPrim(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

extern u8 D_004365D8[8];

extern u8 D_004365E0[8];

extern void itfDrawThreePanelBorderBands(PanelRect *bounds, u32 *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command);

extern u32 D_003B4760[];

#include "dds/sdf_packet_alloc.h"

extern void *sdf2DMakeStdPacket(void *packet, s32 packetBytes);

extern u8 D_003B45E8[], D_003B4600[], D_003B4618[];

extern u8 D_003B4630[], D_003B4648[], D_003B4660[];

extern u8 D_004365A8[8], D_004365B0[8];

extern u8 D_003B4670[];

extern u8 D_003B4680[];

extern u8 D_004365B8[8];

extern u8 D_004365C0[8];

/* Invoke the indexed panel handler; panel and kind are trusted. */
void itfPnlDrawTag(dds_itfpanel_t *panel, sdfDrawTag_t *list);

/* Emit top/bottom vertices for four columns; collapse overlapping inner columns to the midpoint.
   Coordinates and inset remain in the renderer's native integer units. */
void CreateVertexNormal(DrawVertex *vertices, s32 x0, s32 y0, s32 x1, s32 y1);

extern s32 D_003B44D8[4];

/* Emit the outer frame grid, then the table-spaced top/bottom edge rows. */
void CreateVertexMesWin(DrawVertex *vertices, s32 x0, s32 y0, s32 x1, s32 y1);

/* Emit the outer frame grid, then the close top/bottom edge rows. */
void CreateVertexSelWin(DrawVertex *vertices, s32 x0, s32 y0, s32 x1, s32 y1);

/* Write the rectangle corners in top-left, top-right, bottom-right, bottom-left order. */
void CreateVertexPlain(DrawVertex *v, s32 x0, s32 y0, s32 x1, s32 y1);

/* Write two rows of three columns; the middle column is the right edge minus a fixed strip width. */
void CreateVertexLeftFade(DrawVertex *v, s32 x0, s32 y0, s32 x1, s32 y1);

/* Write four fixed horizontal columns with inset top/bottom coordinates.
   x0 and x1 are intentionally unused; do not replace the fixed horizontal positions. */
void CreateVertexFacility(UiSpriteBandPayload *base, s32 x0, s32 y0, s32 x1, s32 y1);

/* Populate RGBA rows at bytes 0x40/0x50; the first row is transparent.
   The existing vertex-typed view addresses the same color records as the quad consumer. */
void CreateColorNormal(DrawVertex *panelData, s32 red, s32 green, s32 blue, s32 alpha);

/* Write the two color rows, then scale six template rows; the first fourth component is one. */
void CreateColorMesWin(u8 *base, u32 red, u32 green, u32 blue, u32 alpha);

/* Write the two color rows, then scale four template rows; the first fourth component is one. */
void CreateColorSelWin(u8 *base, u32 red, u32 green, u32 blue, u32 alpha);

/* Set the base color, one alpha-scaled gradient row, and two rows at full color scale. */
void CreateColorPlain(u8 *base, u32 red, u32 green, u32 blue, u32 alpha);

/* Set the four color words at byte 0x20, consumed as a color record by the quad path.
   Preserve the existing vertex-typed access and signed arguments. */
void CreateColorPera1(DrawVertex *panelData, s32 red, s32 green, s32 blue, s32 alpha);

/* Populate RGBA rows at bytes 0x30/0x40; the first row is transparent.
   The six-index quad path consumes its color rows from this same base offset. */
void CreateColorLeftFade(DrawVertex *panelData, s32 red, s32 green, s32 blue, s32 alpha);

/* Set both panel colors to the same blue tint with zero alpha. */
void CreateColorFacility(UiSpriteBandPayload *panel);

extern u8 D_003B45C8[];

extern u8 D_003B45D8[];

/* Draw the panel's three flat quads from one vertex/color buffer; each quad uses its own 4-byte index rows. */
void DrawNormal(dds_itfpanel_t *panel, sdfDrawTag_t *command);

/* Update seven alpha words, then emit three wide and six side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawMesWin(dds_itfpanel_t *panel, sdfDrawTag_t *command);

/* Update five alpha words, then emit three wide and two side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawSelWin(dds_itfpanel_t *panel, sdfDrawTag_t *command);

void DrawPlain(dds_itfpanel_t *panel, sdfDrawTag_t *command);

/* Update the first color row's alpha and emit one indexed quad. */
void DrawPera1(dds_itfpanel_t *panel, sdfDrawTag_t *command);

/* Bracket the six-index panel draw with table-state two and its native reset to zero. */
void DrawLeftFade(dds_itfpanel_t *panel, sdfDrawTag_t *command);

/*BEGIN itfDrawFramedPanelWithTextureStrips*/
extern u32 D_003B4690[];

extern DrawColorRec D_003B4700;

extern DrawColorRec D_003B4710;

/* Draw the framed panel: two bordered bands, then six texture-bound strips whose
 * rectangles are rebuilt from the live panel bounds. */
void itfDrawFramedPanelWithTextureStrips(dds_itfpanel_t *panel, sdfDrawTag_t *command);

extern DrawColorRec D_003B4720;

extern DrawColorRec D_003B4730;

/* Bind the texture for the left cap, then draw the center and reversed right
 * cap. Insets are captured before the first submission; Y bounds are live. */
void DrawSpkWin2(dds_itfpanel_t *panel, sdfDrawTag_t *command);

extern u8 D_003B4740[];

extern u8 D_003B4750[];

extern DrawColorRec D_003B4470;

extern DrawColorRec D_003B4480;

extern DrawColorRec D_003B4490;

extern DrawColorRec D_003B44A0;

/* Draw the three inset flat quads, then split a tall texture into top, stretch
 * and bottom bands. The short path uses the live rectangle in the panel. */
void DrawFacility(dds_itfpanel_t *panel, sdfDrawTag_t *command);

/* Copy bounds and draw the tinted border; alpha uses the native signed 103/128 scale. */
void itfDrawTintedPanelRect(dds_itfpanel_t *panel, sdfDrawTag_t *command);

/* Append two GS A+D state writes: TEST_1 (0x47), then ALPHA_1 (0x42). */
void DrawTestAlphaPrim(sdfDrawTag_t *list);

/* Capture the script window, reset its held queue, and switch to page three.
   Invalid windows still return the native command-complete value of one. */
s32 itfPanelAcquireHold(void);

/* Clean up the active held window and reset its queue; an inactive hold is a no-op. */
s32 itfPanelReleaseHold(void);

/* Queue script words in native second/first/third field order; first and third narrow to s16.
   Inactive/full queues are ignored with status one; negative queuedCount is not checked. */
s32 itfCommandQueueHeldPanelEntry(void);

s32 itfStepHeldPanelMessages(void);

extern void dds3SetFilter2Para(u32);

extern void dds3SetFilter2RGBA(u32);

extern void dds3Filter2On(s32);

extern void dds3Filter2Off(s32);

extern s32 D_00438F28;

/* Configure the fade on timer zero, wait thirty ticks, then consume a negative option status.
   Keep both timer reads: setup tests a second read, while later comparisons use the cached one. */
s32 itfCommandSelectOptionWithFade(void);
/* End of declarations from src/dds2/game/code_001A1B08.c */

/* Declarations from dds-decomp src/dds2/interface/itfMesManager.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "sdk_libc.h"

#include "dds/fr_font.h"

#include "dds/fr_font_context.h"

#include "dds/itf.h"

#include "dds/itf_text_slots_api.h"

_FRQ *MessageRequestEx(s32 x, s32 y, s32 channel0, s32 channel1,
    s32 channel2, s32 channel3, const u8 *encodedText, _FRQ *sub);

/* Test the active text-interface flags and return the requested bits. */
u32 frGetMessageResult(u32 mask);

void frSetDrawPosition(struct _FRQ *glyph, s32 x, s32 y);

/* Replace each child render word along the borrowed glyph chain. */
void frSetFontColor(struct _FRQ *parentNode, u32 colorWord);

/* Update the first-option byte of every child on this glyph chain. */
void frSetFontColorNo(struct _FRQ *glyph, u8 firstOption);

#include "dds/sdk_ps2.h"

#include "dds/btl_enc_bg_entry.h"

#include "dds/common.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/itf_mes_entry_start_api.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/itf.h"

#include "dds/itf_message_manager_task.h"

#include "dds/itf_option_bit_index_api.h"

#include "dds/snd_pad.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_resource.h"

#include "dds/sdf_pac_reloc.h"

#include "dds/fr_font_temporary_entry_api.h"

#include "dds/itf_mes_state_api.h"

#include "dds/itf_text_slots_api.h"

#include "dds/itf_mes_window.h"

#include "dds/itf_mes_panel_mode.h"

#include "dds/itf_panel_api.h"

#include "dds/fld_area_work.h"

#include "dds/fr_font_measure.h"

#include "dds/eff_resource_slots.h"

#include "dds/sdf_chip.h"

#include "dds/kwln.h"

#include "dds/kwln_task_create.h"

#include "dds/pcp_vu0.h"

#include "dds/btl_scene_fade.h"

#include "dds/btl_resource.h"

#include "dds/eff.h"

#include "dds/itf_panel_draw.h"

#include "dds/btl_state.h"

#include "dds/btl_ui.h"

#include "dds/sdf.h"

#include "dds/sdf_projection.h"

#include "dds/btl_action.h"

#include "dds/scr.h"

#include "dds/dat_state.h"

#include "dds/mnu_result.h"

#include "dds/dat_command.h"

#include "dds/sdf_sif_command.h"

#include "dds/mdl_resource_table.h"

#include "dds/kwln_task_lifecycle.h"

extern u32 itfMessageFlags;

typedef mesflow_tag ItfMesWindowRec;

extern ItfMesDebugState D_003B4770;

void fldNoopWithMode(s32 mode);

void itfMesSetTextSlotFromValue(ItfMesWindowIndex window, s32 slotIndex, s32 value, s32 selector);

extern void frFontCenterGlyphChainAt(s32 x, s32 y, _FRQ *glyphChain);

void itfMesInitializePanelPlacementSprite(dds_instanceMes_tag *panel);

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

/* Return the loaded message-window texture resource, not a window index. */
sdfTexHandle_t *itfMesGetWindowTexture(void);

/* Set requested bits in the separate message-global word. */
void itfMesSetFlags(u32 mask);

/* Clear requested bits in the separate message-global word. */
void itfMesClearFlags(u32 mask);

void itfMesBuildEntryGlyph(dds_instanceMes_tag *mes);

/* Install an entry's table into the window, rebuild its text glyphs and the
 * selected-item caption, then mark the entry block active. */
ItfMesEntryStartResult itfMesStartEntry(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex, s32 itemIndex);

extern s32 itfInitTextDrawArgs(const u8 *encodedText, _FRQ *sub);

/* Cache the selected table and narrowed item index, then initialize its text.
 * Empty tables return 0; nonempty tables return the text initializer's result. */
s32 itfMesSelectTableItemText(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex, s32 itemIndex);

/* Build the option panel around the widest measured row, using this game's
 * fixed row spacing. Preserve the existing panel creation/layout order. */
void itfMesBuildOptionFrame(dds_instanceMes_tag *mes);

extern const char *datUnitNames;

extern const char *D_00435E4C;

extern const char *D_00435E5C;

extern const char *D_00435E60;

extern const char *datSkillNames;

extern char D_003A41A8[][0x20];

extern char D_003A47E8[][0x20];

extern char *D_00386048[];

extern char D_004365F0[];

extern void itfConvertText(char *dst, const char *src);

extern u16 *txtFormatNumberU16(s32 value, u16 *dst);

/* Copy one of the built-in interface strings into a window replacement slot. */
void itfMesSetTextSlotFromValue(ItfMesWindowIndex window, s32 slotIndex, s32 value, s32 selector);

extern void itfMesConfigurePanelBoundsAndFadeLimit(ItfMesBlkA4 *blk, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit);

extern char *D_003B4990[];

extern char *D_00436600[2];

extern char D_00436608[];

extern char D_00436610[];

extern char D_00436618[];

extern char D_00436620[];

extern char D_00436628[];

extern SdfPoolNode kwlnPositionedTextSurface;

extern ItfMesWindowRec D_00452960[];

/* Return the entry's item-address table or branch sentinel 1; index unchecked. */
ItfMesEntryItemsResult itfMesGetWindowEntryItems(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex);

/* Read the installed sub-resource's entry count without slot/resource validation. */
u32 itfMesGetEntryCount(ItfMesWindowIndex window);

extern s32 frFontCountGlyphBands(_FRQ *glyph);

extern u32 itfGetTextLipsUnitId(void);

extern u32 itfGetTextLipsMotionId(void);

/* Rebuild colored entry glyphs, synchronize interface flags/hooks and cache
 * the span count. Existing span helpers require a valid created chain. */
void itfMesBuildEntryGlyph(dds_instanceMes_tag *mes);

/* Append text for each clear mask bit. Masked items are still consumed, while
 * y advances only for built text. Return NULL if nothing was built. */
_FRQ *itfMesBuildNodeRows(const u32 *items, s32 itemCount, u32 mask, s32 x, s32 y, s32 renderValue);

/* Resolve the zero-based visible-option ordinal to its clear bit, or 32 if
 * absent. Keep the predecrement; the ordinal itself is not validated. */
ItfMesOptionBitIndex itfMesFindOptionBitIndex(ItfMesVisibleOptionOrdinal optionOrdinal, u32 optionMask);

/* Enable parent contexts whose first child's tested flag is zero.
 * NULL chain is allowed, but every visited node must have a first child. */
void itfMesEnableUnflaggedNodeContexts(_FRQ *glyph);

extern char *D_00436600[2] __attribute__((section(".sdata")));

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

extern const UiQuadColor D_00414D50;

#include "dds/sdf_packet_alloc.h"

extern void itfPnlDraw(dds_itfpanel_t *sprite, SdfPoolNode *surface);

/* Enable context rendering for every font object in the linked chain. */
void frFontEnableNodeContextModes(_FRQ *fontObject);

void itfMesInitializePanelPlacementSprite(dds_instanceMes_tag *panel);

void itfInitializeCursorResetState(ItfMesBlk40 *cursor);

extern void itfMesConfigurePanelBoundsAndFadeLimit(ItfMesBlkA4 *block, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit);

void itfResetWindowResourceBlock(ItfMesBlkA4 *block);

void btlReleaseEffectResourceHandles(dds_instanceMes_tag *effect);

u16 *txtFormatNumberU16(s32 number, u16 *out);

/* Select the message panel's bounds by layout mode (2 is the wide panel) and
 * reset its fade channels; nonpositive limits default to 0x80. */
void itfMesConfigurePanelBoundsAndFadeLimit(ItfMesBlkA4 *block, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit);

void itfMesUpdatePanelFades(dds_instanceMes_tag *panel);

void itfUpdateSoundSelectionTransition(dds_instanceMes_tag *panel);

void itfMesAdvanceEntryText(dds_instanceMes_tag *mes);

void itfUpdateBattleDisplayAndFadeIndicator(dds_instanceMes_tag *panel);

void itfMesUpdatePanelFades(dds_instanceMes_tag *panel);

void itfMesAdvanceEntryText(dds_instanceMes_tag *mes);

extern ItfMesOptionBitIndex itfMesFindOptionBitIndex(ItfMesVisibleOptionOrdinal optionOrdinal, u32 optionMask);

extern void itfMesShiftPanelVertically(dds_instanceMes_tag *panel, s32 dy);

void itfUpdateSoundSelectionTransition(dds_instanceMes_tag *panel);

void itfMesShiftPanelVertically(dds_instanceMes_tag *panel, s32 dy);

extern void itfMesRenderActivePanelSprites(dds_instanceMes_tag *);

extern void itfDrawSoundSelectorFadeLayers(dds_instanceMes_tag *);

extern void itfDrawSoundSelectorOptionPanel(dds_instanceMes_tag *);

extern void itfDrawSelectedSoundRow(dds_instanceMes_tag *);

void itfUpdateSoundSelectorPanel(dds_instanceMes_tag *panel);

void itfMesRenderActivePanelSprites(dds_instanceMes_tag *panel);

extern DrawColorRec D_003B49B8[];

/* Draw the selected sound row and its expanding fade outline. */
void itfDrawSelectedSoundRow(dds_instanceMes_tag *panel);

extern u8 D_003B49F8[];

extern u8 D_003B49E8[];

extern s32 D_003B4A08[];

/* Draw the sound selector frame, its fade layer and the expanding timer outline. */
void itfDrawSoundSelectorFadeLayers(dds_instanceMes_tag *panel);

extern SdfPoolNode D_00380708;

extern s32 D_003B4A18[];

extern u8 D_00436630[5];

extern u8 D_00436638[5];

void itfQueueOffsetTexturedRect(s32 *bounds, s32 *region, s32 x, s32 y,
                   s32 alpha, sdfTexHandle_t *texture, sdfDrawTag_t *command);

extern const s32 D_00414D60[12][4];

extern s16 D_00438F34;

extern s32 D_00438F30;

extern s16 D_00438F38;

extern SdfPoolNode D_003805A8;

extern const s32 D_00414E20[7][4];

extern void sdfSubmitGsTestOneRegisterPacket(u64 data, u32 kind);

extern void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind);

void itfMesDrawWindowBackground(s32 mode);

extern const s32 D_00414E20[7][4] __attribute__((aligned(8)));

extern s32 D_00438F2C;

extern u8 D_00438F36;

extern s16 D_00452E60[8];

void itfUpdateSoundSelectorOptions(void);

extern s8 D_0037F531[];

/* Start the window entry, then poll completion and restore its normal flags. */
s32 itfMesScriptStartEntryAndWait(void);
/* End of declarations from src/dds2/interface/itfMesManager.c */

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00186970);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_001869B8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00186A00);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", itfCommandSelectOptionWithFade);

/* Return the option-block signed status word, not the separate entry status byte. */
s16 itfMesMngGetSelPhase(s32 msgid) {
    return pinst[msgid].mes->select.phase;
}

void itfMesMngSetSelPhase(s32 msgid, s16 phase)
{
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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00186FD0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00187018);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00187088);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00187488);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00187498);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_001874B0);

/* Destroy an installed nonnegative window slot; no upper-bound validation. */
void itfMesMngRelease(s32 id) {
    if (id >= 0 && pinst[id].mes != NULL) {
        DestructFlow(id);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_001875F8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00187658);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesSelectTableItemText);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_001879C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00187AD8);

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

s16 func_r1_00188010(s32 id) {
    return pinst[id].mes->select.select;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00188030);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00188050);

u8 *itfMesMngGetVar(s32 id, s32 index) {
    return (u8 *)pinst[id].mes->variable.pptr[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00188098);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_001883C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00188420);

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

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A600);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A610);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A620);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A630);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngDebugProc);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00188B38);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesGetWindowEntryItems);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesGetEntryCount);

u8 *GetMessagePagePtr(itfMesBinMesHeader_t *table, s32 index)
{
    s32 count = table->page;

    if (index < 0 || index >= count) {
        return NULL;
    }
    return EE_ADDR_AS(u8 *, table->addr[index]);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00188D00);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_001890A8);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_001892F0);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_001895B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_001895F8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189638);

INCLUDE_ASM(const s32, "interface/itfMesManager", OpenCheckSpeakerWindow);

void InitializeSpeaker(Speaker_tag *pspeaker, s32 defaultpos) {
    if (defaultpos != 0) {
        pspeaker->x = 0x3D0;
        pspeaker->y = 0x988;
    }
    pspeaker->pfont = 0;
    pspeaker->num = 0xffff;
}

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189888);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_001898C8);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189928);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189978);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189980);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189A58);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189AE0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189B28);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189B68);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189D58);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189D68);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_00189FF8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018A2E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018A3C0);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018A658);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018A978);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018AB68);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018AB98);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018ABB0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018AE00);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018B060);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018B178);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018B5C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018B5D8);

s32 CalcMsgMng_Process(void) {
    itfMesLink_t *node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        CalcMesFlow(node->state);
        node = node->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMsgMng_Process);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMsgMng_Shutdown);

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
    UiQuadColor color = D_00414D50;

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018C030);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018C7B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_r1_0018CAD0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A6A0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A730);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A760);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A7B0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A7D0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A7E0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A810);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A820);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A830);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A840);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_r1_0044A910);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C258);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C260);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C268);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C278);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C280);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C288);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C290);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C298);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C2A0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C2A8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_r1_0046C2B0);

