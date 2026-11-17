#ifndef SBTLAI_H
#define SBTLAI_H

#include "common.h"
#include "enums.h"

struct SBTLUNIT_WORK;
struct sdfMemHandle_t;

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
    u8 pad043[0x48 - 0x43];
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

s8 CheckAIScript(struct SBTLUNIT_WORK *wk);
s32 Proc_sbtlAI(SBTLAI_WORK *aiwk);
void Support_SetAisyo(s32 unitIndex, s32 mode);
s32 SEquationno_00(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_05(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_07(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_08(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_09(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_0a(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_0b(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_0c(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_0d(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_0e(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_10(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_11(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_12(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_14(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_15(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_16(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_17(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 TEquationno_0c(struct SBTLUNIT_WORK *wk);
s32 TEquationno_0d(struct SBTLUNIT_WORK *wk);
s32 TEquationno_0e(struct SBTLUNIT_WORK *wk);
s32 sbtlAI_SetSeikaku_Action(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckSkillEquation(struct SBTLUNIT_WORK *wk, s16 adr, SKLID skillid);
s32 sbtlCheckTargetEquation(struct SBTLUNIT_WORK *wk);
s32 sbtlGetTargetUnit(struct SBTLUNIT_WORK *wk);
struct SBTLAI_WORK *sbtlLeaf64(void);
struct SBTLAI_WORK *Create_sbtlAI(void);
void sbtlRefSbtl17(struct SBTLUNIT_WORK *wk);
s32 sbtlProcScript(struct SBTLUNIT_WORK *wk);
void sbtlEndAction(struct SBTLUNIT_WORK *wk);

/* The battle AI script, registered by sbtlCreateAIScript. */
extern u8 *ai_scr;

#endif
