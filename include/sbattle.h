#ifndef SBATTLE_H
#define SBATTLE_H

#include "common.h"
#include "enums.h"
#include "kernel.h"
#include "sdf_types.h"

typedef struct SBTLMAIN_DATA {
    sdfVector4_t pos;
    sdfVector4_t rot;
    f32 offset_x;
    f32 offset_z;
    s32 encountno;
    s32 type;
    s16 mode;
    s16 stock_id;
} SBTLMAIN_DATA;

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

struct SBDEBUG_FLGWORK {
    u8 mode;
    SBDEBUG_MODE nextmodeno;
    SBDEBUG_MODE backmodeno;
    SBDEBUG_FLGTYPE sttype;
    SBDEBUG_FLG stflg;
};
typedef struct SBDEBUG_FLGWORK SBDEBUG_FLGWORK;

/* Debug menu cursor. */
struct SBDEBUG_CSRWORK {
    s32 point;
    s32 offset;
    s32 maxp;
    s32 dummy;
};
typedef struct SBDEBUG_CSRWORK SBDEBUG_CSRWORK;

/* Battle debug work (HD SBTLDEBUG_WORK), the members decompiled code reads. */
typedef struct SBTLDEBUG_WORK {
#ifdef VERSION_RAIDOU2
    u8 pad000[0x490];
#else
    u8 pad000[0x7B0];
#endif
    s32 nef_poseture;
    f32 nef_sokudo;
    s32 ef_mode;
    s32 ef_id;
    s32 ef_num;
    s32 ef_nowpage;
    s32 ef_flgp;
    s32 ef_dirflgp;
    s32 ef_stendp;
    s8 ef_wkflg[16];
#ifdef VERSION_RAIDOU2
    u8 pad4C4[0x16AC - 0x4C4];
#else
    u8 pad7E4[0x19CC - 0x7E4];
#endif
    s32 acterID;
#ifndef VERSION_RAIDOU2
    u8 pad19D0[0x1C08 - 0x19D0];
    char log[0xC00];
    s32 logLength;
#endif
} SBTLDEBUG_WORK;

extern SBTLDEBUG_WORK evtStageTestState;

typedef s32 (*sbtlDebugModeFunc_t)(void);
typedef s32 (*sbtlDebugHitEditorDrawFunc_t)(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);

s32 gohCmbSelInit(dds3ProcessID_t *id);
s32 MainProc(dds3ProcessID_t *id);
s32 ModelSelectDraw(dds3ProcessID_t *id);
s32 ModelSelectProc(dds3ProcessID_t *id);
s32 MotionChngProc(dds3ProcessID_t *id);
s32 sbtlAddPrev(struct SBDEBUG_CSRWORK *swk, s32 prev, s32 motion);
s32 sbtlBattleBed(void);
s32 sbtlBattleEffhit(dds3ProcessID_t *id);
s32 sbtlBattleHitpath(void);
s32 sbtlBattleMdlhit(dds3ProcessID_t *id);
s32 sbtlBSTDraw(dds3ProcessID_t *id);
s32 sbtlBSTProc(dds3ProcessID_t *id);
s32 sbtlCallBtlDebugDrawNormalListCol(dds3ProcessID_t *id);
s32 sbtlCallBtlDebugSetStatusflg(dds3ProcessID_t *id);
s32 sbtlCallBtlSearchUnitID31(dds3ProcessID_t *id);
void sbtlCallDrawMiscFontAlpha(struct SBTLUNIT_WORK *wk);
s32 sbtlCallFileCheckEnd(void);
s32 sbtlCallGetUnit(dds3ProcessID_t *id);
s32 sbtlCallRestartBattle(void);
s32 sbtlCheckOpenMenu(s32 swt, s32 flg);
s32 sbtlCopyData(void);
s32 sbtlCreateUnit2(s16 minor, SBTLUNIT_SIDE side, const sdfVector4_t *pos);
s32 sbtlDebug_ChangeModel(void);
s32 sbtlDebug_CheckLoadEncountData(void);
s32 sbtlDebug_CheckMainPanel(struct SBTLUNIT_WORK *wk);
s32 sbtlDebug_DeleteHitList(void);
s32 sbtlDebug_Draw(void);
s32 sbtlDebug_DrawC(struct SBTLDMG_HITWORK *wk, u32 rgba, s32 start, s32 end, s32 flg, s32 mode);
s32 sbtlDebug_DrawClrcle_Line(f32 x, f32 y, f32 z, f32 r, u32 rgba);
void sbtlDebug_DrawHitCircle(const sdfVector4_t *center, const sdfVector4_t *rot, f32 rad, f32 area_r1, f32 area_r2);
void sbtlDebug_DrawNormalList_Col(char **wk, s32 num, s32 nowpoint, s32 point, s32 x, s32 y);
void sbtlDebug_DrawONOFFList_Motion(struct MOTIONDATA_WORK *m_data, s32 x, s32 y, s32 num);
void sbtlDebug_DrawTCursor(const sdfVector4_t *pos, u32 rgba);
void sbtlDebug_EffHitDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBDEBUG_PATHDATA *path, const sdfVector4_t *center);
void sbtlDebug_EffHitIdou(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBDEBUG_PATHDATA *path, const sdfVector4_t *poss, const sdfVector4_t *rott);
void sbtlDebug_EffHitPathDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk);
s32 sbtlDebug_EPLParaDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, s32 point);
void sbtlDebug_HitTypeMoveDraw(struct SBTLSKILLHIT_TBL *hwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlDebug_Init(void);
s32 sbtlDebug_LoadMHTData(s32 point, s32 flg);
void sbtlDebug_MDLHitDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, const sdfVector4_t *center);
s32 sbtlDebug_MOVEMODE_Func_Open(void);
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
s32 sbtlEditDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlEffHitEditorDraw_Functbl_Cylinder(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlEffHitEditorDraw_Functbl_Ring(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlEffHitEditorDraw_Functbl_Sphere(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlEPLDraw(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, const sdfVector4_t *poss, const sdfVector4_t *rott);
void sbtlEPLLoadDraw(void);
s32 sbtlFRAME(dds3ProcessID_t *id);
s32 sbtlFunc15(dds3ProcessID_t *id);
void sbtlFunc16(u32 flg, u32 *tbl, s32 x, s32 y, s32 num);
s32 sbtlFunc17(void);
s32 sbtlFunc21(dds3ProcessID_t *id);
s32 sbtlFunc26(dds3ProcessID_t *id);
s32 sbtlFunc28(void);
s32 sbtlGetMotionNum(s32 id);
s32 sbtlSetMDLHitPara(struct SBTLSKILLHIT_TBL *hwk, struct SBDEBUG_CSRWORK *wk, s32 flg);
struct SBTLUNIT_WORK *sbtlGetUnit(s32 point);
s32 sbtlLeaf100(dds3ProcessID_t *id);
s32 sbtlLeaf102(dds3ProcessID_t *id);
s32 sbtlLeaf104(dds3ProcessID_t *id);
s32 sbtlLeaf105(dds3ProcessID_t *id);
s32 sbtlLeaf106(dds3ProcessID_t *id);
s32 sbtlLeaf108(void);
s32 sbtlLeaf116(dds3ProcessID_t *id);
s32 sbtlLeaf119(dds3ProcessID_t *id);
s32 sbtlLeaf122(dds3ProcessID_t *id);
s32 sbtlLeaf95(void);
s32 sbtlLeaf98(dds3ProcessID_t *id);
void sbtlLoadDraw(void);
s32 sbtlMDLHitEditorDraw_Functbl_Cylinder(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlMDLHitEditorDraw_Functbl_Ring(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlMDLHitEditorDraw_Functbl_Sphere(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, struct SBTLSKILLHIT_POSWORK *para, struct SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path);
s32 sbtlMDLHitEditor_DrawMotion(struct SBDEBUG_CSRWORK *wk, struct SBTLSKILLHIT_TBL *hwk, s32 nowp);
s32 sbtlMDLMODE_Func_Edit(void);
s32 sbtlMDLMODE_Func_Open(void);
s32 sbtlMDLMODE_Func_PageList(void);
s32 sbtlMOVEMODE_Func_Edit(void);
s32 sbtlMOVEMODE_Func_PageList(void);
s32 sbtlNEFMODE_Func_Edit(void);
s32 sbtlNEFMODE_Func_Load(void);
s32 sbtlNEFMODE_Func_Open(void);
s32 sbtlNEFMODE_Func_PageList(void);
s32 sbtlNEFMODE_Func_Save(void);
s32 sbtlOpti(dds3ProcessID_t *id);
s32 sbtlPacPBFile(void);
s32 sbtlParaDraw(dds3ProcessID_t *id);
s32 sbtlParaProc(dds3ProcessID_t *id);
void sbtlPBLoadDraw(void);
s32 sbtlRefSbtl56(dds3ProcessID_t *id);
s32 sbtlRefSbtl67(dds3ProcessID_t *id);
s32 sbtlResetEPLData(s32 i);
void sbtlResetMotionMovePos(void);
void sbtlSetEffHitPara_S(struct SBTLSKILLHIT_TBL *hwk, struct SBDEBUG_CSRWORK *wk, s32 flg);
void sbtlSetModelData(struct SBTLUNIT_WORK *unit, MODELTYPE major, s32 point);
s32 sbtlSetMotionMoveData_Buffer(void);
s32 sbtlSetPathMotion(struct SBTLSKLHIT_PACDATAWORK *pac, s32 point);
s32 sbtlSetPreViewModel(void);
void sbtlSetUnitPara(struct SBTLUNIT_WORK *unit, s32 flg, s32 point);
void sbtlSetVisualPara(struct SBTLUNIT_WORK *unit, struct SBDEBUG_CSRWORK *wk, s32 flg);
s32 sbtlSizeofData(void);
s32 sbtlSkillChangeProc(dds3ProcessID_t *id);
s32 sbtlSousaProc(dds3ProcessID_t *id);
s32 sbtlUnitChngDraw(dds3ProcessID_t *id);
s32 sbtlUnitChngProc(dds3ProcessID_t *id);
s32 sbtlVisualDraw(dds3ProcessID_t *id);
s32 sbtlVisualProc(dds3ProcessID_t *id);


#endif
