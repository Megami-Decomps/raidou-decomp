#include "common.h"
#include "dds3Basic.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"
#include "sBtlEff.h"
#include "sbtlDMG.h"
#include "sBtlCalc.h"
#include "sBtlSM.h"
#include "sBtlItem.h"
#include "sdatUnit.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "dds3KernelMain.h"
#include "fld.h"
#include "newdata.h"
#include "code_r1_0027CBE8.h"
#include "effStat.h"
#include "sBtlDvltk.h"

INCLUDE_ASM(const s32, "code_r1_0024AC78", sbtlSbattle2);

s32 sbtlSbattle3(void) {
    if (dds3SearchProcessName(sbtlData85) == NULL) {
        return 0;
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    wk_sbtl->flg |= 0x40;
    if (!(wk_sbtl->flg & 0x8000000)) {
        effCallStopStat();
        wk_sbtl->flg |= 0x8000000;
        sbtlCallEffectShutdown3(0);
    }
    return 1;
}

s32 sbtlCallBtlGetPlayerData(void)
{
    s32 ret;
    SBTL_PLAYER_DATA *player;

    ret = 0;
    player = sbtlGetPlayerData();
    if ((wk_sbtl->battleMode == 0 || wk_sbtl->battleMode == 2) && wk_sbtl->summonIndex[0] >= 0
        && wk_sbtl->stockNum > 0) {
        if (sbtlBtlNakamaSetUpSub(&player->stock[wk_sbtl->summonIndex[0]])) {
            ret = sbtlCheckMagCost(&player->stock[wk_sbtl->summonIndex[0]],
                                            player->stock[wk_sbtl->summonIndex[0]].devilId, 0) != 0;
        }
    }
    return ret;
}

s32 CheckStopBattle(void) {
    if (dds3SearchProcessName(sbtlData85) == NULL) {
        return 0;
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    return (wk_sbtl->flg & 0x100040) == 0x40;
}

s32 RestartBattle(void)
{
    if (dds3SearchProcessName(sbtlData85) == NULL) {
        return 0;
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    if (wk_sbtl->flg & 0x8000000) {
        effStopEffectStop();
        wk_sbtl->flg &= ~0x8000000;
    }
    wk_sbtl->flg &= ~0x40;
    return 1;
}

s32 sbtlCallBtlDeleteOnData(void) {
    sbtlFunc2();
    sbtlDeleteOnData(1);
    btlLoadSbPack();
    sbtlLoadOnMemMsg();
    sbtlForceDelete_EffectWork(2);
    sbtlForceDelete_AttackHitData();
    sbtlInitEffectWork();
    sbtlInitLoadAttackHitData_pData();
    sbtlBattleSBPACK();
    while (!sbtlLoadEndCheck_PlayerOnMemData()) {
    }
    while (!sbtlLoadPlayerOnMemData()) {
    }
    return 1;
}

s32 sbtlCallBtlMapInit(void)
{
    sdfMemHandle_t *handle;

    sbtlFunc2();
    sbtlMap_Init();
    btlLoadSbPack();
    handle = sdfMemAllocDirect(sizeof(SBTL_WORK));
    wk_sbtl = sdfMemLock(handle);
    memset(wk_sbtl, 0, sizeof(SBTL_WORK));
    wk_sbtl->handle = handle;
    wk_sbtl->mode = 1;
    handle = sdfMemAllocDirect(sizeof(SBTL_PACDATA_WORK));
    sbtlData84 = sdfMemLock(handle);
    memset(sbtlData84, 0, sizeof(SBTL_PACDATA_WORK));
    sbtlData84->handle = handle;
    handle = sdfMemAllocDirect(sizeof(SBTL_WORK2));
    sbtlData83 = sdfMemLock(handle);
    memset(sbtlData83, 0, sizeof(SBTL_WORK2));
    sbtlData83->handle = handle;
    sbtlInitEffectWork();
    sbtlInitLoadAttackHitData_pData();
    sbtlBattleSBPACK();
    while (!sbtlLoadEndCheck_PlayerOnMemData()) {
    }
    while (!sbtlLoadPlayerOnMemData()) {
    }
    return 1;
}

void sbtlMap_Init(void)
{
    memset(btlLoadSbPackData, 0, sizeof(btlLoadSbPackData));
}
