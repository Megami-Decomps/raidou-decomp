#ifndef SBTLDVLTK_H
#define SBTLDVLTK_H

#include "common.h"
#include "enums.h"

struct SBTLDVLTK_WORK;
struct SBTLUNIT_WORK;

char *GetDrawText(u32 rgba, struct SBTLUNIT_WORK *wk, SBTLDVLTK_MODE mode, struct SBTLDVLTK_WORK *_dwk);
s32 sbtlCheckMsgMemType(void);
s32 sbtlExtCommand_DVLTK_DSP(void);
s32 sbtlExtCommand_DVLTK_WAIT(void);
s32 sbtlExt_ChkMes(void);
s32 sbtlExt_CreateBuffer(s32 DevilID, s32 ArrayIndex);
s32 sbtlExt_EndMes(void);
void sbtlExt_Free(s32 ArrayIndex);
s32 sbtlExt_FreeBuffer(void);
s32 sbtlExt_MesReq(s32 DevilID, SBTLDVLTK_MODE Type);
s32 sbtlExt_MesReqEx(SBTLDVLTK_MODE Type, s32 ArrayIndex);
s32 sbtlLoadOnMemMsg(void);

#endif
