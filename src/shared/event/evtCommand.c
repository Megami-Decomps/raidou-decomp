#include "common.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "event.h"
#include "kernel.h"
#include "scr.h"
#include "field.h"
#include "sdfFrame.h"
#include "evtCommand.h"
#include "sdfQuat.h"
#include "evtUnitManager.h"
#include "scrTraceCode.h"
#include "dds3EffectObjectBasic.h"
#include "evtStage.h"
#include "evtSound.h"
#include "dds3AdminiProcess.h"
#include "sdfPrint.h"
#include "evtPicture.h"
#include "dds3KernelCore.h"
#include "dds3WorldBasic.h"
#include "itfMesManager.h"
#include "sdfDelayedResrc.h"
#include "evtEventPack.h"
#include "evtSky.h"
#include "dds3CameraObjectBasic.h"
#include "dds3Basic.h"

struct effResourceHeader_t;

scrProcessWork_s *scrGetScriptHandle(void);
extern void datDebugPrintf(const char *fmt, ...);
dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);
extern addr_t evtKutiUnit;

/* Canonical provider signatures; callers retain their s32 parameter values. */
extern s32 evtStartBgmBySoundIdAndFade(s32 eventId, s32 variation);
extern void evtRefreshTaskEffectTexture(s32 taskId, s32 key);
extern s32 scrGetMessageHandle(void);
void dds3DeleteSceneBasicData(dds3Basic_t *basic);
extern const char evtMsgLightDirZero[];

#include "sdk_libc.h"

extern void Eve_DistortionBlurSetDefTex(void);
extern void evtSetBokasi(s32 sw);
extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(struct effResourceHeader_t *effdata, void *pos, void *rot);
extern const char evtMsgEffectBeFailed[];
extern const char evtMsgEffectBeNotFound[];

typedef struct effEventWork_t effEventWork_t;

void effEveBEDCutSetScale(effEventWork_t *hEveBEDCut, f32 scale);
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
extern const char evtMsgBeOk[];
extern const char evtMsgLoadBeStart[];
extern const char evtMsgReadBeStart[];
extern const char evtMsgPmvRunNoProc[];
extern const char evtMsgTexBeNoProc[];

typedef u8 sdfTextureClampMode_t;

#include "basic.h"
#include "dds3obj.h"
#include "ps2asm.h"

extern s32 dds3GetMapNo(void *scene);
extern void dds3SetActiveCameraSceneBasic(void *pSceneBasic, dds3Basic_t *pCameraBasic);
extern s32 dds3AllocObjectId(void);
extern void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r);
extern u32 dds3DrawFlag;
extern void fldSetNextField(s32 command, s32 para);
extern const char evtMsgNameNotFound[];
extern const char evtMsgEffmg1LoadHint[];
extern const char evtMsgEffmg2LoadHint[];
extern const char evtMsgModelMotionDeprecated[];
extern const char evtMsgModelMoveMotDeprecated[];
extern void evtUnlinkLightObjectBasic(evtUnit_t *h, s32 frame);
extern s32 scrGetCommandTimer(void);

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

extern const char evtMsgPmvLoadNoProc[];
extern const char evtMsgSetSkyAlphaLow[];
extern const char evtMsgSetSkyAlphaHigh[];
extern s32 *dds3GetActiveUnitSceneBasic(void *pSceneBasic);
extern void fldSetFilterAnime(s32 on);
extern void fldSkySetAlpha(s32 alp);
extern void fldSkySetFilter(s32 idx, s32 mode, s32 hokan);
extern dds3Basic_t *dds3SceneBasicSearchName(void *scene, const char *name);
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
extern const char evtMsgEffectLoadHint[];
extern const char evtMsgObjPathPara0[];
extern const char evtMsgObjPathPara1[];
extern const char evtMsgObjPathWaitNoObj[];
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
u32 evtCallGetIntPara5(void);
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

/* Forward two VM integers as the admin request's two-word payload. */
s32 evtCommand_CALL_SAVE(void);
extern const char evtMsgIdNotFound[32];
extern const char evtMsgIdNotFoundWarn[32];
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
extern const char evtMsgObjPathMoveNoObj[32];
extern const char evtMsgObjPathWaitNoObj[32];
extern const char evtMsgLoadBeStart[16];
extern const char evtMsgBeOk[16];
extern const char evtMsgReadBeStart[16];
extern const char evtMsgDelBeStart[16];
extern const char evtMsgPmvLoadNoProc[72];
extern const char evtMsgPmvRunNoProc[72];
extern const char evtMsgTexBeNoProc[72];
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

/* Acquire the background texture, then release the temporary file allocation. */
extern const char evtMsgTexLoadNoProc[72];
extern const char evtMsgSetSkyAlphaLow[40];

#ifdef VERSION_RAIDOU1
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

#include "include_asm.h"
#include "draw3d.h"
#include "billboard.h"
#include "sdf_types.h"
#include "sdf_lighting.h"
#include "libsdf.h"
#include "model.h"
#include "motion.h"
#include "fpu.h"
#include "dds3UnitObjectBasic.h"
#include "code_r1_001C4B78.h"
#include "effPCPMisc_00224D10.h"

void evtPolygonMovieClearFlagBits(struct dds3ProcessID_t *task,
                                  EvtPolygonMovieWorkFlags flags);

#else
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

#include "include_asm.h"
#include "draw3d.h"
#include "billboard.h"
#include "sdf_types.h"
#include "sdf_lighting.h"
#include "libsdf.h"
#include "model.h"
#include "motion.h"
#include "fpu.h"
#include "dds3UnitObjectBasic.h"
#include "code_r1_001C4B78.h"
#include "effPCPMisc_00224D10.h"

void evtPolygonMovieClearFlagBits(struct dds3ProcessID_t *task,
                                  EvtPolygonMovieWorkFlags flags);

#endif

/* Signed 32-bit scalar values carried in script VM integer slots and APIs. */
typedef s32 ScrVmInteger;

/* Script command operands and return values encode scheduler task pointers in
 * signed 32-bit VM integer slots. Preserve all pointer bits at that boundary. */
typedef dds3ProcessID_t *ScrProcessHandle;

static inline ScrProcessHandle scrVmIntegerToProcessHandle(ScrVmInteger value)
{
    return (ScrProcessHandle)(u32)value;
}

static inline ScrVmInteger scrProcessHandleToVmInteger(ScrProcessHandle handle)
{
    return (s32)(u32)handle;
}

/* Selects both the world-node tag and its per-kind scene slot. */
typedef s32 EffWorldKind;

ScrCommandResult evtCommandSignalAdminAtTimerZero(void);
ScrCommandResult evtCommandSubmitEvent(void);

/* Forward the first VM integer as an opaque four-byte admin payload. */
ScrCommandResult evtCommandSubmitScalarAdminRequest(void);
ScrCommandResult evtCommandAddEffectUnitToWorld(void);
#ifdef VERSION_RAIDOU1
extern char D_003AC8E8[], D_003AC8F8[], D_003AC908[];
#else
extern char D_00421E58[], D_00421E68[], D_00421E78[];
#endif
ScrCommandResult evtCommandReattachObjectPath(void);
#ifdef VERSION_RAIDOU1

/* Create an event/scene movie task at the owning task's priority, hold its
 * viewer setup until the script releases it,
 * and return its handle through the VM result. Missing work skips creation. */
#endif
ScrCommandResult evtCommandStartPolygonMovie(void);

#ifdef VERSION_RAIDOU1
#else
#endif

enum EvtTargetInfoFlags {
    EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR = 0x8
};

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * Node payloads depend on the slot. */
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

struct sdfGrOt_t;

struct EffNodeDescriptor;

#ifdef VERSION_RAIDOU2
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

#endif
extern f32 sdfFaceDivSoften, sdfFaceDivBias;

#define EVT_UNIT_FLAG_MOTION_START_PENDING 0x80

#define EVT_UNIT_FLAG_MOTION_WOBBLE 0x40

#define EVT_UNIT_FLAG_USE_PATH_ROTATION 0x10

#ifdef VERSION_RAIDOU1
/* Declared before first use (were implicit int). */
#else
extern void evtArmEffectObjectPendingValue(dds3Basic_t *object, s32 value);
extern void evtResetObjectPendingValue(dds3Basic_t *object);

#endif
struct SoundMixer;

extern void evtResetObjectPendingValue(dds3Basic_t *object);
extern void evtArmEffectObjectPendingValue(dds3Basic_t *object, s32 value);
extern u32 sdfGetLodChunkValue(sdfModelRootNode_t *model);
ScrCommandResult evtOpModelLodChg(void);
ScrCommandResult evtOpDeprecatedModelMotion(void);
ScrCommandResult evtOpSetUnitParams5(void);
ScrCommandResult evtOpSetUnitParams6(void);
ScrCommandResult evtCmdDestroySelectedWorldUnit(void);
ScrCommandResult evtOpStartUnitTransitionTowardWorldObject(void);
#ifdef VERSION_RAIDOU1
f32 evtGetShortestAngleDelta(f32 fromDegrees, f32 toDegrees);
#else
ScrCommandResult evtScriptSetUnitTableValueAndReset(void);
f32 evtGetShortestAngleDelta(f32 a, f32 b);
#endif
ScrCommandResult evtOpTurnUnitRelativeToWorldNode(void);
ScrCommandResult evtOpRestoreUnitReferenceAngle(void);

#ifdef VERSION_RAIDOU1
struct EffNodeDescriptor;
#else
#endif

#ifdef VERSION_RAIDOU1
struct dds3Basic_t;
#else

/* Signed selector added to an event sound's packed track ID.
 * evtTransEventBgm preserves the existing negative value as a no-track ID. */
typedef s32 EvtSoundVariation;

u32 evtOpcodeSetBgmVolumePan(void);
u32 evtOpcodeInitializeEffectSoundChannel(void);
#endif

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

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara);
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCallBtlCountClear);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara2);
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtMODELDSPBUSTUP);
#endif

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

#ifdef VERSION_RAIDOU1
ScrCommandResult evtOpModelLodChg(void) {
    s32 lod;
    dds3Basic_t *world;
    dds3Basic_t *unit;
    EffectObjectData *data;
    dds3ObjectBaseControlWorkData_t *modelHolder;
    mdlObj_t *context;
    sdfModelRootNode_t *model;
    s32 max;

    lod = scrGetIntPara(1);
    printf("call: MODEL_LOD_CHG(int,int)\n");
    world = dds3GetActiveScene();
    unit = dds3SearchSceneBasic(world, scrGetIntPara(0), 5);
    if (unit == NULL) {
        printf("warning!! MODEL_LOD_CHG(int,int) unit pointer null\n");
        return 1;
    }
    data = unit->workdata;
    if (data->modelHolder == NULL) {
        printf("warning!! MODEL_LOD_CHG(int,int) workbase pointer null\n");
        return 1;
    }
    modelHolder = data->modelHolder;
    if (modelHolder->pModel == NULL) {
        printf("warning!! MODEL_LOD_CHG(int,int) mh pointer null\n");
        return 1;
    }
    context = (mdlObj_t *)modelHolder->pModel;
    model = context->inner;
    if (model == NULL) {
        printf("warning!! MODEL_LOD_CHG(int,int) root pointer null\n");
        return 1;
    }
    max = (s32)sdfGetLodChunkValue(model);
    if (max < lod) {
        printf("warning!! MODEL_LOD_CHG(int,int) lodno over!! max=%d setval=%d\n", max, lod);
        return 1;
    }
    model->lod = lod;
    printf("success: MODEL_LOD_CHG(int,int)\n");
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtWarningMODELLOD);
#endif

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

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetActiveScene2);

#endif
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
    dds3SetSclPoseSet(unit, &scl);
    return 1;
}

#ifdef VERSION_RAIDOU1
u32 evtCallGetIntPara5(void)
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
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_WAIT_EXMOT);

INCLUDE_ASM(const s32, "event/evtCommand", evtMODELMOTION);
#endif

#ifdef VERSION_RAIDOU1
const char evtMsgModelMotionDeprecated[64] = "\xa2\xa8\xc3\xed\xb0\xd5\x3a\x4d\x4f\x44\x45\x4c\x5f\x4d\x4f\x54\x49\x4f\x4e\x28\x29\xa4\xb3\xa4\xce\xa5\xb3\xa5\xde\xa5\xf3\xa5\xc9\xa4\xcf\xba\xa3\xb8\xe5\xbb\xc8\xcd\xd1\xb6\xd8\xbb\xdf\xa4\xcb\xa4\xca\xa4\xea\xa4\xde\xa4\xb9\xa1\xa3\x0a";

INCLUDE_ASM(const s32, "event/evtCommand", gohScrCmd_GOH_EXIT);
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara);
#endif

INCLUDE_ASM(const s32, "event/evtCommand", evtMODELEXMOTION);

INCLUDE_ASM(const s32, "event/evtCommand", KutiFunction);

INCLUDE_ASM(const s32, "event/evtCommand", evtLipsExecFunction);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "event/evtCommand", evtLipsStopFunction_evtCommand);
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetActiveScene3);
#endif

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

#ifdef VERSION_RAIDOU1
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
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_MOVE_MOT);
#endif

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

#ifdef VERSION_RAIDOU1
f32 evtGetShortestAngleDelta(f32 fromDegrees, f32 toDegrees) {
    f32 difference;

    if (fromDegrees < 0.0f || toDegrees < 0.0f) {
        fromDegrees += 360.0f;
        toDegrees += 360.0f;
    }
    fromDegrees = (s32)fromDegrees % 360;
    toDegrees = (s32)toDegrees % 360;
    difference = fromDegrees - toDegrees;
    if (difference > 180.0f || difference < -180.0f) {
        if (fromDegrees < toDegrees) {
            fromDegrees += 360.0f;
        } else {
            toDegrees += 360.0f;
        }
    }
    return toDegrees - fromDegrees;
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
#else
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
#endif

#ifdef VERSION_RAIDOU1
u32 evtCommand_EFFECT_BE(void) {
    u8 firstVector[16] QWORD_ALIGNED;
    u8 secondVector[16] QWORD_ALIGNED;
    s32 taskId;
    s32 key;
    struct effResourceHeader_t *descriptor;
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

#endif
const char evtMsgLightDirZero[40] = "WARNING: MODEL_LIGHT_DIR length zero\n";

#ifdef VERSION_RAIDOU1
const char evtMsgEffectBeNotFound[32] = "EFFECT_BE not fount RID = %d\n";
#else
INCLUDE_ASM(const s32, "event/evtCommand", datCommand_DEVIL_IN);
#endif

#ifdef VERSION_RAIDOU1
const char evtMsgEffectBeFailed[24] = "\x45\x46\x46\x45\x43\x54\x5f\x42\x45\x28\x29\xc5\xd0\xcf\xbf\xbc\xba\xc7\xd4\x0a";
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFECT_LOAD);
#endif

#ifdef VERSION_RAIDOU1
const char evtMsgEffectLoadHint[48] = "\xa5\xb9\xa5\xaf\xa5\xea\xa5\xd7\xa5\xc8\xa5\xb3\xa5\xde\xa5\xf3\xa5\xc9\x45\x46\x46\x45\x43\x54\x5f\x4c\x4f\x41\x44\x28\x29\xa4\xc7\xc2\xd0\xbe\xdd\xa5\xd5\xa5\xa1\xa5\xa4\xa5\xeb\x0a";

const char evtMsgNameNotFound[24] = "\x25\x73\xa4\xac\xb8\xab\xa4\xc4\xa4\xab\xa4\xea\xa4\xde\xa4\xbb\xa4\xf3\x21\x21\x0a";
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFBED_LOAD);
#endif

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

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "event/evtCommand", evtCallMemset);
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCallMemset2);
#endif

#ifdef VERSION_RAIDOU1
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

INCLUDE_RODATA(const s32, "event/evtCommand", evtStrEffeplLoad);

const char evtMsgEffmg1LoadHint[48] = "\xa5\xb9\xa5\xaf\xa5\xea\xa5\xd7\xa5\xc8\xa5\xb3\xa5\xde\xa5\xf3\xa5\xc9\x45\x46\x46\x4d\x47\x31\x5f\x4c\x4f\x41\x44\x28\x29\xa4\xc7\xc2\xd0\xbe\xdd\xa5\xd5\xa5\xa1\xa5\xa4\xa5\xeb\x0a";
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFMG1_LOAD);
#endif

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
    object = dds3AddMG12EffectObjectBasic2(1, (struct effResourceHeader_t *)resource);
    if (object == NULL) {
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->serialID);
    }
    return 1;
}

#ifdef VERSION_RAIDOU1
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
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFMG2_LOAD);
#endif

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
    object = dds3AddMG12EffectObjectBasic2(2, (struct effResourceHeader_t *)resource);
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

#ifdef VERSION_RAIDOU1
u32 evtCommand_SEARCH_ID(void)
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_SEARCH_ID);

s32 evtCommand_RTRN_TITLE(void)
#endif
{
#ifdef VERSION_RAIDOU1
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
#else
    dds3AdminiCallSequence(2, 0, 0, 0);
#endif
    return 1;
}
#ifdef VERSION_RAIDOU1

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_RTRN_TITLE);
#endif

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_STAFF);

s32 evtCommand_CALL_ENDSAVE(void)
{
    dds3AdminiCallSequence(5, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", titleCallNameEntry);

#ifdef VERSION_RAIDOU1
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
#else
INCLUDE_ASM(const s32, "event/evtCommand", titleGetPressAnyButton);
#endif

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

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "event/evtCommand", evtCallMemset2);
#else
INCLUDE_ASM(const s32, "event/evtCommand", CallBattleSub2_evtCommand);
#endif

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
#ifdef VERSION_RAIDOU2

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_FIELD3);
#endif

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

#ifdef VERSION_RAIDOU1
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
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_FIELD_CLS);
#endif

#ifdef VERSION_RAIDOU1
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
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_FIELD_CLS_STOP);
#endif

#ifdef VERSION_RAIDOU1
s32 evtCommand_UNIT_CLS(void)
{
    evtFreeAllUnit();
    printf("unit all clear !!\n");
    datDebugPrintf("unit all clear.\n");
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtUnitAllClear);
#endif

#ifdef VERSION_RAIDOU1
s32 evtCommand_UNIT_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    printf("unit all clear2 !!\n");
    datDebugPrintf("unit all clear2.\n");
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_UNIT_CLS);
#endif

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
    dds3GetRadiusPoseSet(dds3AddCameraObjectBasicTypeFree(id, &pos, &rot), 1.0f);
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

#ifdef VERSION_RAIDOU1
const char evtMsgObjPathPara0[16] = "PARA0:%08x\n";
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCallGetIntPara2);
#endif

#ifdef VERSION_RAIDOU1
const char evtMsgObjPathPara1[16] = "PARA1:%08x\n";

INCLUDE_RODATA(const s32, "event/evtCommand", evtMsgObjPathMoveNoObj);

INCLUDE_ASM(const s32, "event/evtCommand", evtPARA);

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
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_OBJ_PATH_WAIT);
#endif

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

#ifdef VERSION_RAIDOU1
const char evtMsgObjPathWaitNoObj[32] = "e OBJ_PATH_WAIT : obj==NULL\n";

#endif
const char evtMsgLoadBeStart[16] = "load BE (%d)..\n";

const char evtMsgBeOk[16] = "BE ok! (%d)\n";

const char evtMsgReadBeStart[16] = "read BE (%d)..\n";

const char evtMsgDelBeStart[16] = "del BE (%d)..\n";
#ifdef VERSION_RAIDOU1

INCLUDE_RODATA(const s32, "event/evtCommand", evtMsgPmvLoadNoProc);
#endif

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

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "event/evtCommand", evtMsgPmvRunNoProc);

#endif
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
#ifdef VERSION_RAIDOU1

INCLUDE_RODATA(const s32, "event/evtCommand", evtMsgTexBeNoProc);
#endif

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

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "event/evtCommand", evtOpcodeSetBgmVolumePan);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommandStartBgmWithFade);
#else
u32 evtOpcodeSetBgmVolumePan(void) {
    s32 eventId;
    EvtSoundVariation variation;

    eventId = scrGetIntPara(0);
    variation = scrGetIntPara(1);
    evtPlaySe(eventId, variation);
    return 1;
}
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "event/evtCommand", evtOpcodeRefreshTaskData);

INCLUDE_ASM(const s32, "event/evtCommand", evtOpcodeInitializeEffectSoundChannel);
#else
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
#endif

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
#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "event/evtCommand", evtMsgTexLoadNoProc);

#endif
const char evtMsgSetSkyAlphaLow[40] = "warning : SET_SKY_A alpha < -255\n";

addr_t evtKutiUnit = 0;

const char evtMsgSetSkyAlphaHigh[0x28] = "warning : SET_SKY_A alpha > 255\n";

INCLUDE_SDATA(const s32, "event/evtCommand", evtLipsStopFunction_evtCommandData);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtCommand", evtCallMemsetData);
#else
INCLUDE_SDATA(const s32, "event/evtCommand", evtCallMemset2Data);
#endif

