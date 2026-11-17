#ifndef ITF_MES_MANAGER_H
#define ITF_MES_MANAGER_H

#include "common.h"
#include "font.h"
#include "interface.h"
#include "sdf_resource.h"

/* A message window as the manager keeps it. stat's bits 3-5 follow its
 * selection: 0x10 requested, 0x18 drawing, 0x20 taking the pad. */
typedef struct itfMesBinMesHeader_t itfMesBinMesHeader_t;
typedef struct itfMesResource_t itfMesResource_t;
typedef struct Speaker_tag Speaker_tag;
typedef struct message_tag message_tag;
typedef struct defkey_tag defkey_tag;
typedef struct select_tag select_tag;
typedef struct window_tag window_tag;
typedef struct variable_tag variable_tag;
typedef struct curctr_tag curctr_tag;
typedef struct instanceMes_tag instanceMes_tag;

struct Speaker_tag {
    u32 x;
    u32 y;
    frFontHandle_t *pfont;
    u16 num;
    u8 pad0E[2];
};

struct message_tag {
    u32 x;
    s32 y;
    itfMesBinMesHeader_t *table;
    frFontHandle_t *pfont;
#ifdef VERSION_RAIDOU2
    /* Release keeps the previous text's glyph chain here until the next one and
     * sets limit to 128; InitializeMessage leaves limit alone. */
    frFontHandle_t *pfontPrev;
    s32 limit;
#endif
    s8 phase;
    u8 textInterfaceMask; /* 0x11: result of itfTestTextInterfaceMask(3). */
    u8 color[4];
    s16 lines;
    s16 page;
    s16 maxpage;
};

struct defkey_tag {
    s16 key;
    s16 index;
};

struct select_tag {
    u32 x;
    u32 y;
    frFontHandle_t *pfont;
    u32 mask;
    s16 phase;
    s16 select;
    s16 select_imm;
    s16 lines;
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    s32 widestRowX;
    s32 widestRowWidth;
#endif
    s16 glyphColorIntensity;
    s16 numdef;
    defkey_tag defkey[15];
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    u8 unk60[4];
    u16 selState[2]; /* itfMesMngRequestSelect sets 0 and 4, the release clears both */
#endif
};

/* The speaker frame and message panel occupy +0x9C/+0xA0 (Raidou 1 +0xA8/+0xAC,
 * Raidou 2 +0xB4/+0xB8). */
struct window_tag {
    itfpanel_t *spk;
    itfpanel_t *mes;
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    itfpanel_t *overlay;
#endif
    s32 mes_offset[4];
    s32 red;
    s32 green;
    s32 blue;
    s32 fadeLimit;
};

struct variable_tag {
    u32 pptr[0x20];
    sdfMemHandle_t *pmh[0x20];
};

/* Native 0xC-byte fade record shared by message windows and sound UI state. */
struct curctr_tag {
    u8 isdraw;
    u8 pad1;
    s16 phase;
    s16 ctr0;
    s16 ctr1;
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    /* Field names from the HD build; the four floats are PS2 only. */
    s16 frame;
    s16 lf_wait;
    u16 lf_after_wait;
    s16 cut_wait;
    u32 ctr_key;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
#else
    union {
        u32 ctr_key;
        struct {
            u16 ctr_keyLow;
            u16 ctr_keyHigh;
        };
    }; /* 0x08: entry advances count ctr_key before triangle skip. */
#endif
};

struct instanceMes_tag {
    u32 stat;
    itfMesResource_t *pdata;
    u8 *pext; /* 0x08: relocated font resource bytes from the MSG1 trailing entry. */
    s32 z;
    s16 otindex;
    s16 type;
    Speaker_tag speaker;
    message_tag message;
#ifdef VERSION_RAIDOU2
    u8 unk48[4];
#endif
    select_tag select;
    window_tag window;
    variable_tag variable;
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    curctr_tag curctr[2]; /* [1].isdraw bit 1: DrawMesFlow queues the window packets. */
#else
    curctr_tag curctr;
#endif
    addr_t func_kuti;
};
#if defined(VERSION_RAIDOU1)
SIZE_CHECK(instanceMesSizeCheck, instanceMes_tag, 0x220); /* itfMesMngInitialize */
#elif defined(VERSION_RAIDOU2)
SIZE_CHECK(instanceMesSizeCheck, instanceMes_tag, 0x22C);
#endif

/* pinst[window].mes: NULL while the window is free. */
typedef struct { instanceMes_tag *mes; u8 unk04[0x10]; } itfMesInstance_t;

s32 MesMngSelect(void);
s32 itfCommandQueueHeldPanelEntry(void);
u32 itfMesMngCloseSelectWindow(void);
s32 itfMesMngCloseWindow(void);
s32 itfMesMngIgnoreConfig(void);
s32 itfMesMngInitialize(void *data);
void itfMesMngMarkBitStat(s32 id, u32 flag);
void itfMesMngClearBitStat(s32 id, u32 flag);
void itfMesMngSetSelect(s32 id, s32 selno);
s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index);
s32 itfMesMngMaskSelect(void);
s32 itfMesMngMessage(void);
u32 itfMesMngOpenSelectWindow(void);
s32 itfMesMngOpenWindow(void);
void itfMesMngRelease(s32 id);
s32 itfMesMngSelect(void);
s32 itfMesMngSelectDefKey(void);
void itfMesMngSetKutiCallback(s32 id, void (*func)(void));
s32 itfMesMngSetMesPosDefault(void);
void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);
s32 itfMesMngSetMesPositionScript(void);
void itfMesMngSetSelPhase(s32 msgid, s16 phase);
s16 itfMesMngGetSelPhase(s32 msgid);
s8 itfMesMngGetPhase(s32 msgid);
s32 itfMesMngGetSelectNo(s32 msgid);
s32 itfMesMngChkWaitTag(s32 msgid);
s16 itfMesMngGetSelect(s32 id);
void itfMesMngRequestSelectRelease(s32 id);
void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker);
void itfMesMngSetVarImmediate(s32 id, u32 index, const char *pstr);
s32 itfMesMngSetVarScr(void);
s32 itfMesMngValidateNext(void);
s32 itfMesMngWindowType(void);
s32 itfMesMngWindowTypeAlpha(void);
s32 itfMesMng_SEL_SELNO(void);
s32 itfPanelAcquireHold(void);

enum {
    ITF_MES_PANEL_MODE_STANDARD = 0,
    ITF_MES_PANEL_MODE_STANDARD_VARIANT = 1,
    ITF_MES_PANEL_MODE_WIDE = 2,
    ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION = 3
};


void CalcMesFlow_itf(instanceMes_tag *pinst);
void CalcMessage(instanceMes_tag *pinst);
void CalcSelect(instanceMes_tag *pinst);
void CalcWindow(instanceMes_tag *pinst);
void ReadytoMessage(instanceMes_tag *pinst);
void itfCallChangeFontPosition(instanceMes_tag *pinst, s32 y);
u16 *ConvertZenkakuNumber(s32 buf, u16 *num);
void DebugDrawWindowLine(itfpanel_t *meswin);
void DebugMoveWinAlpha(window_tag *pwin);
void DebugMoveWinVert(window_tag *pwin, s32 flag);
s32 DrawMesFlow(instanceMes_tag *pinst);
void DrawWindow(instanceMes_tag *pinst);
void FadeOutWindow(void);
void itfLeaf13(s32 power, s32 timer);
frFontHandle_t *GetFontHandleUnit(u32 *list, s32 count, u32 mask, s32 x, s32 y, s32 depth);
void GetLastMessagePara(frFontHandle_t *pfont, message_tag *pmessage);
void InputPasswdDraw(void);
/* State shared by itfInputPasswd and InputPasswdDraw. */
extern s32 InputPasswdDrawWork;
extern s32 InputPasswdDrawWork2;
extern s16 itfCallGrAllocPacketBuff3Work;
extern u8 itfInputPasswdWork;
extern s16 itfCallGrAllocPacketBuff3Work2;
void OpenCheckSpeakerWindow(instanceMes_tag *pinst);
void OpenMessageWindow(instanceMes_tag *pinst);
void RaiseMessageSpeedMax(frFontHandle_t *pfont);
void RaiseMessageSpeedMaxAll(frFontHandle_t *pfont);
void ReleaseAllWindow_itf(instanceMes_tag *pinst);
void itfMesMngChangeWindowTypeSub(s32 id, s32 type, s32 alpha);
s32 SelectItem(instanceMes_tag *pinst);
void SelectItemMove(select_tag *psel, s32 direct);
s32 itfInputPasswd(void);
void itfMesMngChangeWindowType_itf(s32 id, s32 type, s32 alpha);
void itfMesMngClearCursor(s32 id);
void itfMesMngSetPhase(s32 msgid, s8 phase);
s32 itfMesMngGetMesCurrentPage(s32 id);
s32 itfMesMngGetMesPage(s32 id);
void itfMesMngInitializeManager(void);
void itfMesMngMsgWndCls(s32 msgid);
void itfMesMngRequestSelect_itf(s32 id, s32 select);
void itfMesMngSetVar_itf(s32 id, s32 index, s32 num, s32 type);

s32 itfMesMngRequestMessage_itf(s32 window, s32 entryIndex, s32 itemIndex);
s32 itfMesSelectTableItemText(s32 window, s32 entryIndex, s32 itemIndex);
u32 itfGetMessageType(s32 window, s32 entryIndex);
sdfTexHandle_t *itfMesGetWindowTexture(void);
/* Line colour DebugDrawWindowLine outlines a message window with. */
extern itfColor_t DebugDrawWindowLineData;
s32 itfPanelReleaseHold(void);

#endif
