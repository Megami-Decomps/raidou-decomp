#ifndef SBTLDVLTK_H
#define SBTLDVLTK_H

#include "common.h"
#include "enums.h"

struct SBTLDVLTK_LISTWORK;
struct SBTLDVLTK_WORK;
struct SBTLUNIT_WORK;
struct dds3ProcessID_t;
struct sprHandle_t;

extern struct dds3ProcessID_t *sbtlData104;
extern struct sprHandle_t *sbtlData105;
extern s32 sbtlData106; /* the last devil talk text width */

struct sprHandle_t *sbtlGetSpriteHandle(void);
s32 sbtlDrawText(struct SBTLDVLTK_WORK *dwk);
void sbtlAnimWaku(struct SBTLDVLTK_WORK *dwk);
#ifdef VERSION_RAIDOU2 /* Raidou 1's sbtlDrawText under its Raidou 2 label */
s32 sbtlDrawText(struct SBTLDVLTK_WORK *dwk);
#endif

char *GetDrawText(u32 rgba, struct SBTLUNIT_WORK *wk, SBTLDVLTK_MODE mode, struct SBTLDVLTK_WORK *_dwk);
s32 sbtlCallBtlCheckMsgMemType(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckMsgMemType(void);
s32 sbtlSearchDvltk(struct SBTLDVLTK_LISTWORK *lwk, struct SBTLUNIT_WORK *wk);
struct SBTLDVLTK_WORK *sbtlCreateDvltk(struct SBTLDVLTK_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SBTLDVLTK_MODE mode);
struct SBTLDVLTK_WORK *sbtlCreateDvltk_Priority(struct SBTLDVLTK_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SBTLDVLTK_MODE mode);
s32 sbtlCheckPriority_Battle(s32 a, s32 b);
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
