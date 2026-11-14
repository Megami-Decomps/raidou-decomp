#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtCommand.c */
#include "common.h"

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

extern s32 func_001FA158(s32 eventId, s32 variation);

extern void datAddKotowariPoint(s32 type, s32 point);

extern void func_001FA310(s32 taskId, s32 key);

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

u32 evt_1E9168(void);

u32 evt_1E91A8(void);

u32 evt_1E91E8(void);

s32 evt_1E9220(void);

s32 evt_1E9248(void);

s32 evt_1E9270(void);

u32 evt_1E9298(void);

u32 evt_1E92D8(void);

u32 evt_1E92F8(void);

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

INCLUDE_ASM(const s32, "event/evtCommand", func_001A8098);

INCLUDE_ASM(const s32, "event/evtCommand", func_001A8130);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtOpQueueWorldObjectPendingValue);

INCLUDE_ASM(const s32, "event/evtCommand", evtOpClearWorldObjectPendingValue);

INCLUDE_RODATA(const s32, "event/evtCommand", D_r1_0044DE78);

INCLUDE_ASM(const s32, "event/evtCommand", evtOpModelLodChg);

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

u32 evt_1E50C0(void)
{
    void *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    evtSetUnitMotion(unit, scrGetIntPara(1), scrGetIntPara(2), scrGetIntPara(3), 0);
    printf(evtMsgModelMotionDeprecated);
    return 1;
}

const char evtMsgModelMotionDeprecated[64] = "\xa2\xa8\xc3\xed\xb0\xd5\x3a\x4d\x4f\x44\x45\x4c\x5f\x4d\x4f\x54\x49\x4f\x4e\x28\x29\xa4\xb3\xa4\xce\xa5\xb3\xa5\xde\xa5\xf3\xa5\xc9\xa4\xcf\xba\xa3\xb8\xe5\xbb\xc8\xcd\xd1\xb6\xd8\xbb\xdf\xa4\xcb\xa4\xca\xa4\xea\xa4\xde\xa4\xb9\xa1\xa3\x0a";

INCLUDE_ASM(const s32, "event/evtCommand", func_001A8A58);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8B40);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8C50);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A8C68);

INCLUDE_ASM(const s32, "event/evtCommand", func_001A8D38);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_MOVE2);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9230);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9388);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9548);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A95A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001A9640);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_WAIT_HOKAN);

u32 evtCommand_MODEL_MOVE_MOT(void)
{
    s32 key;
    void *unit;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    if (unit == NULL) {
        return 1;
    }
    evtSetUnitMotion(unit, scrGetIntPara(1), 5, 7, 1);
    printf(evtMsgModelMoveMotDeprecated);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_ROT);

INCLUDE_ASM(const s32, "event/evtCommand", evtGetShortestAngleDelta);

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

u32 evtCommand_EFFECT_BE(void) {
    u8 firstVector[16] QWORD_ALIGNED;
    u8 secondVector[16] QWORD_ALIGNED;
    s32 taskId;
    s32 key;
    struct effPCPAgiraoFirePartList_t *descriptor;
    dds3Basic_t *object;

    memset(firstVector, 0, sizeof(firstVector));
    memset(secondVector, 0, sizeof(secondVector));
    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    descriptor = evtGetResorceData(taskId, key);
    if (descriptor == NULL) {
        key = scrGetIntPara(1);
        datDebugPrintf(evtMsgEffectBeNotFound, key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = dds3AddD3PEffectObjectBasic2(descriptor, firstVector, secondVector);
    if (object == NULL) {
        printf(evtMsgEffectBeFailed, 1);
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->serialID);
    }
    return 1;
}

u32 evtCommand_EFFECT_LOAD(void)
{
    evtVec_t pos QWORD_ALIGNED;
    evtVec_t scale QWORD_ALIGNED;
    dds3Basic_t *basic;

    memset(&pos, 0, sizeof(pos));
    memset(&scale, 0, sizeof(scale));
    basic = dds3AddD3PEffectObjectBasic(scrGetStrPara(0), &pos, &scale);
    if (basic == NULL) {
        printf(evtMsgEffectLoadHint);
        printf(evtMsgNameNotFound, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

u32 evtCommand_EFFBED_LOAD(void)
{
    evtVec_t pos QWORD_ALIGNED;
    evtVec_t scale QWORD_ALIGNED;
    dds3Basic_t *basic;

    memset(&pos, 0, sizeof(pos));
    memset(&scale, 0, sizeof(scale));
    scale.w = 1.0f;
    basic = dds3AddBEDEffectObjectBasic(scrGetStrPara(0), &pos, &scale);
    if (basic == NULL) {
        printf(evtMsgEffectLoadHint);
        printf(evtMsgNameNotFound, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

const char evtMsgModelMoveMotDeprecated[64] = "\xa2\xa8\xc3\xed\xb0\xd5\x3a\x4d\x4f\x44\x45\x4c\x5f\x4d\x4f\x56\x45\x5f\x4d\x4f\x54\x28\x29\xa4\xb3\xa4\xce\xa5\xb3\xa5\xde\xa5\xf3\xa5\xc9\xa4\xcf\xba\xa3\xb8\xe5\xbb\xc8\xcd\xd1\xb6\xd8\xbb\xdf\xa4\xcb\xa4\xca\xa4\xea\xa4\xde\xa4\xb9\xa1\xa3\x0a";

const char evtMsgLightDirZero[40] = "WARNING: MODEL_LIGHT_DIR length zero\n";

const char evtMsgEffectBeNotFound[32] = "EFFECT_BE not fount RID = %d\n";

const char evtMsgEffectBeFailed[24] = "\x45\x46\x46\x45\x43\x54\x5f\x42\x45\x28\x29\xc5\xd0\xcf\xbf\xbc\xba\xc7\xd4\x0a";

const char evtMsgEffectLoadHint[48] = "\xa5\xb9\xa5\xaf\xa5\xea\xa5\xd7\xa5\xc8\xa5\xb3\xa5\xde\xa5\xf3\xa5\xc9\x45\x46\x46\x45\x43\x54\x5f\x4c\x4f\x41\x44\x28\x29\xa4\xc7\xc2\xd0\xbe\xdd\xa5\xd5\xa5\xa1\xa5\xa4\xa5\xeb\x0a";

const char evtMsgNameNotFound[24] = "\x25\x73\xa4\xac\xb8\xab\xa4\xc4\xa4\xab\xa4\xea\xa4\xde\xa4\xbb\xa4\xf3\x21\x21\x0a";

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

INCLUDE_ASM(const s32, "event/evtCommand", func_001AA6F8);

u32 evtCommand_EFFMG1_LOAD(void)
{
    dds3Basic_t *basic;

    basic = dds3AddMG12EffectObjectBasic(1, scrGetStrPara(0));
    if (basic == NULL) {
        printf(evtMsgEffmg1LoadHint);
        printf(evtMsgNameNotFound, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0044E298);

const char evtMsgEffmg1LoadHint[48] = "\xa5\xb9\xa5\xaf\xa5\xea\xa5\xd7\xa5\xc8\xa5\xb3\xa5\xde\xa5\xf3\xa5\xc9\x45\x46\x46\x4d\x47\x31\x5f\x4c\x4f\x41\x44\x28\x29\xa4\xc7\xc2\xd0\xbe\xdd\xa5\xd5\xa5\xa1\xa5\xa4\xa5\xeb\x0a";

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

u32 evtCommand_EFFMG2_LOAD(void)
{
    dds3Basic_t *basic;

    basic = dds3AddMG12EffectObjectBasic(2, scrGetStrPara(0));
    if (basic == NULL) {
        printf(evtMsgEffmg2LoadHint);
        printf(evtMsgNameNotFound, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

const char evtMsgEffmg2LoadHint[48] = "\xa5\xb9\xa5\xaf\xa5\xea\xa5\xd7\xa5\xc8\xa5\xb3\xa5\xde\xa5\xf3\xa5\xc9\x45\x46\x46\x4d\x47\x32\x5f\x4c\x4f\x41\x44\x28\x29\xa4\xc7\xc2\xd0\xbe\xdd\xa5\xd5\xa5\xa1\xa5\xa4\xa5\xeb\x0a";

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFMG2_POS);

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

u32 evtCommand_SEARCH_ID(void)
{
    void *scene;
    dds3Basic_t *basic;

    scene = dds3GetControlScene();
    basic = dds3SceneBasicSearchName(scene, scrGetStrPara(0));
    if (basic == NULL) {
        printf(evtMsgIdNotFound, scrGetStrPara(0));
        datDebugPrintf(evtMsgIdNotFoundWarn, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_001AB368);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_STAFF);

s32 evtCommand_CALL_ENDSAVE(void)
{
    dds3AdminiCallSequence(5, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AB408);

const char evtMsgIdNotFound[32] = "ID : id not found!! <%s>\n";

const char evtMsgIdNotFoundWarn[32] = "WARNING: ID not found! <%s>\n";

s32 evtCommand_CALL_NEXT(void)
{
    s32 para0 = scrGetIntPara(0);
    s32 para1 = scrGetIntPara(1);
    s32 para2 = scrGetIntPara(2);
    s32 command;

    printf("CALL_NEXT(%d,%d,%d)\n", para0, para1, para2);
    command = scrGetIntPara(1);
    fldSetNextField(command, scrGetIntPara(2));
    return 1;
}

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

INCLUDE_ASM(const s32, "event/evtCommand", func_001AB6A0);

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

s32 evtCommand_FIELD_CLS(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    dds3DrawFlag |= 0x2000000;
    printf("event stage shutdown !!\n");
    datDebugPrintf("field shutdown.\n");
    return 1;
}

s32 evtCommand_FIELD_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    dds3DrawFlag |= 0x2000000;
    printf("event stage shutdown2 !!\n");
    datDebugPrintf("field shutdown2.\n");
    return 1;
}

s32 evtCommand_UNIT_CLS(void)
{
    evtFreeAllUnit();
    printf("unit all clear !!\n");
    datDebugPrintf("unit all clear.\n");
    return 1;
}

s32 evtCommand_UNIT_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    printf("unit all clear2 !!\n");
    datDebugPrintf("unit all clear2.\n");
    return 1;
}

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

INCLUDE_ASM(const s32, "event/evtCommand", func_001AC060);

const char evtMsgObjPathPara0[16] = "PARA0:%08x\n";

const char evtMsgObjPathPara1[16] = "PARA1:%08x\n";

INCLUDE_RODATA(const s32, "event/evtCommand", D_0044E610);

INCLUDE_ASM(const s32, "event/evtCommand", func_001AC1D8);

s32 evtCommand_OBJ_PATH_WAIT(void)
{
    void *obj;
    s32 stat;

    obj = evtSearchBasic(6, scrGetIntPara(0));
    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            printf(evtMsgObjPathPara0, scrGetIntPara(0));
            printf(evtMsgObjPathPara1, scrGetIntPara(1));
            printf(evtMsgObjPathWaitNoObj);
            return 1;
        }
    }
    stat = evtCheckPathStat(obj);
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

const char evtMsgObjPathWaitNoObj[32] = "e OBJ_PATH_WAIT : obj==NULL\n";

const char evtMsgLoadBeStart[16] = "load BE (%d)..\n";

const char evtMsgBeOk[16] = "BE ok! (%d)\n";

const char evtMsgReadBeStart[16] = "read BE (%d)..\n";

const char evtMsgDelBeStart[16] = "del BE (%d)..\n";

INCLUDE_RODATA(const s32, "event/evtCommand", D_0044E6A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_r1_001AC738);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommandClearPolygonMovieFlag);

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

INCLUDE_RODATA(const s32, "event/evtCommand", D_0044E700);

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

INCLUDE_RODATA(const s32, "event/evtCommand", D_0044E780);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtOpcodeSetBgmVolumePan);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommandStartBgmWithFade);

INCLUDE_ASM(const s32, "event/evtCommand", evtOpcodeRefreshTaskData);

INCLUDE_ASM(const s32, "event/evtCommand", evtOpcodeInitializeEffectSoundChannel);

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
INCLUDE_RODATA(const s32, "event/evtCommand", D_0044E7E8);

const char evtMsgSetSkyAlphaLow[40] = "warning : SET_SKY_A alpha < -255\n";

addr_t evtKutiUnit = 0;

const char evtMsgSetSkyAlphaHigh[0x28] = "warning : SET_SKY_A alpha > 255\n";

INCLUDE_SDATA(const s32, "event/evtCommand", D_r1_0046C5E0);

INCLUDE_SDATA(const s32, "event/evtCommand", D_0046C5E4);

