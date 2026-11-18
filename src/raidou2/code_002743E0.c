#include "common.h"
#include "dds3Basic.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3ObjectBase.h"
#include "mdlManager.h"
#include "dds3UnitObjectBasic.h"
#include "sdk_libc.h"
#include "sBtlUnit.h"
#include "sbtlWork.h"

INCLUDE_ASM(const s32, "code_002743E0", sbtlRefSbtl8);

INCLUDE_ASM(const s32, "code_002743E0", sbtlLeaf62);

INCLUDE_ASM(const s32, "code_002743E0", sbtlGetDevilData);

INCLUDE_ASM(const s32, "code_002743E0", sbtlRefSbtl34);

INCLUDE_ASM(const s32, "code_002743E0", sbtlRefSbtl35);

INCLUDE_ASM(const s32, "code_002743E0", sbtlGetHelperPos);

INCLUDE_ASM(const s32, "code_002743E0", sbtlDeleteHitHelper);

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetItem);

INCLUDE_RODATA(const s32, "code_002743E0", sbtlGetHelperPosTable);

INCLUDE_ASM(const s32, "code_002743E0", sbtlItemIDDrop);

INCLUDE_ASM(const s32, "code_002743E0", sbtlRefSbtl9);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallBtlGetNakama);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCheckSummonUnit_Dead_or_ALive);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCheckSummonUnit_Exist);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCheckSummonUnit);

INCLUDE_ASM(const s32, "code_002743E0", sbtlGetStockPoint);

INCLUDE_ASM(const s32, "code_002743E0", sbtlEntryStockData_Huuma);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallBitOn);

INCLUDE_ASM(const s32, "code_002743E0", sbtlHuumaParamUp);

struct SBTLUNIT_WORK *sbtlSetSummon(s32 major, s32 minor, s32 index)
{
    struct SBTLUNIT_WORK *wk;
    s32 id;

    wk = sbtlCreateUnit(major, minor, 2);
    id = sbtlEntryUnitList(wk_sbtl->unitList, wk);
    wk->GroupID = index;
    wk->field1A0 = id;
    wk->model->mode = 9;
    wk->model->stat = 1;
    wk->status = 0;
    sbtlResetAIPattern(wk, 0);
    wk->color = 0x808080;
    wk_sbtl->field424++;
    return wk;
}

INCLUDE_ASM(const s32, "code_002743E0", sbtlResetSummon);

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetSummon_sbtlUnit);

INCLUDE_ASM(const s32, "code_002743E0", SetReturn);

INCLUDE_ASM(const s32, "code_002743E0", sbtlResetSummon_E);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallBtlCheckDamageTarget);

void sbtlResetAction_Flag(struct SBTLUNIT_WORK *wk)
{
    if (wk_sbtl->battleMode != 3 && ((wk->field414 & 0x80) || (wk->field414 & 1))) {
        wk->model->flags &= ~0x4000;
        wk->flg &= ~0x4000;
    }
}

INCLUDE_ASM(const s32, "code_002743E0", ResetAction);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallBtlSearchUnitID20);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallBtlCheckDamageTarget2);

/* NON_MATCHING: 45 of 46 words match.  */
#ifdef NON_MATCHING
s32 sbtlCreateEnemyList(void)
{
    s32 i;
    struct SBTLUNIT_WORK *wk;

    sbtlClearUnitList(wk_sbtl->list2D4);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 2) && sbtlCheckDamageTarget(wk) != 0) {
            sbtlEntryUnitList(wk_sbtl->list2D4, wk);
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002743E0", sbtlCreateEnemyList);
#endif

/* NON_MATCHING: 70 of 72 words match.  */
#ifdef NON_MATCHING
struct SBTLUNIT_LISTWORK *sbtlCreateSideList(u8 side, s32 flg)
{
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLUNIT_WORK *wk;
    s32 mask;
    s32 i;

    mask = 0;
    lwk = sbtlCreateUnitList(48);
    switch (flg) {
    case 0:
        mask = side & 7;
        break;
    case 1:
        mask = (side & 2) ? 1 : 2;
        break;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (mask & wk->field414 & 0x3F) && CheckLiveTarget(wk) != 0 && !(wk->vstatus & 0x10000)) {
            sbtlEntryUnitList(lwk, wk);
        }
    }
    return lwk;
}
#else
INCLUDE_ASM(const s32, "code_002743E0", sbtlCreateSideList);
#endif

INCLUDE_ASM(const s32, "code_002743E0", sbtlCreateSideList_sbtlUnit);

INCLUDE_ASM(const s32, "code_002743E0", SetMotion_Core);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCheckLoopMotion);

INCLUDE_ASM(const s32, "code_002743E0", sbtlPlayerDirectDmg);

INCLUDE_ASM(const s32, "code_002743E0", sbtlClrCtrl);

INCLUDE_RODATA(const s32, "code_002743E0", sbtlStrZanbatou);

INCLUDE_RODATA(const s32, "code_002743E0", sbtlPlayerDirectDmgTable);

INCLUDE_ASM(const s32, "code_002743E0", Proc_sbtlUnit);

INCLUDE_ASM(const s32, "code_002743E0", AllDeleteEnemy);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCheckUnit_Exec_R);

s32 sbtlCheckUnit_Exec(void)
{
    struct SBTLUNIT_LISTWORK *lwk = wk_sbtl->unitList;
    struct SBTLUNIT_WORK *wk;
    s32 i;

    if (lwk == NULL) {
        return 1;
    }
    for (i = 0; i < lwk->number; i++) {
        wk = lwk->pUnitL[i].pUnit;
        if (wk != NULL && (u16)(wk->mode - 1) < 2) {
            return 0;
        }
    }
    return 1;
}

s32 sbtlModelDataLoad(dds3ProcessID_t *id)
{
#ifdef FIX_BUGS
    return 0;
#endif
}

INCLUDE_ASM(const s32, "code_002743E0", sbtlModelLbBattle);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallUnitRegist);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallBtlSetModelDataLoadEnd);

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetModelDataLoadEnd);

INCLUDE_ASM(const s32, "code_002743E0", CheckLoadData);

INCLUDE_ASM(const s32, "code_002743E0", sbtlGetLight_SetPos);

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetLight_SetPos);

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetUnitLight_Dir);

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetUnitLight);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallSetUnitLightAndAmbient3);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallToMatrix);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallMemset3);

INCLUDE_ASM(const s32, "code_002743E0", MotinProc);

s32 sbtlReleaseUnitList_ID(struct SBTLUNIT_LISTWORK *lwk, s32 id)
{
    lwk->pUnitL[id].ID = 0;
    lwk->pUnitL[id].pUnit = NULL;
    lwk->count--;
    return lwk->pUnitL[id].ID;
}

u32 UnitRegist(MODELTYPE major, s32 minor)
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    u32 id;
    dds3Basic_t *basic;
    mdlObj_t *model;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    id = dds3AllocObjectId();
    basic = dds3AddUnitObjectBasic(id, &pos, &rot);
    dds3ObjectBaseOnFlag(basic, 32);
    dds3GetRadiusPoseSet(basic, 10.0f);
    dds3SetActiveUnitSceneBasic(dds3GetControlScene(), basic);
    dds3ObjectBaseMakeUnit(basic, major, minor);
    model = dds3ObjectBaseGetModelHandle(basic);
    if (model->first != NULL) {
        mdlLoopMotion(dds3ObjectBaseGetModelHandle(basic), 0, 0);
    }
    dds3ObjectBaseSetPadControl(basic, NULL);
    return id;
}

INCLUDE_ASM(const s32, "code_002743E0", GetCommListCnt);

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetPlayerData);

INCLUDE_ASM(const s32, "code_002743E0", sbtlGetPlayer_HitTarget);

INCLUDE_ASM(const s32, "code_002743E0", sbtlIsDeadBgn);

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetRunAway);

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetReturnStock_Enemy);

INCLUDE_ASM(const s32, "code_002743E0", sbtlBgnWait);

INCLUDE_ASM(const s32, "code_002743E0", sbtlEndWait);

INCLUDE_ASM(const s32, "code_002743E0", sbtlBgnWaitALL);

INCLUDE_ASM(const s32, "code_002743E0", sbtlBgnWait_TimeStopSkill);

INCLUDE_ASM(const s32, "code_002743E0", sbtlBgnWaitALL_TimeStopSkill);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallBtlGetBadStatus);

INCLUDE_ASM(const s32, "code_002743E0", sbtlEndWaitALL);

INCLUDE_ASM(const s32, "code_002743E0", sbtlChkHaveMemSkl);

INCLUDE_ASM(const s32, "code_002743E0", sbtlChkHaveMemSklParty);

INCLUDE_ASM(const s32, "code_002743E0", sbtlGetKajaParamEx);

INCLUDE_RODATA(const s32, "code_002743E0", sbtlCallBtlCreateUnitTable);

INCLUDE_RODATA(const s32, "code_002743E0", GetCommListCntTable);

