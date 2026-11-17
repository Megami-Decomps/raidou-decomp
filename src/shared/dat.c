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
#include "dds3KernelCore.h"
#include "EventBit.h"
#include "fld.h"
#include "scr.h"
#include "sBtlDvltk.h"
#include "sbtlWork.h"
#include "sBtlSkill.h"
#include "sBtlCalc.h"

extern u32 datCallCheckStrongSwordAttackData;
extern u32 cmpLoadModelTablesData;
extern u32 datData6;
extern u32 datCallGetStockDevilIDData;

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
typedef struct sdfChannelEntry_t {
    void **slot;
    u32 unused;
} sdfChannelEntry_t;

/* The channel loader scans these entries through the first null slot. */
typedef struct sdfChannel_t {
    const char *path;
    sdfChannelEntry_t entries[20];
} sdfChannel_t;

typedef char sdfChannelEntry_size_must_be_8[(sizeof(sdfChannelEntry_t) == 8) ? 1 : -1];

typedef char sdfChannel_size_must_be_0xA4[(sizeof(sdfChannel_t) == 0xA4) ? 1 : -1];

#define SDF_PACKED_STATUS_BIT 0x8000

#define SDF_PACKED_CHANNEL_MASK 0x7fff

#define SDF_UNIT_ENEMY 0x20

extern char dds3GlobalWorkProcName[];

#define SDF_FLOAT_COUNTER_FROZEN_FLAG 0x00000008

#define SDF_FLOAT_COUNTER_WRAP_FLAG 0x00000020

void *dds3GetWorldNodeData(dds3Basic_t *node);
void evtLoadValueVectorIntoVu(dds3Basic_t *node);
void evtLoadValueSecondaryVectorIntoVu(dds3Basic_t *node);

extern const char sdfStrSys[10];
void sdfReloadChannelSegments(sdfChannel_t *table);

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

void dds3GlobalWorkInit(void) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(sizeof(dds3GlobalWork_t));
    dds3GlobalWork_t *work = sdfMemLock(handle);

    memset(work, 0, sizeof(dds3GlobalWork_t));
    work->memHandle = handle;
    work->unk1274 = 0;
    work->unk1278 = 0;
    dds3StartProcess(dds3GlobalWorkProcName, DDS3_PRIORITY_GLOBALWORK, 0, 0, dds3GlobalWorkMainProc, NULL, work);
    DDS3_GBWK = work;
    UserInit();
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/kernel/dds3GlobalWork.c:179, near, same function 0.92 (words 20/20, calls 0.75, shape 1.00) */
void datGlobalWorkDestroy(void)
{
    sdfMemHandle_t *handle;

    dds3KillProcessName(dds3GlobalWorkProcName, 0);
    datGlobalWorkDestroySub();
    handle = DDS3_GBWK->memHandle;
    sdfMemUnlock(handle);
    sdfMemFreeDirect(handle);
    DDS3_GBWK = NULL;
}

s32 dds3GlobalWorkMainProc(dds3ProcessID_t *id) {
    DDS3_GBWK->unk1274++;
    DDS3_GBWK->unk1278++;
    return 0;
}

#ifdef VERSION_RAIDOU1
void UserInit(void) {
    dds3GlobalWorkClearFlags();
    evtInitGlobalWork();
    datCallMemset();
    fldGbInit_System(0);
    datLoadLvUpTables();
    cmpLoadModelTables();
    sbtlLoadOnMemMsg();
}
#else
INCLUDE_ASM(const s32, "dat", UserInit);
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
void dds3GlobalWorkUserInit2(s32 mode) {
    sdfMemHandle_t *handle;

    if (mode == 1) {
        datCallBitCheck();
        handle = DDS3_GBWK->memHandle;
        memset(DDS3_GBWK, 0, sizeof(dds3GlobalWork_t));
        DDS3_GBWK->memHandle = handle;
    }
    DDS3_GBWK->unk1274 = 0;
    DDS3_GBWK->unk1278 = 0;
    dds3GlobalWorkClearFlags();
    evtInitGlobalWork();
    datCallMemset();
    fldGbInit_System(0);
    if (mode == 1) {
        NewGame2Pop();
    }
}
#else
INCLUDE_ASM(const s32, "dat", datLeaf);

INCLUDE_ASM(const s32, "dat", datLeaf2);

INCLUDE_ASM(const s32, "dat", datCallNewGame2Push);
#endif

void datLeaf42(void) {
}

void datLeaf43(void) {
}

s32 datAllReleaseScriptSub(void) {
    return 0;
}

#ifdef VERSION_RAIDOU1
/* Load a table's file and copy each size-prefixed segment into its own block. */
void sdfReloadChannelSegments(sdfChannel_t *table) {
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
void datCallMemset(void) {
    s16 *item;
    dds3GlobalWork_t *gbwk;
    s32 i;

    memset(&DDS3_GBWK->playerData, 0, sizeof(SBTL_PLAYER_DATA));
    gbwk = DDS3_GBWK;
    gbwk->playerData.unit.level = 1;
    gbwk->playerData.unit.flag |= 3;
    DDS3_GBWK->playerData.mag = 1000;
    DDS3_GBWK->playerData.val24 = 1000;
    DDS3_GBWK->playerData.maka = 2000;
    DDS3_GBWK->playerData.swordAtk = 10;
    item = DDS3_GBWK->playerData.item;
    for (i = 255; i >= 0; i--) {
        *item++ = 0;
    }
    sbtlInitPlayerData();
}
#else
INCLUDE_ASM(const s32, "dat", datInitPlayer);

INCLUDE_ASM(const s32, "dat", datGetPlayerParam);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK2);
#else
INCLUDE_ASM(const s32, "dat", datSetPlayerParam);
#endif

#ifdef VERSION_RAIDOU1
s32 datGetStockDevilID(s32 index) {
    sdatUnitWork_t *stock = &DDS3_GBWK->playerData.stock[index];

    if (stock->flag & 1) {
        return stock->devilId;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK10);

INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK11);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", effCloneParticleSharedResource);
#else
INCLUDE_ASM(const s32, "dat", datAdjustStock);
#endif

INCLUDE_ASM(const s32, "dat", datGetEmptyStock);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "dat", datCheckSameDevil);

s32 datCallGetStockDevilID(u32 id) {
    s32 i;
    s32 j;

    for (i = 0; i < DDS3_GBWK->playerData.partyNum; i++) {
        if (datGetStockDevilID(i) == id) {
            return i;
        }
    }
    if (datCheckSameDevil(id) >= 0) {
        for (i = 0; i < datCallGetStockDevilIDData / 2; i++) {
            for (j = 0; j < DDS3_GBWK->playerData.partyNum; j++) {
                if (datGetStockDevilID(j) == EE_ADDR_AS(u16 *, datData6)[i]) {
                    return j;
                }
            }
        }
    }
    return -1;
}
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
s32 datCheckJidouSkillPrioSub(SKLID skill) {
    if (sbtlCallBtlDrawRedGgData[skill].type & 0x1000) {
        return 2;
    }
    return (u32)(skill - 240) < 48;
}
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

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 21 of 78 words match. Branch layout: retail tests prio == 1 first but lays the prio == 2 block inline and returns 0 through bnel. */
#ifdef NON_MATCHING
s32 datCheckSkillEnable(sdatUnitWork_t *unit, SKLID skill) {
    s32 prio;
    s32 ret;

    if (skill == 0) {
        return 0;
    }
    prio = datCheckJidouSkillPrioSub(skill);
    switch (prio) {
    case 2:
        if (unit->flag & 8) {
            return 1;
        }
        if (unit->level < gohData7[unit->devilId].level + 1) {
            return 0;
        }
        unit->flag |= 8;
        return 1;
    case 1:
        ret = CheckJidouSkillPrio(unit, skill);
        if (ret == 1) {
            if (unit->flag & 0x10) {
                return ret;
            }
        } else if (ret == 2) {
            if (unit->flag & 0x20) {
                return 1;
            }
        }
        if (unit->level < gohData7[unit->devilId].level + 2) {
            return 0;
        }
        unit->flag |= 0x30;
        return 1;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "dat", datCheckSkillEnable);
#endif
#else
INCLUDE_ASM(const s32, "dat", datCheckSkillEnable);
#endif

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

#ifdef VERSION_RAIDOU1
void datLoadLvUpTables(void) {
    sdfDevLoadFileEx("/data/sdatLvUpDevilGift.dat", &sdatLvUpDevilGiftTbl, &sdatLvUpDevilGiftSize);
    sdfDevLoadFileEx("/data/sdatLvUpDevilParamUp.dat", (u32 *)&sdatLvUpDevilParamUp, &sdatLvUpDevilParamUpSize);
    sdfDevLoadFileEx("/data/sdatLvUpPlayerParamUp.dat", &sdatLvUpPlayerParamUp, &sdatLvUpPlayerParamUpSize);
    sdfDevLoadFileEx("/data/sdatLvUpPlayerRankUp.dat", &sdatLvUpPlayerRankUp, &sdatLvUpPlayerRankUpSize);
}
#else
INCLUDE_ASM(const s32, "dat", datLoadLvUpTables);
#endif

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

s32 datCheckDying(sdatUnitWork_t *pUnit) {
    return pUnit->now.hp * 100 / pUnit->now.maxhp < 25;
}

#ifdef VERSION_RAIDOU1
s32 datLeaf45(sdatUnitWork_t *acter, sdatUnitWork_t *unit, SKLID sklid) {
    return sbtlGetDamage_GB(acter, unit, sklid);
}
#else
INCLUDE_ASM(const s32, "dat", datLeaf45);
#endif

#ifdef VERSION_RAIDOU1
s32 datTakumiKoukaDSub(sdatUnitWork_t *unit, s32 itemID) {
    return sbtlCallBtlGetItemtbl3(unit, itemID);
}
#else
INCLUDE_ASM(const s32, "dat", datTakumiKoukaDSub);
#endif

s32 datMoonAddMoveSub(void) {
    return sbtlCheckDrink();
}

s32 sdatCalc_GetBadStatus(sdatUnitWork_t *unit) {
    return sbtlGetBadStatus(unit);
}

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
INCLUDE_SDATA(const s32, "dat", dds3GlobalWorkProcName);

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

INCLUDE_SDATA(const s32, "dat", sdatLvUpDevilGiftTbl);

INCLUDE_SDATA(const s32, "dat", sdatLvUpDevilGiftSize);

INCLUDE_SDATA(const s32, "dat", sdatLvUpDevilParamUp);

INCLUDE_SDATA(const s32, "dat", sdatLvUpDevilParamUpSize);

INCLUDE_SDATA(const s32, "dat", sdatLvUpPlayerParamUp);

INCLUDE_SDATA(const s32, "dat", sdatLvUpPlayerParamUpSize);

INCLUDE_SDATA(const s32, "dat", sdatLvUpPlayerRankUp);

INCLUDE_SDATA(const s32, "dat", sdatLvUpPlayerRankUpSize);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "dat", datCallCheckStrongSwordAttackData);

INCLUDE_SDATA(const s32, "dat", cmpLoadModelTablesData);

INCLUDE_SDATA(const s32, "dat", datData6);

INCLUDE_SDATA(const s32, "dat", datCallGetStockDevilIDData);

#endif
