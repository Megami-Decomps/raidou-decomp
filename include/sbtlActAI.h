#ifndef SBTLACTAI_H
#define SBTLACTAI_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"

struct SBTLUNIT_WORK;

/* AI table entry, 0x46C bytes; only scr_no is named. */
typedef struct AITBLDATA_WORK {
    s32 scr_no;
    u8 pad004[0x468];
} AITBLDATA_WORK;

AITBLDATA_WORK *sbtlGetAITBL(s32 devilId);

void PosOut(struct SBTLUNIT_WORK *wk);
s32 Proc(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlDeleteHitHelper(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_BAttack(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_N_Escape(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_N_GardeT(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_N_GuardH(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_N_Kaihi(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_N_Renkei(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_N_Sabato(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_N_Wait(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_SAttack(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_SUMMON(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_Skl_S(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIInit_TurnMV(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIProc_Attack(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIProc_N_Escape(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIProc_N_Kaihi(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIProc_N_Sabato(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIProc_N_Wait(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIProc_Skl_S(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIProc_Skl_SMV(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIProc_Skl_SRT(struct SBTLUNIT_WORK *wk);
s32 sbtlAAIProc_TurnMV(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckEnemyTarget(struct SBTLUNIT_WORK *wk, s32 id);
s32 sbtlCheckSearchEnemy(struct SBTLUNIT_WORK *wk, s32 id, s8 mode);
s32 sbtlEAIInit_BtlInit(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIInit_BtlWait(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIInit_Summon(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIProc_BtlAI(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIProc_Chase(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIProc_Ctact(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIProc_Dummy(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIProc_Guard(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIProc_Movef(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIProc_Search(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIProc_Summonw(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIProc_Wait(struct SBTLUNIT_WORK *wk);
s32 sbtlGetMovePos_MapHit(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, sdfVector4_t *pos, f32 dis, f32 deg, s32 flg);
struct SBTLUNIT_WORK *sbtlGetWarpUnit_ID(struct SBTLUNIT_WORK *wk);
s32 sbtlMVSupport(struct SBTLUNIT_WORK *wk);
s32 sbtlPrivAreaCheck(struct SBTLUNIT_WORK *wk, s32 flg);
void sbtlResetAAI(struct SBTLUNIT_WORK *wk);
s32 sbtlSetGroupAction(struct SBTLUNIT_WORK *wk, s32 targetid, u32 mode, s8 flg);
void sbtlSetNoMP(struct SBTLUNIT_WORK *wk);
s32 sbtlSetSearchEnemyMode(struct SBTLUNIT_WORK *wk, s32 targetid);
s32 sbtlSetSkillData_AI(struct SBTLUNIT_WORK *wk, SKLID skillid);

#endif
