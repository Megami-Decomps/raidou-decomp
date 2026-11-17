#ifndef FLDFILERESOLVER_H
#define FLDFILERESOLVER_H

#include "common.h"
#include "sdf_types.h"

struct mdlObj_t;

s32 PlayerCalc_Nml(void);
void PlayerMotionSe(s32 hisMotion, s32 upMotion);
s32 fldCatGetNakamaSub(void);
void fldDrawFrushOt(s32 r, s32 g, s32 b, s32 alp, s32 scl, s32 ot);
s32 fldPlayerCalc(void);
void fldPlayer_MoveCalc(void);
void fldPlayer_MoveStart(f32 sx, f32 sy, f32 sz, f32 sr, f32 ex, f32 ey, f32 ez, f32 er);
void fldAiboTaskSub(void);
void mapAiboAdd(f32 x, f32 y, f32 z);
void mapAiboFoceSet(f32 x, f32 y, f32 z);
void mapAiboFoceSet3(f32 x, f32 y, f32 z);
void mapAiboGet(f32 *x, f32 *y, f32 *z);
void mapAiboMinAlpDown(s32 rgba);
void mapAiboTask(void);
void mapAiboWakeUp(s32 mode);
void mapSummonDvAdd(f32 x, f32 y, f32 z);
void mapSummonDvFoceSet(f32 x, f32 y, f32 z);
void mapSummonDvFoceSet2(f32 x, f32 y, f32 z);
void mapSummonDvGet(f32 *x, f32 *y, f32 *z);
void mapSummonDvTask(void);
void rstDrawEvoBgFade(s32 Motion, f32 spd);
s32 sndSeStatus(void);
void mapAiboInit(void);
void mapSummonDvInit(void);
void NakamaMotion(s32 hisMotion, s32 upMotion, f32 speed);
void fldCallNakamaMotion3DSE(s32 hisMotion, s32 upMotion);
void PlayerMotionSe2(s32 se, f32 x, f32 z);
s32 PlayerCalc_Nml_fld(void);
extern s32 map_player_move_mode;
/* PlayerCalc_Nml_fld: last screen direction and the camera angle taken for it. */
extern f32 fldCallEncountTestFlagData2;
extern s32 fldCallEncountTestFlagData3;
void fldSetUnitSub3(s32 motion, f32 speed);
void fldLeaf18(s32 hisMotion, s32 upMotion, f32 hokan);
void fldLeaf19(s32 hisMotion, s32 upMotion, f32 hokan);
f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz);
s32 mapCallSetPlayerTargetData5(s32 se, struct mdlObj_t *model, f32 x, f32 z);
void fldSetCameraObjectTransitionFlag(void);

/* Aibo and summoned-devil trails: position rings read behind the player. */
extern f32 map_aibo_buff[128][3];
extern f32 map_aibo_pos[4];
extern s32 map_aibo_buff_read_idx;
extern s32 map_aibo_buff_write_idx;
extern f32 map_aibo_spd_max;
extern f32 map_aibo_rot;
extern s32 gMapAiboWakeUpMode;
extern s32 gMapAiboMotionBugCnt;
extern f32 map_summondv_buff[256][3];
extern s32 mapWork3[256];
extern f32 map_summondv_pos[4];
extern s32 map_summondv_buff_read_idx;
extern s32 map_summondv_buff_add_idx;
extern f32 mapSummonDvTaskData2;
extern f32 map_summondv_rot;
extern s32 gMapNakamaRunAtumaru;
extern sdfVector4_t mapSummonDvTaskTable;
extern sdfVector4_t VRF_Disable;
extern s32 mapAiboTaskData2;
extern f32 mapData56;
extern f32 mapData57;
extern f32 mapData58;
extern struct SBTLSOUND_FOOTWORK mapSummonDvTaskData;
extern s32 PlayerMotionSe2Data;

#endif
