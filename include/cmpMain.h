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
#else
    dds3ProcessID_t *charPID;    /* 0x0C */
    u8 unk10[4];
#endif
    u8 step;                     /* 0x14 */
    u8 flag;                     /* 0x15: bit 0 ends the process */
    s8 unk16;                    /* 0x16 */
    u8 unk17[5];
    s32 markID;                  /* 0x1C */
    s32 count;                   /* 0x20 */
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
