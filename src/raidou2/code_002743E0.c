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
#include "sBtlItem.h"
#include "ps2asm.h"
#include "evtUnitManager.h"
#include "sbtlActAI.h"
#include "sdfQuat.h"
#include "sBtlCalc.h"

INCLUDE_ASM(const s32, "code_002743E0", sbtlRefSbtl8);

INCLUDE_ASM(const s32, "code_002743E0", sbtlLeaf62);

INCLUDE_ASM(const s32, "code_002743E0", sbtlGetDevilData);

INCLUDE_ASM(const s32, "code_002743E0", sbtlRefSbtl34);

INCLUDE_ASM(const s32, "code_002743E0", sbtlRefSbtl35);

INCLUDE_ASM(const s32, "code_002743E0", sbtlGetHelperPos);

INCLUDE_ASM(const s32, "code_002743E0", sbtlDeleteHitHelper);

s32 sbtlSetItem(struct SBTLUNIT_WORK *wk)
{
    SBTL_DEVIL_DATA *data = sbtlIfDrawActionSub(wk->unit.devilId);
    s32 i;

    for (i = 0; i < 4; i++) {
        sbtlItemIDDrop(&data->item[i], &wk->pos);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "code_002743E0", sbtlGetHelperPosTable);

INCLUDE_ASM(const s32, "code_002743E0", sbtlItemIDDrop);

INCLUDE_ASM(const s32, "code_002743E0", sbtlRefSbtl9);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallBtlGetNakama);

s32 sbtlCheckSummonUnit_Dead_or_ALive(s32 idx) {
    struct SBTLUNIT_WORK *wk;
    u32 status;

    if (wk_sbtl->member_num < 1) {
        return 2;
    }
    wk = sbtlGetNakama(idx);
    if (wk != NULL) {
        status = wk->status;
        if (status == 10 || status == 40) {
            return 0;
        }
        return 2;
    }
    return 2;
}

INCLUDE_ASM(const s32, "code_002743E0", sbtlCheckSummonUnit_Exist);

/* NON_MATCHING: 33 of 40 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckSummonUnit(void) {
    s32 i;
    u8 flg0;
    u8 flg1;
    s32 result = wk_sbtl->field64C < 1;

    if (wk_sbtl->member_num > 0) {
        flg0 = wk_sbtl->summonflg[wk_sbtl->summonIndex[0]];
        if (flg0 == 1) {
            result = 3;
        } else {
            flg1 = wk_sbtl->summonflg[wk_sbtl->summonIndex[1]];
            if (flg1 == 1) {
                result = 3;
            } else if (flg0 == 2 && flg1 == 2) {
                result = 2;
            }
        }
    } else {
        for (i = 0; i < 20; i++) {
            if (wk_sbtl->summonflg[i] != 0) {
                result = 3;
            }
        }
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_002743E0", sbtlCheckSummonUnit);
#endif

INCLUDE_ASM(const s32, "code_002743E0", sbtlGetStockPoint);

INCLUDE_ASM(const s32, "code_002743E0", sbtlEntryStockData_Huuma);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallBitOn);

INCLUDE_ASM(const s32, "code_002743E0", sbtlHuumaParamUp);

struct SBTLUNIT_WORK *sbtlSetSummon(s32 major, s32 minor, s32 index, s32 flg)
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

/* NON_MATCHING: 70 of 72 words match.  */
#ifdef NON_MATCHING
struct SBTLUNIT_LISTWORK *sbtlCreateSideList_sbtlUnit(u8 side, s32 flg)
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
        if (wk != NULL && (mask & wk->field414 & 0x3F) && sbtlCheckDamageTarget(wk) != 0 && !(wk->vstatus & 0x10000)) {
            sbtlEntryUnitList(lwk, wk);
        }
    }
    return lwk;
}
#else
INCLUDE_ASM(const s32, "code_002743E0", sbtlCreateSideList_sbtlUnit);
#endif

/* NON_MATCHING: 52 of 58 words match.  */
#ifdef NON_MATCHING
s32 SetMotion_Core(struct SBTLUNIT_WORK *wk) {
    s32 next = wk->next_motionno;

    if (wk->motionno == next && sbtlCheckLoopMotion(wk, wk->motionID)) {
        if (wk->hModel != NULL) {
            wk->hModel->first->speed = wk->m_rate;
        }
        return 1;
    }
    wk->motionno = wk->next_motionno;
    wk->motionID = wk->next_motionID;
    if (wk->hModel != NULL) {
        wk->hModel->first->speed = wk->m_rate;
        sbtlPlayViceSE(wk, wk->motionno, wk->beforeframe);
        if (sbtlCheckLoopMotion(wk, wk->motionID)) {
            mdlSmoothLoopMotion(wk->hModel, 0, wk->motionno, wk->beforeframe, wk->beforeframe);
        } else {
            mdlSmoothSingleMotion(wk->hModel, 0, wk->motionno, wk->beforeframe, wk->beforeframe);
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002743E0", SetMotion_Core);
#endif

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

void sbtlCallBtlSetModelDataLoadEnd(struct SBTLUNIT_WORK *wk) {
    switch (wk->unit.devilId) {
    case 500:
    case 501:
    case 630:
    case 654:
    case 655:
    case 656:
    case 670:
    case 674:
    case 675:
    case 677:
    case 678:
    case 679:
        wk->gstat = 4;
        break;
    default:
        wk->gstat = 5;
        if (!(wk_sbtl->flg & 0x10000)) {
            sbtlSetModelDataLoadEnd(wk);
        }
        break;
    }
    wk_sbtl->flg &= ~0x20000000;
}

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetModelDataLoadEnd);

INCLUDE_ASM(const s32, "code_002743E0", CheckLoadData);

s32 sbtlGetLight_SetPos(struct SBTLUNIT_WORK *wk, s8 lightno)
{
    if (wk->light_buff[0] == lightno) {
        return 1;
    }
    if (wk->light_buff[1] == lightno) {
        return 2;
    }
    return 3;
}

s32 sbtlSetLight_SetPos(struct SBTLUNIT_WORK *wk, s8 lightno)
{
    s8 no = sbtlGetLight_SetPos(wk, lightno);

    if (no >= 3) {
        no = wk->field4F3;
        if (++wk->field4F3 >= 3) {
            wk->field4F3 = 1;
        }
    }
    wk->light_buff[no - 1] = lightno;
    return no;
}

s32 sbtlSetUnitLight_Dir(struct SBTLUNIT_WORK *wk, const sdfVector4_t *dir, s32 flg)
{
    s32 idx = 0;

    if (flg == 4) {
        if (!(wk->vstatus & 0x40000)) {
            idx = 1;
        } else if (!(wk->vstatus & 0x80000)) {
            idx = 2;
        }
    } else {
        idx = flg;
    }
    if (idx >= 4) {
        return 0;
    }
    if (wk->flg & 0x800) {
        return 0;
    }
    switch (idx) {
    case 1:
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_STOREVECTOR_NOMEM(&wk->light);
        evtSetUnitLightDir(wk->hEveUnit, 0);
        break;
    case 2:
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_STOREVECTOR_NOMEM(&wk->light.dir1);
        evtSetUnitLightDir1(wk->hEveUnit, 1);
        break;
    case 3:
    default:
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_STOREVECTOR_NOMEM(&wk->light.dir2);
        evtSetUnitLightDir2(wk->hEveUnit, 2);
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetUnitLight);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallSetUnitLightAndAmbient3);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallToMatrix);

s32 sbtlCheckMotionMove(struct SBTLUNIT_WORK *wk)
{
    sdfVector4_t dir = { 0.0f, 0.0f, -1.0f, 0.0f };
    sdfVector4_t pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    f32 move;
    f32 frame;

    if (wk_sbtl->fieldCA2 && wk == sbtlGetPChara()) {
        return 0;
    }
    if (wk->flg & SBTLUNIT_FLG_KAIHIMUTEKI) {
        return 0;
    }
    if (wk->hModel == NULL) {
        return 0;
    }
    frame = wk->hModel->first->frame;
    wk->field415 = 0;
    move = sbtlGetMotionMove(wk, frame);
    if (move != 0.0f) {
        SDF_VU0_LOADVECTOR(&wk->rot);
        ToMatrix();
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_SCALEVECTOR(move);
        SDF_VU0_LOADVECTOR_B(&wk->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&pos);
        if (!(wk->flg & SBTLUNIT_FLG_NODMGROT) && !sbtlCheckMapHit_Cross(&wk->pos, &pos, move) &&
            (wk->unit.devilId != 0 || (wk->next_motionno != 41 && wk->next_motionno != 42))) {
            return 0;
        }
        SDF_VU0_LOADVECTOR(&pos);
        SDF_VU0_STOREVECTOR(&wk->pos);
        wk->field415 = 1;
    }
    return 1;
}

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

s32 GetCommListCnt(s32 talk, s32 type) {
    s32 cntTalk[4] = {1, 2, 3, 0};
    s32 cnt[4] = {0, 4, 5, 6};

    if (talk == 0) {
        return cnt[type];
    }
    return cntTalk[type];
}

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetPlayerData);

INCLUDE_ASM(const s32, "code_002743E0", sbtlGetPlayer_HitTarget);

s32 sbtlIsDeadBgn(struct SBTLUNIT_WORK *wk) {
    if ((wk->field414 & 2) && wk->status == 9 && (wk->flg2 & 0x10)) {
        return 0;
    }
    return wk->unit.now.hp < 1;
}

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetRunAway);

INCLUDE_ASM(const s32, "code_002743E0", sbtlSetReturnStock_Enemy);

INCLUDE_ASM(const s32, "code_002743E0", sbtlBgnWait);

INCLUDE_ASM(const s32, "code_002743E0", sbtlEndWait);

INCLUDE_ASM(const s32, "code_002743E0", sbtlBgnWaitALL);

INCLUDE_ASM(const s32, "code_002743E0", sbtlBgnWait_TimeStopSkill);

INCLUDE_ASM(const s32, "code_002743E0", sbtlBgnWaitALL_TimeStopSkill);

INCLUDE_ASM(const s32, "code_002743E0", sbtlCallBtlGetBadStatus);

INCLUDE_ASM(const s32, "code_002743E0", sbtlEndWaitALL);

s32 sbtlChkHaveMemSkl(sdatUnitWork_t *unit, SKLID id) {
    s16 badst;
    s16 power;
    SKLID found = 0;
    SKLID skl;
    s32 i;

    if (unit == NULL) {
        return 0;
    }
    {
        unit->autost_cnt = 0;
        unit->autost_cnt2 = 0;
        for (i = 0; i < 6; i++) {
            skl = sbtlGetAutoSkill_ID(unit, i);
            if (skl != 0 && id == sbtlGetAutoSkillPara(skl, &badst, &power)) {
                found = skl;
                unit->autost_cnt += badst;
                unit->autost_cnt2 += power;
            }
        }
    }
    return found;
}

INCLUDE_ASM(const s32, "code_002743E0", sbtlChkHaveMemSklParty);

/* NON_MATCHING: 23 of 24 words match.  */
#ifdef NON_MATCHING
f32 sbtlGetKajaParamEx(struct SBTLUNIT_WORK *wk, s32 type, s32 *count) {
    s32 i;
    s32 n;
    f32 para;
    f32 result = 0.0f;

    if (wk != NULL && (u32)type < 3) {
        n = 0;
        para = 0.0f;
        for (i = 0; i < 2; i++) {
            if (wk->kaja[type][i].m_cnt != 0) {
                n++;
                para += wk->kaja[type][i].m_fPara;
            }
        }
        if (count != NULL) {
            *count = n;
        }
        result = para;
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_002743E0", sbtlGetKajaParamEx);
#endif

