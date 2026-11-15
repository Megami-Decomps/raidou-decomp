#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds3obj.h"
#include "field.h"
#include "kernel.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "dds/btl.h"
#include "dds/dds3_basic_array_cursor.h"
#include "dds/dds3_slot1_handler_api.h"

void dds3FreeBasicArrayHandle(WorldIndexNode *node);
dds3Basic_t *dds3GetBasicArray(const dds_dds3BasicArrayHandle_t *object);
u16 dds3GetBasicArrayCount(const dds_dds3BasicArrayHandle_t *object);

/* Return the borrowed secondary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetControlScene(void);
u32 dds3MatterObjectBasicGetAlpMode(const struct dds3Basic_t *object);

/* Object data word 8 stores a kind-dependent 32-bit payload. */
void dds3MatterObjectBasicSetAlpMode(struct dds3Basic_t *object, u32 value);

/* Set the callback on an object's slot-1 handler; returns whether it exists. */
u8 dds3ObjectBaseSetPrimaryControl(struct dds3Basic_t *object,
                               Dds3MoverUpdate update);

WorldIndexNode *dds3SceneBasicGetList(dds3Basic_t *object, s32 kind);

/* Compatible redeclarations keep the broad object header unchanged. */
Dds3BasicArrayCursorResult dds3SetFirstBasicArray(dds_dds3BasicArrayHandle_t *object);
Dds3BasicArrayCursorResult dds3SetNextBasicArray(dds_dds3BasicArrayHandle_t *object);
extern void *memcpy(void *, const void *, unsigned int);

#include "dds/ee_mmi.h"
#include "dds/fpu.h"

extern char D_0039FCA0[];
extern dds3Basic_t *fldPlayerObject;
extern u32 fldReleaseUnitPadData[];

struct dds3Basic_t;

void fldEncountClearFlag(u32 mask);

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
extern void evtDisableMoonDraw(void);

struct dds_dds3ObjectBaseControlWorkData_t;

extern char D_0039FD30[], D_0039FD40[];
extern char D_0039FD30[], D_0039FD40[];

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    return fsqrtf(dx * dx + dy * dy + dz * dz);
}

INCLUDE_ASM(const s32, "code_0012C998", mapGetCircleCircleH);

INCLUDE_ASM(const s32, "code_0012C998", mapGetEyeTgtRot);

INCLUDE_ASM(const s32, "code_0012C998", mapCallAtan2);

INCLUDE_ASM(const s32, "code_0012C998", mapGetEyeTgt_Rot_OfsPos);

INCLUDE_ASM(const s32, "code_0012C998", mapGetTaniVector);

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

INCLUDE_ASM(const s32, "code_0012C998", NewGameInit);

INCLUDE_ASM(const s32, "code_0012C998", mapProcSequenceSub);

INCLUDE_ASM(const s32, "code_0012C998", mapRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_0012C998", mapRefDDS3GBWK2);

INCLUDE_ASM(const s32, "code_0012C998", mapRefDDS3GBWK3);

INCLUDE_ASM(const s32, "code_0012C998", mapCallBitCheck);

INCLUDE_ASM(const s32, "code_0012C998", mapRndEncParamSet);

INCLUDE_ASM(const s32, "code_0012C998", mapGetPlayersBasic);

INCLUDE_ASM(const s32, "code_0012C998", mapSetPlayersVisible);

INCLUDE_ASM(const s32, "code_0012C998", mapCallProcessCheck);

INCLUDE_ASM(const s32, "code_0012C998", mapRefBasic);

INCLUDE_ASM(const s32, "code_0012C998", mapCallObjectBaseCheckFlag);

INCLUDE_ASM(const s32, "code_0012C998", mapRefBasic2);

INCLUDE_ASM(const s32, "code_0012C998", effCallObjectBaseCheckFlag);

INCLUDE_ASM(const s32, "code_0012C998", effRefDvBasic);

INCLUDE_ASM(const s32, "code_0012C998", effBTLFieldColorDraw);

INCLUDE_ASM(const s32, "code_0012C998", effRefDvBasic2);

INCLUDE_ASM(const s32, "code_0012C998", fldFreeInputPanel);

INCLUDE_ASM(const s32, "code_0012C998", mapRaidouBackUpPosReset);

INCLUDE_ASM(const s32, "code_0012C998", mapCallGetRotPoseSet);

INCLUDE_ASM(const s32, "code_0012C998", mapCallGetRotPoseSet2);

INCLUDE_ASM(const s32, "code_0012C998", fldCtnGetSub);

INCLUDE_ASM(const s32, "code_0012C998", fldSetSceneParamBattleReturnSub);

INCLUDE_ASM(const s32, "code_0012C998", tipsProcessCheck);

INCLUDE_ASM(const s32, "code_0012C998", tipsGetSkilZeniSagasiCheckSub);

INCLUDE_ASM(const s32, "code_0012C998", tipsGetSkilZeniSagasiCheckSub2);

INCLUDE_ASM(const s32, "code_0012C998", tipsGetSkilTakaraSagasiCheckSub);

INCLUDE_ASM(const s32, "code_0012C998", mapGetSkilTakaraSagasiCheckSub);

INCLUDE_ASM(const s32, "code_0012C998", mapGetSkilTakaraSagasiCheckSub2);

INCLUDE_ASM(const s32, "code_0012C998", mapTakaraChkOpen);

INCLUDE_ASM(const s32, "code_0012C998", mapLeaf);

INCLUDE_ASM(const s32, "code_0012C998", fldGbLocationSavePointStore);

INCLUDE_ASM(const s32, "code_0012C998", mapChkTakaraEX);

INCLUDE_RODATA(const s32, "code_0012C998", mapGetEyeTgtRotTable);

INCLUDE_RODATA(const s32, "code_0012C998", mapRaidouBackUpPosResetTable);

INCLUDE_RODATA(const s32, "code_0012C998", mapCallGetRotPoseSetTable);

INCLUDE_RODATA(const s32, "code_0012C998", mapCallGetRotPoseSet2Table);

INCLUDE_ASM(const s32, "code_0012C998", fldReturnSavePointPositionRestore);

INCLUDE_ASM(const s32, "code_0012C998", fldCallChkTakaraEX);

INCLUDE_ASM(const s32, "code_0012C998", sdfMcPollMkdir);

INCLUDE_ASM(const s32, "code_0012C998", sdfLoadMapFromMemorySub);

INCLUDE_ASM(const s32, "code_0012C998", sbtlRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_0012C998", sbtlCountClear);

INCLUDE_ASM(const s32, "code_0012C998", sbtlTitleNewGameStartSub);

INCLUDE_ASM(const s32, "code_0012C998", fclCombineDraw);

INCLUDE_ASM(const s32, "code_0012C998", fclMapAllShadow);

INCLUDE_ASM(const s32, "code_0012C998", mapCutFileLoad);

INCLUDE_ASM(const s32, "code_0012C998", mapCallMemset);

INCLUDE_ASM(const s32, "code_0012C998", mapSetSceneParam_BattleReturn);

INCLUDE_ASM(const s32, "code_0012C998", fldElvReset);

INCLUDE_ASM(const s32, "code_0012C998", fldCallLoadArea);

INCLUDE_ASM(const s32, "code_0012C998", PlayerGetMdlLoadID);

INCLUDE_ASM(const s32, "code_0012C998", fldPlayerALoad);

INCLUDE_ASM(const s32, "code_0012C998", fldPlayerALoad_OnDataCheck);

INCLUDE_ASM(const s32, "code_0012C998", fldPlayerALoadStart);

INCLUDE_ASM(const s32, "code_0012C998", fldPlayerALoadEndCheck);

INCLUDE_ASM(const s32, "code_0012C998", fldPlayerAFree);

INCLUDE_ASM(const s32, "code_0012C998", fldCallPlayerALoad);

INCLUDE_ASM(const s32, "code_0012C998", fldCallMemFreeDelayedDirect);

INCLUDE_ASM(const s32, "code_0012C998", fldCallMemFreeDelayedDirect2);

INCLUDE_ASM(const s32, "code_0012C998", fldNakamaCashLoad_Start);

INCLUDE_ASM(const s32, "code_0012C998", fldNakamaCashLoad_Resrc);

INCLUDE_ASM(const s32, "code_0012C998", nbResultFreeTextures);

INCLUDE_ASM(const s32, "code_0012C998", fldNakamaCashLoad_Block);

INCLUDE_ASM(const s32, "code_0012C998", fldNakamaCashLoad_RndEncRetStart);

INCLUDE_ASM(const s32, "code_0012C998", fldNakamaCashLoad_EndCheck);

INCLUDE_ASM(const s32, "code_0012C998", fldSetUnitSub);

INCLUDE_ASM(const s32, "code_0012C998", fldCallMemFreeDelayedDirect3);

INCLUDE_ASM(const s32, "code_0012C998", fldModelNpcLb);

INCLUDE_ASM(const s32, "code_0012C998", fldAiboCashLoad_Resrc);

INCLUDE_ASM(const s32, "code_0012C998", fldCallPackDecode);

INCLUDE_ASM(const s32, "code_0012C998", fldAiboCashLoad_Block);

INCLUDE_ASM(const s32, "code_0012C998", fldRefBasic);

INCLUDE_ASM(const s32, "code_0012C998", fldAiboCashLoad_EndCheck);

INCLUDE_ASM(const s32, "code_0012C998", fldSetUnitSub2);

INCLUDE_ASM(const s32, "code_0012C998", fldLoadMapResourceSub);

INCLUDE_ASM(const s32, "code_0012C998", fldLoadMapResourceSub2);

INCLUDE_ASM(const s32, "code_0012C998", fldCallCtnFree);

INCLUDE_ASM(const s32, "code_0012C998", fldCleanupFieldScene);

INCLUDE_ASM(const s32, "code_0012C998", fldRegistUnitPad);

void fldReleaseUnitPad(void) {
    if (fldPlayerObject != 0) {
        fldEncountClearFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(fldPlayerObject, 0);
    }
    fldReleaseUnitPadData[0] = 4;
}
INCLUDE_SDATA(const s32, "code_0012C998", playerModel);

INCLUDE_SDATA(const s32, "code_0012C998", raidoBasic);

INCLUDE_SDATA(const s32, "code_0012C998", raidoModel);

INCLUDE_SDATA(const s32, "code_0012C998", aiboBasic);

INCLUDE_SDATA(const s32, "code_0012C998", aiboModel);

INCLUDE_SDATA(const s32, "code_0012C998", summonDvBasic);

INCLUDE_SDATA(const s32, "code_0012C998", summonDvModel);

INCLUDE_SDATA(const s32, "code_0012C998", cameraBasic);

INCLUDE_SDATA(const s32, "code_0012C998", fldData9);

INCLUDE_SDATA(const s32, "code_0012C998", fldReturnSavePointPositionRestoreData);

INCLUDE_SDATA(const s32, "code_0012C998", nbData21);

INCLUDE_SDATA(const s32, "code_0012C998", Process_WarpIdx);

INCLUDE_SDATA(const s32, "code_0012C998", effQueuedResourceNameRecord2);

INCLUDE_SDATA(const s32, "code_0012C998", effQueuedResourceNameRecord3);

INCLUDE_SDATA(const s32, "code_0012C998", fldJDataLoadData);

INCLUDE_SDATA(const s32, "code_0012C998", fldJDataLoadData2);

INCLUDE_SDATA(const s32, "code_0012C998", fldData10);

INCLUDE_SDATA(const s32, "code_0012C998", fldPlayerALoadEndCheckData);

INCLUDE_SDATA(const s32, "code_0012C998", fldData11);

INCLUDE_SDATA(const s32, "code_0012C998", nbResultFreeTexturesData);

INCLUDE_SDATA(const s32, "code_0012C998", nbResultFreeTexturesData2);

INCLUDE_SDATA(const s32, "code_0012C998", fldNakamaCashLoad_ResrcData);

INCLUDE_SDATA(const s32, "code_0012C998", fldNakamaCashLoad_EndCheckData);

INCLUDE_SDATA(const s32, "code_0012C998", fldData12);

INCLUDE_SDATA(const s32, "code_0012C998", fldCallPackDecodeData);

INCLUDE_SDATA(const s32, "code_0012C998", fldCallPackDecodeData2);

INCLUDE_SDATA(const s32, "code_0012C998", fldAiboCashLoad_ResrcData);

INCLUDE_SDATA(const s32, "code_0012C998", fldData13);

