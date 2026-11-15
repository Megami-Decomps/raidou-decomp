#include "common.h"
#include "evtMoon.h"
#include "dds3ObjectBase.h"
#include "dds3MatterObjectBasic.h"
#include "fldProcess.h"

#include "dds3obj.h"
#include "field.h"
#include "kernel.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "basic.h"
#include "ps2asm.h"
#include "fpu.h"
#include "dds3UnitObjectBasic.h"

/* Kind-1 value cursors use these halfwords at 00110490. The same prefix is
 * used by separate 0x10-byte world index nodes (0010FEC8). */
typedef struct dds_dds3BasicArrayHandle_t {
    s16 headIndex;
    s16 tailIndex;
    s16 cursorIndex;
    u16 entryCount;
} dds_dds3BasicArrayHandle_t;

typedef char WorldValueIndices_size_must_be_0x08[(sizeof(dds_dds3BasicArrayHandle_t) == 0x08) ? 1 : -1];

/* Cursor advancement returns whether cursorIndex refers to a linked entry. */
typedef u32 Dds3BasicArrayCursorResult;

enum {
    DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED = 0,
    DDS3_BASIC_ARRAY_CURSOR_HAS_ENTRY = 1
};

void dds3FreeBasicArrayHandle(WorldIndexNode *node);
dds3Basic_t *dds3GetBasicArray(const dds_dds3BasicArrayHandle_t *object);
u16 dds3GetBasicArrayCount(const dds_dds3BasicArrayHandle_t *object);

/* Return the borrowed secondary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetControlScene(void);

/* Object data word 8 stores a kind-dependent 32-bit payload. */

/* Set the callback on an object's slot-1 handler; returns whether it exists. */

WorldIndexNode *dds3SceneBasicGetList(dds3Basic_t *object, s32 kind);

/* Compatible redeclarations keep the broad object header unchanged. */
Dds3BasicArrayCursorResult dds3SetFirstBasicArray(dds_dds3BasicArrayHandle_t *object);
Dds3BasicArrayCursorResult dds3SetNextBasicArray(dds_dds3BasicArrayHandle_t *object);

extern char D_0039FCA0[];
extern dds3Basic_t *fldPlayerObject;
extern u32 fldReleaseUnitPadData[];

struct dds3Basic_t;

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz);
void fldToggleWorldNodeState(s64 clearMode);
extern u64 D_003BAB70[], D_003BAB78[], D_003BAB80[], D_003BAB88[], D_003BAB90[];
extern u64 D_003BAB98[], D_003BABA0[], D_003BABA8[], D_003BABB0[];

typedef struct FieldVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FieldVec4;

void fldReleaseUnitPad(void);

/* Area-state consumption markers are separate from shared game-state flags. */
enum {
    FIELD_TRANSITION_CONSUMED_FLAG = 2,
    FIELD_SECONDARY_SCENE_CONSUMED_FLAG = 4,
    FIELD_PRIMARY_SCENE_CONSUMED_FLAG = 8
};

enum {
    FIELD_ENCOUNTER_ROLL_ENTRY_COUNT = 16
};

#define FIELD_ENCOUNTER_PERCENT_RANGE 100U

extern char D_0039FCA0[], D_0039FCB0[];

struct dds_dds3ObjectBaseControlWorkData_t;

extern char D_0039FD30[], D_0039FD40[];
extern char D_0039FD30[], D_0039FD40[];

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    return fsqrtf(dx * dx + dy * dy + dz * dz);
}

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetCircleCircleH);

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetEyeTgtRot);

INCLUDE_ASM(const s32, "code_r1_00128378", mapCallAtan2);

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetEyeTgt_Rot_OfsPos);

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetTaniVector);

/* For kind-6 objects in mode 4, set mode 3 or clear it to 0.
 * status covers the count, payload mode word and cursor result. */
void fldToggleWorldNodeState(s64 clearMode) {
    WorldIndexNode *valueChain;
    s32 status;
    dds3Basic_t *worldObject;

    valueChain = dds3SceneBasicGetList(dds3GetControlScene(), 6);
    status = dds3GetBasicArrayCount((dds_dds3BasicArrayHandle_t *)valueChain);
    if (status == 0) {
        return;
    }
    dds3SetFirstBasicArray((dds_dds3BasicArrayHandle_t *)valueChain);
    do {
        worldObject = dds3GetBasicArray((dds_dds3BasicArrayHandle_t *)valueChain);
        status = dds3MatterObjectBasicGetAlpMode(worldObject);
        if (status == 4) {
            if (clearMode == 0) {
                dds3MatterObjectBasicSetAlpMode(worldObject, 3);
            }
            else {
                dds3MatterObjectBasicSetAlpMode(worldObject, 0);
            }
        }
        status = dds3SetNextBasicArray((dds_dds3BasicArrayHandle_t *)valueChain);
    } while (status != DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED);
    dds3FreeBasicArrayHandle(valueChain);
}

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_00128378", mapRefDDS3GBWK);
#else
INCLUDE_ASM(const s32, "code_r1_00128378", NewGameInit);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", mapProcSequenceSub);
#ifdef VERSION_RAIDOU2

INCLUDE_ASM(const s32, "code_r1_00128378", mapRefDDS3GBWK);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", mapRefDDS3GBWK2);

INCLUDE_ASM(const s32, "code_r1_00128378", mapRefDDS3GBWK3);
#ifdef VERSION_RAIDOU1

INCLUDE_ASM(const s32, "code_r1_00128378", mapRefDDS3GBWK4);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", mapCallBitCheck);

INCLUDE_ASM(const s32, "code_r1_00128378", mapRndEncParamSet);

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetPlayersBasic);

INCLUDE_ASM(const s32, "code_r1_00128378", mapSetPlayersVisible);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_00128378", mapCheckDying);
#else
INCLUDE_ASM(const s32, "code_r1_00128378", mapCallProcessCheck);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", mapRefBasic);

INCLUDE_ASM(const s32, "code_r1_00128378", mapCallObjectBaseCheckFlag);

INCLUDE_ASM(const s32, "code_r1_00128378", mapRefBasic2);

INCLUDE_ASM(const s32, "code_r1_00128378", effCallObjectBaseCheckFlag);

INCLUDE_ASM(const s32, "code_r1_00128378", effRefDvBasic);

INCLUDE_ASM(const s32, "code_r1_00128378", effBTLFieldColorDraw);

INCLUDE_ASM(const s32, "code_r1_00128378", effRefDvBasic2);

INCLUDE_ASM(const s32, "code_r1_00128378", fldFreeInputPanel);

INCLUDE_ASM(const s32, "code_r1_00128378", mapRaidouBackUpPosReset);

INCLUDE_ASM(const s32, "code_r1_00128378", mapCallGetRotPoseSet);

INCLUDE_ASM(const s32, "code_r1_00128378", mapCallGetRotPoseSet2);

INCLUDE_ASM(const s32, "code_r1_00128378", fldCtnGetSub);

INCLUDE_ASM(const s32, "code_r1_00128378", fldSetSceneParamBattleReturnSub);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "code_r1_00128378", tipsProcessCheck);

INCLUDE_ASM(const s32, "code_r1_00128378", tipsGetSkilZeniSagasiCheckSub);

INCLUDE_ASM(const s32, "code_r1_00128378", tipsGetSkilZeniSagasiCheckSub2);

INCLUDE_ASM(const s32, "code_r1_00128378", tipsGetSkilTakaraSagasiCheckSub);

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetSkilTakaraSagasiCheckSub);

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetSkilTakaraSagasiCheckSub2);

INCLUDE_ASM(const s32, "code_r1_00128378", mapTakaraChkOpen);

INCLUDE_ASM(const s32, "code_r1_00128378", mapLeaf);

INCLUDE_ASM(const s32, "code_r1_00128378", fldGbLocationSavePointStore);

INCLUDE_ASM(const s32, "code_r1_00128378", mapChkTakaraEX);

#endif
INCLUDE_RODATA(const s32, "code_r1_00128378", mapGetEyeTgtRotTable);

INCLUDE_RODATA(const s32, "code_r1_00128378", mapRaidouBackUpPosResetTable);

INCLUDE_RODATA(const s32, "code_r1_00128378", mapCallGetRotPoseSetTable);

INCLUDE_RODATA(const s32, "code_r1_00128378", mapCallGetRotPoseSet2Table);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_00128378", fldFirstInit);
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldReturnSavePointPositionRestore);

INCLUDE_ASM(const s32, "code_r1_00128378", fldCallChkTakaraEX);

INCLUDE_ASM(const s32, "code_r1_00128378", sdfMcPollMkdir);

INCLUDE_ASM(const s32, "code_r1_00128378", sdfLoadMapFromMemorySub);

INCLUDE_ASM(const s32, "code_r1_00128378", sbtlRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_r1_00128378", sbtlCountClear);

INCLUDE_ASM(const s32, "code_r1_00128378", sbtlTitleNewGameStartSub);

INCLUDE_ASM(const s32, "code_r1_00128378", fclCombineDraw);

INCLUDE_ASM(const s32, "code_r1_00128378", fclMapAllShadow);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", mapCutFileLoad);

INCLUDE_ASM(const s32, "code_r1_00128378", mapCallMemset);

INCLUDE_ASM(const s32, "code_r1_00128378", mapSetSceneParam_BattleReturn);

INCLUDE_ASM(const s32, "code_r1_00128378", fldElvReset);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_00128378", fldReadyScene);
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldCallLoadArea);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_00128378", fldModelFieldLB);
#else
INCLUDE_ASM(const s32, "code_r1_00128378", PlayerGetMdlLoadID);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", fldPlayerALoad);

INCLUDE_ASM(const s32, "code_r1_00128378", fldPlayerALoad_OnDataCheck);

INCLUDE_ASM(const s32, "code_r1_00128378", fldPlayerALoadStart);

INCLUDE_ASM(const s32, "code_r1_00128378", fldPlayerALoadEndCheck);

INCLUDE_ASM(const s32, "code_r1_00128378", fldPlayerAFree);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_00128378", fldJDataLoad);
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldCallPlayerALoad);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", fldCallMemFreeDelayedDirect);

INCLUDE_ASM(const s32, "code_r1_00128378", fldCallMemFreeDelayedDirect2);

INCLUDE_ASM(const s32, "code_r1_00128378", fldNakamaCashLoad_Start);

INCLUDE_ASM(const s32, "code_r1_00128378", fldNakamaCashLoad_Resrc);

INCLUDE_ASM(const s32, "code_r1_00128378", nbResultFreeTextures);

INCLUDE_ASM(const s32, "code_r1_00128378", fldNakamaCashLoad_Block);

INCLUDE_ASM(const s32, "code_r1_00128378", fldNakamaCashLoad_RndEncRetStart);

INCLUDE_ASM(const s32, "code_r1_00128378", fldNakamaCashLoad_EndCheck);

INCLUDE_ASM(const s32, "code_r1_00128378", fldSetUnitSub);

INCLUDE_ASM(const s32, "code_r1_00128378", fldCallMemFreeDelayedDirect3);

INCLUDE_ASM(const s32, "code_r1_00128378", fldModelNpcLb);

INCLUDE_ASM(const s32, "code_r1_00128378", fldAiboCashLoad_Resrc);

INCLUDE_ASM(const s32, "code_r1_00128378", fldCallPackDecode);

INCLUDE_ASM(const s32, "code_r1_00128378", fldAiboCashLoad_Block);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_00128378", fldAiboCashLoad_RndEncRetStart);
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldRefBasic);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", fldAiboCashLoad_EndCheck);

INCLUDE_ASM(const s32, "code_r1_00128378", fldSetUnitSub2);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_00128378", cmpModelFreeResource);
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldLoadMapResourceSub);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_00128378", cmpCallModelFreeResource);
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldLoadMapResourceSub2);

INCLUDE_ASM(const s32, "code_r1_00128378", fldCallCtnFree);

INCLUDE_ASM(const s32, "code_r1_00128378", fldCleanupFieldScene);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", fldRegistUnitPad);

void fldReleaseUnitPad(void) {
    if (fldPlayerObject != 0) {
        fldEncountClearFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(fldPlayerObject, 0);
    }
    fldReleaseUnitPadData[0] = 4;
}
INCLUDE_SDATA(const s32, "code_r1_00128378", playerModel);

INCLUDE_SDATA(const s32, "code_r1_00128378", raidoBasic);

INCLUDE_SDATA(const s32, "code_r1_00128378", raidoModel);

INCLUDE_SDATA(const s32, "code_r1_00128378", aiboBasic);

INCLUDE_SDATA(const s32, "code_r1_00128378", aiboModel);

INCLUDE_SDATA(const s32, "code_r1_00128378", summonDvBasic);

INCLUDE_SDATA(const s32, "code_r1_00128378", summonDvModel);

INCLUDE_SDATA(const s32, "code_r1_00128378", cameraBasic);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldData9);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "code_r1_00128378", mapSetSceneParam_BattleReturnData);
#else
INCLUDE_SDATA(const s32, "code_r1_00128378", fldReturnSavePointPositionRestoreData);

INCLUDE_SDATA(const s32, "code_r1_00128378", nbData21);

INCLUDE_SDATA(const s32, "code_r1_00128378", Process_WarpIdx);
#endif

INCLUDE_SDATA(const s32, "code_r1_00128378", effQueuedResourceNameRecord2);

INCLUDE_SDATA(const s32, "code_r1_00128378", effQueuedResourceNameRecord3);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldJDataLoadData);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldJDataLoadData2);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldData10);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldPlayerALoadEndCheckData);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldData11);

INCLUDE_SDATA(const s32, "code_r1_00128378", nbResultFreeTexturesData);

INCLUDE_SDATA(const s32, "code_r1_00128378", nbResultFreeTexturesData2);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldNakamaCashLoad_ResrcData);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldNakamaCashLoad_EndCheckData);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldData12);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldCallPackDecodeData);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldCallPackDecodeData2);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldAiboCashLoad_ResrcData);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldData13);

