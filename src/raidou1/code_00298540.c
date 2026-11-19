#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "dds3Basic.h"
#include "dds3obj.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sdfDifficulty.h"
#include "sBtlUnit.h"
#include "sBtlEff.h"
#include "sBtlSound.h"
#include "sBtlScript.h"
#include "sbtlDMG.h"
#include "sBtlCalc.h"
#include "sBtlSM.h"
#include "sBtlItem.h"
#include "sdatUnit.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "mdlManager.h"
#include "code_r1_0021CE60.h"
#include "sdfDelayedResrc.h"
#include "fileManager.h"
#include "sdfFile.h"
#include "dds3Randomize.h"
#include "sdfSound.h"
#include "sbtlSub.h"
#include "sbtlSub_0027ED08.h"
#include "sdfQuat.h"
#include "sBtlSkill.h"
#include "sBtlAI.h"
#include "sBtlDvltk.h"
#include "sbtlActAI.h"
#include "evtUnitManager.h"
#include "dds3UnitObjectBasic.h"
#include "sBtlPath.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "ps2asm.h"
#include "dds3KernelDraw.h"
#include "dds3KernelMain.h"
#include "fld.h"
#include "sBtlPlayer.h"
#include "dds3PadManager.h"
#include "scrScriptProcess.h"
#include "itfMesManager.h"
#include "sBtlTalk.h"
#include "sdfVu0.h"
#include "code_r1_0027CBE8.h"
#include "sBtlPanel.h"
#include "AW_XAtkPad.h"
#include "EventBit.h"
#include "fldProcess.h"
#include "sdfMath.h"
#include "sbtlTubeSupport.h"
#include "sdfSystem.h"
#include "sBtlEncount.h"
#include "sBtlVisual.h"
#include "sbtlWork.h"

/* body: matched in raidou @ 9c5e6e0: src/raidou2/code_002C5D78.c:386, match, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
struct SBTLUNIT_WORK *sbtlSetEncountData(s32 devil, s32 posid, s32 groupID)
{
    return SetEncountData_Sub_sbtlEncount(devil, posid, groupID, 0);
}

struct SBTLUNIT_WORK *sbtlAddAWXAtkPadSub(s32 devilId, s32 point, s32 arg)
{
    return SetEncountData_Sub_sbtlEncount(devilId, point, arg, 1);
}

/* body: matched in raidou @ 61f09dc: src/raidou1/code_00298540.c:67, near, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
struct SBTLUNIT_WORK *sbtlAddAWXAtkPadSub2(s32 devilId, s32 point, s32 arg)
{
    return SetEncountData_Sub_sbtlEncount(devilId, point, arg, 2);
}

struct SBTLUNIT_WORK *sbtlSetEncountData4(s32 devil, s32 point, u32 addst) {
    struct SBTLUNIT_WORK *wk = SetEncountData_Sub_sbtlEncount(devil, point, addst, 1);

    wk->aiwk->scrflag |= 0x800000;
    return wk;
}

/* NON_MATCHING: 225 of 263 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetEncountData_Unit(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    SBTL_DEVIL_DATA *devil;
    SBTL_MOTION_VISUAL *visual;
    SBTL_UNIT_SUB *sub;
    s32 i;
    s32 field1A0;
    SBTLUNIT_MODEL *model;

    devil = sbtlGetDevilData_sbtlSub(wk->field400);
    visual = sbtlGetVisualData(2, devil->motionId);
    sub = sbtlGetMotionVisualData(2, devil->motionId, 0);
    unit = sbtlSetSummon(1, devil->motionId, wk->GroupID, 0);
    unit->unit.level = devil->level;
    unit->unit.devilId = wk->field400;
    unit->field400 = devil->field0A;
    unit->field404 = devil->aisyoId;
    unit->unit.aisyo = *sbtlGetAisyotbl(devil->aisyoId);
    unit->aisyoBase = unit->unit.aisyo;
    if (devil->minorno_d != 0) {
        unit->minor_d = devil->minorno_d;
        unit->flg |= 0x40;
    }
    unit->flg |= devil->flag;
    unit->unit.backup.str = devil->resist[0];
    unit->unit.now.hp = devil->stat18;
    unit->unit.backup.vit = devil->resist[1];
    unit->unit.backup.hp = devil->stat18;
    unit->unit.backup.tec = devil->resist[2];
    unit->unit.backup.maxhp = devil->stat1C;
    unit->unit.backup.luck = devil->resist[3];
    unit->unit.now.mp = devil->stat20;
    unit->unit.backup.mp = devil->stat20;
    unit->unit.backup.maxmp = devil->stat24;
    for (i = 0; i < 9; i++) {
        unit->haveskill[i + 2] = devil->param[i];
    }
    sbtlCheckAutoSkill_Enemy(unit);
    sbtlCheckBattlePara(unit);
    unit->unit.seikaku = devil->field0F;
    field1A0 = unit->field1A0;
    VU0_LOAD_VF(vf10, &wk->pos);
    VU0_STORE_VF(vf10, &unit->pos);
    VU0_LOAD_VF(vf10, &wk->rot);
    VU0_STORE_VF(vf10, &unit->rot);
    VU0_LOAD_VF(vf10, &wk->rotBase);
    VU0_STORE_VF(vf10, &unit->rotBase);
    if (unit->flg & 0x80000) {
        VU0_LOAD_VF(vf10, &unit->pos);
        VU0_STORE_VF(vf10, &unit->posSaved);
    }
    CheckAIScript(unit);
    unit->field414 = 2;
    unit->mv_spd.z = 0;
    unit->field424 = 0;
    unit->max_mvspd.z = visual->field04;
    unit->model->field0FC = field1A0;
    unit->model->field100 = 0;
    unit->model->flags = ((unit->model->flags | 0x140) & ~0xA0) | 0x28;
    unit->field428 = visual->field08;
    VU0_LOAD_VF(vf10, &unit->pos);
    VU0_STORE_VF(vf10, &unit->model->vec20);
    VU0_STORE_VF(vf10, &unit->model->vec30);
    model = unit->model;
    model->field12C = devil->field64;
    model->field130 = devil->field60;
    unit->visual[0] = visual->scale;
    unit->visual[1] = visual->scale;
    unit->visual[2] = visual->scale;
    unit->visual[3] = visual->scale;
    unit->visualKind = visual->kind;
    model->field11C = visual->field10;
    model->field120 = sub->field04;
    model->field124 = devil->field60;
    model->field128 = devil->field64;
    if (unit->flg & 0x8000000) {
        unit->motion_setno = devil->field42;
    }
    unit->actMax = wk->actMax;
    sbtlSetactcounter(unit);
    unit->actCounter = wk->actCounter;
    if (visual->field30 > 0) {
        sbtlGetDevilData(unit, visual->field32, visual->field30);
        unit->flg |= 0x1000;
    }
    sbtlCreateEff(wk_sbtl->effList, unit, 14, 0, &unit->pos.x, 0x80808080, 0);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00298540", sbtlSetEncountData_Unit);
#endif

void sbtlSetactcounter(struct SBTLUNIT_WORK *wk)
{
    f32 rate[5] = { 0.25f, 0.5f, 1.0f, 1.0f, 2.0f };

    if (wk->field414 & 1) {
        wk->actMax = wk->actMax * rate[wk->unit.seikaku];
    }
}

s32 sbtlClearGeneratorWork(void)
{
    s32 i;

    for (i = 0; i < 13; i++) {
        sb_gen_work[i].use = 0;
        sb_gen_work[i].zan_cnt = 0;
        sb_gen_work[i].wait = 0;
        sb_gen_work[i].penc = NULL;
    }
    sb_gen_d_flag = 0;
    return 1;
}

s32 sbtlCallBtlClearGeneratorWork(s32 stageNo)
{
    SB_ENCOUNT1 *enc;
    s32 i;
    s32 j;

    sbtlClearGeneratorWork();
    for (i = 0; i < 13; i++) {
        enc = &sbtlData100[stageNo].enc1[i];
        if (enc->type >= 2 && enc->type < 6) {
            for (j = 0; j < 13; j++) {
                if (sb_gen_work[j].use == 0) {
                    sb_gen_work[j].use = 1;
                    sb_gen_work[j].zan_cnt = enc->zan_num;
                    sb_gen_work[j].wait = GetIntervalRand(0, 30);
                    sb_gen_work[j].penc = enc;
                    break;
                }
            }
        }
    }
    return 1;
}

void sbtlGeneratorD_On(void) {
    sb_gen_d_flag = 1;
}

s32 sbtlCheckGeneratorGroupID(s32 group)
{
    s32 i;

    for (i = 0; i < 13; i++) {
        if (sb_gen_work[i].use != 0 && sb_gen_work[i].penc->groupID == group && sb_gen_work[i].zan_cnt > 0) {
            return 1;
        }
    }
    return 0;
}

s32 sbtlCheckGenerator_ND(void) {
    SB_GENERATOR_WORK *gen = sb_gen_work;
    s32 i;

    for (i = 0; i < 13; i++, gen++) {
        if (gen->use && gen->penc->type - 2 < 3U && sbtlCheckGeneratorGroupID(gen->penc->groupID)) {
            return 1;
        }
    }
    return 0;
}

s32 sbtlCheckGenerator_D(void) {
    SB_GENERATOR_WORK *gen = sb_gen_work;
    struct SB_ENCOUNT1 *enc;
    s32 i;

    for (i = 0; i < 13; i++, gen++) {
        if (gen->use) {
            enc = gen->penc;
            if (enc->type == 5 && sbtlCheckGeneratorGroupID(enc->groupID) && wk_sbtl->field424 < enc->disp_max) {
                return 1;
            }
        }
    }
    return 0;
}

s32 sbtlChkBossEventStart_EnemySummonWait(void) {
    SB_GENERATOR_WORK *gen = sb_gen_work;
    s32 i;

    for (i = 0; i < 13; i++, gen++) {
        if (gen->use && gen->zan_cnt > 0) {
            return 1;
        }
    }
    return 0;
}

s32 sbtlCallBtlGetNumUnitGroupID(void)
{
    SB_ENCOUNT1 *enc;
    s32 num;
    s32 i;

    for (i = 0; i < 13; i++) {
        if (sb_gen_work[i].use == 0) {
            continue;
        }
        enc = sb_gen_work[i].penc;
        if (enc->type == 3) {
            if (sbtlGetNumUnit_GroupID(wk_sbtl->unitList, enc->groupID2) > 0) {
                continue;
            }
            if (sbtlCheckGeneratorGroupID(enc->groupID2)) {
                continue;
            }
        }
        if (enc->type == 2 || enc->type == 3) {
            if (sbtlGetNumUnit_GroupID(wk_sbtl->unitList, enc->groupID) < enc->disp_max) {
                if (sb_gen_work[i].wait > 0) {
                    sb_gen_work[i].wait--;
                }
                if (sb_gen_work[i].wait == 0) {
                    sbtlSetEncountData(enc->devil, enc->pos, enc->groupID);
                    sb_gen_work[i].wait = GetIntervalRand(0, 30) + 100;
                    if (--sb_gen_work[i].zan_cnt <= 0) {
                        sb_gen_work[i].use = 0;
                    }
                }
            }
        }
        if (enc->type == 4) {
            if (sbtlGetNumUnit_GroupID(wk_sbtl->unitList, enc->groupID2) <= 0 &&
                !sbtlCheckGeneratorGroupID(enc->groupID2)) {
                sb_gen_work[i].use = 0;
                continue;
            }
            if (sbtlGetNumUnit_GroupID(wk_sbtl->unitList, enc->groupID) < enc->disp_max) {
                if (sb_gen_work[i].wait > 0) {
                    sb_gen_work[i].wait--;
                }
                if (sb_gen_work[i].wait == 0) {
                    sbtlSetEncountData(enc->devil, enc->pos, enc->groupID);
                    sb_gen_work[i].wait = GetIntervalRand(0, 30) + 100;
                }
            }
        }
        if (enc->type == 5 && sb_gen_d_flag != 0) {
            num = sbtlGetNumUnit_GroupID(wk_sbtl->unitList, enc->groupID);
            if (num < enc->disp_max) {
                if (sb_gen_work[i].wait > 0) {
                    sb_gen_work[i].wait--;
                }
                if (sb_gen_work[i].wait == 0) {
                    sbtlSetEncountData(enc->devil, enc->pos, enc->groupID);
                    sb_gen_work[i].wait = GetIntervalRand(0, 30) + 30;
                    if (num + 1 >= enc->disp_max) {
                        sb_gen_d_flag = 0;
                    }
                }
            }
        }
    }
    return 1;
}

/* NON_MATCHING: 46 of 56 words match.  */
#ifdef NON_MATCHING
s32 sbtlGetRndEncJoken(s32 id) {
    datRndEncTbl_t *tbl;
    s32 i = 0;

    do {
        tbl = &sbtlData101[id][i];
        if (tbl->flag0 == 0 || (tbl->flag0j == 0 ? !evtBitCheck(tbl->flag0) : evtBitCheck(tbl->flag0))) {
            if (tbl->flag1 == 0) {
                return i;
            }
            if (tbl->flag1j != 0 ? evtBitCheck(tbl->flag1) : !evtBitCheck(tbl->flag1)) {
                return i;
            }
        }
        i++;
    } while (i < 3);
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00298540", sbtlGetRndEncJoken);
#endif

s32 sbtlGetRndEncMax(s32 id)
{
    datRndEncTbl_t *tbl;
    s32 total;
    s32 i;
    s32 base;

    if (id == 0) {
        return 0;
    }
    tbl = &sbtlData101[id][sbtlGetRndEncJoken(id)];
    total = 0;
    for (i = 0; i < 8; i++) {
        total += tbl->ritu[i].ritu;
    }
    if (total == 0) {
        return 0;
    }
    if (encmax > 0) {
        return encmax;
    }
    base = datRndEncBase - tbl->normalritu * 48 / 100;
    if (base < 50) {
        base = 50;
    }
    encmax = dds3GetRandIntA(NULL, base * 2 - 96) + 48;
    return encmax;
}

s32 sbtlGetRndEncCnt(void)
{
    return DDS3_GBWK->encountcnt;
}

/* NON_MATCHING: 51 of 129 words match.  */
#ifdef NON_MATCHING
s32 sbtlRndEncCalc(s32 id, f32 length)
{
    datRndEncTbl_t *tbl;
    datRndEncRitu_t *ritu;
    u32 max;
    s32 steps;
    s32 i;
    s32 r;
    s32 sum;
    s32 encountId;
    u32 add;

    if (id == 0 || length == 0.0f) {
        return 0;
    }
    tbl = &sbtlData101[id][sbtlGetRndEncJoken(id)];
    max = sbtlGetRndEncMax(id);
    if (max == 0) {
        return 0;
    }
    DDS3_GBWK->encountkyori += length;
    if (DDS3_GBWK->encountkyori < 100.0f) {
        return 0;
    }
    steps = DDS3_GBWK->encountkyori / 100.0f;
    i = 0;
    DDS3_GBWK->encountkyori -= steps * 100.0f;
    while (i++ < steps) {
        if (DDS3_GBWK->unk1C94 & 0x22) {
            add = dds3GetRandIntA(NULL, 7) + 6;
        } else {
            add = dds3GetRandIntA(NULL, 3) + 2;
        }
        DDS3_GBWK->encountcnt += SDF_DIFFICULTY_ENCOUNTER_STEP(add);
    }
    if (DDS3_GBWK->encountcnt > 65000) {
        DDS3_GBWK->encountcnt = 65000;
    }
    if (DDS3_GBWK->encountcnt < max) {
        return 0;
    }
    r = dds3GetRandIntA(NULL, 1000);
    ritu = tbl->ritu;
    sum = 0;
    for (i = 0; i < 8; i++, ritu++) {
        if (ritu->encountid == 0) {
            continue;
        }
        sum += ritu->ritu;
        if (r < sum) {
            encountId = ritu->encountid;
            if ((DDS3_GBWK->unk1C94 & 0x11) && !sbtlGetLevelAverage(encountId)) {
                sbtlResetRndEncCount();
                return 0;
            }
            g_rndenc1_map = ritu->field04;
            g_rndenc1_floor = ritu->type;
            encmax = 0;
            return encountId;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00298540", sbtlRndEncCalc);
#endif

/* NON_MATCHING: 60 of 61 words match.  */
#ifdef NON_MATCHING
s32 sbtlGetLevelAverage(s32 encountno) {
    SB_ENCOUNT1 *enc;
    s32 devil;
    s32 sum;
    s32 num;
    s32 i;

    if (encountno == 0) {
        return 0;
    }
    sum = 0;
    num = 0;
    for (i = 0; i < 13; i++) {
        enc = &sbtlData100[encountno].enc1[i];
        devil = 0;
        if (enc->type == 1 || enc->type == 6) {
            devil = enc->devil;
        }
        if (devil != 0) {
            num++;
            sum += sbtlGetDevilData_sbtlSub(devil)->level;
        }
    }
    if (num == 0) {
        return 0;
    }
    return sum / num + DDS3_GBWK->playerData.unit.level / 4 >= DDS3_GBWK->playerData.unit.level;
}
#else
INCLUDE_ASM(const s32, "code_00298540", sbtlGetLevelAverage);
#endif

void sbtlResetRndEncCount(void)
{
    DDS3_GBWK->encountkyori = 0.0f;
    DDS3_GBWK->encountcnt = 0;
    encmax = 0;
}

void sbtlRndEncTopPriorityMap(s32 *map, s32 *floor) {
    *map = g_rndenc1_map;
    *floor = g_rndenc1_floor;
}

/* NON_MATCHING: 64 of 171 words match.  */
#ifdef NON_MATCHING
void sbtlGetDevilInfo(s32 id, SB_RNDENC_DEVILINFO *pDevilInfo) {
    s32 list[64][2];
    s32 num = 0;
    datRndEncTbl_t *tbl;
    datRndEncRitu_t *ritu;
    SB_ENCOUNT1 *enc;
    s32 devil;
    s32 found;
    s32 total;
    s32 rnd;
    s32 sum;
    s32 i;
    s32 j;
    s32 k;

    pDevilInfo->devil_id[0] = 0;
    pDevilInfo->devil_id[1] = 0;
    pDevilInfo->devil_id[2] = 0;
    pDevilInfo->num = 0;
    if (id == 0) {
        return;
    }
    tbl = &sbtlData101[id][sbtlGetRndEncJoken(id)];
    for (i = 0; i < 64; i++) {
        list[i][0] = 0;
        list[i][1] = 0;
    }
    ritu = tbl->ritu;
    for (i = 0; i < 8; i++, ritu++) {
        if (ritu->encountid == 0 || ritu->ritu == 0) {
            continue;
        }
        enc = sbtlData100[ritu->encountid].enc1;
        for (j = 0; j < 13; j++, enc++) {
            if (enc->type == 0) {
                continue;
            }
            switch (enc->devil) {
            case 218:
                devil = 217;
                break;
            case 214:
                devil = 126;
                break;
            case 283:
            case 284:
                devil = 282;
                break;
            default:
                devil = enc->devil;
                break;
            }
            found = 0;
            for (k = 0; k < num; k++) {
                if (list[k][0] == devil) {
                    found = 1;
                    list[k][1] += ritu->ritu;
                }
            }
            if (!found) {
                list[num][0] = devil;
                list[num][1] = ritu->ritu;
                num++;
                if (num == 64) {
                    break;
                }
            }
        }
    }
    if (num >= 4) {
        pDevilInfo->num = 3;
        for (i = 0; i < 3; i++) {
            total = 0;
            for (k = 0; k < num; k++) {
                total += list[k][1];
            }
            rnd = dds3GetRandIntA(NULL, total);
            sum = 0;
            for (k = 0; k < num; k++) {
                if (rnd < sum + list[k][1]) {
                    list[k][1] = 0;
                    pDevilInfo->devil_id[i] = list[k][0];
                    break;
                }
                sum += list[k][1];
            }
        }
    } else {
        pDevilInfo->num = num;
        pDevilInfo->devil_id[0] = list[0][0];
        pDevilInfo->devil_id[1] = list[1][0];
        pDevilInfo->devil_id[2] = list[2][0];
    }
}
#else
INCLUDE_ASM(const s32, "code_00298540", sbtlGetDevilInfo);
#endif

/* NON_MATCHING: 72 of 107 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetNextPathData(struct SBTLUNIT_WORK *wk) {
    SBTLENMY_ACTDATA *act;
    SBTLENMY_PATHDATA *path;
    SBTLUNIT_MODEL *ai;
    sdfVector4_t pos;
    s32 cnt;
    u32 flg;

    act = sbtlGetPathACTData(0, wk->model->path_datap);
    path = sbtlGetPathData(act->pathid);
    flg = act->flg;
    ai = wk->model;
    if (!(flg & 0x10)) {
        ai->path_mode = 1;
    }
    switch (ai->path_mode) {
    case 0:
        wk->model->path_cnt = GetIntervalRand(0, act->frame);
        wk->model->path_mode = 1;
        return 0;
    case 1:
        if (!(flg & 0x10)) {
            ai->path_cnt = 0;
            break;
        }
        ai->path_cnt++;
        if (ai->path_cnt <= 0) {
            ai->path_cnt = 0;
        } else if (ai->path_cnt >= act->frame) {
            ai->path_cnt = act->frame;
        }
        if (ai->path_cnt < act->frame) {
            return 0;
        }
        ai->path_cnt = 0;
        break;
    default:
        ai->path_cnt = 0;
        break;
    }
    ai->path_mode = 0;
    ai->path_datap = act->jmpid;
    if (act->flg & 2) {
        SDF_VU0_LOADVECTOR(&path->nowpos);
        SDF_VU0_STOREVECTOR(&pos);
    } else if (act->flg & 1) {
        SDF_VU0_LOADVECTOR(&path->nextpos);
        SDF_VU0_STOREVECTOR(&pos);
    } else {
        SDF_VU0_LOADVECTOR(&path->nextpos);
        SDF_VU0_STOREVECTOR(&pos);
    }
    pos.x += main_data.val20 + ai->path_ofspos.x;
    pos.z += main_data.val24 + ai->path_ofspos.z;
    pos.y += sbtlFree(pos.x, pos.z);
    sbtlSetNormalAction(wk, act->type, 1, act->frame, &pos);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00298540", sbtlSetNextPathData);
#endif

s16 sbtlGetPathDataCount(s16 id) {
    return tbldatacnt[id];
}

SBTLENMY_ACTDATA *sbtlGetPathACTData(s16 id, s16 no) {
    return &data[id][no];
}

SBTLENMY_PATHDATA *sbtlGetPathData(s16 id) {
    return &pathdata[id];
}
