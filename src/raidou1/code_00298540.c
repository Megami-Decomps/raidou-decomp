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

INCLUDE_ASM(const s32, "code_00298540", Choice1);

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

INCLUDE_ASM(const s32, "code_00298540", sbtlGetRndEncJoken);

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

INCLUDE_ASM(const s32, "code_00298540", sbtlGetDevilInfo);

INCLUDE_ASM(const s32, "code_00298540", sbtlSetNextPathData);

s16 sbtlGetPathDataCount(s16 id) {
    return tbldatacnt[id];
}

SBTLENMY_ACTDATA *sbtlGetPathACTData(s16 id, s16 no) {
    return &data[id][no];
}

SBTLENMY_PATHDATA *sbtlGetPathData(s16 id) {
    return &pathdata[id];
}
