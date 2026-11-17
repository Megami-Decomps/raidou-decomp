#ifndef FLDPROCESS_H
#define FLDPROCESS_H

#include "common.h"

void fldEncountClearFlag(u32 flag);

typedef struct {
    u32 flags;
    s32 stage;
} fldSequenceController_t;

extern fldSequenceController_t fldAutoMapSeqFlags;

struct fldSceneParam_t;
struct dds3ProcessID_t;
struct dds3PoseSet_t;
struct mdlObj_t;

void fldProcTrigSequence(struct fldSceneParam_t *srcParam, s32 mode);
s32 ProcSequence(struct dds3ProcessID_t *id);
s32 fldProcDraw(struct dds3ProcessID_t *id);
void mapCallMap(s32 map, s32 flor, s32 point, char *label);

/* Globals ProcSequence uses outside fldGlobalBase. */
extern s32 fldProcShutDownData;
extern s32 ProcSequenceData2;
extern s32 ProcSequenceData3;
extern s32 ProcSequenceData4;
extern s32 ProcSequenceData5;
extern s32 ProcSequenceData6;
extern s32 ProcSequenceData7;
extern s32 ProcSequenceData8;
extern s32 mapNakamaTaskData;
extern s32 mapNakamaTaskData2;
extern s32 s_MapNakamaColorCnt;
extern s32 s_MapNakamaColorNow;
extern struct SBTLMAIN_DATA ProcSequenceData; /* battle start request for sequence 19 */
extern s32 fldData14;
extern s32 gMapRndEncLoad_Mode;
extern s32 gSubFloorID;
extern u32 dds3BackColor;
extern struct dds3Basic_t *cameraBasic;
extern struct dds3Basic_t *raidoBasic;
extern struct dds3Basic_t *aiboBasic;
extern struct dds3Basic_t *summonDvBasic;
extern struct mdlObj_t *playerModel;
extern struct mdlObj_t *aiboModel;
extern struct mdlObj_t *summonDvModel;
extern struct mdlObj_t *raidoModel;
extern s32 bg_tex_load_rec;
extern s32 bg_lay_load_rec;
extern s32 bg_tex_load_last;
extern s32 bg_lay_load_last;

void fldProcShutDown(void);
s32 sndProcSequenceSub4(void);
s32 sndProcSequenceSub3(void);
s32 sndProcSequenceSub2(void);
void sndCallWaitSema(void);
s32 sndSePlay(void);
void mapCallSetPlayerTargetData4(void);
s32 mapGetLight(f32 x, f32 z);
s32 mapCallSetPlayerTargetData6(f32 x, f32 z);
s32 mapBadMafuHajiki(void);
void ProcMskSub_A(void);
s32 fldProcMskSubBSub(void);
s32 fldProcMskSubBSub2(void);
void dds3VisibleSceneBasic(struct dds3Basic_t *scene, s32 visible);
void dds3FadeIn2(s32 frames);
void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);
void dds3FadeOut2(s32 frames);
s32 dds3FadeCheck(void);
void dds3ProcSequenceSub(s32 mode);
void dds3RefCameraTM(void);
void dds3RefCameraTM2(void);
void dds3SetActiveScene(struct dds3Basic_t *scene);
void mapBgDrawOnOff(s32 on);
void mapBgSetScroll(f32 x, f32 y, f32 z, s32 mode);
void fldReleaseUnitPad(void);
void mdlSetMotionSpeed(s32 speed);
void fldCallGetRotPoseSet(void);
void mdlMotionPause(struct mdlObj_t *model);
void mapWaitCbScreenFadeBSub(s32 mode);
void fldRefTexLoadLast(void);
s32 mapGetFloorLight(s32 floor, f32 x, f32 z);
s32 fldEncountTestFlag(u32 flag);
void fldAutoMapSeqSetFlag(u32 flag);
s32 fldAutoMapSeqTestFlag(u32 flag);
void fldEncountSetFlag(u32 flag);
s32 fldLeaf6(struct dds3PoseSet_t *pose, struct dds3Basic_t *basic);
void mapSoundKankyoSeSet(s32 map, s32 floor, s32 cut);
void mapSoundSmgLoadChk(s32 map);
void p2sdPlaySE_Vol(s32 se, s32 vol);
s32 mapBigClipChkF(s32 cut, f32 x, f32 y, f32 z);
void mapBadEffLoad(s32 slot, s32 status);
void mapBadEffDrawTask(void);
void fldProcSequenceSub4(f32 x, f32 z, f32 *outX, f32 *outZ);
void fldProcSequenceSub5(f32 x, f32 z, s32 floor, f32 *outX, f32 *outZ);
void fldCallFindSceneEntryData(f32 x, f32 z, f32 x2, f32 z2, s32 map, s32 floor, s32 spot);
void mapWallObjTask(f32 x, f32 y, f32 z);
s32 sbtlProcSequenceSub(s32 arg);
s32 pbProcSequenceSub(void);
s32 scrSearchScriptName(char *name);
void mapAM_MaskOpen_ID(s32 map, s32 floor, s32 arg);
void mapAM_MaskSetCurrent(s32 map, s32 floor);
void mapRefDDS3GBWK3(s32 map, s32 floor, s32 arg);
void fldCallSoundKankyoSeSet(s32 on);
void mapBgTexLoad(s32 on);
s32 mapProcSequenceSub5(f32 x, f32 z);
void mapProcSequenceSub9(s32 arg);
void mapNakamaSkilIs_SetBit(s32 id);
s32 fldSaveLabel(void);
void cmpProcessStartEx_cmpMain(s32 mode);
s32 slRetuenFromLoadSub(void);
s32 ProcRandomEncountCheck_fld(void);
s32 p2sdCallBitCheck(s32 map, s32 floor);
void nbSetEncountMode(s32 mode);
void nbProcSequenceSub(s32 arg);
#ifdef VERSION_RAIDOU2
void nbCameraInit(void);
void itfGeneralMenuFreeSpriteHnd(void);
void UnloadRequest(s32 mode);
#endif
void mnuQueueTitleSoundStreamEntry(s32 id);
void mapTitleStart_fld(s32 a, s32 b, s32 c, s32 d);
s32 mapSoundSmgLoadChk_fld(void);
void mapProcSequenceSub12(s32 arg);
s32 mapGetCut_FXY(f32 x, f32 z);
s32 mapCallTitleStartFld(s32 arg);
s32 mapNakamaSummonTask(void);
s32 mapNakamaReturnTask(void);
s32 mapNakamaSearchTask_fld(void);
s32 mapNakamaSpotTask_fld(void);
s32 mapNakamaGankTask_fld(void);
s32 mapNakamaKoukTask(void);
s32 mapNakamaHakaTask(void);
s32 mapNakamaReiTask(void);
s32 mapNakamaChikaraTask(void);
s32 mapNakamaDokusinTask(void);
s32 mapNakamaIroTask(void);
s32 mapNakamaHikouTask(void);
s32 mapNakamaSinnTask(void);
s32 mapNakamaSpotTask(void);
s32 mapNakamaGankTask(void);
s32 mapNakamaTakumiTask(void);
s32 mapNakamaJoinTask(void);
s32 mapNakamaBranchTask(void);
void mapIgnite(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 mapIfDrawActionMaster(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 mapIfDrawActionNum(s32 *out);
void mapCallPreCalcStrWidth(s32 nakama, s32 kind, s32 x, s32 y);
void mapCallPreCalcStrWidth2(s32 nakama, s32 kind, s32 x, s32 y);
char *mapScriptGet_Pos(f32 x, f32 y, f32 z, f32 r, s32 *attr, f32 *objx, f32 *objy, f32 *objz, s32 *type);
s32 mapCallSetPlayerTargetData(f32 x, f32 z, f32 r);
s32 mapCallSetPlayerTargetData2(s32 *attr, f32 x, f32 z, f32 r);
void fldHirukoYukaGo_Idx(s32 idx);
void mapNakamaSkil_SetBit(void);
void mapNakamaSkil_ClearBit(void);
s32 fldPlayerALoad_OnDataCheck(void);
s32 fldHirukoYukaMovetask(void);
s32 fldHirukoYukaGo_Idx2_B_fld(s32 arg);
s32 fldGetFldBgNameList(f32 x, f32 z);
s32 effProcSequenceSub(void);
s32 datCallGetStockDevilID(u32 id);
void mapDebugLightSet(s32 arg);

void DecisionFeedIn(void);
void encStart(void);
void evtInitEventViewer(void);
void fldCallGetRandA3(void);
void fldCallNakamaWakeUp(void);
void fldCallNakamaWakeUp2(void);
void fldDestroyTitleTask(void);
void fldGbInit_Field(void);
s32 cfgChkFlagChange(void);
void fldHirukoYukaClear(void);
/* Moving-floor (hiruko yuka) state (HD MAP_H_YUKA_MOVE_WORK_t, R1 layout). */
typedef struct MAP_H_YUKA_MOVE_WORK_t {
    s32 uniq;
    s32 word04;
    s32 word08;
    f32 vec0C[3];
    f32 vec18[3];
    f32 vec24[3];
    f32 vec30[3];
    s32 word3C;
    s32 word40;
} MAP_H_YUKA_MOVE_WORK_t;
extern MAP_H_YUKA_MOVE_WORK_t fldHirukoYukaMovetaskWork;
void fldPlayerMotion_Single(void);
void fldProcSequenceSub3(void);
void fldProcSequenceSub6(s32 mode);
void fldReadyScene(void);
void fldSetSnowActive(void);
void mapCallBitCheck(void);
void mapCallNakamaWakeUp4(void);
void mapCallNakamaWakeUp5(void);
void mapCallNakamaWakeUp6(void);
void mapCallSetMotionSpeed(void);
void mapCallSetOt(s32 mode);
void mapCallSetOt7(void);
void mapCallSetOt8(void);
void mapFunc(void);
void fldCallTexFreeDelayed(void);
void mapEffEplAllCalcTask(void);
void mapIdouGo_Idx(s32 idx);
void mapIfMenuInit(void);
void mapIfDrawMsk(void);
void mapIsFldKeyFreeSub(void);
void mapIsFldKeyFreeSub3(void);
void mapIsFldKeyFreeSub4(void);
void fldCallSetOt2(void);
void mapProcShutDownSub2(s32 mode);
void mapNakamaReturnStart(void);
void mapNakamaStockOnCheck_fld(void);
void mapProcSequenceSub10(void);
void mapProcSequenceSub11(void);
void mapProcSequenceSub4(void);
void mapProcSequenceSub6(void);
void mapProcSequenceSub7(void);
void mapProcSequenceSub8(void);
void mapTakaraOpen(void);
void mapTakaraOpen_fld(s32 index);
void mapLeaf11(void);
extern char nbStrMapSetTakaraOpen02[];
void mdlChkMotionEnd(void);
extern char fldStrBadMafuHajikiLabel[];
extern char fldStrMwallDevilonlyLabel[];
extern char fldStrMap13WarpLabel[];
extern char fldStrRunwarpLabel[];
extern char fldStrHasigoLabel[];
extern char fldStrDexitwarpLabel[];
extern char fldStrElvLabel[];
extern char ProcSequenceStrBunriHajikiLabel[];
extern char fldStrBunriHajiki3Label[];
s32 MskL2Task(void);
void mapCallNakamaWakeUp(void);
void mapCallNakamaWakeUp3(void);
void mapNakamaZeniStart(void);
void mapNakamaHiroStart(void);
void ProcessStart_cmpMap(void);
void fldDrwZAlloc(void);

extern struct dds3Basic_t *playerBasic;
extern s32 ProcRandomEncountCheck_fldData3; /* moon age of the last treasure search */
extern s32 ProcRandomEncountCheck_fldData4; /* moon age of the last money search */
extern s32 ProcRandomEncountCheck_fldData5; /* moon age of the last takumi search */
extern const char fldKiretaLabel[];
extern const char ProcRandomEncountCheck_fldStrTakaraSagasiDvlItem[];
extern const char ProcRandomEncountCheck_fldStrZeniSagasiDvlYen[];
/* The last mapCallMap request (map 0 repeats it) and its script name buffer. */
extern struct fldSceneParam_t mapCallScriptBuf;
extern char mapCallMapFmt[];
extern s32 mapCallLastMap;
extern s32 mapCallLastFloor;
extern s32 mapCallLastPoint;
extern char *mapCallLastLabel;
void fldBgZoomTest(s32 mode);
void fldPlayerMotReset(void);
void mapSetSceneParam(struct fldSceneParam_t *param, s32 field, s32 floor, s32 point);
void scrAllReleaseScript(void);
s32 fldCallDelayedResrcCheck(void);
void fldExitEventSub(void);

#endif
