#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "basic.h"
#include "datCalc.h"
#include "newdata.h"
#include "sdk_libc.h"
#include "dds/btl.h"
#include "dds/dds3_path.h"
#include "dds/dds3_task_create.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/dds3obj.h"
#include "dds/eff_random_mod.h"
#include "dds/eff_transform.h"
#include "dds/mdl_flag_api.h"
#include "dds/sdf_dev_state.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdk_libc.h"

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

extern void *memcpy(void *, const void *, unsigned int);

/* Allocate from the chip heap through 0x400 bytes; larger requests use the
 * general heap and return its locked payload address. */
void *sdfAllocAuto(s32 size);
void sdfFileClose(sdfFileCmdHandle_t *state);
u32 sdfFileLength(sdfFileCmdHandle_t *state);

/* Stable state APIs. Callback factories keep caller-specific callback types. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path);
void sdfFileRead(sdfFileCmdHandle_t *state, void *buffer, s32 byteCount);

/* Release the sdfAllocAuto base in its source heap; interior pointers are
 * invalid. */
s32 sdfFreeAuto(void *data);
struct sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 requestedBytes);
void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);
void *sdfMemLock(struct sdfMemHandle_t *allocation);

#include "dds/sdk_ps2.h"
#include "dds/evt_run_context_api.h"
#include "dds/common.h"
#include "dds/scr.h"
#include "dds/sdf_vu0.h"
#include "dds/sdf_channel.h"
#include "dds/scr_work_task_api.h"
#include "dds/dat_save_words.h"
#include "dds/sdf_float_counter.h"
#include "dds/mnu_flag_snapshot.h"
#include "dds/evt_world_source_transform.h"
#include "dds/pcp_vu0.h"
#include "dds/btl_action.h"
#include "dds/dat_state.h"
#include "dds/dat_affinity_selector.h"
#include "dds/dat_effective_affinity_api.h"
#include "dds/dat_world_field_flags.h"
#include "dds/dat_unit_has_skill_api.h"
#include "dds/evt_randomize_entry_api.h"
#include "dds/dat_command.h"
#include "dds/sdf.h"
#include "dds/fld_runtime_reset_api.h"

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

#include "dds/sdf_debug_api.h"

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

INCLUDE_ASM(const s32, "dat", datCallGlobalWorkClearFlags);

void datGlobalWorkDestroySub(void) {
}

INCLUDE_ASM(const s32, "dat", datCallBitCheck);

INCLUDE_ASM(const s32, "dat", NewGame2Pop);

INCLUDE_ASM(const s32, "dat", dds3GlobalWorkUserInit2);

void datLeaf42(void) {
}

void datLeaf43(void) {
}

INCLUDE_ASM(const s32, "dat", datAllReleaseScriptSub);

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

INCLUDE_ASM(const s32, "dat", datFunc24);

INCLUDE_ASM(const s32, "dat", datCallMemset);

INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK2);

INCLUDE_ASM(const s32, "dat", datGetStockDevilID);

INCLUDE_ASM(const s32, "dat", effCloneParticleSharedResource);

INCLUDE_ASM(const s32, "dat", datGetEmptyStock);

INCLUDE_ASM(const s32, "dat", datCheckSameDevil);

INCLUDE_ASM(const s32, "dat", datCallGetStockDevilID);

INCLUDE_ASM(const s32, "dat", datGetStockDevilNums);

INCLUDE_ASM(const s32, "dat", datCreateDevilUnitExSub);

INCLUDE_ASM(const s32, "dat", datCheckJidouSkillPrioSub);

INCLUDE_ASM(const s32, "dat", datFunc4);

INCLUDE_ASM(const s32, "dat", datLeaf44);

INCLUDE_ASM(const s32, "dat", CheckJidouSkillPrio);

INCLUDE_ASM(const s32, "dat", datCheckSkillEnable);

INCLUDE_ASM(const s32, "dat", datGetSkillCopyIndex);

INCLUDE_ASM(const s32, "dat", datReplaceHaveSkill);

INCLUDE_ASM(const s32, "dat", datAddSkill);

INCLUDE_ASM(const s32, "dat", datCreateDevilUnitEx);

INCLUDE_ASM(const s32, "dat", datCreateDevilUnit);

INCLUDE_ASM(const s32, "dat", datAddCheckDevilUnit);

INCLUDE_ASM(const s32, "dat", datCallAddCheckDevilUnit);

INCLUDE_ASM(const s32, "dat", datCallCreateDevilUnitEx);

INCLUDE_ASM(const s32, "dat", datAddDevil);

INCLUDE_ASM(const s32, "dat", datCallGetStockDevilID2);

INCLUDE_ASM(const s32, "dat", datLoadLvUpTables);

INCLUDE_ASM(const s32, "dat", cmpLoadModelTables);

INCLUDE_ASM(const s32, "dat", datGetPlayerExp);

INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpNums);

INCLUDE_ASM(const s32, "dat", datGetPlayerMaxHP);

INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpParamSub);

INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpParam);

INCLUDE_ASM(const s32, "dat", datGetPlayerLevelUpUnit);

INCLUDE_ASM(const s32, "dat", datGetDevilLevelUpNumsSub);

INCLUDE_ASM(const s32, "dat", datGetDevilLevelUpNums);

INCLUDE_ASM(const s32, "dat", datGetDevilMaxHP);

INCLUDE_ASM(const s32, "dat", datGetDevilMaxMP);

INCLUDE_ASM(const s32, "dat", datCallGetRandA);

INCLUDE_ASM(const s32, "dat", datGetDevilLevelUpParam);

INCLUDE_ASM(const s32, "dat", datGetDevilLevelUpUnit);

INCLUDE_ASM(const s32, "dat", datCallGetPlayerMaxHP);

INCLUDE_ASM(const s32, "dat", datCalcRestParamPlySub);

INCLUDE_ASM(const s32, "dat", datCalcParamSub);

INCLUDE_ASM(const s32, "dat", datGetSwordParamSub);

INCLUDE_ASM(const s32, "dat", datGetSwordAisyo);

INCLUDE_ASM(const s32, "dat", datCheckStrongSwordDevil);

INCLUDE_ASM(const s32, "dat", datCheckStrongSwordAttack);

INCLUDE_ASM(const s32, "dat", datCallCheckStrongSwordAttack);

INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK3);

INCLUDE_ASM(const s32, "dat", datCallBitCheck2);

INCLUDE_ASM(const s32, "dat", datCheckDying);

INCLUDE_ASM(const s32, "dat", datLeaf45);

INCLUDE_ASM(const s32, "dat", datTakumiKoukaDSub);

INCLUDE_ASM(const s32, "dat", datMoonAddMoveSub);

INCLUDE_ASM(const s32, "dat", sdatCalc_GetBadStatus);

INCLUDE_ASM(const s32, "dat", datRefDDS3GBWK4);

INCLUDE_RODATA(const s32, "dat", datCheckStrongSwordAttackTable);

INCLUDE_SDATA(const s32, "dat", sdfRuntimeTaskName);

INCLUDE_SDATA(const s32, "dat", datCallBitCheckData3);

INCLUDE_SDATA(const s32, "dat", datCallBitCheckData4);

INCLUDE_SDATA(const s32, "dat", datData2);

INCLUDE_SDATA(const s32, "dat", datData3);

INCLUDE_SDATA(const s32, "dat", DDS3_GBWK);

INCLUDE_SDATA(const s32, "dat", evtCallMemset9Data);

INCLUDE_SDATA(const s32, "dat", datEnemyRecords);

INCLUDE_SDATA(const s32, "dat", evtData12);

INCLUDE_SDATA(const s32, "dat", datEncountCalcSubData);

INCLUDE_SDATA(const s32, "dat", datBattleAdjustmentRecords);

INCLUDE_SDATA(const s32, "dat", datLeaf66Data);

INCLUDE_SDATA(const s32, "dat", rstCallGetRandAData);

INCLUDE_SDATA(const s32, "dat", datData4);

INCLUDE_SDATA(const s32, "dat", sbtlSetHangekiFlagData);

INCLUDE_SDATA(const s32, "dat", datLoadLvUpTablesData);

INCLUDE_SDATA(const s32, "dat", datData5);

INCLUDE_SDATA(const s32, "dat", datLoadLvUpTablesData2);

INCLUDE_SDATA(const s32, "dat", rstCallBitCheck2Data);

INCLUDE_SDATA(const s32, "dat", rstCallBitCheck2Data2);

INCLUDE_SDATA(const s32, "dat", datCallCheckStrongSwordAttackData);

INCLUDE_SDATA(const s32, "dat", cmpLoadModelTablesData);

INCLUDE_SDATA(const s32, "dat", datData6);

INCLUDE_SDATA(const s32, "dat", datCallGetStockDevilIDData);

