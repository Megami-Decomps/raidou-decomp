#ifndef FCLMISC_H
#define FCLMISC_H

#include "common.h"
#include "basic.h"

s32 fclChkMessageDraw(void);
s32 fclChkMessageInvalid(void);
s32 fclChkSelMessage(void);
s32 fclEndMessageEx(s32 ClsMode);
void fclFilterOFF(void);
void fclFreeStage(void);
void fclInitDstEff2Sub(s32 instance);
s32 fclGetSelMessagePos(void);
void fclInitPanelPos(s32 X, s32 Y, u32 Z, u16 W, u16 H, s32 Work, void *pPanelPos);
void fclSetMessageInvalid(s32 Mode);
s32 fclSetModelLight(char *pKeyWord, dds3Basic_t *Model);
s32 fclStartSelMessage(s32 SelMsgNo);

#endif
