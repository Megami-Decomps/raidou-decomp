#ifndef EVTCOMMAND_H
#define EVTCOMMAND_H

#include "common.h"

u32 evtCommandStartBgmWithFade(void);
u32 evtCommand_BGM_PLAY_E(void);
u32 evtCommand_BGM_TRANS_E(void);
u32 evtCommand_BGM_VOL_UP(void);
u32 evtCommand_BOKASI_OFF(void);
u32 evtCommand_BOKASI_ON(void);
s32 evtCommand_CALL_BATTLE2(void);
s32 evtCommand_CALL_ENDSAVE(void);
s32 evtCommand_CALL_EVENT(void);
s32 evtCommand_CALL_FIELD2(void);
s32 evtCommand_CALL_FIELD3(void);
s32 evtCommand_CALL_NEXT(void);
s32 evtCommand_CALL_STAFF(void);
s32 evtCommand_CAM_CLS(void);
s32 evtCommand_CAM_CREATE(void);
s32 evtCommand_CAM_PATH_MOVE(void);
s32 evtCommand_CAM_PATH_WAIT(void);
s32 evtCommand_CAM_POS_XYZ(void);
s32 evtCommand_CAM_ROT_Q(void);
s32 evtCommand_CAM_ROT_XYZ(void);
u32 evtCommand_CAM_SEL_FOVY(void);
s32 evtCommand_CHG_MOON(void);
u32 evtCommand_CHK_BE(void);
u32 evtCommand_DEL_BE(void);
u32 evtCommand_EFFBED_BE(void);
u32 evtCommand_EFFBED_BE_EVO(void);
u32 evtCommand_EFFBED_LOAD(void);
u32 evtCommand_EFFBED_START(void);
u32 evtCommand_EFFBED_START_EVO(void);
u32 evtCommand_EFFECT_BE(void);
u32 evtCommand_EFFECT_CLS(void);
s32 evtCommand_EFFECT_LINK(void);
s32 evtCommand_EFFECT_LINK2(void);
u32 evtCommand_EFFECT_LOAD(void);
s32 evtCommand_EFFECT_OFF(void);
s32 evtCommand_EFFECT_ON(void);
s32 evtCommand_EFFECT_PATH_MOVE(void);
s32 evtCommand_EFFECT_PATH_WAIT(void);
u32 evtCommand_EFFECT_POS(void);
u32 evtCommand_EFFECT_POS_XYZ(void);
u32 evtCommand_EFFECT_PRESET(void);
u32 evtCommand_EFFECT_ROT_XYZ(void);
u32 evtCommand_EFFECT_UNLINK(void);
u32 evtCommand_EFFMG1_BE(void);
u32 evtCommand_EFFMG1_LOAD(void);
s32 evtCommand_EFFMG1_POS(void);
u32 evtCommand_EFFMG2_BE(void);
u32 evtCommand_EFFMG2_LOAD(void);
s32 evtCommand_EFFMG2_POS(void);
u32 evtCommand_EFFMG2_PRESET(void);
u32 evtCommand_FADEOUT_E(void);
s32 evtCommand_FIELD_ANIME_START(void);
s32 evtCommand_FIELD_ANIME_STOP(void);
s32 evtCommand_FIELD_BE(void);
s32 evtCommand_FIELD_CLS(void);
s32 evtCommand_FIELD_CLS_STOP(void);
s32 evtCommand_FIELD_DSP(void);
s32 evtCommand_GET_MOON(void);
s32 evtCommand_GET_MOON16(void);
u32 evtCommand_KUTI(void);
u32 evtCommand_KUTISET(void);
s32 evtCommand_LIGHT_PATH_MOVE(void);
s32 evtCommand_LIGHT_PATH_WAIT(void);
s32 evtCommand_LOAD_BE(void);
u32 evtCommand_LOAD_SOUND_E(void);
u32 evtCommand_MODEL_A(void);
u32 evtCommand_MODEL_BE(void);
u32 evtCommand_MODEL_DSP(void);
u32 evtCommand_MODEL_EXWAIT(void);
u32 evtCommand_MODEL_LIGHT_DIR(void);
u32 evtCommand_MODEL_LIGHT_LINK(void);
u32 evtCommand_MODEL_LIGHT_OFF(void);
u32 evtCommand_MODEL_LIGHT_ON(void);
u32 evtCommand_MODEL_LIGHT_RGB(void);
u32 evtCommand_MODEL_LIGHT_UNLINK(void);
u32 evtCommand_MODEL_LIGHT_WAIT(void);
u32 evtCommand_MODEL_MOVE2(void);
u32 evtCommand_MODEL_MOVE_MOT(void);
u32 evtCommand_MODEL_OFF(void);
u32 evtCommand_MODEL_ON(void);
u32 evtCommand_MODEL_PLAYER(void);
u32 evtCommand_MODEL_POS_XYZ(void);
u32 evtCommand_MODEL_RGB(void);
u32 evtCommand_MODEL_ROT(void);
u32 evtCommand_MODEL_ROT_XYZ(void);
u32 evtCommand_MODEL_SCL_XYZ(void);
u32 evtCommand_MODEL_SHADOW_OFF(void);
u32 evtCommand_MODEL_SHADOW_ON2(void);
u32 evtCommand_MODEL_WAIT(void);
u32 evtCommand_MODEL_WAIT_EXMOT(void);
u32 evtCommand_MODEL_WAIT_HOKAN(void);
s32 evtCommand_MOON_START(void);
s32 evtCommand_MOON_STOP(void);
s32 evtCommand_OBJ_PATH_MOVE(void);
s32 evtCommand_OBJ_PATH_WAIT(void);
s32 evtCommand_PATH_DIR(void);
s32 evtCommand_PATH_LOOP_OFF(void);
s32 evtCommand_PATH_LOOP_ON(void);
s32 evtCommand_PATH_RATE(void);
s32 evtCommand_PATH_START(void);
s32 evtCommand_PATH_STOP(void);
s32 evtCommand_PMV_RUN(void);
s32 evtCommand_READ_BE(void);
s32 evtCommand_RESTORE_BATTLE(void);
s32 evtCommand_RTRN_TITLE(void);
u32 evtCommand_SEARCH_ID(void);
u32 evtCommand_SET_SKY(void);
u32 evtCommand_SET_SKY_A(void);
u32 evtCommand_TEX_BE(void);
u32 evtCommand_TEX_CLS(void);
u32 evtCommand_TEX_LOAD(void);
u32 evtCommand_TEX_OFF(void);
u32 evtCommand_TEX_ON(void);
s32 evtCommand_UNIT_CLS(void);
s32 evtCommand_UNIT_CLS_STOP(void);
u32 evtOpcodeRefreshTaskData(void);
s32 fldCommand_PAD_DISABLE(void);

/* A command remains at its current instruction while pending. Any nonzero
 * callback result completes it and lets the interpreter consume its operands. */
typedef u32 ScrCommandResult;

void CallBattleSub_evtCommand(s32 encno, s32 eventno);
void KutiFunction(void);
void evtCallGetActiveScene5(void);
s32 evtFldProcSequence(void);
void evtLipsExecFunction(s32 unit_uniqid, s32 motion_no);
void titleCallNameEntry(void);

void evtLipsStopFunction_evtCommand(s32 motionNo);

/* "fldProcSequence", the field process name. */
extern const char evtStrFldProcSequence[];

#endif
