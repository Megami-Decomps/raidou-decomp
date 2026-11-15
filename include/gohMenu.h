#ifndef GOHMENU_H
#define GOHMENU_H

#include "common.h"
#include "enums.h"
#include "kernel.h"

dds3ProcessFunc_t DrawProcess_gohMenu(dds3ProcessID_t *PID);
s32 ProcessCheck_gohMenu(void);
void ProcessEnd_gohMenu(void);
void ProcessStart_gohMenu(void);
s32 gohCallBitCheck4(GOH_MENU_ID MenuID);
void gohCallDrawBack(void);
s32 gohCheckMovePrev(void);
s8 gohGetBackStat(void);
void gohSetDraw(s32 a_bDraw);
void gohSetDrawBack(s32 a_bDraw, s32 a_bFade);
void gohSetEnableSelect(GOH_MENU_ID MenuID, s32 Bool);
void gohSetFocus(s32 a_bFocus);
void gohShutdown(dds3ProcessID_t *PID);
void gohStart(void);

#endif
