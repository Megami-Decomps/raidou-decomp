#ifndef SBATTLE_H
#define SBATTLE_H

#include "common.h"
#include "enums.h"
#include "kernel.h"
#include "sdf_types.h"

struct MOTIONDATA_WORK;
struct SBDEBUG_CSRWORK;
struct SBDEBUG_FILELISTWORK;
struct SBDEBUG_FLGWORK;
struct SBDEBUG_PATHDATA;
struct SBTLDMG_HITLISTWORK;
struct SBTLDMG_HITWORK;
struct SBTLSKILLHIT_POSWORK;
struct SBTLSKILLHIT_TBL;
struct SBTLSKILLIDOU_WORK;
struct SBTLSKLHIT_PACDATAWORK;
struct SBTLUNIT_WORK;

s32 gohCmbSelInit(dds3ProcessID_t *id);
s32 sbtlAddPrev(struct SBDEBUG_CSRWORK *swk, s32 prev, s32 motion);
void sbtlCallDrawMiscFontAlpha(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckOpenMenu(s32 swt, s32 flg);
s32 sbtlCreateUnit2(s16 minor, SBTLUNIT_SIDE side, const sdfVector4_t *pos);
s32 sbtlDebug_ChangeModel(void);
s32 sbtlDebug_CheckLoadEncountData(void);
s32 sbtlDebug_CheckMainPanel(struct SBTLUNIT_WORK *wk);
s32 sbtlDebug_DeleteHitList(void);
s32 sbtlDebug_Draw(void);
s32 sbtlDebug_DrawC(struct SBTLDMG_HITWORK *wk, u32 rgba, s32 start, s32 end, s32 flg, s32 mode);
s32 sbtlDebug_DrawClrcle_Line(f32 x, f32 y, f32 z, f32 r, u32 rgba);
void sbtlDebug_DrawHitCircle(const sdfVector4_t *center, const sdfVector4_t *rot, f32 rad, f32 area_r1, f32 area_r2);
void sbtlDebug_DrawNormalList_Col(char *wk, s32 num, s32 nowpoint, s32 point, s32 x, s32 y);
void sbtlDebug_DrawONOFFList_Motion(struct MOTIONDATA_WORK *m_data, s32 x, s32 y, s32 num);
void sbtlDebug_DrawTCursor(const sdfVector4_t *pos, u32 rgba);
s32 sbtlDebug_EPLParaDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, s32 point);
void sbtlDebug_EffHitDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBDEBUG_PATHDATA *path, const sdfVector4_t *center);
void sbtlDebug_EffHitIdou(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBDEBUG_PATHDATA *path, const sdfVector4_t *poss, const sdfVector4_t *rott);
void sbtlDebug_EffHitPathDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk);
void sbtlDebug_HitTypeMoveDraw(struct SBTLSKILLHIT_TBL *hwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlDebug_Init(void);
s32 sbtlDebug_LoadMHTData(s32 point, s32 flg);
void sbtlDebug_MDLHitDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, const sdfVector4_t *center);
s32 sbtlDebug_MoveTypeDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path, s32 point);
s32 sbtlDebug_SetMDLMOVEPara(struct SBTLSKILLHIT_TBL *hwk, struct SBDEBUG_CSRWORK *wk, s32 flg);
void sbtlDebug_SetStatusflg(s32 nowp, struct SBDEBUG_FLGWORK *wk, s8 flg);
void sbtlDebug_SetVisualData(struct SBTLUNIT_WORK *wk);
void sbtlDebug_UnitPanelCursor(struct SBDEBUG_CSRWORK *wk);
s32 sbtlDecPrev(struct SBDEBUG_CSRWORK *swk, s32 prev, s32 motion);
s32 sbtlDeleteFileList(struct SBDEBUG_FILELISTWORK *lwk);
s32 sbtlDeletePrevModel(void);
void sbtlDrawFloat_Num(s32 x, s32 y, f32, s32 point, s32 flg);
s32 sbtlDrawHitList(struct SBTLDMG_HITLISTWORK *lwk);
void sbtlDrawInt_Num(s32 x, s32 y, s32, s32 point, s32 flg);
s32 sbtlDrawPoseture(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, const sdfVector4_t *pose);
void sbtlDrawString(s32 x, s32 y, char *buff, s32 point, s32 flg);
s32 sbtlDrawUnitChangeList(s32 x, s32 y);
s32 sbtlEPLDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, const sdfVector4_t *poss, const sdfVector4_t *rott);
void sbtlEPLLoadDraw(void);
s32 sbtlEditDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlEffHitEditorDraw_Functbl_Cylinder(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlEffHitEditorDraw_Functbl_Ring(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlEffHitEditorDraw_Functbl_Sphere(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
void sbtlFunc16(u32 flg, u32 *tbl, s32 x, s32 y, s32 num);
s32 sbtlGetMotionNum(s32 id);
struct SBTLUNIT_WORK *sbtlGetUnit(s32 point);
void sbtlLoadDraw(void);
s32 sbtlMDLHitEditor_DrawMotion(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, s32 nowp);
s32 sbtlMDLMODE_Func_Open(void);
s32 sbtlMDLMODE_Func_PageList(void);
s32 sbtlMOVEMODE_Func_Edit(void);
s32 sbtlMOVEMODE_Func_PageList(void);
s32 sbtlNEFMODE_Func_Load(void);
s32 sbtlNEFMODE_Func_PageList(void);
void sbtlPBLoadDraw(void);
s32 sbtlResetEPLData(s32 i);
void sbtlResetMotionMovePos(void);
void sbtlSetEffHitPara_S(struct SBTLSKILLHIT_TBL *hwk, struct SBDEBUG_CSRWORK *wk, s32 flg);
void sbtlSetModelData(struct SBTLUNIT_WORK *unit, MODELTYPE major, s32 point);
s32 sbtlSetMotionMoveData_Buffer(void);
s32 sbtlSetPathMotion(struct SBTLSKLHIT_PACDATAWORK *pac, s32 point);
s32 sbtlSetPreViewModel(void);
void sbtlSetUnitPara(struct SBTLUNIT_WORK *unit, s32 flg, s32 point);
void sbtlSetVisualPara(struct SBTLUNIT_WORK *unit, struct SBDEBUG_CSRWORK *wk, s32 flg);

#endif
