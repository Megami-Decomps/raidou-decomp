#ifndef SBTLAI_H
#define SBTLAI_H

#include "common.h"
#include "enums.h"

struct SBTLUNIT_WORK;
struct SBTLSKILLP_TBL;
struct sdfMemHandle_t;

#ifdef VERSION_RAIDOU2
/* AI state of a unit, 0x118 bytes, made by Create_sbtlAI. Partial raidou2 layout: only the fields read so far are named. */
typedef struct SBTLAI_WORK {
    struct sdfMemHandle_t *handle;
    s16 stat;
    u16 mode;
    u16 backmode;
    u8 pad00A[0x2];
    u32 status;
    s16 act_p;
    u8 pad012[0x5E - 0x12];
    u8 bstside; /* bit 0: confused unit stays on its own side */
    u8 pad05F[0x64 - 0x5F];
    s32 syscnt_stop;
    s32 field4C;
    s8 scr_actcheckflg;
    u8 pad06D[0x3];
    u32 scrflag;
    s32 scrcnt;
    s32 cmpscrcnt;
    s32 scrAutoCnt;
    s32 scrCmpCnt;
    s32 scrStep;
    s32 scrStep2;
    s32 scrStep3;
    s32 scrListID; /* slot in wk_sbtl->scrUnitList, -1 for none */
    f32 scrReactDist;
    s32 scrReactSkillID;
    s32 scrTargetCode;
    s32 scrSkillID;
    s16 scrMagHp[5];
    s16 scrMagNum[5];
    s8 scrMagFlg[5];
    u8 pad0BD[0x3];
    s32 actcnt;
    s32 cmpactcnt;
    u32 cmndFrm;
    u32 cmndFrmMax;
    SKLID cmndSkl;
    s32 cmndTgt;
    s32 cmndStk;
    s32 bCmndEnd;
    SKLID dfltOdrSkl;
    s32 dfltOdrTgt;
    s32 dfltOdrStk;
    SKLID rsvSkl;
    s32 rsvTgt;
    s32 rsvStk;
    s32 atkCnt;
    s32 bFarOk;
    SKLID usedSkl;
    u8 pad104[0x118 - 0x104];
} SBTLAI_WORK;
#else
/* AI state of a unit, 0x90 bytes, made by Create_sbtlAI. */
typedef struct SBTLAI_WORK {
    struct sdfMemHandle_t *handle;
    s16 stat;
    u16 mode;
    u16 backmode;
    u32 status;
    s16 act_p;
    u8 pad012[0x42 - 0x12];
    u8 bstside; /* bit 0: confused unit stays on its own side */
    u8 hcnt;
    s16 meirei_nobk; /* SBTLPNL_MENU */
    u8 pad046[0x48 - 0x46];
    s32 syscnt_stop;
    s32 field4C;
    s8 scr_actcheckflg;
    u8 pad051[0x3];
    u32 scrflag;
    s32 scrcnt;
    s32 cmpscrcnt;
    s32 scrAutoCnt;
    s32 scrCmpCnt;
    s32 scrStep;
    s32 scrStep2;
    s32 scrStep3;
    s32 scrListID;
    u8 pad078[0x80 - 0x78];
    s32 scrTargetCode;
    s32 scrSkillID;
    s32 actcnt;
    u8 pad08C[0x90 - 0x8C];
} SBTLAI_WORK;
#endif

s8 CheckAIScript(struct SBTLUNIT_WORK *wk);
s32 Proc_sbtlAI(SBTLAI_WORK *aiwk);
void Support_SetAisyo(s32 unitIndex, s32 mode);
s32 SEquationno_00(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_05(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_07(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_08(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_09(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_0a(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_0b(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_0c(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_0d(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_0e(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_10(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_11(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_12(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_14(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_15(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_16(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 SEquationno_17(struct SBTLUNIT_WORK *wk, s16 skillid);
s32 CheckBSTTarget(struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *tbl);
s32 SetNextTarget(struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *tbl, s32 no);
#ifdef VERSION_RAIDOU2
/* the summon slot (0 or 1) of wk, or -1 */
s32 GetMeireiNo(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *sbtlGetAnotherEnemy(struct SBTLUNIT_WORK *wk);
#endif
u8 TEquationno_01(struct SBTLUNIT_WORK *wk);
u8 TEquationno_0c(struct SBTLUNIT_WORK *wk);
u8 TEquationno_0d(struct SBTLUNIT_WORK *wk);
u8 TEquationno_0e(struct SBTLUNIT_WORK *wk);
s32 sbtlAI_SetSeikaku_Action(struct SBTLUNIT_WORK *wk);
/* One step of a skill equation: run SEquationno_<id>, then op/flag decide the percentage change. */
typedef struct SBTL_SKILLEQ_STEP {
    s16 id;
    u8 op; /* 1 add, 2 subtract, 255 fail the check */
    s8 flag;
    s16 value;
} SBTL_SKILLEQ_STEP;

extern SBTL_SKILLEQ_STEP sbtlCheckSkillEquationData[];
extern SBTL_SKILLEQ_STEP sbtlCheckSkillEquationData2[];
extern s32 (*sbtlCheckSkillEquationData3[])(struct SBTLUNIT_WORK *wk, s16 skillid);

s32 sbtlCheckSkillEquation(struct SBTLUNIT_WORK *wk, s16 *adr, s16 skillid);
s32 sbtlCheckTargetEquation(struct SBTLUNIT_WORK *wk);
s32 sbtlGetTargetUnit(struct SBTLUNIT_WORK *wk);
struct SBTLAI_WORK *sbtlLeaf64(void);
struct SBTLAI_WORK *Create_sbtlAI(void);
void sbtlRefSbtl17(struct SBTLUNIT_WORK *wk);
s32 sbtlProcScript(struct SBTLUNIT_WORK *wk);
void sbtlEndAction(struct SBTLUNIT_WORK *wk);

/* The battle AI script, registered by sbtlCreateAIScript. */
extern u8 *ai_scr;
/* The script registered as SBTL_WORK.hDEADScript. */
extern u8 *sbtlCallRegistScript2Data;

#endif
