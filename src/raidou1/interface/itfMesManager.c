#include "common.h"

/* Declarations from smt3-decomp src/smt3/interface/itfMesManager.c */
#include "interface.h"

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

/* The message instances are the state words of itfMesMngManager.nodes. */
SYMBOL_AT_REGION(itfMesInstance_t, pinst, itfMesMngManager, 0x2C);

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

void itfMesMngSetKutiCallback(s32 id, void (*func)(void));

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

extern u8 *DDS3_GBWK;

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
/* End of declarations from src/smt3/interface/itfMesManager.c */

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00186970);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001869B8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00186A00);

s8 itfMesMngGetPhase(s32 msgid)
{
    return pinst[msgid].mes->message.phase;
}

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 msgid, s8 phase) {
    pinst[msgid].mes->message.phase = phase;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00186A68);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00186AC8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00186B20);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00186BC8);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00186DA0);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00186FD0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187018);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187088);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetMesPosDefault);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187478);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187488);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187498);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001874B0);

/* Destroy an installed nonnegative window slot; no upper-bound validation. */
void itfMesMngRelease(s32 id) {
    if (id >= 0 && pinst[id].mes != NULL) {
        DestructFlow(id);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001875F8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187658);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187840);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001879C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187AD8);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187D80);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187DF0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00187E80);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00188010);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00188030);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00188050);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetVar);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00188098);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001883C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00188420);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSelectDefKey);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetWidth);

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesMngSetDefaultSpeed(s32 id, u8 speed) {
    pinst[id].mes->message.color[3] = speed;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetKutiCallback);

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

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A600);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A610);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A620);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A630);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngDebugProc);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00188B38);

INCLUDE_ASM(const s32, "interface/itfMesManager", DestructFlow);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00188C70);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00188CA8);

u8 *GetMessagePagePtr(itfMesBinMesHeader_t *table, s32 index)
{
    s32 count = table->page;

    if (index < 0 || index >= count) {
        return NULL;
    }
    return EE_ADDR_AS(u8 *, table->addr[index]);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00188D00);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001890A8);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001892F0);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001895B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001895F8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189638);

INCLUDE_ASM(const s32, "interface/itfMesManager", OpenCheckSpeakerWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeSpeaker);

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeMessage);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189888);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001898C8);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189928);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189978);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189980);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189A58);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189AE0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189B28);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189B68);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189D58);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189D68);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_00189FF8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018A2E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018A3C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", SelectItemMove);

INCLUDE_ASM(const s32, "interface/itfMesManager", SelectItemDefKey);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018A658);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018A978);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018AB68);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018AB98);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018ABB0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018AE00);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018B060);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018B178);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018B5C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018B5D8);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", DebugChangeInstance);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018BF28);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018C030);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018C7B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0018CAD0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A6A0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A730);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A760);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A7B0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A7D0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A7E0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A810);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A820);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A830);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A840);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_0044A910);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C258);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C260);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C268);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C278);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C280);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C288);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C290);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C298);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C2A0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C2A8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0046C2B0);

