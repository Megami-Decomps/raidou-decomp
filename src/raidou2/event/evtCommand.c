#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtCommand.c */
#include "common.h"

#include "newdata.h"

#include "sdf_resource.h"

#include "event.h"

#include "kernel.h"

#include "scr.h"

#include "field.h"

#include "sdfFrame.h"

struct effPCPAgiraoFirePartList_t;

extern dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, const struct effPCPAgiraoFirePartList_t *effdata);

extern dds3ProcessID_t *evtStartPolygonMovie2(u32 priority, s32 eventno, s32 cutno);

void scrSetIntReturnValue(s32 i);

extern s32 scrGetIntPara(s32 n);

extern s32 evtLoadUnit(s32 major, s32 minor);

void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

dds3ProcessID_t *evtSearchEventPack(u32 eventno);

dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);

scrProcessWork_s *scrGetScriptHandle(void);

extern void datDebugPrintf(const char *fmt, ...);

extern s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);

void evtSetUnitPos(u32 unitID, u32 posID);

void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

void Mult(void);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

f32 scrGetFloatPara(s32 n);

struct dds3Basic_t *dds3GetActiveScene(void);

extern void *memset(void *a, s32 b, size_t c);

extern addr_t evtKutiUnit;

extern void evtRunUnitKutiMotion(addr_t h);

extern void itfMesMngSetKutiCallback(s32 id, void (*func)(void));

extern evtUnit_t *evtGetUnitHandle(s32 serial);

extern void evtSetUnitMove3(evtUnit_t *h, s32 pathID, s32 para1, s32 para2, s32 movesys, s32 movedir, s32 rotsys);

extern void evtSetUnitRot2(evtUnit_t *h, s32 posID, s32 para1);

extern void fldCamSyukanSuriawase(void);

/* Canonical provider signatures; callers retain their s32 parameter values. */
extern s32 evtPlayEventBgm(s32 eventId, s32 variation);

extern void evtTransEventBgm(s32 eventId, s32 variation);

extern s32 evtStopEventBgm(s32 eventId, s32 variation);

extern s32 evtPlaySe(s32 eventno, s32 seno);

extern s32 evtStartBgmBySoundIdAndFade(s32 eventId, s32 variation);

extern void datAddKotowariPoint(s32 type, s32 point);

extern void evtRefreshTaskEffectTexture(s32 taskId, s32 key);

extern s32 scrGetMessageHandle(void);

extern void evtSetUnitKutiMotion(evtUnit_t *h, s32 group, s32 motion);

s32 mdlChkMotionEndEx(const mdlObj_t *aHandle, s32 aGroup);

void dds3DeleteSceneBasicData(dds3Basic_t *basic);

void evtUnitLightOn(evtUnit_t *h);

void evtSetUnitLightDir(evtUnit_t *h, s32 frame);

extern const char evtMsgLightDirZero[];

void *evtGetResorceData(u32 eventno, s32 rid);

#include "sdk_libc.h"

/* Declared here because no header carries these. */
extern s32 datGetKotowariLank(s32 lank);

extern s32 p2sdBgmVolDown(void);

extern s32 p2sdBgmVolUp(void);

extern void Eve_DistortionBlurSetDefTex(void);

extern void dds3AdminiExitSequence(void);

extern void evtSetBokasi(s32 sw);

extern void nbBgmStop(void);

extern void dds3EffectObjectBasicStatOn(void *basic, s32 s);

extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(struct effPCPAgiraoFirePartList_t *effdata, void *pos, void *rot);

extern const char evtMsgEffectBeFailed[];

extern const char evtMsgEffectBeNotFound[];

extern void * dds3AddBEDEffectObjectBasic2(void *data, void *pos, void *rot);

typedef struct effEventWork_t effEventWork_t;

void effEveBEDCutSetScale(effEventWork_t *hEveBEDCut, f32 scale);

extern void dds3StartBEDEffectObjectBasic(void *basic, u32 cutno);

extern f32 evtCalcEffBedScale(s32);

enum {
    OBJECT_TRANSFORM_FLAG_UPDATE_PENDING = 1,
    OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID = 2,
    OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION = 8
};

typedef struct evtModelHeader_t {
    u8 pad00[4];
    u32 flags;
    u8 pad08[8];
    f32 positionX;
    f32 positionY;
    f32 positionZ;
} evtModelHeader_t;

void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);

enum {
    EVT_SCALAR_REQUEST_SIZE = 4,
    EVT_PAIRED_REQUEST_SIZE = 8,
    EVT_WORLD_OBJECT_KIND_FIRST = 4,
    EVT_WORLD_OBJECT_KIND_LIMIT = 10,
    EVT_CAMP_TASK_READY_VALUE = 2
};

void scrAllReleaseScript(void);

void CallBattleSub(s32 encno, s32 eventno);

void CallBattleSub2(s32 encno);

char *scrGetStrPara(s32 n);

void fldBgmFadeOut(void);

void fldSetSceneParam2(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName);

void fldSetSceneParam3(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName);

struct dds3Basic_t *fldGetUnitFollowCamera(void);

void *evtSearchBasic(s32 type, s32 serial);

struct dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 modelRangeID);

typedef s32 (*dds3MoverUpdate_t)(struct dds3PoseSet_t *, struct dds3Basic_t *);

typedef struct dds3SlotResource_t {
    struct dds3Basic_t *target;
    dds3PathBasicControl_t *path;
    dds3MoverUpdate_t update;
    struct dds3Basic_t *sourceObject;
} dds3SlotResource_t;

dds3PathBasicControl_t *dds3MoverBasicGetPathControl(struct dds3Basic_t *pMoverBasic);

void dds3ObjectBaseCreateMover(void *pObjectBasic);

void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);

s32 fldCommand_GetHojiID(char *pName);

s32 fldGbSetGimic(s32 fld, s32 area, s32 hojiID, s32 toplast);

extern void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);

extern const char evtMsgObjPathPara0[], evtMsgObjPathPara1[], evtMsgObjPathMoveNoObj[];

void evtEnableMoonCalc(void);

void evtDisableMoonCalc(void);

s32 evtGetAgeOfMoon(void);

s32 evtGetAgeOfMoon16(void);

void evtSetAgeOfMoon(s32 age);

dds3ProcessID_t *evtLoadEventPack(s32 eventno);

s32 evtGetEventPackStat(u32 eventno);

extern const char evtMsgBeOk[];

void sdfPrintf(const char *str, ...);

extern const char evtMsgLoadBeStart[];

extern const char evtMsgReadBeStart[];

extern const char evtMsgPmvRunNoProc[];

void evtPathRate(dds3Basic_t *basic, f32 rate);

void evtPathDir(dds3Basic_t *basic, u32 dir);

dds3ProcessID_t *evtStartPictureProcess(s32 priority, struct sdfTexHandle_t *texpath);

extern const char evtMsgTexBeNoProc[];

typedef u8 sdfTextureClampMode_t;

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

struct sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *aTex);

extern sdfTexHandle_t *DeadMoonTmx;

void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);

void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);

void evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a);

#include "basic.h"

#include "dds3obj.h"

#include "ps2asm.h"

extern void dds3EffectObjectPresetMG12(dds3Basic_t *eff);

extern s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1,
                                      dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4);

extern void dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target);

extern s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target,
                                                 s32 index);

extern void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic);

extern void *dds3GetControlScene(void);

extern s32 dds3GetMapNo(void *scene);

extern void dds3SetActiveCameraSceneBasic(void *pSceneBasic, dds3Basic_t *pCameraBasic);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 serialID, f32 *pos, f32 *rot);

extern s32 dds3AllocObjectId(void);

extern void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r);

extern u32 dds3DrawFlag;

extern void evtEffectLoopControl(dds3Basic_t *eff, s32 sw);

extern s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);

extern s32 evtCheckPathStat(dds3Basic_t *obj);

extern s32 evtRegistBEField(s32 eventno, s32 rid);

extern s32 evtLoadArea(s32 fld, s32 area);

extern s32 evtStageAnimeControl(s32 type, s32 mode);

extern void evtFreeAllUnit(void);

extern void evtFreeStage(void);

extern void fldSetNextField(s32 command, s32 para);

extern s32 sdfDelayedResrcCheck(void);

extern const char evtMsgNameNotFound[];

extern const char evtMsgEffmg1LoadHint[];

extern const char evtMsgEffmg2LoadHint[];

extern void dds3ObjectBaseOnFlag(void *basic, s32 flag);

extern void dds3ObjectBaseOffFlag(void *pObjectBasic, s32 flag);

extern void dds3SetSclPoseSet(void *pBasic, u128 *scl);

extern s32 evtCheckUnitWait(evtUnit_t *h);

extern void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode);

extern void evtUnitLightOff(evtUnit_t *h, s32 frame);

extern u8 evtUnitLightAnimeStat(evtUnit_t *h);

extern const char evtMsgModelMotionDeprecated[];

extern const char evtMsgModelMoveMotDeprecated[];

extern void func_001E3C30(void *unit, s32 p1, s32 p2, s32 p3, s32 p4, s32 p5);

extern void evtLinkLightObjectBasic(evtUnit_t *h, dds3Basic_t *light, s32 frame);

extern void evtUnlinkLightObjectBasic(evtUnit_t *h, s32 frame);

extern void evtPathStop(dds3Basic_t *basic);

extern void evtPathStart(dds3Basic_t *basic);

extern void evtPathLoop(dds3Basic_t *basic, s32 sw);

extern void evtLoadEventBgm(s32 event);

extern s32 evtCheckEventBgmLoaded(s32 event);

extern s32 scrGetCommandTimer(void);

extern void evtSetSkyAlpha(s32 frame, s32 a);

extern s32 dds3ExistProcess(void *id);

extern f32 sdfSin(f32 aRadius);

extern void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh,
                         s32 rgba, sdfTexHandle_t *th);

/* Rows of 0x310 bytes; only the three size fields the bed scale reads are
 * named here. */
typedef struct {
    u8 unk00[0x10];
    f32 formsize;                   /* 0x10 */
    f32 formheight;                 /* 0x14 */
    f32 formscale;                  /* 0x18 */
    u8 unk1C[0x310 - 0x1C];
} datModelParam_t;

extern datModelParam_t datModelParamTable[];

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct {
    u32 word;
    u8 pad04[12];
} evtColorWork_t;

#define EVT_RGBA_SCALE_128 0x43000000

extern void evtPictureSwap(void *id1, void *id2, s32 pat, s32 prm);

extern void evtPictureSetSize(void *id, s32 sw, s32 sh, s32 tw, s32 th);

extern void evtPictureSetPos(void *id, s32 x0, s32 y0, s32 x1, s32 y1, s32 time);

extern void evtPictureSetAlpha(void *id, s32 alp0, s32 alp1, s32 time);

extern void evtPictureSetZoom(void *id, s32 zoom0, s32 zoom1, s32 time);

extern s32 evtPictureStat(void *id);

extern void fldProc_SisetuReturnPoint(s32 fld, s32 area, char *pName);

extern void fldBgmStop(void);

extern void fldPauseBgm(void);

extern void p2sdPlaySE(s32 no, s32 volume, s32 pan);

extern void encStart(void);

extern s32 evtNextEventNo;

extern void evtSetPolygonMovieFlag(void *id, s32 flag);

extern void evtResetPolygonMovieFlag(void *id, s32 flag);

extern u8 evtCheckPolygonMovieFlag(void *id, s32 flag);

extern void sdfTexFreeDelayed(void *aHandle);

extern void evtSetUnitMove2(evtUnit_t *h, s32 movetype, s32 posID, s32 motion,
                            s32 para1, s32 para2, void *func, void *data);

extern const char evtMsgPmvLoadArgs[];

extern const char evtMsgPmvLoadNoProc[];

extern const char evtMsgPmvLoadStart[];

extern const char evtMsgPmvLoadEnd[];

extern const char evtMsgPmvRun2Begin[];

extern const char evtMsgPmvRun2End[];

extern void evtPictureDrawOn(void *id);

extern void evtPictureDrawOff(void *id);

extern void evtKillPictureProcess(void *id);

extern const char evtMsgLightPathMoveError[];

extern const char evtMsgLightPathMoveErrorDbg[];

extern const char evtMsgSetSkyAlphaLow[];

extern const char evtMsgSetSkyAlphaHigh[];

extern s32 *dds3GetActiveUnitSceneBasic(void *pSceneBasic);

extern void fldSetFilterAnime(s32 on);

extern void fldSkySetAlpha(s32 alp);

extern void fldSkySetFilter(s32 idx, s32 mode, s32 hokan);

extern const char evtMsgBeOk[];

extern dds3Basic_t *dds3SceneBasicSearchName(void *scene, const char *name);

extern dds3Basic_t *dds3AddMG12EffectObjectBasic(s32 type, const char *file);

extern void *evtStartPictureProcess2(s32 priority, const char *texdata);

extern const char evtMsgTexLoadNoProc[];

extern const char evtMsgIdNotFound[];

extern const char evtMsgIdNotFoundWarn[];

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} evtVec_t;

extern dds3Basic_t *dds3AddD3PEffectObjectBasic(const char *file, evtVec_t *pos, evtVec_t *rot);

extern dds3Basic_t *dds3AddBEDEffectObjectBasic(const char *file, evtVec_t *pos, evtVec_t *rot);

extern const char evtMsgEffectLoadHint[];

extern void evtKillEventPack(s32 eventno);

extern const char evtMsgObjPathPara0[];

extern const char evtMsgObjPathPara1[];

extern const char evtMsgObjPathWaitNoObj[];

extern const char evtMsgDelBeStart[];

void *evtSearchBasic(s32 type, s32 serial);

u32 evtCommand_MODEL_PLAYER(void);

u32 evtCommand_MODEL_DSP(void);

u32 evtCommand_MODEL_BE(void);

u32 evtCommand_MODEL_POS(void);

u32 evtCommand_MODEL_SHADOW_ON(void);

u32 evtCommand_MODEL_SHADOW_ON2(void);

u32 evtCommand_MODEL_SHADOW_OFF(void);

u32 evtCommand_MODEL_POS_XYZ(void);

u32 evtCommand_MODEL_ROT_XYZ(void);

u32 evtCommand_MODEL_SCL_XYZ(void);

u32 evt_1E50C0(void);

u32 evt_1E5158(void);

u32 evt_1E51F8(void);

void evtRunKutiMotion(void);

u32 evtCommand_KUTISET(void);

u32 evtCommand_KUTI(void);

u32 evtCommand_MODEL_WAIT(void);

u32 evtCommand_MODEL_EXWAIT(void);

u32 evt_1E54B8(void);

u32 evtCommand_MODEL_MOVE(void);

u32 evtCommand_MODEL_MOVE2(void);

u32 evtCommand_MODEL_WAIT_EXMOT(void);

/* Store the script-supplied value for the selected event unit. */
u32 evtCommand_MODEL_WAIT_MOT(void);

/* Store the two script-supplied world-unit state values. */
u32 evtCommand_MODEL_WAIT_HOKAN(void);

u32 evtCommand_MODEL_MOVE_MOT(void);

u32 evtCommand_MODEL_ROT(void);

u32 evtCommand_MODEL_ON(void);

u32 evtCommand_MODEL_OFF(void);

u32 evtCommand_MODEL_LIGHT_ON(void);

u32 evtCommand_MODEL_LIGHT_OFF(void);

u32 evtCommand_MODEL_LIGHT_WAIT(void);

u32 evtCommand_MODEL_LIGHT_RGB(void);

u32 evtCommand_MODEL_LIGHT_RGB(void);

u32 evtCommand_MODEL_LIGHT_DIR(void);

u32 evtCommand_MODEL_LIGHT_DIR(void);

u32 evtCommand_MODEL_RGB(void);

u32 evtCommand_MODEL_RGB(void);

u32 evtCommand_MODEL_A(void);

u32 evtCommand_MODEL_A(void);

u32 evtCommand_MODEL_LIGHT_LINK(void);

u32 evtCommand_MODEL_LIGHT_UNLINK(void);

u32 evtCommand_EFFECT_BE(void);

u32 evtCommand_EFFECT_LOAD(void);

u32 evtCommand_EFFBED_LOAD(void);

extern const char evtMsgModelMotionDeprecated[64];

extern const char evtMsgModelMoveMotDeprecated[64];

extern const char evtMsgLightDirZero[40];

extern const char evtMsgEffectBeNotFound[32];

extern const char evtMsgEffectBeFailed[24];

extern const char evtMsgEffectLoadHint[48];

extern const char evtMsgNameNotFound[24];

u32 evtCommand_EFFBED_BE(void);

u32 evtCommand_EFFBED_BE_EVO(void);

u32 evtCommand_EFFBED_START(void);

f32 evtCalcEffBedScale(s32 uid);

u32 evtCommand_EFFBED_START_EVO(void);

u32 evtCommand_EFFMG1_LOAD(void);

extern const char evtMsgEffmg1LoadHint[48];

u32 evtCommand_EFFMG1_BE(void);

u32 evtCommand_EFFMG2_LOAD(void);

extern const char evtMsgEffmg2LoadHint[48];

u32 evtCommand_EFFMG2_BE(void);

u32 evtCommand_EFFMG2_PRESET(void);

u32 evtCommand_EFFECT_PRESET(void);

u32 evtCommand_EFFECT_CLS(void);

u32 evtCommand_EFFECT_POS_XYZ(void);

u32 evtCommand_EFFECT_ROT_XYZ(void);

u32 evtCommand_EFFECT_POS(void);

s32 evtCommand_EFFMG1_POS(void);

s32 evtCommand_EFFMG2_POS(void);

s32 evtCommand_EFFECT_ON(void);

s32 evtCommand_EFFECT_LINK(void);

s32 evtCommand_EFFECT_LINK2(void);

u32 evtCommand_EFFECT_UNLINK(void);

s32 evtCommand_EFFECT_OFF(void);

s32 evtCommand_EFFECT_PATH_MOVE(void);

s32 evtCommand_EFFECT_PATH_WAIT(void);

u32 evtCommand_SEARCH_ID(void);

s32 evtCommand_RTRN_TITLE(void);

s32 evtCommand_CALL_STAFF(void);

s32 evtCommand_CALL_ENDSAVE(void);

u32 evtCommand_CALL_COMBINE(void);

u32 evtCommand_CALL_RECOVER(void);

u32 evtCommand_CALL_SHOP(void);

u32 evtCommand_CALL_RAG(void);

/* Forward two VM integers as the admin request's two-word payload. */
s32 evtCommand_CALL_SAVE(void);

u32 evtCommand_CALL_PUZZLEBOY(void);

extern const char evtMsgIdNotFound[32];

extern const char evtMsgIdNotFoundWarn[32];

s32 evtCommand_CALL_NEXT(void);

/* The event ID becomes a four-byte dispatch payload. */
s32 evtCommand_CALL_EVENT(void);

u32 evtCommand_EXIT_EVENT(void);

void CallBattleSub(s32 encno, s32 eventno);

s32 evtCommand_CALL_BATTLE(void);

void CallBattleSub2(s32 encno);

s32 evtCommand_CALL_BATTLE2(void);

s32 evtCommand_RESTORE_BATTLE(void);

/* A field-sequence request is the 0xA0-byte scene parameter block. */
s32 evtCommand_CALL_FIELD(void);

/* Use the current group's stage with the VM's kind and sequence-name inputs. */
s32 evtCommand_CALL_FIELD2(void);

/* Build the current group's field request using the alternate initializer. */
s32 evtCommand_CALL_FIELD3(void);

s32 evtCommand_FIELD_BE(void);

s32 evtCommand_FIELD_DSP(void);

s32 evtCommand_FIELD_ANIME_START(void);

s32 evtCommand_FIELD_ANIME_STOP(void);

s32 evtCommand_FIELD_CLS(void);

s32 evtCommand_FIELD_CLS_STOP(void);

s32 evtCommand_UNIT_CLS(void);

s32 evtCommand_UNIT_CLS_STOP(void);

u32 evtCommand_CAM_SEL(void);

u32 evtCommand_CAM_SEL_FOVY(void);

s32 evtCommand_CAM_PATH_MOVE(void);

s32 evtCommand_CAM_PATH_WAIT(void);

s32 evtCommand_CAM_CREATE(void);

s32 evtCommand_CAM_CLS(void);

s32 evtCommand_CAM_POS_XYZ(void);

/* Convert the three degree inputs before composing and storing a quaternion. */
s32 evtCommand_CAM_ROT_XYZ(void);

s32 evtCommand_CAM_ROT_Q(void);

s32 evtCommand_OBJ_PATH_MOVE(void);

s32 evtCommand_OBJ_PATH_WAIT(void);

s32 evtCommand_MOON_START(void);

s32 evtCommand_MOON_STOP(void);

s32 evtCommand_GET_MOON(void);

s32 evtCommand_GET_MOON16(void);

s32 evtCommand_CHG_MOON(void);

/* Create a missing camp task, then wait for its ready value.
 * No VM context completes immediately; a pending task returns zero. */
s32 evtCommand_LOAD_BE(void);

/* Start the camp task only if no task with this ID exists yet.
 * A missing VM context completes without creating a task. */
s32 evtCommand_READ_BE(void);

u32 evtCommand_CHK_BE(void);

u32 evtCommand_DEL_BE(void);

u32 evtCommand_PMV_LOAD(void);

u32 evtCommand_PMV_RUN2(void);

u32 evtCommand_PMV_LOAD_END_WAIT(void);

/* Create and return an event/scene movie task without setting bit 0.
 * The owning scheduler task supplies the viewer task's priority. */
s32 evtCommand_PMV_RUN(void);

s32 evtCommand_PATH_STOP(void);

s32 evtCommand_PATH_START(void);

/* Apply the float state, then update a room flag when its parsed number is positive.
 * Values above 0.5 set that flag; the VM float parameter is deliberately re-read. */
s32 evtCommand_PATH_RATE(void);

/* Apply the integer state and update a positive parsed room number's flag.
 * Unlike the float command, an integer value of zero sets the room flag. */
s32 evtCommand_PATH_DIR(void);

s32 evtCommand_PATH_LOOP_ON(void);

s32 evtCommand_PATH_LOOP_OFF(void);

s32 evtCommand_LIGHT_PATH_MOVE(void);

s32 evtCommand_LIGHT_PATH_WAIT(void);

extern const char evtMsgObjPathPara0[16];

extern const char evtMsgObjPathPara1[16];

extern const char evtMsgObjPathMoveNoObj[32];

extern const char evtMsgObjPathWaitNoObj[32];

extern const char evtMsgLoadBeStart[16];

extern const char evtMsgBeOk[16];

extern const char evtMsgReadBeStart[16];

extern const char evtMsgDelBeStart[16];

extern const char evtMsgPmvLoadArgs[24];

extern const char evtMsgPmvLoadNoProc[72];

extern const char evtMsgPmvLoadStart[24];

extern const char evtMsgPmvLoadEnd[16];

extern const char evtMsgPmvRun2Begin[16];

extern const char evtMsgPmvRun2End[16];

extern const char evtMsgPmvRunNoProc[72];

extern const char evtMsgLightPathMoveError[24];

extern const char evtMsgLightPathMoveErrorDbg[32];

extern const char evtMsgTexBeNoProc[72];

u32 evtCommand_TEX_BE(void);

u32 evtCommand_TEX_LOAD(void);

u32 evtCommand_TEX_ON(void);

u32 evtCommand_TEX_OFF(void);

u32 evtCommand_TEX_CLS(void);

u32 evtCommand_TEX_SWAP(void);

extern void evtPictureSetOt(dds3ProcessID_t *id, s32 ot);

extern void evtPictureSetPri(dds3ProcessID_t *id, s32 pri);

u32 evtCommand_TEX_OT(void);

u32 evtCommand_TEX_PRI(void);

u32 evtCommand_TEX_SIZE(void);

u32 evtCommand_TEX_POS(void);

u32 evtCommand_TEX_ALP(void);

u32 evtCommand_TEX_ZOOM(void);

u32 evtCommand_TEX_STAT(void);

u32 evtCommand_SET_SKY(void);

u32 evtCommand_SET_SKY_A(void);

u32 evtCommand_BOKASI_ON(void);

u32 evtCommand_BOKASI_OFF(void);

u32 evtCommand_LOAD_SOUND_E(void);

u32 evtCommand_BGM_PLAY_E(void);

u32 evtCommand_BGM_TRANS_E(void);

u32 evtCommand_FADEOUT_E(void);

u32 evtCommand_BGM_VOL_DOWN(void);

u32 evtCommand_BGM_VOL_UP(void);

u32 evtCommand_SE_PLAY_E(void);

u32 evtCommandStartBgmWithFade(void);

u32 evt_1E91A8(void);

u32 evt_1E91E8(void);

s32 evt_1E9220(void);

s32 evt_1E9248(void);

s32 evt_1E9270(void);

u32 evtOpcodeRefreshTaskData(void);

u32 evt_1E92D8(void);

u32 evtCommandSetDrawFlagWhileWaiting(void);

void evtDrawDeadMoonSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                           s32 rgb, sdfTexHandle_t *th, s32 ox, s32 oy,
                           f32 cnt, f32 alpha0, f32 alpha1, f32 scale0, f32 scale1);

/* Acquire the background texture, then release the temporary file allocation. */
extern const char evtMsgTexLoadNoProc[72];

extern const char evtMsgSetSkyAlphaLow[40];

extern const char evtMsgSetSkyAlphaHigh[];

void nbLoadDeadMoonTex(void);

void nbFreeDeadMoonTex(void);

extern addr_t evtKutiUnit;

extern sdfTexHandle_t *DeadMoonTmx;
/* End of declarations from src/smt3/event/evtCommand.c */

/* Declarations from dds-decomp src/dds2/event/evtCommand.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#define SMT3_DEFINES_EVT_CAMP_TASK_READY_VALUE 1

#define SMT3_DEFINES_EVT_PAIRED_REQUEST_SIZE 1

#define SMT3_DEFINES_EVT_SCALAR_REQUEST_SIZE 1

#define SMT3_DEFINES_EVT_WORLD_OBJECT_KIND_FIRST 1

#define SMT3_DEFINES_EVT_WORLD_OBJECT_KIND_LIMIT 1

#define SMT3_DEFINES_OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID 1

#define SMT3_DEFINES_OBJECT_TRANSFORM_FLAG_UPDATE_PENDING 1

#define SMT3_DEFINES_OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION 1

#define SMT3_DEFINES_datModelParam_t 1

#define SMT3_DEFINES_dds3MoverUpdate_t 1

#define SMT3_DEFINES_dds3SlotResource_t 1

#define SMT3_DEFINES_effEventWork_t 1

#define SMT3_DEFINES_evtColorWork_t 1

#define SMT3_DEFINES_evtModelHeader_t 1

#define SMT3_DEFINES_evtVec_t 1

#define SMT3_DEFINES_sdfTextureClampMode_t 1

#include "dds/eff_transform.h"

/* Sparse world-node identities recovered from the paired operation tables and
 * concrete creators. Unlisted values stay unnamed; COUNT remains the full
 * directory extent rather than an enum-completeness claim. */
enum {
    EFF_WORLD_KIND_WORLD_INFO = 0,
    EFF_WORLD_KIND_EVENT_TABLE = 1,
    EFF_WORLD_KIND_WORLD_INDEX_OWNER = 2,
    EFF_WORLD_KIND_SLOT_RING = 3,
    EFF_WORLD_KIND_CAMERA = 4,
    EFF_WORLD_KIND_FOLLOW_MODEL = 5,
    EFF_WORLD_KIND_EFFECT_TRANSFORM = 6,
    EFF_WORLD_KIND_EFFECT_OBJECT = 7,
    EFF_WORLD_KIND_RESOURCE_OWNER = 8,
    EFF_WORLD_KIND_LIGHT = 9,
    EFF_WORLD_KIND_SCRIPT_OBJECT = 10,
    EFF_WORLD_KIND_ACTION_B = 11,
    EFF_WORLD_KIND_ACTION_D = 13,
    EFF_WORLD_KIND_ACTION_10 = 16,
    EFF_WORLD_KIND_TRANSFORM_SOURCE = 17,
    EFF_WORLD_KIND_COUNT = 18
};

#include "dds/sdk_ps2.h"

#include "dds/fld_area_work.h"

#include "dds/evt_encounter_request_api.h"

#include "dds/fld.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/common.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/dds3_camera_update_flags.h"

#include "dds/sdf_delayed_resource_check_api.h"

#include "dds/sdf_vu0.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/evt_stage_anime_control_api.h"

#include "dds/dds3_task_add_child.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/evt_world_object_api.h"

#include "dds/evt_polygon_movie_flags_api.h"

#include "dds/fld_resource_resolver.h"

#include "dds/dds3_path.h"

#include "dds/dds3obj.h"

#include "dds/dds3_object_control_slot.h"

#include "dds/dds3_world_selection.h"

#include "dds/eff_dependency.h"

#include "dds/eff_object.h"

#include "dds/fld_player_scene_state.h"

#include "dds/eff_inner_float_api.h"

#include "dds/pcp_vu0.h"

#include "dds/kwln.h"

#include "dds/evt_task.h"

#include "dds/evt_world.h"

#include "dds/evt_path_direction.h"

#include "dds/evt_event_pack.h"

#include "dds/scr.h"

#include "dds/scr_command_result.h"

#include "dds/scr_command_timer_api.h"

#include "dds/scr_process_handle_api.h"

#include "dds/scr_string_parameter_api.h"

#include "dds/scr_arg_read.h"

#include "dds/evt_solar.h"

#include "dds/dds3_world_counter.h"

/* Selects both the world-node tag and its per-kind scene slot. */
typedef s32 EffWorldKind;

#include "dds/sdf_debug_api.h"

extern s32 D_00389780[];

void dds3RefreshStoredVec3(dds3Basic_t *object);

extern char D_00421ED8[];

extern char D_00421F08[];

extern char D_00421F68[];

void fldSetMapSlotValueFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled);

extern char D_00421FB0[];

extern char D_00421FC8[];

extern void fldSetDeferredFieldCommand(s32 a, s32 b);

ScrCommandResult evtCommandSignalAdminAtTimerZero(void);

/* Queue the encounter and retain a positive follow-up event ID for dispatch. */
void evtSubmitEventRequest(FldBattleSceneRecordIndex sceneRecordIndex,
                           EvtScriptEventId followupEventId);

ScrCommandResult evtCommandSubmitEvent(void);

/* Stop BGM and submit the event payload with immediate dispatch enabled. */
void evtSubmitEventRequestImmediate(FldBattleSceneRecordIndex sceneRecordIndex);

/* Forward the first VM integer as an opaque four-byte admin payload. */
ScrCommandResult evtCommandSubmitScalarAdminRequest(void);

ScrCommandResult evtCommandAddEffectUnitToWorld(void);

extern char D_00421E58[], D_00421E68[], D_00421E78[];

extern char D_00421E98[];

ScrCommandResult evtCommandReattachObjectPath(void);

/* Create an event/scene movie task using the active camp resource, hold its
 * viewer setup until the script releases it, and return its handle through the
 * VM result. Missing work skips creation. */
extern dds3ProcessID_t * evtViewerCreateTask(u32 priority, s32 eventId, s32 sceneId);

ScrCommandResult evtCommandStartPolygonMovie(void);
/* End of declarations from src/dds2/event/evtCommand.c */

/* Declarations from dds-decomp src/dds2/event/evtUnitManager.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#define SMT3_DEFINES_EFF_WORLD_KIND_ACTION_10 1

#define SMT3_DEFINES_EFF_WORLD_KIND_ACTION_B 1

#define SMT3_DEFINES_EFF_WORLD_KIND_ACTION_D 1

#define SMT3_DEFINES_EFF_WORLD_KIND_CAMERA 1

#define SMT3_DEFINES_EFF_WORLD_KIND_COUNT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_EFFECT_OBJECT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_EFFECT_TRANSFORM 1

#define SMT3_DEFINES_EFF_WORLD_KIND_EVENT_TABLE 1

#define SMT3_DEFINES_EFF_WORLD_KIND_FOLLOW_MODEL 1

#define SMT3_DEFINES_EFF_WORLD_KIND_LIGHT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_RESOURCE_OWNER 1

#define SMT3_DEFINES_EFF_WORLD_KIND_SCRIPT_OBJECT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_SLOT_RING 1

#define SMT3_DEFINES_EFF_WORLD_KIND_TRANSFORM_SOURCE 1

#define SMT3_DEFINES_EFF_WORLD_KIND_WORLD_INDEX_OWNER 1

#define SMT3_DEFINES_EFF_WORLD_KIND_WORLD_INFO 1

#include "dds/btl.h"

#include "dds/dds3_path.h"

#include "dds/dds3_scene_membership.h"

#include "dds/dds3obj.h"

#include "dds/eff_quaternion_nlerp.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_move_end_api.h"

#include "dds/evt_unit_path_advance_api.h"

#include "dds/evt_world.h"

#include "dds/mdl.h"

#include "dds/mdl_motion_api.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_float_counter.h"

#include "dds/sdf_map_position_api.h"

#include "dds/sdf_model_scalars.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdk_libm.h"

enum EvtTargetInfoFlags {
    EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR = 0x8
};

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * DDS1 and DDS2 use the same observed layout. Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

/* Blend the quaternion in vf10 toward vf11 along the shorter arc using
 * blendAmount, normalize the result, and leave it in vf10. Providers:
 * DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void Lerp(f32 blendAmount);

enum SdfMotionStateValue {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
};

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void ToMatrix(void);

f32 acosf(f32 x);

void dds3AddRotPoseSet(dds3Basic_t *object, u128 *vector);

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(struct dds3Basic_t *object);

void dds3DeletePathControl(dds3PathBasicControl_t *path);

void dds3GetPosPoseSet(dds3Basic_t *object);

void dds3GetPosPoseSetMCR(dds3Basic_t *object, u128 *vector);

void dds3GetRotPoseSet(dds3Basic_t *object);

void dds3PathBasicControlGetPos(dds3PathBasicControl_t *path);

f32 dds3PathBasicControlGetRatio(const SdfFloatCounter *counter);

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *path);

void dds3PathBasicControlSetRatio(SdfFloatCounter *counter, f32 multiplier);

Dds3SceneMembershipResult dds3SceneBasicExistBasicFromType(
    const struct dds3Basic_t *object, s32 slotIndex, const struct dds3Basic_t *targetNode);

EvtUnitMoveEndResult evtCheckMoveEnd(evtUnitHandle_t *unit);

EvtUnitPathAdvanceResult evtGetNextMovePos(evtUnitHandle_t *unit);

u32 mdlGetColor(const dds3ModelHandle_t *ctx);

/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNumOfCut(const dds3ModelHandle_t *ctx, s32 searchId);

/* VU helpers transfer vectors through vf10 and basis rows through vf28-vf30. */
void mdlGetPosition(dds3ModelHandle_t *ctx);

void mdlSetColor(dds3ModelHandle_t *ctx, u32 value);

void mdlSetMotionSpeedEx(dds3ModelHandle_t *ctx, MdlMotionSearchId searchId, f32 frameStep);

void mdlSmoothLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                          f32 blendDurationFrames);

void mdlSmoothSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                       f32 blendDurationFrames);

extern void *memcpy(void *, const void *, unsigned int);

/* Angles are in radians; cosine applies a pi/2 shift to the sine input. */
f32 sdfAtan2(f32 y, f32 x);

void *sdfChipAllocDirect(s32 size);

void sdfChipFreeDirect(void *memory);

/* On success, write the requested MPOS record's position or matrix to VU state. */
SdfMapPositionLookupResult sdfModelGetMarkPos(
    struct dds_sdfModelRootNode_t *model, s32 id);

void sdfModelResetFaceDivParm(struct dds_sdfModelRootNode_t *model);

void sdfModelSetFaceDivParm(struct dds_sdfModelRootNode_t *model, f32 first, f32 second);

#include "dds/sdk_ps2.h"

struct sdfGrOt_t;

struct EffNodeDescriptor;

#include "dds/common.h"

#include "dds/scr_command_result.h"

#include "dds/sdf_map_position_api.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/dds3_object_flags.h"

#include "dds/sdf_vu0.h"

#include "dds/btl_model_record.h"

#include "dds/sdf_model_scalars.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_chip.h"

#include "dds/dds3_path.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_object_key.h"

#include "dds/evt_unit_path_mode.h"

#include "dds/evt_path_direction.h"

#include "dds/evt_unit_motion_api.h"

#include "dds/evt_unit_motion_callback_result.h"

#include "dds/evt_unit_motion_step_result.h"

#include "dds/evt_unit_move_end_api.h"

#include "dds/evt_unit_path_advance_api.h"

#include "dds/evt_unit_wait_api.h"

#include "dds/evt_unit_facing_result_api.h"

#include "dds/evt_unit_light_animation_status.h"

#include "dds/evt_unit_vector_slot.h"

#include "dds/evt_unit_motion_slot_flags.h"

#include "dds/evt_unit_transition_flags.h"

#include "dds/evt_unit_motion_setup.h"

#include "dds/sdf_float_counter.h"

#include "dds/evt_unit_motion_slot.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/evt_unit_vertical_move.h"

#include "dds/eff_transform.h"

#include "dds/eff_object.h"

#include "dds/dds3obj.h"

#include "dds/mdl.h"

#include "dds/sdf_draw.h"

#include "dds/pcp_vu0.h"

#include "dds/ee_mmi.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/eff_quaternion_nlerp.h"

extern f32 *D_0037F770[];

extern u8 kwlnDefaultColorVector[];

extern void evtGetUnitDirVector(evtUnitHandle_t *unit);

typedef struct PcpScatterWork4 PcpScatterWork4;

/* func_001730D0 */
struct PcpScatterWork4 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x12C];
    f32 unk17C;
    u32 unk180;
    u32 scatterObject;
    u32 ownedBuffer;
};

/* Length of the path's vec4 trajectory sampled at 20 steps of the value multiplier. */
f32 GetSplineLength(dds3PathBasicControl_t *path);

extern f32 sdfFaceDivSoften, sdfFaceDivBias;

#include "dds/dds3_world_primary.h"

#include "dds/dds3_scene_membership.h"

extern void dds3RotationArc(void);

/* Advance the model, light-colour and directional transitions for one event unit. */
void evtAdvanceUnitVisualTransitions(evtUnitHandle_t *unit);

/* vu0 routine: load vf10 with the unit's colour (flag 0x100), its target's vector, or the default */
void GetBaseLightRGB(evtUnitHandle_t *unit);

/* vu0 routine: load the target info secondColor vector when unit-owned. */
void GetBaseAmbientRGB(evtUnitHandle_t *unit);

/* vu0 routine: load vf10 with the unit's vector, or the default one */
void GetBaseLightDir(evtUnitHandle_t *unit);

extern void evtApplyMatchingUnitSlotEndpoints(evtUnitHandle_t *unit);

/* Refresh endpoint render work after the value-change flag is cleared; defer
 * to the matching-slot selector when no target or color transition is active. */
void evtRefreshUnitEndpointWork(evtUnitHandle_t *unit);

void evtSetUnitValueTransition(evtUnitHandle_t *unit, dds3Basic_t *target, s32 duration);

void evtEndUnitValueTransition(evtUnitHandle_t *unit, s32 duration);

void evtUnitSetValueAndFlag(evtUnitHandle_t *unit, u32 value);

void evtUnlinkExternalLightPacket(evtUnitHandle_t *unit);

evtUnitHandle_t *evtGetUnitHandle2(dds3Basic_t *unit);

extern f32 D_004215D0[];

evtUnitHandle_t *evtCreateUnitTransitionWork(dds3Basic_t *effObj, dds3ModelHandle_t *owner);

s32 evtDeleteUnitHandle(evtUnitHandle_t *transitionWork);

EvtUnitMotionState evtGetUnitStat(evtUnitHandle_t *unit);

/* Callers pass the promoted full-width value; only the low halfword is stored. */
void evtSetWaitMotion(evtUnitHandle_t *unit, s32 storedParameter);

void evtSetWaitMotionSpeed(evtUnitHandle_t *unit, f32 transitionMotionScale);

void evtSetWaitHokanFrame(evtUnitHandle_t *unit, s32 firstParameter, s32 secondParameter);

void evtActivateStoredUnitMotionSlot(evtUnitHandle_t *unit);

void evtConfigureUnitMotionSlot(evtUnitHandle_t *unit, s32 slot, s32 slotAValue, s32 slotBValue,
                                s32 slotCValue, EvtUnitMotionSlotMode mode);

#define EVT_UNIT_FLAG_MOTION_START_PENDING 0x80

#define EVT_UNIT_FLAG_MOTION_WOBBLE 0x40

extern const s32 D_004215E0[];

extern const s32 D_004215F0[];

extern void evtVu0SubVectorXZAxis(f32 *a, f32 *b);

extern void FromRotation(f32 angle);

EvtUnitFacingTargetResult evtRotateUnitTowardTarget(evtUnitHandle_t *unit, f32 *dir, f32 scale);

/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with evtRotateUnitTowardTarget. */
EvtUnitFacingTargetResult evtUnitRotationQuat(evtUnitHandle_t *unit, f32 *quat, f32 angle);

EvtUnitMotionStepResult evtUnitMoveType0(evtUnitHandle_t *unit);

#include "dds/sdf_debug_api.h"

EvtUnitVerticalMoveStepCount evtUnitPrepareVerticalMoveSteps(evtUnitHandle_t *unit);

#define EVT_UNIT_FLAG_USE_PATH_ROTATION 0x10

EvtUnitMotionStepResult evtUnitMoveTypePathTrace(evtUnitHandle_t *unit);

void evtCopyUnitTargetVector(evtUnitHandle_t *work, const f32 src[4]);

#include "dds/sdf_trig.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/sdf_unique_chunk_value_api.h"

#include "dds/dds3_task_add_child.h"

#include "dds/fpu.h"

#include "dds/mdl_motion_api.h"

#include "dds/mdl_motion_end_status.h"

#include "dds/mdl_world_object_source_vector_api.h"

#include "dds/itf_mes_window.h"

#include "dds/evt_world.h"

#include "dds/evt_world_source_transform.h"

#include "dds/evt_model_header.h"

#include "dds/evt_lips_link.h"

#include "dds/evt_reset_unit_vector_slots_api.h"

#include "dds/evt_task.h"

#include "dds/evt_event_pack.h"

#include "dds/eff_object.h"

#include "dds/eff_object_dispatch_api.h"

#include "dds/eff_event.h"

#include "dds/eff_node_descriptor.h"

#include "dds/scr.h"

#include "dds/scr_string_parameter_api.h"

#include "dds/scr_arg_read.h"

extern evtUnitHandle_t *evtWindowMotionUnit;

extern s32 D_004371F0;

extern u32 sdfGetLodChunkValue(dds_sdfModelRootNode_t *model);

extern EvtUnitVectorSlot evtUnitVectorSlots[10];

extern BtlActorStatusRecord *D_00435DF0;

extern void mnuInitializeCampPanelVisualDefaults(f32 *firstVector, f32 *secondVector, f32 *thirdVector, f32 *scalarA, f32 *scalarB);

#include "dds/dds3_world_secondary.h"

extern void evtSetUnitMove1(evtUnitHandle_t *work, s32 motionSubmode, const f32 vector[4], s32 unused, s32 frames, s32 valueB6, EvtUnitMotionCallback callback, s32 unusedLast);

extern void evtArmEffectObjectPendingValue(dds3Basic_t *object, s32 value);

extern void evtResetObjectPendingValue(dds3Basic_t *object);

extern u8 D_00421AC0[];

extern dds3Basic_t *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *descriptor, void *firstVector, void *secondVector);

struct SoundMixer;

extern char D_00421A78[];

extern dds3Basic_t *effObjSpawnLoadedResourceEffect(const char *resourceName, void *firstVector, void *secondVector);

extern u8 D_00421A90[];

extern u8 D_00421B20[];

extern u8 D_00421B70[];

extern void evtEndUnitValueTransition(evtUnitHandle_t *unit, s32 duration);

extern char D_00421AF8[];

extern f32 evtComputeClampedModelScale(s32);

/* Start a bounded vector transition; detach any previous secondary-world source. */
void evtSetUnitRot1(evtUnitHandle_t *work, const f32 vector[4], s32 frames);

/* Configure a mode-one vector transition, without retaining a world source. */
void evtSetUnitMove1(evtUnitHandle_t *work, s32 motionSubmode, const f32 vector[4], s32 unused, s32 frames, s32 valueB6, EvtUnitMotionCallback callback, s32 unusedLast);

s32 evtStartUnitModeWithValue(evtUnitHandle_t *eventUnit, EvtUnitMotionCallback callback);

extern void evtAdvanceUnitVisualTransitions(evtUnitHandle_t *);

extern EvtUnitMotionStepResult evtUnitMoveType0(evtUnitHandle_t *);

extern EvtUnitMotionStepResult evtUnitMoveTypePathTrace(evtUnitHandle_t *);

extern void evtRefreshUnitEndpointWork(evtUnitHandle_t *);

EvtUnitMotionStepResult evtUpdateUnitMotion(dds3PoseSet_t *unused, dds3Basic_t *object);

void evtResetUnitVectorSlots(void);

void evtSetSlotVectors(s32 slotIndex, EvtUnitVectorSlotState slotState, evtUnitHandle_t *unitHandle,
                       const f32 *firstEndpoint, const f32 *secondEndpoint, const f32 *color);

void evtSetSlotVector(s32 slotIndex, f32 x, f32 y);

/* Copy the second vector of the slot bound to `unitHandle` (else the first state-2 slot). */
EvtUnitVectorSlotLookupResult evtCopyUnitVectorSlotEndpoint(evtUnitHandle_t *unitHandle, f32 *out);

/* Select a matching slot and write its endpoint render work. */
void evtApplyMatchingUnitSlotEndpoints(evtUnitHandle_t *unit);

/* Find auxiliary coordinates for the unit-bound slot, else the shared fallback. */
EvtUnitVectorSlotLookupResult evtFindUnitSlotAuxCoordinates(
    evtUnitHandle_t *unit, f32 *outX, f32 *outY);

ScrCommandResult evtOpModelLodChg(void);

extern char D_00421810[];

ScrCommandResult evtOpDeprecatedModelMotion(void);

ScrCommandResult evtOpSetUnitParams5(void);

ScrCommandResult evtOpSetUnitParams6(void);

void evtLipsExecFunction(s32 unitUniqueId, s32 motionId);

void evtLipsStopFunction(void);

ScrCommandResult evtCmdDestroySelectedWorldUnit(void);

ScrCommandResult evtOpStartUnitTransitionTowardWorldObject(void);

ScrCommandResult evtScriptSetUnitTableValueAndReset(void);

f32 evtGetShortestAngleDelta(f32 a, f32 b);

extern f32 EulerAngleAxisY(void);

ScrCommandResult evtOpTurnUnitRelativeToWorldNode(void);

ScrCommandResult evtOpRestoreUnitReferenceAngle(void);

extern const char D_00421A30[38];

extern const char D_00421A58[30];

extern const char D_00421AD8[30];

f32 evtComputeClampedModelScale(s32 modelRecordIndex);

extern const char D_00421B50[30];

extern const char D_00421BA0[30];
/* End of declarations from src/dds2/event/evtUnitManager.c */

/* Declarations from dds-decomp src/dds2/game/code_00242CB8.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#define SMT3_DEFINES_EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR 1

#define SMT3_DEFINES_EVT_WORLD_SLOT_MOVIE 1

#define SMT3_DEFINES_EVT_WORLD_SLOT_UNIT 1

#define SMT3_DEFINES_EvtTargetInfoFlags 1

#define SMT3_DEFINES_SDF_MOTION_STATE_INITIALIZED 1

#define SMT3_DEFINES_SDF_MOTION_STATE_SAMPLING 1

#define SMT3_DEFINES_SDF_MOTION_STATE_SUSPENDED 1

#define SMT3_DEFINES_SDF_MOTION_STATE_TERMINAL 1

#define SMT3_DEFINES_SDF_MOTION_STATE_UNINITIALIZED 1

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/evt_solar.h"

#include "dds/kwln.h"

#include "dds/mdl_flag_api.h"

u32 dds3GetProcessData(const dds3ProcessID_t *task);

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

void dds3SetProcessData(dds3ProcessID_t *task, u32 userValue);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

void evtBitOff(s32 flag);

extern dds3ProcessID_t *moonprocess_id;

#include "dds/common.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/scr_process_handle_api.h"

#include "dds/dds3_task_add_child.h"

#include "dds/sdf_chip.h"

#include "dds/dat_state.h"

#include "dds/dat_world_field_flags.h"

#include "dds/scr.h"

#include "dds/scr_command_timer_api.h"

#include "dds/scr_string_parameter_api.h"

#include "dds/kwln.h"

#include "dds/dds3_process_registration.h"

#include "dds/dds3_task_create.h"

#include "dds/evt_world.h"

#include "dds/evt_picture.h"

#include "dds/evt_event_pack.h"

#include "dds/evt_event_bgm_readiness.h"

#include "dds/evt_sound_variation.h"

#include "dds/evt_solar.h"

#include "dds/dds3_task_runner.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/mdl_flag_api.h"

extern void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration);

extern f32 gMoonCnt;

extern char D_00437208[];

extern char D_00422050[];

extern s32 fileMenuTaskExists(void);

extern u32 dds3NullDrawProc(void);

#include "dds/sdf_debug_api.h"

extern char D_00422030[];

extern char D_00421FE8[];

u32 evtOpcodeSetBgmVolumePan(void);

u32 evtOpcodeInitializeEffectSoundChannel(void);

void evtClearSolarOverlayControl(void);

/* Reset the field-transition expiry counter at world offset 0xA4C. */
void evtResetFieldTransitionCounter(void);

/* Fold phases 9-15 back toward zero for the symmetric solar animation. */
EvtMirroredSolarPhase evtGetMirroredSolarPhase(void);

void evtSetSolarOverlayFullyVisible(void);

void evtDisableMoonDraw(void);

void evtEnableMoonDraw2(void);

void evtDisableMoonDraw2(void);

dds3TaskUpdateResult_t evtUpdateSolarOverlayFade(dds3ProcessID_t *task);

extern s32 D_004371FC;

extern void fldEsutoMaClearMsg(void);

extern void fldRiberaMaClearMsg(void);

extern void fldRifutoMaClearMsg(void);

extern void fldLightMaClearMsg(void);

/* Advance the enabled solar clock and expire its associated field controls. */
void evtAdvanceSolarClockAndExpireControls(f32 travelDistance);

dds3TaskUpdateResult_t evtCreateSolarOverlayWork(dds3ProcessID_t *task);

void evtFreeSolarOverlayWork(dds3ProcessID_t *task);

/* One scheduler task owns the overlay; a second start leaves its state intact. */
void evtEnsureSolarOverlayTaskAndResetPhase(void);

void evtStopSolarOverlay(void);
/* End of declarations from src/dds2/game/code_00242CB8.c */

void *evtSearchBasic(s32 type, s32 serial)
{
    void *scene;

    scene = dds3GetActiveScene();
    return dds3SearchSceneBasic(scene, serial, type);
}

u32 evtCommand_MODEL_PLAYER(void)
{
    void *scene;
    s32 *basic;
    s32 key;

    scene = dds3GetActiveScene();
    basic = dds3GetActiveUnitSceneBasic(scene);
    key = -1;
    if (basic != NULL) {
        key = basic[1];
    }
    scrSetIntReturnValue(key);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8000);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8040);

INCLUDE_ASM(const s32, "event/evtCommand", func_001BDDE0);

INCLUDE_ASM(const s32, "event/evtCommand", func_001BDE80);

u32 evtCommand_MODEL_BE(void) {
    s32 eventId;
    s32 resourceId;
    s32 modelKey;
    dds3ProcessID_t *motionSeTask;

    if (scrGetScriptHandle() == 0) {
        return 1;
    }
    eventId = scrGetIntPara(0);
    resourceId = scrGetIntPara(1);
    modelKey = evtInitMotionSeProcess(eventId, resourceId);
    if (modelKey < 0) {
        datDebugPrintf("MODEL_BE not fount RID = %d!\n", scrGetIntPara(1));
        return 1;
    }
    eventId = scrGetIntPara(0);
    resourceId = scrGetIntPara(1);
    motionSeTask = evtLoadModelSe(modelKey, eventId, resourceId);
    if (motionSeTask != 0) {
        dds3LinkProcess(evtSearchEventPack(scrGetIntPara(0)), motionSeTask);
    }
    /* BUG: the success path ends in this void call with no return. Retail
     * returns whatever v0 holds, which is nowpwork, a non-null pointer left by
     * scrSetIntReturnValue. CodeFunc_Comm reads 0 as "command still pending",
     * so a port that returns garbage can stall the script forever. */
    scrSetIntReturnValue(modelKey);
}

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A82E8);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8490);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8500);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8570);

u32 evtOpQueueWorldObjectPendingValue(void) {
    dds3Basic_t *ctx;
    s32 worldObjectKey;
    dds3Basic_t *unit;
    u32 ret = 1;

    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    evtArmEffectObjectPendingValue(unit, scrGetIntPara(1));
    return 1;
}

u32 evtOpClearWorldObjectPendingValue(void) {
    dds3Basic_t *ctx;
    s32 worldObjectKey;
    dds3Basic_t *unit;
    u32 ret = 1;

    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    evtResetObjectPendingValue(unit);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_r1_0044DE78);

INCLUDE_ASM(const s32, "event/evtCommand", func_001BE420);

u32 evtCommand_MODEL_POS_XYZ(void)
{
    sdfVector4_t pos;
    void *scene;
    s32 key;
    void *unit;

    memset(&pos, 0, 0x10);
    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return 1;
    }
    pos.x = scrGetFloatPara(1);
    pos.y = scrGetFloatPara(2);
    pos.z = scrGetFloatPara(3);
    dds3SetPosPoseSet(unit, (u128 *)&pos);
    return 1;
}

u32 evtCommand_MODEL_ROT_XYZ(void) {
    sdfVector4_t v;
    void *ctx;
    s32 worldObjectKey;
    void *unit;
    f32 toRad;
    f32 x;
    f32 y;

    memset(&v, 0, 0x10);
    v.w = 1.0f;
    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return 1;
    }
    toRad = 0.017453293f;
    x = scrGetFloatPara(1) * toRad;
    y = scrGetFloatPara(2) * toRad;
    FromEuler(x, y, 0.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    FromEuler(0.0f, 0.0f, scrGetFloatPara(3) * toRad);
    Mult();
    SDF_VU0_STOREVECTOR(&v);
    dds3SetRotPoseSet(unit, (u128 *)&v);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_001BE658);

u32 evtCommand_MODEL_SCL_XYZ(void)
{
    sdfVector4_t scl;
    void *scene;
    s32 key;
    void *unit;

    memset(&scl, 0, 0x10);
    scl.w = 1.0f;
    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return 1;
    }
    scl.x = scrGetFloatPara(1);
    scl.y = scrGetFloatPara(2);
    scl.z = scrGetFloatPara(3);
    dds3SetSclPoseSet(unit, (u128 *)&scl);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_001BE790);

INCLUDE_ASM(const s32, "event/evtCommand", func_001BE818);

INCLUDE_ASM(const s32, "event/evtCommand", func_001BEB18);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8B40);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8C50);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8C68);

INCLUDE_ASM(const s32, "event/evtCommand", func_001BED70);

u32 evtCommand_KUTISET(void) {
    evtUnit_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 firstSlot = scrGetIntPara(1);
        s32 secondSlot = scrGetIntPara(2);
        evtSetUnitKutiMotion(unit, firstSlot, secondSlot);
        evtKutiUnit = EE_ADDR(unit);
    }
    {
        s32 window = scrGetMessageHandle();
        if (window < 0) {
            return 1;
        }
        itfMesMngSetKutiCallback(window, evtRunKutiMotion);
    }
    return 1;
}

u32 evtCommand_KUTI(void) {
    evtUnit_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 firstSlot = scrGetIntPara(1);
        s32 secondSlot = scrGetIntPara(2);
        evtSetUnitKutiMotion(unit, firstSlot, secondSlot);
        evtRunUnitKutiMotion(EE_ADDR(unit));
        evtKutiUnit = EE_ADDR(unit);
    }
    {
        s32 window = scrGetMessageHandle();
        if (window < 0) {
            return 1;
        }
        itfMesMngSetKutiCallback(window, evtRunKutiMotion);
    }
    return 1;
}

u32 evtCommand_MODEL_WAIT(void)
{
    s32 key;
    void *unit;
    u32 ret = 1;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    if (unit == NULL) {
        return ret;
    }
    return evtCheckUnitWait(unit) != 0;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_EXWAIT);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9040);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9098);

u32 evtCommand_MODEL_MOVE2(void) {
    evtUnit_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    {
        s32 pathObjectKey = scrGetIntPara(1);
        s32 frames = scrGetIntPara(5);
        s32 directionOffset = scrGetIntPara(6);
        s32 pathFollowMode = scrGetIntPara(2);
        s32 dirFlag = scrGetIntPara(4);
        s32 sideMode = scrGetIntPara(3);
        evtSetUnitMove3(unit, pathObjectKey, frames, directionOffset, pathFollowMode, dirFlag, sideMode);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9230);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9388);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9548);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A95A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9640);

/* Store the two script-supplied world-unit state values. */
u32 evtCommand_MODEL_WAIT_HOKAN(void) {
    s32 worldObjectKey;
    evtUnit_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->waitBlendStart = scrGetIntPara(1);
    unit->waitBlendEnd = scrGetIntPara(2);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_MOVE_MOT);

u32 evtCommand_MODEL_ROT(void) {
    s32 unitObjectKey;
    evtUnit_t *unit;
    s32 worldObjectKey;
    s32 frames;

    unitObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(unitObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    worldObjectKey = scrGetIntPara(1);
    frames = scrGetIntPara(2);
    evtSetUnitRot2(unit, worldObjectKey, frames);
    return 1;
}

f32 evtGetShortestAngleDelta(f32 a, f32 b) {
    f32 diff;

    if (a < 0.0f || b < 0.0f) {
        a += 360.0f;
        b += 360.0f;
    }
    a = (s32)a % 360;
    b = (s32)b % 360;
    diff = a - b;
    if (diff > 180.0f || diff < -180.0f) {
        if (a < b) {
            a += 360.0f;
        } else {
            b += 360.0f;
        }
    }
    return b - a;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A98C0);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9B30);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_ON);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_OFF);

u32 evtCommand_MODEL_LIGHT_ON(void) {
    s32 worldObjectKey;
    evtUnit_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    evtUnitLightOn(unit);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_OFF(void)
{
    s32 key;
    void *unit;
    s32 time;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    time = scrGetIntPara(1);
    evtUnitLightOff(unit, time);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_WAIT(void)
{
    s32 key;
    void *unit;
    s32 stat;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    stat = evtUnitLightAnimeStat(unit);
    return stat == 0;
}

u32 evtCommand_MODEL_LIGHT_RGB(void)
{
    evtColorWork_t work[2];
    evtUnit_t *unit;
    u32 light;
    u32 ambient;
    u32 scale;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    SDF_VU0_SETVECTOR_X(scrGetFloatPara(2));
    SDF_VU0_SETVECTOR_Y(scrGetFloatPara(3));
    SDF_VU0_SETVECTOR_Z(scrGetFloatPara(4));
    SDF_VU0_VECTOR_W_SET_ZERO();
    scale = EVT_RGBA_SCALE_128;
    MMI_RGBA_PACK_UNIT(light, scale);
    work[0].word = light;
    SDF_VU0_SETVECTOR_X(scrGetFloatPara(5));
    SDF_VU0_SETVECTOR_Y(scrGetFloatPara(6));
    SDF_VU0_SETVECTOR_Z(scrGetFloatPara(7));
    SDF_VU0_VECTOR_W_SET_ONE();
    MMI_RGBA_PACK_UNIT(ambient, scale);
    work[1].word = ambient;
    evtSetUnitLightAndAmbient(unit, scrGetIntPara(1), light, ambient);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_DIR(void)
{
    sdfVector4_t dir;
    evtUnit_t *unit;
    f32 mag;

    memset(&dir, 0, sizeof(dir));
    unit = evtGetUnitHandle(scrGetIntPara(0));
    dir.x = scrGetFloatPara(2);
    dir.y = scrGetFloatPara(3);
    dir.z = scrGetFloatPara(4);
    mag = sdfFabs(dir.x);
    if (mag <= 0.01f) {
        mag = sdfFabs(dir.y);
        if (mag <= 0.01f) {
            mag = sdfFabs(dir.z);
            if (mag <= 0.01f) {
                datDebugPrintf(evtMsgLightDirZero);
                return 1;
            }
        }
    }
    SDF_VU0_LOADVECTOR(&dir);
    evtSetUnitLightDir(unit, scrGetIntPara(1));
    return 1;
}

u32 evtCommand_MODEL_RGB(void)
{
    evtColorWork_t work[1];
    evtUnit_t *unit;
    u32 color;
    s32 duration;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    SDF_VU0_SETVECTOR_X(scrGetFloatPara(2));
    SDF_VU0_SETVECTOR_Y(scrGetFloatPara(3));
    SDF_VU0_SETVECTOR_Z(scrGetFloatPara(4));
    SDF_VU0_VECTOR_W_SET_ZERO();
    MMI_RGBA_PACK_128_S2(color);
    work[0].word = color;
    duration = scrGetIntPara(1);
    /* Qualified cc1 loop notes 106/126 keep the unit argument before the
     * duration argument; the no-block control reverses those two copies.
     * See docs/levers/control-flow.md. */
    do {
        evtSetUnitColorRGB(unit, duration, color);
    } while (0);
    return 1;
}

u32 evtCommand_MODEL_A(void)
{
    evtColorWork_t work[1];
    evtUnit_t *unit;
    u32 color;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    SDF_VU0_LOADIDENTITYVECTOR();
    SDF_VU0_SETVECTOR_W(scrGetFloatPara(2));
    MMI_RGBA_PACK_128_S2(color);
    work[0].word = color;
    evtSetUnitColorA(unit, scrGetIntPara(1), color);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_LINK(void)
{
    s32 key;
    void *unit;
    void *target;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    target = evtSearchBasic(9, scrGetIntPara(2));
    if (target == NULL) {
        return 1;
    }
    evtLinkLightObjectBasic(unit, target, scrGetIntPara(1));
    return 1;
}

u32 evtCommand_MODEL_LIGHT_UNLINK(void)
{
    s32 key;
    void *unit;
    s32 time;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    time = scrGetIntPara(1);
    evtUnlinkLightObjectBasic(unit, time);
    return 1;
}

const char evtMsgLightDirZero[40] = "WARNING: MODEL_LIGHT_DIR length zero\n";

INCLUDE_ASM(const s32, "event/evtCommand", func_001C0208);

INCLUDE_ASM(const s32, "event/evtCommand", func_001C02E0);

INCLUDE_ASM(const s32, "event/evtCommand", func_001C0370);

u32 evtCommand_EFFBED_BE(void) {
    sdfVector4_t position;
    sdfVector4_t scale;
    s32 taskId;
    s32 key;
    void *resource;
    dds3Basic_t *object;

    memset(&position, 0, sizeof(position));
    memset(&scale, 0, sizeof(scale));
    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    resource = evtGetResorceData(taskId, key);
    if (resource == NULL) {
        key = scrGetIntPara(1);
        datDebugPrintf("EFFBED_BE not fount RID = %d\n", key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = (dds3Basic_t *)dds3AddBEDEffectObjectBasic2(resource, &position, &scale);
    if (object == NULL) {
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->serialID);
    }
    return 1;
}

u32 evtCommand_EFFBED_BE_EVO(void) {
    return 1;
}

u32 evtCommand_EFFBED_START(void)
{
    s32 key;
    dds3Basic_t *basic;
    u32 entry;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        entry = scrGetIntPara(1);
        if (entry >= 3U) {
            datDebugPrintf("error EFFBED_START cut no over = %d\n", entry);
            return 1;
        } else {
            dds3StartBEDEffectObjectBasic(basic, entry);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AA5C8);

u32 evtCommand_EFFBED_START_EVO(void) {
    void *unit;

    unit = evtSearchBasic(7, scrGetIntPara(0));
    if (unit != NULL) {
        s32 entryId = scrGetIntPara(1);
        s32 modelRecordIndex;
        if ((u32)entryId >= 3) {
            datDebugPrintf("error EFFBED_START cut no over = %d\n", entryId);
            return 1;
        }
        dds3StartBEDEffectObjectBasic(unit, entryId);
        modelRecordIndex = scrGetIntPara(2);
        if (modelRecordIndex >= 0) {
            f32 value = evtCalcEffBedScale(modelRecordIndex);
            void *target = *(void **)(*(u8 **)((u8 *)unit + 0x18) + 0x2C);
            if (target != NULL) {
                effEveBEDCutSetScale(target, value);
            }
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_001C06A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_001C07A8);

u32 evtCommand_EFFMG1_BE(void) {
    s32 taskId;
    s32 key;
    void *resource;
    dds3Basic_t *object;

    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    resource = evtGetResorceData(taskId, key);
    if (resource == NULL) {
        key = scrGetIntPara(1);
        datDebugPrintf("EFFMG1_BE not fount RID = %d\n", key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = dds3AddMG12EffectObjectBasic2(1, (struct effPCPAgiraoFirePartList_t *)resource);
    if (object == NULL) {
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->serialID);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_001C08B8);

u32 evtCommand_EFFMG2_BE(void) {
    s32 taskId;
    s32 key;
    void *resource;
    dds3Basic_t *object;

    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    resource = evtGetResorceData(taskId, key);
    if (resource == NULL) {
        key = scrGetIntPara(1);
        datDebugPrintf("EFFMG2_BE not fount RID = %d\n", key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = dds3AddMG12EffectObjectBasic2(2, (struct effPCPAgiraoFirePartList_t *)resource);
    if (object == NULL) {
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->serialID);
    }
    return 1;
}

u32 evtCommand_EFFMG2_PRESET(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3EffectObjectPresetMG12(basic);
    }
    return 1;
}

u32 evtCommand_EFFECT_PRESET(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3EffectObjectPresetMG12(basic);
    }
    return 1;
}

u32 evtCommand_EFFECT_CLS(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3DeleteSceneBasicData(basic);
    }
    return 1;
}

u32 evtCommand_EFFECT_POS_XYZ(void) {
    dds3Basic_t *obj;
    evtModelHeader_t *header;

    obj = evtSearchBasic(7, scrGetIntPara(0));
    header = (evtModelHeader_t *)obj->workdata;
    if (header->flags & 4) {
        header->positionX = scrGetFloatPara(1);
        header->positionY = scrGetFloatPara(2);
        header->positionZ = scrGetFloatPara(3);
    } else {
        obj->poseset->pos.x = scrGetFloatPara(1);
        obj->poseset->pos.y = scrGetFloatPara(2);
        obj->poseset->pos.z = scrGetFloatPara(3);
        obj->poseset->flag = (obj->poseset->flag | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) & ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    }
    return 1;
}

u32 evtCommand_EFFECT_ROT_XYZ(void) {
    sdfVector4_t v;
    dds3Basic_t *obj;
    f32 toRad;
    f32 x;
    f32 y;

    memset(&v, 0, 0x10);
    v.w = 1.0f;
    obj = evtSearchBasic(7, scrGetIntPara(0));
    if (!(((evtModelHeader_t *)obj->workdata)->flags & 4)) {
        toRad = 0.017453293f;
        x = scrGetFloatPara(1) * toRad;
        y = scrGetFloatPara(2) * toRad;
        FromEuler(x, y, 0.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        FromEuler(0.0f, 0.0f, scrGetFloatPara(3) * toRad);
        Mult();
        SDF_VU0_STOREVECTOR(&v);
        dds3SetRotPoseSet(obj, (u128 *)&v);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFECT_POS);

s32 evtCommand_EFFMG1_POS(void)
{
    dds3Basic_t *effect;
    dds3Basic_t *point0;
    dds3Basic_t *point1;

    effect = evtSearchBasic(7, scrGetIntPara(0));
    if (effect == NULL) {
        datDebugPrintf("EFFMG1_POS mg1 ID error!\n");
        return 1;
    }
    point0 = evtSearchBasic(7, scrGetIntPara(1));
    point1 = evtSearchBasic(7, scrGetIntPara(2));
    if (point0 == NULL || point1 == NULL) {
        datDebugPrintf("EFFMG1_POS point ID not found!\n");
        return 1;
    }
    if (dds3EffectObjectSetMG12Pos(effect, point0, point1, NULL, NULL) == 0) {
        datDebugPrintf("EFFMG1_POS set error!\n");
        return 1;
    }
    return 1;
}

s32 evtCommand_EFFMG2_POS(void)
{
    dds3Basic_t *effect;
    dds3Basic_t *point0;
    dds3Basic_t *point1;
    dds3Basic_t *point2;
    dds3Basic_t *point3;

    effect = evtSearchBasic(7, scrGetIntPara(0));
    if (effect == NULL) {
        datDebugPrintf("EFFMG2_POS mg2 ID error!\n");
        return 1;
    }
    point0 = evtSearchBasic(7, scrGetIntPara(1));
    point1 = evtSearchBasic(7, scrGetIntPara(2));
    point2 = evtSearchBasic(7, scrGetIntPara(3));
    point3 = evtSearchBasic(7, scrGetIntPara(4));
    if (point0 == NULL || point1 == NULL || point2 == NULL || point3 == NULL) {
        datDebugPrintf("EFFMG2_POS point ID not found!\n");
        return 1;
    }
    if (dds3EffectObjectSetMG12Pos(effect, point0, point1, point2, point3) == 0) {
        datDebugPrintf("EFFMG2_POS set error!\n");
        return 1;
    }
    return 1;
}

s32 evtCommand_EFFECT_ON(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        evtEffectLoopControl(basic, 1);
        dds3EffectObjectBasicStatOn(basic, 1);
    }
    return 1;
}

s32 evtCommand_EFFECT_LINK(void)
{
    dds3Basic_t *effect = evtSearchBasic(7, scrGetIntPara(0));
    dds3Basic_t *owner;
    s32 type;

    if (effect == NULL) {
        return 1;
    }
    type = 4;
    do {
        owner = evtSearchBasic(type, scrGetIntPara(1));
        type++;
    } while (type < 10 && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    dds3EffectObjectBasicLinkObject(effect, owner);
    return 1;
}

s32 evtCommand_EFFECT_LINK2(void)
{
    dds3Basic_t *effect = evtSearchBasic(7, scrGetIntPara(0));
    dds3Basic_t *owner;
    s32 type;

    if (effect == NULL) {
        return 1;
    }
    type = 4;
    do {
        owner = evtSearchBasic(type, scrGetIntPara(1));
        type++;
    } while (type < 10 && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    dds3EffectObjectBasicLinkObjectIndex(effect, owner, scrGetIntPara(2));
    return 1;
}

u32 evtCommand_EFFECT_UNLINK(void)
{
    dds3Basic_t *basic;

    basic = evtSearchBasic(7, scrGetIntPara(0));
    if (basic == NULL) {
        return 1;
    }
    dds3EffectObjectBasicUnlinkObject(basic);
    return 1;
}

s32 evtCommand_EFFECT_OFF(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        evtEffectLoopControl(basic, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFECT_PATH_MOVE);

s32 evtCommand_EFFECT_PATH_WAIT(void)
{
    s32 key = scrGetIntPara(0);
    dds3Basic_t *basic = evtSearchBasic(7, key);
    s32 stat;

    if (basic == NULL) {
        datDebugPrintf("EFFECT_PATH_WAIT not found eff!\n");
        return 1;
    }
    stat = evtCheckPathStat(basic);
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "event/evtCommand", func_001C1200);

s32 evtCommand_RTRN_TITLE(void)
{
    dds3AdminiCallSequence(2, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_STAFF);

s32 evtCommand_CALL_ENDSAVE(void)
{
    dds3AdminiCallSequence(5, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AB408);

INCLUDE_ASM(const s32, "event/evtCommand", func_001C1348);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AB4C0);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AB500);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AB530);

s32 evtCommand_CALL_BATTLE(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    CallBattleSub(eventId, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_001C1588);

s32 evtCommand_CALL_BATTLE2(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    CallBattleSub2(eventId);
    return 1;
}

s32 evtCommand_RESTORE_BATTLE(void)
{
    s32 eventId;
    s32 requestMode;

    eventId = scrGetIntPara(0);
    requestMode = scrGetIntPara(1);
    CallBattleSub(eventId, requestMode);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AB7B0);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AB810);

INCLUDE_ASM(const s32, "event/evtCommand", func_001C17A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AB880);

s32 evtCommand_FIELD_BE(void)
{
    s32 event;
    s32 resource;

    sdfPrintf("FIELD_BE start\n");
    event = scrGetIntPara(0);
    resource = scrGetIntPara(1);
    evtRegistBEField(event, resource);
    sdfPrintf("FIELD_BE end\n");
    return 1;
}

s32 evtCommand_FIELD_DSP(void)
{
    s32 area;
    s32 room;
    s32 mapno;

    area = scrGetIntPara(0);
    room = scrGetIntPara(1);
    mapno = (area << 16) + room;
    if (dds3GetMapNo(dds3GetControlScene()) != mapno) {
        area = scrGetIntPara(0);
        room = scrGetIntPara(1);
        evtLoadArea(area, room);
    }
    return 1;
}

s32 evtCommand_FIELD_ANIME_START(void)
{
    s32 value;

    value = scrGetIntPara(0);
    evtStageAnimeControl(0, value);
    return 1;
}

s32 evtCommand_FIELD_ANIME_STOP(void)
{
    s32 value;

    value = scrGetIntPara(0);
    evtStageAnimeControl(1, value);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_001C1948);

INCLUDE_ASM(const s32, "event/evtCommand", func_001C19A8);

INCLUDE_ASM(const s32, "event/evtCommand", func_001C1A08);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_UNIT_CLS);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001ABB48);

u32 evtCommand_CAM_SEL_FOVY(void)
{
    dds3Basic_t *camera;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        camera = evtSearchBasic(4, scrGetIntPara(0));
        *(u32 *)((u8 *)camera->workdata + 0x88) |= 1;
    }
    if (camera == NULL) {
        return 1;
    }
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CAM_PATH_MOVE);

s32 evtCommand_CAM_PATH_WAIT(void)
{
    dds3Basic_t *camera;
    s32 stat;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        camera = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (camera == NULL) {
        return 1;
    }
    stat = evtCheckPathStat(camera);
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

s32 evtCommand_CAM_CREATE(void)
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    s32 id;

    memset(&pos, 0, 0x10);
    memset(&rot, 0, 0x10);
    rot.w = 1.0f;
    id = dds3AllocObjectId();
    dds3GetRadiusPoseSet(dds3AddCameraObjectBasicTypeFree(id, &pos.x, &rot.x), 1.0f);
    scrSetIntReturnValue(id);
    return 1;
}

s32 evtCommand_CAM_CLS(void)
{
    s32 key;
    dds3Basic_t *camera;

    key = scrGetIntPara(0);
    camera = evtSearchBasic(4, key);
    if (camera == NULL) {
        return 1;
    }
    dds3DeleteSceneBasicData(camera);
    return 1;
}

s32 evtCommand_CAM_POS_XYZ(void) {
    void *effectUnit;
    sdfVector4_t position;

    memset(&position, 0, sizeof(position));
    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetUnitFollowCamera();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    position.x = scrGetFloatPara(1);
    position.y = scrGetFloatPara(2);
    position.z = scrGetFloatPara(3);
    dds3SetPosPoseSet(effectUnit, (u128 *)&position);
    return 1;
}

/* Convert the three degree inputs before composing and storing a quaternion. */
s32 evtCommand_CAM_ROT_XYZ(void) {
    void *effectUnit;
    sdfVector4_t quaternion;
    f32 degreesToRadians;
    f32 pitch;

    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetUnitFollowCamera();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    degreesToRadians = 0.017453293f;
    pitch = scrGetFloatPara(1) * degreesToRadians;
    FromEuler(pitch, scrGetFloatPara(2) * degreesToRadians, 0.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    FromEuler(0.0f, 0.0f, scrGetFloatPara(3) * degreesToRadians);
    Mult();
    SDF_VU0_STOREVECTOR(&quaternion);
    dds3SetRotPoseSet(effectUnit, (u128 *)&quaternion);
    return 1;
}

s32 evtCommand_CAM_ROT_Q(void) {
    void *effectUnit;
    sdfVector4_t quaternion;

    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetUnitFollowCamera();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    quaternion.x = scrGetFloatPara(1);
    quaternion.y = scrGetFloatPara(2);
    quaternion.z = scrGetFloatPara(3);
    quaternion.w = scrGetFloatPara(4);
    dds3SetRotPoseSet(effectUnit, (u128 *)&quaternion);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_001C1F80);

INCLUDE_ASM(const s32, "event/evtCommand", func_001C20B8);

INCLUDE_ASM(const s32, "event/evtCommand", func_001C2218);

s32 evtCommand_MOON_START(void)
{
    evtEnableMoonCalc();
    return 1;
}

s32 evtCommand_MOON_STOP(void)
{
    evtDisableMoonCalc();
    return 1;
}

s32 evtCommand_GET_MOON(void)
{
    scrSetIntReturnValue(evtGetAgeOfMoon());
    return 1;
}

s32 evtCommand_GET_MOON16(void)
{
    scrSetIntReturnValue(evtGetAgeOfMoon16());
    return 1;
}

s32 evtCommand_CHG_MOON(void)
{
    evtSetAgeOfMoon(scrGetIntPara(0));
    return 1;
}

/* Create a missing camp task, then wait for its ready value.
 * No VM context completes immediately; a pending task returns zero. */
s32 evtCommand_LOAD_BE(void) {
    s32 taskId = scrGetIntPara(0);
    scrProcessWork_s *commandWork = scrGetScriptHandle();
    char *message;

    if (commandWork == NULL) {
        return 1;
    }
    if (evtSearchEventPack(taskId) == 0) {
        message = evtMsgLoadBeStart;
        datDebugPrintf(message, taskId);
        sdfPrintf(message, taskId);
        dds3LinkProcess(commandWork->ProcessID, evtLoadEventPack(taskId));
        return 0;
    }
    if (evtGetEventPackStat(taskId) == EVT_CAMP_TASK_READY_VALUE) {
        message = evtMsgBeOk;
        datDebugPrintf(message, taskId);
        sdfPrintf(message, taskId);
        return 1;
    }
    return 0;
}

/* Start the camp task only if no task with this ID exists yet.
 * A missing VM context completes without creating a task. */
s32 evtCommand_READ_BE(void) {
    s32 taskId = scrGetIntPara(0);
    scrProcessWork_s *commandWork = scrGetScriptHandle();

    if (commandWork == NULL) {
        return 1;
    }
    if (evtSearchEventPack(taskId) != 0) {
        return 1;
    }
    datDebugPrintf(evtMsgReadBeStart, taskId);
    dds3LinkProcess(commandWork->ProcessID, evtLoadEventPack(taskId));
    return 1;
}

u32 evtCommand_CHK_BE(void)
{
    s32 no;
    s32 ret;

    no = scrGetIntPara(0);
    if (scrGetScriptHandle() != NULL && evtSearchEventPack(no) != 0
            && evtGetEventPackStat(no) == 2) {
        datDebugPrintf(evtMsgBeOk, no);
        ret = 1;
    } else {
        ret = 0;
    }
    scrSetIntReturnValue(ret);
    return 1;
}

u32 evtCommand_DEL_BE(void)
{
    s32 no;

    no = scrGetIntPara(0);
    if (scrGetScriptHandle() == NULL) {
        return 1;
    }
    if (evtSearchEventPack(no) == 0) {
        return 1;
    }
    datDebugPrintf(evtMsgDelBeStart, no);
    evtKillEventPack(no);
    return 1;
}

const char evtMsgLoadBeStart[16] = "load BE (%d)..\n";

const char evtMsgBeOk[16] = "BE ok! (%d)\n";

const char evtMsgReadBeStart[16] = "read BE (%d)..\n";

const char evtMsgDelBeStart[16] = "del BE (%d)..\n";

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AC738);

/* Clear bit 0 and echo the movie ID through the VM result. */
s32 evtCommandClearPolygonMovieFlag(void)
{
    dds3ProcessID_t *movieTask;

    movieTask = scrVmIntegerToProcessHandle(scrGetIntPara(0));
    evtPolygonMovieClearFlagBits(movieTask, 1);
    scrSetIntReturnValue(scrProcessHandleToVmInteger(movieTask));
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AC858);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AC910);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AC9A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AC9F0);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001ACA50);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001ACAB8);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001ACB38);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001ACBB8);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001ACC50);

s32 evtCommand_PATH_LOOP_ON(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathLoop(obj, 1);
    return 1;
}

s32 evtCommand_PATH_LOOP_OFF(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathLoop(obj, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_LIGHT_PATH_MOVE);

s32 evtCommand_LIGHT_PATH_WAIT(void)
{
    s32 stat;

    stat = evtCheckPathStat(evtSearchBasic(9, scrGetIntPara(0)));
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_BE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_LOAD);

u32 evtCommand_TEX_ON(void)
{
    void *id;

    id = EE_ADDR_AS(void *, scrGetIntPara(0));
    if (dds3ExistProcess(id) != 0) {
        evtPictureDrawOn(id);
    }
    return 1;
}

u32 evtCommand_TEX_OFF(void)
{
    void *id;

    id = EE_ADDR_AS(void *, scrGetIntPara(0));
    if (dds3ExistProcess(id) != 0) {
        evtPictureDrawOff(id);
    }
    return 1;
}

u32 evtCommand_TEX_CLS(void)
{
    void *id;

    id = EE_ADDR_AS(void *, scrGetIntPara(0));
    if (dds3ExistProcess(id) != 0) {
        evtKillPictureProcess(id);
    }
    return 1;
}

u32 evtCommand_SET_SKY(void)
{
    s32 r;
    s32 g;

    fldSetFilterAnime(0);
    fldSkySetAlpha(0x80);
    r = scrGetIntPara(0);
    g = scrGetIntPara(1);
    fldSkySetFilter(r, g, scrGetIntPara(2));
    return 1;
}

u32 evtCommand_SET_SKY_A(void)
{
    s32 alpha;

    alpha = scrGetIntPara(1);
    if (alpha < -255) {
        datDebugPrintf(evtMsgSetSkyAlphaLow);
        alpha = -255;
    }
    if (alpha > 255) {
        datDebugPrintf(evtMsgSetSkyAlphaHigh);
        alpha = 255;
    }
    evtSetSkyAlpha(scrGetIntPara(0), alpha);
    return 1;
}

u32 evtCommand_BOKASI_ON(void) {
    evtSetBokasi(1);
    return 1;
}

u32 evtCommand_BOKASI_OFF(void) {
    evtSetBokasi(0);
    return 1;
}

u32 evtCommand_LOAD_SOUND_E(void)
{
    if (scrGetCommandTimer() == 0) {
        evtLoadEventBgm(scrGetIntPara(0));
    } else if (evtCheckEventBgmLoaded(scrGetIntPara(0)) != 0) {
        return 1;
    }
    return 0;
}

u32 evtCommand_BGM_PLAY_E(void) {
    s32 eventId;
    s32 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtPlayEventBgm(eventId, fade);
    return 1;
}

u32 evtCommand_BGM_TRANS_E(void) {
    s32 eventId;
    s32 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtTransEventBgm(eventId, fade);
    return 1;
}

u32 evtCommand_FADEOUT_E(void) {
    s32 eventId;
    s32 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtStopEventBgm(eventId, fade);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AD328);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AD360);

u32 evtOpcodeSetBgmVolumePan(void) {
    s32 eventId;
    EvtSoundVariation variation;

    eventId = scrGetIntPara(0);
    variation = scrGetIntPara(1);
    evtPlaySe(eventId, variation);
    return 1;
}

u32 evtCommandStartBgmWithFade(void) {
    s32 eventId;
    EvtSoundVariation variation;

    eventId = scrGetIntPara(0);
    variation = scrGetIntPara(1);
    evtStartBgmBySoundIdAndFade(eventId, variation);
    return 1;
}

u32 evtOpcodeRefreshTaskData(void) {
    s32 task;
    s32 value;

    task = scrGetIntPara(0);
    value = scrGetIntPara(1);
    evtRefreshTaskEffectTexture(task, value);
    return 1;
}

u32 evtOpcodeInitializeEffectSoundChannel(void) {
    Eve_DistortionBlurSetDefTex();
    return 1;
}

u32 evt_1E92F8(void)
{
    if (scrGetIntPara(0) <= 0) {
        dds3DrawFlag |= 0x2000000;
        return 1;
    }
    if (scrGetCommandTimer() < SDF_FRAMES(scrGetIntPara(0))) {
        dds3DrawFlag |= 0x2000000;
        return 0;
    }
    return 1;
}
const char evtMsgSetSkyAlphaLow[40] = "warning : SET_SKY_A alpha < -255\n";

addr_t evtKutiUnit = 0;

const char evtMsgSetSkyAlphaHigh[0x28] = "warning : SET_SKY_A alpha > 255\n";

INCLUDE_SDATA(const s32, "event/evtCommand", D_r1_0046C5E0);

INCLUDE_SDATA(const s32, "event/evtCommand", D_004DF848);

