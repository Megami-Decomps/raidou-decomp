#ifndef SBTLACTAI_H
#define SBTLACTAI_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"

struct SBTLUNIT_WORK;

/* SBTLUNIT_MODEL flags an AI mode change clears (HD ACTAIFLGMASK). */
#define ACTAIFLGMASK_R1                                                                                        \
    (SBTL_ACTAIFLG_CMBIN | SBTL_ACTAIFLG_STAND | SBTL_ACTAIFLG_SPRAT | SBTL_ACTAIFLG_RANKS | SBTL_ACTAIFLG_POINT | \
     SBTL_ACTAIFLG_SEARCH)
#ifdef VERSION_RAIDOU2
#define ACTAIFLGMASK (ACTAIFLGMASK_R1 | SBTL_ACTAIFLG_SKIPSEARCH)
#else
#define ACTAIFLGMASK ACTAIFLGMASK_R1
#endif

/* Skill IDs the AI's skill-bit checks scan (SBTLUNIT_MODEL useskl and chkskl). */
#ifdef VERSION_RAIDOU2
#define SBTL_AI_SKILL_MAX 1280
#else
#define SBTL_AI_SKILL_MAX 512
#endif

/* One action choice: percent weight, packed skill code and packed target code. */
typedef struct AIACT_TBL {
    s16 per;
    u8 pad02[0x2];
    u32 sklid;
    u32 targetid;
} AIACT_TBL;

/* Three packed check codes (function << 20 | argument) and the jump table their results select from. */
typedef struct AICHK_WORK {
    u32 code[3];
    s8 jmptbl[8];
} AICHK_WORK;

/* AI table entry, 0x46C bytes. */
typedef struct AITBLDATA_WORK {
    s32 scr_no;
    AICHK_WORK act[2];
    AIACT_TBL acttbl[8][4];
    s32 pat_tbl[8][22];
} AITBLDATA_WORK;

AITBLDATA_WORK *sbtlGetAITBL(s32 devilId);

/* AI tables, one AITBLDATA_WORK per devil (HD ai_tbldata). */
extern AITBLDATA_WORK *ai_tbldata;

/* EAI init and proc handlers, indexed by SBTLAI_WORK mode (HD EAIInit_Func, EAIProc_Func). */
extern s32 (*EAIInit_Func[])(struct SBTLUNIT_WORK *wk);
extern s32 (*EAIProc_Func[])(struct SBTLUNIT_WORK *wk);
/* AAI init and proc handlers, indexed by SBTLUNIT_MODEL mode (HD AAIInit_Func, AAIProc_Func). */
extern s32 (*AAIInit_Func[])(struct SBTLUNIT_WORK *wk);
extern s32 (*AAIProc_Func[])(struct SBTLUNIT_WORK *wk);
/* HD CheckFuncTbl: AI check functions indexed by a check code's top 12 bits. */
extern s32 (*CheckFuncTbl[])(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx);

s32 sbtlCheckStatus_Move(struct SBTLUNIT_WORK *wk);
s32 sbtlBtlCheckEquationProcSub(s16 code);
u8 sbtlCallBtlGetBadStatus6(struct SBTLUNIT_WORK *wk, u8 flg);
s32 sbtlCheckMapHit(const sdfVector4_t *pos0, const sdfVector4_t *ppos, f32 dis);
void sbtlSetAIWkStat(struct SBTLUNIT_WORK *wk);
s32 SetUnitHitApproach(struct SBTLUNIT_WORK *wk);
s32 sbtlGetAIPatternData(s32 id, s32 p1, s32 p2);
s32 sbtlRefSbtl22(struct SBTLUNIT_WORK *wk);
s32 sbtlBtlUseTableDataProcSub(s32 type);
s32 sbtlBtlMVSupportSub(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock);
void sbtlSetUseSkill(struct SBTLUNIT_WORK *wk, SKLID skillid);
void SetUseskl(struct SBTLUNIT_WORK *wk, SKLID sklid);
void sbtlResetUseskl(struct SBTLUNIT_WORK *wk, SKLID sklid);
void sbtlLeaf65(struct SBTLUNIT_WORK *wk, SKLID sklid);
s32 sbtlCallBtlSetChkskl2(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx);
s32 sbtlCallBtlSetChkskl3(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx);
s32 sbtlCallBtlSetUseskl(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx);
s32 sbtlCallBtlSetUseskl2(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx);
void SetAction_ProgramSkill(struct SBTLUNIT_WORK *wk, SKLID skillID);
void sbtlSetWalk(struct SBTLUNIT_WORK *wk, f32 length);
void sbtlBS_MUNAKA_Init_Imad_Restart(struct SBTLUNIT_WORK *wk);
s32 sbtlSetNoMP(struct SBTLUNIT_WORK *wk);
void sbtlRefSbtl21(struct SBTLUNIT_WORK *wk);
void sbtlSetNoMP_sbtlActAI(struct SBTLUNIT_WORK *wk);
s32 sbtlBtlProcSub(struct SBTLUNIT_WORK *wk);
s32 sbtlProc(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckEquation(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlCheckEquation(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlSearchUnitID21(struct SBTLUNIT_WORK *wk, s32 num, s32 idx);
s32 sbtlSetAIData(struct SBTLUNIT_WORK *wk, s32 code, s32 arg, s32 codet, s32 arg2);
s32 sbtlUseTableDataProc(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckEquationProc(struct SBTLUNIT_WORK *wk);
s32 sbtlEAIInit_None(struct SBTLUNIT_WORK *wk);
s32 sbtlSetNormalAction(struct SBTLUNIT_WORK *wk, u16 type, s8 flg, s32 time, s32 arg4);
s32 sbtlGetUseTable_Target_sbtlActAI(struct SBTLUNIT_WORK *wk, s32 type, s32 index);
s32 sbtlAAIInit_Attack(struct SBTLUNIT_WORK *wk);
s32 GetSenkaiPointSub(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, sdfVector4_t *pos, s32 area);
s32 sbtlGetEscapePoint(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, sdfVector4_t *pos, s32 area);
s32 sbtlBtlMVSupportSub2(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock);

s32 PosOut(struct SBTLUNIT_WORK *wk);
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
s32 sbtlCheckSearchEnemy(struct SBTLUNIT_WORK *wk, s32 *targetid, s8 mode);
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
s32 sbtlSetSearchEnemyMode(struct SBTLUNIT_WORK *wk, s32 targetid);
s32 sbtlSetSkillData_AI(struct SBTLUNIT_WORK *wk, SKLID skillid);

struct SBTLEFF_LISTWORK;
s32 CalcBadStatus(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *SetEncountData_Sub(f32 *pos, f32 *rot);
s32 sbtlCallBtlSearchUnitID4(s32 no, s32 flg);
s32 sbtlCallMemset12(struct SBTLUNIT_WORK *wk);
s32 sbtlCallMemset5(struct SBTLUNIT_WORK *wk);
s32 sbtlCallSin(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckMeshiyose(struct SBTLUNIT_WORK *wk);
s32 CheckLoadData_sbtlScript(void);
struct SBTLUNIT_WORK *sbtlCheckCharmTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckMotionMove(struct SBTLUNIT_WORK *wk);
s32 sbtlDeletePlayer(struct SBTLUNIT_WORK *wk);
void sbtlGeneratorD_On(void);
void sbtlResetBit(u32 *flg, s32 no); /* clears bit no of *flg */
s32 sbtlGetHelperPos(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, s32 devilid);
s32 sbtlModelNodeFadeout(struct SBTLUNIT_WORK *wk, char *pName);
s32 sbtlProcSub(struct SBTLEFF_LISTWORK *lwk, s32 effectid, u8 flg);
struct SBTLUNIT_WORK *sbtlSetEncountData4(s32 devil, s32 point, u32 addst);
s32 sbtlSetMapFade(s32 frame, u32 abgr, s16 mode);
s32 sbtlSetMapFade_H(s32 frame, u32 abgr, s16 mode);

#endif
