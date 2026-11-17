#ifndef SBTLMENU_H
#define SBTLMENU_H

#include "common.h"
#include "kernel.h"

struct SBTLUNIT_WORK;
struct sdfMemHandle_t;

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

void ProcessStart_sbtlMenu(void);
s32 cmpUpdateSword(void);
struct SBTLUNIT_WORK *fldCallMenuProcessCheck(void);
s32 sbtlCallMemFreeDelayedDirect(void);
void sbtlCampSubSpr(void);
void sbtlCmndCntST(void);
s32 sbtlDrawProcess(dds3ProcessID_t *PID);
s32 sbtlFuncKatana(void);
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

#endif
