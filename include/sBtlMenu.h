#ifndef SBTLMENU_H
#define SBTLMENU_H

#include "common.h"
#include "itf.h"
#include "kernel.h"

struct SBTLUNIT_WORK;
struct sdfMemHandle_t;

#ifdef VERSION_RAIDOU2
/* Battle menu process data (0x48 bytes in R2, only the start fields are read). */
typedef struct sBtlMenu_t {
    struct sdfMemHandle_t *hMem; /* 0x00 */
    dds3ProcessID_t *drawPID;    /* 0x04 */
    dds3ProcessID_t *menuPID;    /* 0x08 */
    dds3ProcessID_t *listPID;    /* 0x0C */
    dds3ProcessID_t *charPID;    /* 0x10 */
    dds3ProcessID_t *nakamaPID;  /* 0x14 */
    u8 step;                     /* 0x18 */
    u8 flag;                     /* 0x19 */
    s16 count;                   /* 0x1A */
    s32 menuID;                  /* 0x1C */
    s32 cntAnalyze;              /* 0x20 */
    s32 type;                    /* 0x24 */
    u8 pad28[0x48 - 0x28];
} sBtlMenu_t;
SIZE_CHECK(sBtlMenu_t_size, sBtlMenu_t, 0x48);
extern const char sbtlStrSbtlmenuMain[];
extern const char sbtlStrSbtlmenuDraw[];
extern dds3ProcessID_t *ProcessCheckData;
extern u8 sbtlFuncEscapeData;
s32 sbtlFuncDecideItem_sbtlMenu(dds3ProcessID_t *id);
s32 sbtlDrawProcess_sbtlMenu(dds3ProcessID_t *id);
s32 ProcessCheck(void);
void sbtlCallProcEnd(sBtlMenu_t *wk);
void IsStart(s32 type);
void sbtlMenuOpen(void);
/* Battle menu strings by menu index. */
extern char *sbtlCallMenuClearMenuAllData[];
void ChkEnableDecideEscape(sBtlMenu_t *wk);
u8 sbtlCheckProcEnd(sBtlMenu_t *wk);
s32 sbtlCheckTalkSelectTarget(struct SBTLUNIT_WORK *unit);
SBTLMENU_ENABLETALK sbtlCheckEnableTalk(struct SBTLUNIT_WORK *unit);
/* Node of each menu select ID for steps 3 to 14. */
extern SBTLMENU_NODE sbtlGetNodeData[16];
SBTLMENU_NODE sbtlGetNode(void);
extern char sbtlData173[]; /* "" */
dds3ProcessID_t *sbtlInitMenuPID(u8 a_max);
void sbtlBtlCallEndSub(void);
void sbtlMenuClose(void);
void StopBattle(void);
void sbtlCallEnd(sBtlMenu_t *wk);
s32 sbtlFuncStart(void);
s32 sbtlFuncMeirei(void);
s32 sbtlFunc23(void);
s32 sbtlCallGetProcessData48(void);
s32 sbtlFuncEscape(void);
s32 sbtlFuncTalk(void);
/* Help text banks by sbtlHelpEx type. */
extern struct frTextBank_t *cmpCallGetProcessData12Data;
extern struct frTextBank_t *sbtlData165;
extern struct frTextBank_t sbtlData131;
extern struct frTextBank_t sbtlData132;
extern struct frTextBank_t sbtlData133;
extern struct frTextBank_t sbtlData134;
#endif
#ifndef VERSION_RAIDOU2
/* Battle menu process data (0x20 bytes in R1). */
typedef struct sBtlMenu_t {
    struct sdfMemHandle_t *hMem; /* 0x00 */
    dds3ProcessID_t *drawPID;    /* 0x04 */
    dds3ProcessID_t *menuPID;    /* 0x08 */
    dds3ProcessID_t *listPID;    /* 0x0C */
    dds3ProcessID_t *charPID;    /* 0x10 */
    u8 step;                     /* 0x14 */
    u8 flag;                     /* 0x15 */
    s16 count;                   /* 0x16 */
    u8 pad18[0x4];
    s32 cntAnalyze;              /* 0x1C */
} sBtlMenu_t;
SIZE_CHECK(sBtlMenu_t_size, sBtlMenu_t, 0x20);
#endif

void ProcessStart_sbtlMenu(void);
s32 cmpUpdateSword(void);
struct SBTLUNIT_WORK *fldCallMenuProcessCheck(void);
s32 sbtlCallMemFreeDelayedDirect(void);
void sbtlCampSubSpr(void);
void sbtlCmndCntST(void);
s32 sbtlDrawProcess(dds3ProcessID_t *PID);
sBtlMenu_t *sbtlFuncKatana(void);
s32 sbtlIsMenuStartScript(void);
void sbtlSetMenuImage(dds3ProcessID_t *PID);
s32 sbtlCallGetProcessData53(dds3ProcessID_t *id);
s32 sbtlCallGetProcessData56(dds3ProcessID_t *id);
s32 cmpSelectUpdate(sBtlMenu_t *menu);
void sbtlCallMenuSetRootMenu(sBtlMenu_t *menu);
void sbtlCallMenuSetFlagMenu(sBtlMenu_t *menu);
void sbtlCallMenuProcessEnd(sBtlMenu_t *menu);
s32 sbtlCallMenuProcessCheck(sBtlMenu_t *menu);
void effPCPAnalyzeBEDDelete(sBtlMenu_t *menu);
dds3ProcessID_t *sbtlFuncDecideItem(u8 num);
s32 sbtlCallGetProcessData46(void);
s32 sbtlCallGetProcessData47(void);
s32 sbtlCallGetProcessData49(void);
s32 sbtlCallGetProcessData50(void);
s32 sbtlCallGetProcessData55(void);
s32 cmpSetSkillListHd(void);
s32 sbtlFuncItem(void);

extern char sbtlFuncDecideItemData[];
/* Menu labels: [0] title, [1..7] root items, [8] cancel, [9..14] orders; the split labels [7] and [8..15] apart. */
extern char *sbtlCallMenuSetRootMenuData[];
extern dds3ProcessID_t *ProcessCheckData;
extern struct sdfMemHandle_t *sbtlData118;
extern struct itfSprList_t sbtlSetMenuImageData[];
extern struct itfSprList_t sbtlSetMenuImageData2[];
extern struct itfSprList_t sbtlSetMenuImageData3[];
extern struct itfSprList_t sbtlSetMenuImageData4[];
#ifdef VERSION_RAIDOU2
extern struct itfSprList_t cmpPartyAnimFuncData[];
extern struct itfSprList_t cmpPartyAnimFuncData2[];
extern struct itfSprList_t cmpPartyAnimFuncData3[];
extern struct itfSprList_t cmpPartyAnimFuncData4[];
#endif

#endif
