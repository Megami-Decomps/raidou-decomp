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
void scrSetIntReturnValue(s32 i);
extern s32 scrGetIntPara(s32 n);
void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
dds3ProcessID_t *evtSearchEventPack(u32 eventno);
dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);
scrProcessWork_s *scrGetScriptHandle(void);
extern void datDebugPrintf(const char *fmt, ...);
extern s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);
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

/* Canonical provider signatures; callers retain their s32 parameter values. */
extern s32 evtPlayEventBgm(s32 eventId, s32 variation);
extern void evtTransEventBgm(s32 eventId, s32 variation);
extern s32 evtStopEventBgm(s32 eventId, s32 variation);
extern s32 evtPlaySe(s32 eventno, s32 seno);
extern s32 evtStartBgmBySoundIdAndFade(s32 eventId, s32 variation);
extern void evtRefreshTaskEffectTexture(s32 taskId, s32 key);
extern s32 scrGetMessageHandle(void);
extern void evtSetUnitKutiMotion(evtUnit_t *h, s32 group, s32 motion);
void dds3DeleteSceneBasicData(dds3Basic_t *basic);
void evtUnitLightOn(evtUnit_t *h);
void evtSetUnitLightDir(evtUnit_t *h, s32 frame);
extern const char evtMsgLightDirZero[];
void *evtGetResorceData(u32 eventno, s32 rid);

#include "sdk_libc.h"

extern void Eve_DistortionBlurSetDefTex(void);
extern void evtSetBokasi(s32 sw);
extern void dds3EffectObjectBasicStatOn(void *basic, s32 s);
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

void CallBattleSub(s32 encno, s32 eventno);
void CallBattleSub2(s32 encno);
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

typedef u8 sdfTextureClampMode_t;

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
extern s32 evtCheckPathStat(dds3Basic_t *obj);
extern s32 evtRegistBEField(s32 eventno, s32 rid);
extern s32 evtLoadArea(s32 fld, s32 area);
extern s32 evtStageAnimeControl(s32 type, s32 mode);
extern void dds3SetSclPoseSet(void *pBasic, u128 *scl);
extern s32 evtCheckUnitWait(evtUnit_t *h);
extern void evtUnitLightOff(evtUnit_t *h, s32 frame);
extern u8 evtUnitLightAnimeStat(evtUnit_t *h);
extern void evtLinkLightObjectBasic(evtUnit_t *h, dds3Basic_t *light, s32 frame);
extern void evtUnlinkLightObjectBasic(evtUnit_t *h, s32 frame);
extern void evtPathLoop(dds3Basic_t *basic, s32 sw);
extern void evtLoadEventBgm(s32 event);
extern s32 evtCheckEventBgmLoaded(s32 event);
extern s32 scrGetCommandTimer(void);
extern void evtSetSkyAlpha(s32 frame, s32 a);
extern s32 dds3ExistProcess(void *id);

/* Rows of 0x310 bytes; only the three size fields the bed scale reads are
 * named here. */
typedef struct {
    u8 unk00[0x10];
    f32 formsize;                   /* 0x10 */
    f32 formheight;                 /* 0x14 */
    f32 formscale;                  /* 0x18 */
    u8 unk1C[0x310 - 0x1C];
} datModelParam_t;

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct {
    u32 word;
    u8 pad04[12];
} evtColorWork_t;

#define EVT_RGBA_SCALE_128 0x43000000

extern void evtPictureDrawOn(void *id);
extern void evtPictureDrawOff(void *id);
extern void evtKillPictureProcess(void *id);
extern const char evtMsgSetSkyAlphaLow[];
extern const char evtMsgSetSkyAlphaHigh[];
extern s32 *dds3GetActiveUnitSceneBasic(void *pSceneBasic);
extern void fldSetFilterAnime(s32 on);
extern void fldSkySetAlpha(s32 alp);
extern void fldSkySetFilter(s32 idx, s32 mode, s32 hokan);

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} evtVec_t;

extern void evtKillEventPack(s32 eventno);
extern const char evtMsgObjPathPara0[];
extern const char evtMsgObjPathPara1[];
extern const char evtMsgDelBeStart[];
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
void evtRunKutiMotion(void);
u32 evtCommand_KUTISET(void);
u32 evtCommand_KUTI(void);
u32 evtCommand_MODEL_WAIT(void);
u32 evtCommand_MODEL_EXWAIT(void);
u32 evtCommand_MODEL_MOVE2(void);
u32 evtCommand_MODEL_WAIT_EXMOT(void);

/* Store the script-supplied value for the selected event unit. */
u32 evtCommand_MODEL_WAIT_MOT(void);
u32 evtCommand_MODEL_WAIT_HOKAN(void);
u32 evtCommand_MODEL_MOVE_MOT(void);
u32 evtCommand_MODEL_ROT(void);
u32 evtCommand_MODEL_ON(void);
u32 evtCommand_MODEL_OFF(void);
u32 evtCommand_MODEL_LIGHT_ON(void);
u32 evtCommand_MODEL_LIGHT_OFF(void);
u32 evtCommand_MODEL_LIGHT_WAIT(void);
u32 evtCommand_MODEL_LIGHT_RGB(void);
u32 evtCommand_MODEL_LIGHT_DIR(void);
u32 evtCommand_MODEL_RGB(void);
u32 evtCommand_MODEL_A(void);
u32 evtCommand_MODEL_LIGHT_LINK(void);
u32 evtCommand_MODEL_LIGHT_UNLINK(void);
u32 evtCommand_EFFECT_BE(void);
u32 evtCommand_EFFECT_LOAD(void);
u32 evtCommand_EFFBED_LOAD(void);
extern const char evtMsgLightDirZero[40];
u32 evtCommand_EFFBED_BE(void);
u32 evtCommand_EFFBED_BE_EVO(void);
u32 evtCommand_EFFBED_START(void);
f32 evtCalcEffBedScale(s32 uid);
u32 evtCommand_EFFBED_START_EVO(void);
u32 evtCommand_EFFMG1_LOAD(void);
u32 evtCommand_EFFMG1_BE(void);
u32 evtCommand_EFFMG2_LOAD(void);
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

/* Forward two VM integers as the admin request's two-word payload. */
s32 evtCommand_CALL_SAVE(void);
s32 evtCommand_CALL_NEXT(void);

/* The event ID becomes a four-byte dispatch payload. */
s32 evtCommand_CALL_EVENT(void);
s32 evtCommand_CALL_BATTLE(void);
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
u32 evtCommand_CAM_SEL_FOVY(void);
s32 evtCommand_CAM_PATH_MOVE(void);
s32 evtCommand_CAM_PATH_WAIT(void);
s32 evtCommand_CAM_CREATE(void);
s32 evtCommand_CAM_CLS(void);
s32 evtCommand_CAM_POS_XYZ(void);
s32 evtCommand_CAM_ROT_XYZ(void);
s32 evtCommand_CAM_ROT_Q(void);
s32 evtCommand_OBJ_PATH_MOVE(void);
s32 evtCommand_OBJ_PATH_WAIT(void);
s32 evtCommand_MOON_START(void);
s32 evtCommand_MOON_STOP(void);
s32 evtCommand_GET_MOON(void);
s32 evtCommand_GET_MOON16(void);
s32 evtCommand_CHG_MOON(void);
s32 evtCommand_LOAD_BE(void);
s32 evtCommand_READ_BE(void);
u32 evtCommand_CHK_BE(void);
u32 evtCommand_DEL_BE(void);

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
extern const char evtMsgLoadBeStart[16];
extern const char evtMsgBeOk[16];
extern const char evtMsgReadBeStart[16];
extern const char evtMsgDelBeStart[16];
u32 evtCommand_TEX_BE(void);
u32 evtCommand_TEX_LOAD(void);
u32 evtCommand_TEX_ON(void);
u32 evtCommand_TEX_OFF(void);
u32 evtCommand_TEX_CLS(void);
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
u32 evtCommandStartBgmWithFade(void);
u32 evtOpcodeRefreshTaskData(void);
u32 evtCommandSetDrawFlagWhileWaiting(void);
extern const char evtMsgSetSkyAlphaLow[40];

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

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

#include "dds/evt_polygon_movie_flags_api.h"
#include "dds/evt_task.h"
#include "dds/scr.h"
#include "dds/scr_process_handle_api.h"

/* Selects both the world-node tag and its per-kind scene slot. */
typedef s32 EffWorldKind;

ScrCommandResult evtCommandSignalAdminAtTimerZero(void);
ScrCommandResult evtCommandSubmitEvent(void);

/* Forward the first VM integer as an opaque four-byte admin payload. */
ScrCommandResult evtCommandSubmitScalarAdminRequest(void);
ScrCommandResult evtCommandAddEffectUnitToWorld(void);
extern char D_00421E58[], D_00421E68[], D_00421E78[];
ScrCommandResult evtCommandReattachObjectPath(void);
ScrCommandResult evtCommandStartPolygonMovie(void);

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "dds/btl.h"

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

enum SdfMotionStateValue {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
};

extern void *memcpy(void *, const void *, unsigned int);

struct sdfGrOt_t;

struct EffNodeDescriptor;

#include "dds/ee_mmi.h"

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

extern f32 sdfFaceDivSoften, sdfFaceDivBias;

#define EVT_UNIT_FLAG_MOTION_START_PENDING 0x80

#define EVT_UNIT_FLAG_MOTION_WOBBLE 0x40

#define EVT_UNIT_FLAG_USE_PATH_ROTATION 0x10

#include "dds/fpu.h"

extern void evtArmEffectObjectPendingValue(dds3Basic_t *object, s32 value);
extern void evtResetObjectPendingValue(dds3Basic_t *object);

struct SoundMixer;

ScrCommandResult evtOpModelLodChg(void);
ScrCommandResult evtOpDeprecatedModelMotion(void);
ScrCommandResult evtOpSetUnitParams5(void);
ScrCommandResult evtOpSetUnitParams6(void);
ScrCommandResult evtCmdDestroySelectedWorldUnit(void);
ScrCommandResult evtOpStartUnitTransitionTowardWorldObject(void);
ScrCommandResult evtScriptSetUnitTableValueAndReset(void);
f32 evtGetShortestAngleDelta(f32 a, f32 b);
ScrCommandResult evtOpTurnUnitRelativeToWorldNode(void);
ScrCommandResult evtOpRestoreUnitReferenceAngle(void);

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "dds/evt_sound_variation.h"

u32 evtOpcodeSetBgmVolumePan(void);
u32 evtOpcodeInitializeEffectSoundChannel(void);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara4);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_DSP);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallBtlCountClear);

INCLUDE_ASM(const s32, "event/evtCommand", evtMODELDSPBUSTUP);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtFldProcSequence);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_SHADOW_OFF);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_SHADOW_ON2);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetActiveScene4);

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

INCLUDE_RODATA(const s32, "event/evtCommand", evtStrFldProcSequence);

INCLUDE_ASM(const s32, "event/evtCommand", evtWarningMODELLOD);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetActiveScene2);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_WAIT_EXMOT);

INCLUDE_ASM(const s32, "event/evtCommand", evtMODELMOTION);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara);

INCLUDE_ASM(const s32, "event/evtCommand", evtMODELEXMOTION);

INCLUDE_ASM(const s32, "event/evtCommand", KutiFunction);

INCLUDE_ASM(const s32, "event/evtCommand", evtLipsExecFunction);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetActiveScene3);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetActiveScene5);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_MOVE);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara6);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallMemset6);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara7);

INCLUDE_ASM(const s32, "event/evtCommand", evtWAITEXMOT);

INCLUDE_ASM(const s32, "event/evtCommand", evtWAITMOT);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara8);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara9);

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

INCLUDE_ASM(const s32, "event/evtCommand", datCommand_DEVIL_IN);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFECT_LOAD);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFBED_LOAD);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCommandEFFBEDSTARTEVOSub);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCallMemset2);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFMG1_LOAD);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFMG2_LOAD);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_SEARCH_ID);

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

INCLUDE_ASM(const s32, "event/evtCommand", titleCallNameEntry);

INCLUDE_ASM(const s32, "event/evtCommand", titleGetPressAnyButton);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_EVENT);

INCLUDE_ASM(const s32, "event/evtCommand", campDhWaitMessageEnd);

INCLUDE_ASM(const s32, "event/evtCommand", CallBattleSub_evtCommand);

s32 evtCommand_CALL_BATTLE(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    CallBattleSub(eventId, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", CallBattleSub2_evtCommand);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara10);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_FIELD2);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_FIELD3);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallBgTexClear);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_FIELD_CLS);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_FIELD_CLS_STOP);

INCLUDE_ASM(const s32, "event/evtCommand", evtUnitAllClear);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_UNIT_CLS);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CAM_SEL);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_OBJ_PATH_MOVE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara2);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_OBJ_PATH_WAIT);

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

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PMV_LOAD);

/* Clear bit 0 and echo the movie ID through the VM result. */
s32 evtCommandClearPolygonMovieFlag(void)
{
    dds3ProcessID_t *movieTask;

    movieTask = scrVmIntegerToProcessHandle(scrGetIntPara(0));
    evtPolygonMovieClearFlagBits(movieTask, 1);
    scrSetIntReturnValue(scrProcessHandleToVmInteger(movieTask));
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PMV_RUN);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara11);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara12);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara13);

INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara14);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PATH_START);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PATH_STOP);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PATH_RATE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PATH_DIR);

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

INCLUDE_ASM(const s32, "event/evtCommand", fldCommand_PAD_DISABLE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_BGM_VOL_UP);

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

u32 evtCallGetIntPara15(void)
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

INCLUDE_SDATA(const s32, "event/evtCommand", evtLipsStopFunction_evtCommandData);

INCLUDE_SDATA(const s32, "event/evtCommand", evtCallMemset2Data);

