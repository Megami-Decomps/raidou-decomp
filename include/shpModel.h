#ifndef SHPMODEL_H
#define SHPMODEL_H

#include "common.h"
#include "basic.h"
#include "enums.h"

struct tagSHP_MODEL_CB;

s32 ChkMotionEnd(s32 a_no);
void CreateModel(struct tagSHP_MODEL_CB *cb);
void SetLight(s32 a_shoptype);
void shpCreateScene(void);
void shpDeleteModel(struct tagSHP_MODEL_CB *cb);
void shpDeleteScene(void);
void shpDrawFieldLine(void);
dds3Basic_t *shpGetBasic(s32 a_objId);
void shpMakeLightPacket(void);
void shpSetColor(s32 a_no, u32 a_col);
void shpSetDebugPrint(s32 a_bPrint);
void shpSetDrwModel(s32 a_no, s32 a_draw);
void shpSetDrwModelAll(s32 a_bDraw);
void shpSetModel(s32 a_objId, MODELTYPE a_majorNo, s32 a_minorNo);
void shpSetMotion(s32 a_no, s32 a_motNo);
void shpSetMotionPause(s32 a_no, s32 a_bPause);
void shpSetMotionSpeed(s32 a_no, f32 a_motSpeed);
void shpSetMove3DDef(s32 a_frame);
void shpSetParamInit(s32 a_objId, MODELTYPE a_majorNo, s32 a_minorNo, s32 a_motionNo, f32 a_posx, f32 a_posy, f32 a_posz, f32 a_roty);
void shpSetPosModel(s32 a_no, f32 a_x, f32 a_y, f32 a_z);
void shpSetRotModel(s32 a_no, f32 a_x, f32 a_y, f32 a_z);
void shpSetRotYModel(s32 a_no, f32 a_y);
void shpSetSclModel(s32 a_no, f32 a_x, f32 a_y, f32 a_z);
void shpSetSclModelEx(s32 a_no, f32 a_scale);
void shpSetSingleMotion(s32 a_no, s32 a_motNo);
void shpSetSwitchMotion(s32 a_no, s32 a_motNo);
void shpSetTrans(s32 a_bTrans);
void shpUpdateCamera(void);

#endif
