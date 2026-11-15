#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "basic.h"
#include "datCalc.h"
#include "newdata.h"
#include "sdk_libc.h"
#include "dds/btl.h"

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


#include "dds/sdf_vu0.h"

#define SDF_PACKED_STATUS_BIT 0x8000

#define SDF_PACKED_CHANNEL_MASK 0x7fff

#define SDF_UNIT_ENEMY 0x20

extern DatEnemyRecord *datEnemyRecords;
extern char sdfRuntimeTaskName[];

#define SDF_FLOAT_COUNTER_FROZEN_FLAG 0x00000008

#define SDF_FLOAT_COUNTER_WRAP_FLAG 0x00000020

void *dds3GetWorldNodeData(dds3Basic_t *node);
void evtLoadValueVectorIntoVu(dds3Basic_t *node);
void evtLoadValueSecondaryVectorIntoVu(dds3Basic_t *node);

/* Load the world node's borrowed data vector into VF10. */
void evtLoadValueVectorIntoVu(dds3Basic_t *node) {
    void *vec = dds3GetWorldNodeData(node);

    SDF_VU0_LOADVECTOR_BARRIER(vec);

}

/* Same, from the second quadword in the borrowed vector. */
void evtLoadValueSecondaryVectorIntoVu(dds3Basic_t *node) {
    void *vec = (void *)((u8 *)dds3GetWorldNodeData(node) + 0x10);

    SDF_VU0_LOADVECTOR_BARRIER(vec);

}

INCLUDE_ASM(const s32, "dat", datGlobalWorkInit);

INCLUDE_ASM(const s32, "dat", datGlobalWorkDestroy);

INCLUDE_ASM(const s32, "dat", UserInit);

INCLUDE_ASM(const s32, "dat", UserInit_dat);

void datGlobalWorkDestroySub(void) {
}

INCLUDE_ASM(const s32, "dat", NewGame2Push);

INCLUDE_ASM(const s32, "dat", datCallBitOn);

INCLUDE_ASM(const s32, "dat", datLeaf);

INCLUDE_ASM(const s32, "dat", datLeaf2);

INCLUDE_ASM(const s32, "dat", datCallNewGame2Push);

void datLeaf42(void) {
}

void datLeaf43(void) {
}

INCLUDE_ASM(const s32, "dat", datAllReleaseScriptSub);

INCLUDE_ASM(const s32, "dat", sdfLoadChannelFileSegment);

INCLUDE_ASM(const s32, "dat", sdfReloadChannelSegments);

INCLUDE_ASM(const s32, "dat", datFunc24);

INCLUDE_ASM(const s32, "dat", datInitPlayer);

INCLUDE_ASM(const s32, "dat", datGetPlayerParam);

INCLUDE_ASM(const s32, "dat", datSetPlayerParam);

INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK10);

INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK11);

INCLUDE_ASM(const s32, "dat", datAdjustStock);

INCLUDE_ASM(const s32, "dat", datGetEmptyStock);

INCLUDE_ASM(const s32, "dat", datGetStockDevilIndex);

INCLUDE_ASM(const s32, "dat", datGetStockDevilNums);

INCLUDE_ASM(const s32, "dat", datCalcDevilMaxParCheck);

INCLUDE_ASM(const s32, "dat", Unlock_2);

INCLUDE_ASM(const s32, "dat", datChangeFromMskGetNoToSKLIDSub);

INCLUDE_ASM(const s32, "dat", fldChangeFromMskGetNoToSKLID);

INCLUDE_ASM(const s32, "dat", datBtlUpdateDamageKajaSub);

INCLUDE_ASM(const s32, "dat", datBtlUpdateDamageKajaSub2);

INCLUDE_ASM(const s32, "dat", datBtlGetCalcDamageExSub);

INCLUDE_ASM(const s32, "dat", datCallCalcDevilMaxParCheck);

INCLUDE_ASM(const s32, "dat", datLeaf44);

INCLUDE_ASM(const s32, "dat", CheckJidouSkillPrio);

INCLUDE_ASM(const s32, "dat", datCheckSkillEnable);

INCLUDE_ASM(const s32, "dat", datGetSkillCopyIndex);

INCLUDE_ASM(const s32, "dat", datReplaceHaveSkill);

INCLUDE_ASM(const s32, "dat", datAddSkill);

INCLUDE_ASM(const s32, "dat", datCreateDevilUnitEx);

INCLUDE_ASM(const s32, "dat", datCreateDevilUnit);

INCLUDE_ASM(const s32, "dat", datAddCheckDevilUnit);

INCLUDE_ASM(const s32, "dat", datAddDevilUnit);

INCLUDE_ASM(const s32, "dat", datAddDevilEx);

INCLUDE_ASM(const s32, "dat", datAddDevil);

INCLUDE_ASM(const s32, "dat", datDelDevil);

INCLUDE_ASM(const s32, "dat", datLoadLvUpTables);

void datLoadGohmadenTable(void) {
}

INCLUDE_ASM(const s32, "dat", datGetPlayerExp);

INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpNums);

INCLUDE_ASM(const s32, "dat", datGetPlayerMaxHP);

INCLUDE_ASM(const s32, "dat", datGetPlayerOnceLevelUpParam);

INCLUDE_ASM(const s32, "dat", datCallGetPlayerLevelUpNums);

INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpUnit);

INCLUDE_ASM(const s32, "dat", datGetDevilExp);

INCLUDE_ASM(const s32, "dat", datGetDevilLevelUpNums);

INCLUDE_ASM(const s32, "dat", datCallCalcDevilMaxParCheck2);

INCLUDE_ASM(const s32, "dat", datGetDevilOnceLevelUpParam);

INCLUDE_ASM(const s32, "dat", datCallGetDevilLevelUpNums);

INCLUDE_ASM(const s32, "dat", datGetDevilLevelUpUnit);

INCLUDE_ASM(const s32, "dat", datCalcNowParam);

INCLUDE_ASM(const s32, "dat", datGetSword4Param);

INCLUDE_ASM(const s32, "dat", datGetSwordParamHP);

INCLUDE_ASM(const s32, "dat", datGetSwordAisyoEx);

INCLUDE_ASM(const s32, "dat", datGetSwordAisyo);

INCLUDE_ASM(const s32, "dat", fldGetRestoreMode);

INCLUDE_ASM(const s32, "dat", datSetPlayerUnitSwordSub);

INCLUDE_ASM(const s32, "dat", datSetPlayerUnitSword);

INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK12);

INCLUDE_ASM(const s32, "dat", datLeaf3);

INCLUDE_ASM(const s32, "dat", datCheckDying);

INCLUDE_ASM(const s32, "dat", datLeaf45);

INCLUDE_ASM(const s32, "dat", datTakumiKoukaDSub);

INCLUDE_ASM(const s32, "dat", datMoonAddMoveSub);

INCLUDE_ASM(const s32, "dat", sdatCalc_GetBadStatus);

INCLUDE_ASM(const s32, "dat", cmpExSubMenuAnimFunc);

INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK13);

INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK14);
INCLUDE_SDATA(const s32, "dat", sdfRuntimeTaskName);

INCLUDE_SDATA(const s32, "dat", datData13);

INCLUDE_SDATA(const s32, "dat", NewGame2PushData3);

INCLUDE_SDATA(const s32, "dat", NewGame2PushData4);

INCLUDE_SDATA(const s32, "dat", datData14);

INCLUDE_SDATA(const s32, "dat", datData15);

INCLUDE_SDATA(const s32, "dat", datData16);

INCLUDE_SDATA(const s32, "dat", datData17);

INCLUDE_SDATA(const s32, "dat", datCallNewGame2PushData);

INCLUDE_SDATA(const s32, "dat", DDS3_GBWK);

INCLUDE_SDATA(const s32, "dat", sbtlData141);

INCLUDE_SDATA(const s32, "dat", sbtlSbattle3Data);

INCLUDE_SDATA(const s32, "dat", evtCallMemset9Data);

INCLUDE_SDATA(const s32, "dat", datEnemyRecords);

INCLUDE_SDATA(const s32, "dat", evtData12);

INCLUDE_SDATA(const s32, "dat", datEncountCalcSubData);

INCLUDE_SDATA(const s32, "dat", datBattleAdjustmentRecords);

INCLUDE_SDATA(const s32, "dat", datLeaf66Data);

INCLUDE_SDATA(const s32, "dat", rstCallGetRandAData);

INCLUDE_SDATA(const s32, "dat", datData4);

INCLUDE_SDATA(const s32, "dat", sbtlSetHangekiFlagData);

INCLUDE_SDATA(const s32, "dat", datLoadLvUpTablesData3);

INCLUDE_SDATA(const s32, "dat", datData18);

INCLUDE_SDATA(const s32, "dat", datLoadLvUpTablesData4);

INCLUDE_SDATA(const s32, "dat", rstCalcRankUpData);

INCLUDE_SDATA(const s32, "dat", datData19);

