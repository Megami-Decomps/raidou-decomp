#ifndef CMPMAIN_H
#define CMPMAIN_H

#include "common.h"
#include "enums.h"
#include "kernel.h"

struct tagCMP_WORK {
    struct sdfMemHandle_t *hMem; /* 0x00 */
    dds3ProcessID_t *drawPID;    /* 0x04 */
    dds3ProcessID_t *menuPID;    /* 0x08: the subscreen's itf menu */
#ifdef VERSION_RAIDOU2
    dds3ProcessID_t *partyPID;   /* 0x0C */
    dds3ProcessID_t *charPID;    /* 0x10 */
    dds3ProcessID_t *submPID;    /* 0x14: the sub menu (cmpGetsubMenuPID) */
#else
    dds3ProcessID_t *charPID;    /* 0x0C */
    u8 unk10[4];
#endif
    u8 step;                     /* 0x14, raidou2 0x18 */
    u8 flag;                     /* 0x15: bit 0 ends the process */
    s8 unk16;                    /* 0x16 */
#ifdef VERSION_RAIDOU2
    u8 unk1B[1];
#else
    u8 unk17[5];
#endif
    s32 markID;                  /* 0x1C */
    s32 count;                   /* 0x20 */
#ifdef VERSION_RAIDOU2
    u8 unk24[8];
    s16 page;                    /* 0x2C: stock list page, 8 entries each */
    u8 unk2E[2];
    s16 pageMax;                 /* 0x30: last stock list page */
    u8 unk32[6];
    /* the Kuda list filter (cmpSetFilterEx) */
    struct sdatUnitWork_t *psUse;   /* 0x38 */
    ecmpKudaSelectMode eSelMode;    /* 0x3C */
    ecmpKudaCMU iAttrId;            /* 0x40 */
    ecmpKudaCMU iSelectAttrId;      /* 0x44 */
    ecmpKudaCMU iDecideAttrId;      /* 0x48 */
    BSTF iBadStatus;                /* 0x4C */
    s32 listNum;                    /* 0x50: entries cmpMenuFilter found on the page */
#endif
};

void cmpCallGetSpriteHnd10(s32 a_sprNo, s32 a_x, s32 a_y, s32 a_alphaRate);
void cmpCallGetSpriteHnd9(s32 a_sprNo, s32 a_x, s32 a_y, s32 a_alphaRate);
void cmpCallProcEnd(struct tagCMP_WORK *wk);
u8 cmpCheckProcEnd(struct tagCMP_WORK *wk);
void cmpDrawKeyHelp(struct tagCMP_WORK *wk, u8 a_alphaRate1, u8 a_alphaRate2);
void cmpDrawProcess(struct tagCMP_WORK *wk, u8 a_alphaRate);
void cmpCallGetSpriteHnd7(struct tagCMP_WORK *wk, u8 a_alphaRate);
void cmpCallGetSpriteHnd8(struct tagCMP_WORK *wk, u8 a_alphaRate);
void cmpCallDrawKeyHelp2(struct tagCMP_WORK *wk, u8 a_alphaRate);
/* The control type of the subscreen menu's last selection with one. */
extern ITFMENU_CTRL_TYPE cmpCallGetProcessData6Data;
s32 cmpCallGetProcessData6(dds3ProcessID_t *PID);
void cmpFunc8(s32 a_x, s32 a_y, s32 a_alphaRate);
CMP_MODE cmpGetEndMode(void);
s32 cmpGetMarkID(void);
dds3ProcessFunc_t cmpProcess(dds3ProcessID_t *PID);
s32 cmpProcessCheck(void);
void cmpProcessEnd(void);
void cmpProcessStart(void);
void cmpProcessStartEx(dds3ProcessID_t *id);
void cmpSetMarkID(s32 a_markID);
extern const char cmpStrStorySelectDebugProc[];

#endif
