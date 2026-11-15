#include "common.h"
#include "sdfMemory.h"
#include "sdfSystem.h"
#include "sdfFile.h"

#include "basic.h"
#include "datCalc.h"
#include "newdata.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "ps2asm.h"
#include "code_0027A1D0.h"
#include "code_r1_0021CE60.h"
#include "dat.h"

extern u32 datCallCheckStrongSwordAttackData;
extern u32 cmpLoadModelTablesData;
extern u32 datData6;
extern u32 datCallGetStockDevilIDData;

/* The 0x4C-byte enemy table supplies initial party vitals/stats, skills and
 * rewards. */
typedef struct DatEnemyRecord {
    u32 flags;            /* 0x00 */
    u8 pad04;
    u8 level;             /* 0x05 */
    u16 hp;               /* 0x06 */
    u16 maxHp;            /* 0x08 */
    u16 mp;               /* 0x0A */
    u16 maxMp;            /* 0x0C */
    s16 affinityTableIndex; /* 0x0E: signed affinity-bank row */
    u8 baseStats[DAT_BASE_STAT_COUNT]; /* 0x10 */
    u8 summonCategory;  /* 0x15: ENEMY_TABLE category used as a secondary actor's action number. */
    u8 usePanelIcons; /* 0x16: nonzero selects panel icon groups instead of numeric level/vitals. */
    u8 pad17;
    u16 skills[8];        /* 0x18 */
    s32 money;            /* 0x28 */
    u16 battleExperienceReward; /* 0x2C: base EXP awarded on enemy defeat. */
    u16 experience;       /* 0x2E */
    u16 huntExperience;   /* 0x30 */
    u8 pad32[4];
    u16 huntPenaltyFlags; /* 0x36: hunt's adverse-outcome mask. */
    u16 huntPenaltyChance; /* 0x38: base percentage before status modifiers. */
    u8 pad3A[4];
    u8 additionalDropItemIds[2]; /* 0x3E: optional item IDs selected with actionChances. */
    u8 actionChances[2];  /* 0x40: bucket thresholds for the additional drop item IDs. */
    u16 overrideFlag;     /* 0x42: model flag gating the override action. */
    u8 overrideAction;    /* 0x44: action selected before the ordinary slots. */
    u8 overrideChance;    /* 0x45: bucket threshold for the override action. */
    s8 indexedValueOverride; /* 0x46: signed value used for enemy selector index zero. */
    u8 tickCount;         /* 0x47: enemy action tick count; zero is read as one. */
    u8 attackSpacing;    /* Repeat-hit interval, scaled by the command's effect data. */
    u8 pad49[3];
} DatEnemyRecord;

typedef char DatEnemyRecordSizeCheck[sizeof(DatEnemyRecord) == 0x4C ? 1 : -1];

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


#ifdef VERSION_RAIDOU1
/* Allocate from the chip heap through 0x400 bytes; larger requests use the
 * general heap and return its locked payload address. */

/* Stable state APIs. Callback factories keep caller-specific callback types. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path);

/* Release the sdfAllocAuto base in its source heap; interior pointers are
 * invalid. */

#endif
/* Each configured segment supplies a destination slot and an opaque word. */
typedef struct SdfChannelEntry {
    void **slot;
    u32 unused;
} SdfChannelEntry;

/* The channel loader scans these entries through the first null slot. */
typedef struct SdfChannel {
    const char *path;
    SdfChannelEntry entries[20];
} SdfChannel;

typedef char SdfChannelEntry_size_must_be_8[(sizeof(SdfChannelEntry) == 8) ? 1 : -1];

typedef char SdfChannel_size_must_be_0xA4[(sizeof(SdfChannel) == 0xA4) ? 1 : -1];

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

extern const char sdfStrSys[10];
void sdfReloadChannelSegments(SdfChannel *table);

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

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCallGlobalWorkClearFlags);
#else
INCLUDE_ASM(const s32, "dat", UserInit_dat);
#endif

void datGlobalWorkDestroySub(void) {
}

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCallBitCheck);
#else
INCLUDE_ASM(const s32, "dat", NewGame2Push);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", NewGame2Pop);
#else
INCLUDE_ASM(const s32, "dat", datCallBitOn);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", dds3GlobalWorkUserInit2);
#else
INCLUDE_ASM(const s32, "dat", datLeaf);

INCLUDE_ASM(const s32, "dat", datLeaf2);

INCLUDE_ASM(const s32, "dat", datCallNewGame2Push);
#endif

void datLeaf42(void) {
}

void datLeaf43(void) {
}

INCLUDE_ASM(const s32, "dat", datAllReleaseScriptSub);

#ifdef VERSION_RAIDOU1
/* Load a table's file and copy each size-prefixed segment into its own block. */
void sdfReloadChannelSegments(SdfChannel *table) {
    sdfFileCmdHandle_t *command;
    struct sdfMemHandle_t *block;
    s32 size;
    u8 *address;
    u32 segmentBytes;
    u32 alignedBytes; /* 001181F4 stores the returned stride at static-chain +0xC. */
    s32 i;

    /* Copy one segment into *slot and report its length rounded up to 16 bytes. */
    s32 sdfLoadChannelFileSegment(void **slot, u32 unused) {
        u32 blocks;
        u32 advance;

        size = *(s32 *)address;
        if (*slot != 0) {
            sdfFreeAuto(*slot);
        }
        *slot = sdfAllocAuto(size);
        memcpy(*slot, address + 4, size);
        segmentBytes = size + 4;
        blocks = segmentBytes >> 4;
        if ((segmentBytes & 0xF) != 0) {
            advance = (blocks + 1) << 4;
        } else {
            advance = blocks << 4;
        }
        alignedBytes = advance;
        return alignedBytes;
    }

    command = sdfFileOpen(table->path);
    size = sdfFileLength(command);
    block = sdfMemAllocFromTailDirect(size);
    address = (u8 *)sdfMemLock(block);
    sdfFileRead(command, address, size);
    sdfFileClose(command);
    for (i = 0; table->entries[i].slot != 0; i++) {
        address += sdfLoadChannelFileSegment(table->entries[i].slot, table->entries[i].unused);
    }
    sdfMemFreeDirect(block);
    printf(sdfStrSys, table->path);
}
#else
INCLUDE_ASM(const s32, "dat", sdfLoadChannelFileSegment);

INCLUDE_ASM(const s32, "dat", sdfReloadChannelSegments);
#endif

INCLUDE_ASM(const s32, "dat", datFunc24);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCallMemset);
#else
INCLUDE_ASM(const s32, "dat", datInitPlayer);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK2);
#else
INCLUDE_ASM(const s32, "dat", datGetPlayerParam);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datGetStockDevilID);
#else
INCLUDE_ASM(const s32, "dat", datSetPlayerParam);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", effCloneParticleSharedResource);
#else
INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK10);

INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK11);

INCLUDE_ASM(const s32, "dat", datAdjustStock);
#endif

INCLUDE_ASM(const s32, "dat", datGetEmptyStock);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCheckSameDevil);

INCLUDE_ASM(const s32, "dat", datCallGetStockDevilID);
#else
INCLUDE_ASM(const s32, "dat", datGetStockDevilIndex);
#endif

INCLUDE_ASM(const s32, "dat", datGetStockDevilNums);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCreateDevilUnitExSub);
#else
INCLUDE_ASM(const s32, "dat", datCalcDevilMaxParCheck);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCheckJidouSkillPrioSub);
#else
INCLUDE_ASM(const s32, "dat", Unlock_2);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datFunc4);
#else
INCLUDE_ASM(const s32, "dat", datChangeFromMskGetNoToSKLIDSub);

INCLUDE_ASM(const s32, "dat", fldChangeFromMskGetNoToSKLID);

INCLUDE_ASM(const s32, "dat", datBtlUpdateDamageKajaSub);

INCLUDE_ASM(const s32, "dat", datBtlUpdateDamageKajaSub2);

INCLUDE_ASM(const s32, "dat", datBtlGetCalcDamageExSub);

INCLUDE_ASM(const s32, "dat", datCallCalcDevilMaxParCheck);
#endif

INCLUDE_ASM(const s32, "dat", datLeaf44);

INCLUDE_ASM(const s32, "dat", CheckJidouSkillPrio);

INCLUDE_ASM(const s32, "dat", datCheckSkillEnable);

INCLUDE_ASM(const s32, "dat", datGetSkillCopyIndex);

INCLUDE_ASM(const s32, "dat", datReplaceHaveSkill);

INCLUDE_ASM(const s32, "dat", datAddSkill);

INCLUDE_ASM(const s32, "dat", datCreateDevilUnitEx);

INCLUDE_ASM(const s32, "dat", datCreateDevilUnit);

INCLUDE_ASM(const s32, "dat", datAddCheckDevilUnit);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCallAddCheckDevilUnit);
#else
INCLUDE_ASM(const s32, "dat", datAddDevilUnit);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCallCreateDevilUnitEx);
#else
INCLUDE_ASM(const s32, "dat", datAddDevilEx);
#endif

INCLUDE_ASM(const s32, "dat", datAddDevil);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCallGetStockDevilID2);
#else
INCLUDE_ASM(const s32, "dat", datDelDevil);
#endif

INCLUDE_ASM(const s32, "dat", datLoadLvUpTables);

#ifdef VERSION_RAIDOU1
void cmpLoadModelTables(void)
{
    sdfDevLoadFileEx("/gohmaden/gohDevilParam.dat", &datCallCheckStrongSwordAttackData, &cmpLoadModelTablesData);
    sdfDevLoadFileEx("/gohmaden/gohSameDevils.dat", &datData6, &datCallGetStockDevilIDData);
}

INCLUDE_ASM(const s32, "dat", datGetPlayerExp);

INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpNums);

INCLUDE_ASM(const s32, "dat", datGetPlayerMaxHP);
#else
void datLoadGohmadenTable(void) {
}

INCLUDE_ASM(const s32, "dat", datGetPlayerExp);

INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpNums);

INCLUDE_ASM(const s32, "dat", datGetPlayerMaxHP);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpParamSub);
#else
INCLUDE_ASM(const s32, "dat", datGetPlayerOnceLevelUpParam);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpParam);
#else
INCLUDE_ASM(const s32, "dat", datCallGetPlayerLevelUpNums);
#endif

INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpUnit);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datGetDevilLevelUpNumsSub);
#else
INCLUDE_ASM(const s32, "dat", datGetDevilExp);
#endif

INCLUDE_ASM(const s32, "dat", datGetDevilLevelUpNums);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datGetDevilMaxHP);
#else
INCLUDE_ASM(const s32, "dat", datCallCalcDevilMaxParCheck2);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datGetDevilMaxMP);
#else
INCLUDE_ASM(const s32, "dat", datGetDevilOnceLevelUpParam);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCallGetRandA);

INCLUDE_ASM(const s32, "dat", datGetDevilLevelUpParam);
#else
INCLUDE_ASM(const s32, "dat", datCallGetDevilLevelUpNums);
#endif

INCLUDE_ASM(const s32, "dat", datGetDevilLevelUpUnit);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCallGetPlayerMaxHP);
#else
INCLUDE_ASM(const s32, "dat", datCalcNowParam);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCalcRestParamPlySub);
#else
INCLUDE_ASM(const s32, "dat", datGetSword4Param);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCalcParamSub);
#else
INCLUDE_ASM(const s32, "dat", datGetSwordParamHP);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datGetSwordParamSub);
#else
INCLUDE_ASM(const s32, "dat", datGetSwordAisyoEx);
#endif

INCLUDE_ASM(const s32, "dat", datGetSwordAisyo);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCheckStrongSwordDevil);
#else
INCLUDE_ASM(const s32, "dat", fldGetRestoreMode);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCheckStrongSwordAttack);
#else
INCLUDE_ASM(const s32, "dat", datSetPlayerUnitSwordSub);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCallCheckStrongSwordAttack);
#else
INCLUDE_ASM(const s32, "dat", datSetPlayerUnitSword);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK3);
#else
INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK12);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCallBitCheck2);
#else
INCLUDE_ASM(const s32, "dat", datLeaf3);
#endif

INCLUDE_ASM(const s32, "dat", datCheckDying);

INCLUDE_ASM(const s32, "dat", datLeaf45);

INCLUDE_ASM(const s32, "dat", datTakumiKoukaDSub);

INCLUDE_ASM(const s32, "dat", datMoonAddMoveSub);

INCLUDE_ASM(const s32, "dat", sdatCalc_GetBadStatus);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK4);
#else
INCLUDE_ASM(const s32, "dat", cmpExSubMenuAnimFunc);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "dat", datCheckStrongSwordAttackTable);
#else
INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK13);
#endif

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK14);
#endif
INCLUDE_SDATA(const s32, "dat", sdfRuntimeTaskName);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", datCallBitCheckData3);
#else
INCLUDE_SDATA(const s32, "dat", datData13);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", datCallBitCheckData4);
#else
INCLUDE_SDATA(const s32, "dat", NewGame2PushData3);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", datData2);
#else
INCLUDE_SDATA(const s32, "dat", NewGame2PushData4);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", datData3);
#else
INCLUDE_SDATA(const s32, "dat", datData14);

INCLUDE_SDATA(const s32, "dat", datData15);

INCLUDE_SDATA(const s32, "dat", datData16);

INCLUDE_SDATA(const s32, "dat", datData17);

INCLUDE_SDATA(const s32, "dat", datCallNewGame2PushData);
#endif

INCLUDE_SDATA(const s32, "dat", DDS3_GBWK);
#ifdef VERSION_RAIDOU2

INCLUDE_SDATA(const s32, "dat", sbtlData141);

INCLUDE_SDATA(const s32, "dat", sbtlSbattle3Data);
#endif

INCLUDE_SDATA(const s32, "dat", evtCallMemset9Data);

INCLUDE_SDATA(const s32, "dat", datEnemyRecords);

INCLUDE_SDATA(const s32, "dat", evtData12);

INCLUDE_SDATA(const s32, "dat", datEncountCalcSubData);

INCLUDE_SDATA(const s32, "dat", datBattleAdjustmentRecords);

INCLUDE_SDATA(const s32, "dat", datLeaf66Data);

INCLUDE_SDATA(const s32, "dat", rstCallGetRandAData);

INCLUDE_SDATA(const s32, "dat", datData4);

INCLUDE_SDATA(const s32, "dat", sbtlSetHangekiFlagData);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", datLoadLvUpTablesData);
#else
INCLUDE_SDATA(const s32, "dat", datLoadLvUpTablesData3);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", datData5);
#else
INCLUDE_SDATA(const s32, "dat", datData18);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", datLoadLvUpTablesData2);
#else
INCLUDE_SDATA(const s32, "dat", datLoadLvUpTablesData4);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", rstCallBitCheck2Data);
#else
INCLUDE_SDATA(const s32, "dat", rstCalcRankUpData);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", rstCallBitCheck2Data2);
#else
INCLUDE_SDATA(const s32, "dat", datData19);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", datCallCheckStrongSwordAttackData);

INCLUDE_SDATA(const s32, "dat", cmpLoadModelTablesData);

INCLUDE_SDATA(const s32, "dat", datData6);

INCLUDE_SDATA(const s32, "dat", datCallGetStockDevilIDData);

#endif
