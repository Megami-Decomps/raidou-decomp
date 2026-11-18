#include "common.h"
#include "dds3Basic.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"
#include "sBtlEff.h"
#include "sBtlSound.h"
#include "sbtlDMG.h"
#include "sBtlCalc.h"
#include "sBtlSM.h"
#include "sBtlEXP.h"
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
#include "sBtlEncount.h"

SBTL_PLAYER_DATA *sbtlGetPlayerData(void)
{
    return &DDS3_GBWK->playerData;
}

sdatUnitWork_t *sbtlGetStockUnitData(s32 id) {
    if (DDS3_GBWK->playerData.stock[id].devilId == 0 || !(DDS3_GBWK->playerData.stock[id].flag & 1)) {
        return NULL;
    }
    return &DDS3_GBWK->playerData.stock[id];
}

/* NON_MATCHING: 106 of 111 words match. scheduling of li a0,9 vs move a1,sp before the compaction loop (retail a1 then a0). Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallMemset(struct SBTLUNIT_WORK *wk, s32 index) {
    sdatUnitWork_t *dest = &DDS3_GBWK->playerData.stock[index];
    s32 skills[10];
    s32 *out;
    s32 left;
    s32 id;

    memset(skills, 0, sizeof(skills));
    out = skills;
    for (left = 9; left >= 0; left--) {
        id = wk->unit.haveskill[9 - left];
        if (id != SKLID_ATK && id != SKLID_NONE && id != SKLID_OFS_PROGRAM) {
            *out++ = id;
        }
    }
    memcpy(wk->unit.haveskill, skills, sizeof(skills));
    *dest = wk->unit;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", sbtlCallMemset);
#endif

/* NON_MATCHING: 145 of 150 words match. order of the loop setup moves (li a0,9 and a1=sp+8 before the two constants. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallMemset2(struct SBTLUNIT_WORK *wk, s32 index) {
    sdatUnitWork_t *src = &DDS3_GBWK->playerData.stock[index];
    s32 skills[10];
    s32 *out;
    s32 left;
    s32 id;

    sbtlCheckStockPara(src);
    wk->unit = *src;
    wk->aisyoBase = wk->unit.aisyo;
    memset(skills, 0, sizeof(skills));
    skills[0] = SKLID_ATK;
    skills[1] = SKLID_OFS_PROGRAM;
    out = &skills[2];
    for (left = 9; left >= 0; left--) {
        id = wk->unit.haveskill[9 - left];
        if (id != SKLID_ATK && id != SKLID_NONE && id != SKLID_OFS_PROGRAM) {
            *out++ = id;
        }
    }
    memcpy(wk->unit.haveskill, skills, sizeof(skills));
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", sbtlCallMemset2);
#endif

/* NON_MATCHING: 6 of 17 words match. 11 of 17 words differ. Retail keeps wk_sbtl in $a1 and sets $v0 = 0 before the field344 test; cc1 uses $v1 and sets it on the return path. */
#ifdef NON_MATCHING
struct SBTLUNIT_WORK *sbtlRefSbtl(void) {
    SBTL_WORK *wk = wk_sbtl;
    struct SBTLUNIT_WORK *unit = NULL;

    if (wk->field344 > 0) {
        unit = sbtlSearchUnit_ID(wk->unitList, wk->stockUnitID[wk->summonIndex[0]]);
    }
    return unit;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", sbtlRefSbtl);
#endif

s32 sbtlLeaf53(sdatUnitWork_t *unit) {
    if (unit->badStatus & 0x8000) {
        return 0;
    }
    return 1;
}

s32 sbtlBtlNakamaSetUpSub(sdatUnitWork_t *unit) {
    if (unit->flag & 1) {
        return (unit->badStatus & 0xC000) == 0;
    }
    return 0;
}

/* NON_MATCHING: 138 of 146 words match. Pbtl and pitem.x[0] loads swap places in the stat sums; vector checked (2 rows, devilId>=81 skips clamp, sums and hp/mp writes agree). */
#ifdef NON_MATCHING
s32 sbtlCheckStockPara(sdatUnitWork_t *unit)
{
    unit->now.maxhp = unit->backup.maxhp + unit->pitem.maxhp + unit->pbtl.maxhp + unit->huuma.maxhp;
    unit->now.maxmp = unit->backup.maxmp + unit->pitem.maxmp + unit->pbtl.maxmp + unit->huuma.maxmp;
    unit->now.str = unit->backup.str + unit->pbtl.str + unit->pitem.str[1] + unit->pitem.str[0] + unit->huuma.str;
    unit->now.vit = unit->backup.vit + unit->pbtl.vit + unit->pitem.vit[1] + unit->pitem.vit[0] + unit->huuma.vit;
    unit->now.tec = unit->backup.tec + unit->pbtl.tec + unit->pitem.tec[1] + unit->pitem.tec[0] + unit->huuma.tec;
    unit->now.luck = unit->backup.luck + unit->pbtl.luck + unit->pitem.luck[1] + unit->pitem.luck[0] + unit->huuma.luck;
    if (unit->devilId < 81) {
        unit->now.maxhp = (unit->now.maxhp <= 0) ? 0 : (unit->now.maxhp < 1000) ? unit->now.maxhp : 999;
        unit->now.maxmp = (unit->now.maxmp <= 0) ? 0 : (unit->now.maxmp < 1000) ? unit->now.maxmp : 999;
        unit->now.str = (unit->now.str <= 0) ? 0 : (unit->now.str < 41) ? unit->now.str : 40;
        unit->now.vit = (unit->now.vit <= 0) ? 0 : (unit->now.vit < 41) ? unit->now.vit : 40;
        unit->now.tec = (unit->now.tec <= 0) ? 0 : (unit->now.tec < 41) ? unit->now.tec : 40;
        unit->now.luck = (unit->now.luck <= 0) ? 0 : (unit->now.luck < 41) ? unit->now.luck : 40;
    }
    unit->now.hp = (unit->now.hp <= 0) ? 0 : (unit->now.hp < unit->now.maxhp) ? unit->now.hp : unit->now.maxhp;
    unit->now.mp = (unit->now.mp <= 0) ? 0 : (unit->now.mp < unit->now.maxmp) ? unit->now.mp : unit->now.maxmp;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", sbtlCheckStockPara);
#endif

/* NON_MATCHING: 0 of 33 words match. 33 of 34 words differ. Retail keeps id, the item pointer base and num in three saved registers and clamps with blez/movz/movn; cc1 allocates two and branches. Logic as retail, including the returned max - total floored at 0. */
#ifdef NON_MATCHING
s32 BattleSkillSort(s32 id, s16 num) {
    s16 *item = &DDS3_GBWK->playerData.item[id];
    s32 max = sbtlGetItemtbl(id)->max_num;
    s16 value = 0;
    s16 total;
    s32 rest;

    total = *item + num;
    *item = total;
    rest = max - total;
    if (total > 0) {
        value = total < max ? total : max;
    }
    *item = value;
    return rest < 0 ? 0 : rest;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", BattleSkillSort);
#endif

/* NON_MATCHING: 18 of 92 words match. retail walks three induction pointers with mode-test code duplicated per path (shared NEG and POS tails). Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlSetMapFade(s32 frame, u32 abgr, s16 mode) {
    SBTL_WORK *wk = wk_sbtl;
    SBTL_WORK *work;
    s32 cur[3];
    s32 tgt[3];
    s16 step[3];
    s32 i;
    s32 from;
    s32 to;
    s32 count;
    s32 delta;

    if (wk == NULL) {
        return 1;
    }
    wk->mf_mode = mode;
    wk->mf_counter = 0;
    wk->startFlg |= 8;
    wk->mf_cmpcounter = frame;
    if (frame == 0) {
        wk->mf_cmpcounter = 1;
    }
    work = wk_sbtl;
    work->mf_abgr_s = abgr;
    cur[0] = work->mf_abgr & 0xFF;
    cur[1] = (work->mf_abgr >> 8) & 0xFF;
    cur[2] = (work->mf_abgr & 0xFF0000) >> 16;
    tgt[0] = work->mf_abgr_s & 0xFF;
    tgt[1] = (work->mf_abgr_s >> 8) & 0xFF;
    tgt[2] = (work->mf_abgr_s & 0xFF0000) >> 16;
    for (i = 0; i < 3; i++) {
        from = cur[i];
        to = tgt[i];
        count = work->mf_cmpcounter;
        if (mode != 0 ? from < to : to >= from) {
            step[i] = (to - from) / count;
            if (step[i] <= 0) {
                step[i] = 1;
            }
        } else {
            delta = from - to;
            step[i] = delta / count;
            if (step[i] <= 0) {
                step[i] = 1;
            }
            step[i] = -step[i];
        }
    }
    work->mf_r = step[0];
    work->mf_g = step[1];
    work->mf_b = step[2];
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", sbtlSetMapFade);
#endif

/* NON_MATCHING: 39 of 71 words match. Retail keeps a dead lh of mf_mode before each colour term and reloads wk_sbtl after the clamp store; vector checked (rows only cover the startFlg-clear return). */
#ifdef NON_MATCHING
s32 sbtlMapFadeProc(void)
{
    if (wk_sbtl == NULL) {
        return 1;
    }
    if (!(wk_sbtl->startFlg & 8)) {
        return 0;
    }
    if (wk_sbtl->flg & 0x8000000) {
        return 0;
    }
    if (wk_sbtl->mf_counter >= wk_sbtl->mf_cmpcounter) {
        wk_sbtl->mf_abgr = wk_sbtl->mf_abgr_s;
        if (wk_sbtl->mf_mode == 1) {
            wk_sbtl->startFlg &= ~8;
        }
    } else {
        wk_sbtl->mf_counter++;
        wk_sbtl->mf_counter = (wk_sbtl->mf_counter <= 0) ? 0
                            : (wk_sbtl->mf_counter < wk_sbtl->mf_cmpcounter) ? wk_sbtl->mf_counter
                                                                             : wk_sbtl->mf_cmpcounter;
        wk_sbtl->mf_abgr = (wk_sbtl->mf_abgr_s & 0xFF000000)
                         | ((wk_sbtl->mf_abgr & 0xFF) + wk_sbtl->mf_r)
                         | (((wk_sbtl->mf_abgr >> 8 & 0xFF) + wk_sbtl->mf_g) << 8)
                         | (((wk_sbtl->mf_abgr >> 16 & 0xFF) + wk_sbtl->mf_b) << 16);
    }
    fldHirukoYukaGo_Idx2_B(wk_sbtl->mf_abgr);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", sbtlMapFadeProc);
#endif

/* NON_MATCHING: 18 of 92 words match. retail walks three induction pointers with mode-test code duplicated per path (shared NEG and POS tails). Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlSetMapFade_H(s32 frame, u32 abgr, s16 mode) {
    SBTL_WORK *wk = wk_sbtl;
    SBTL_WORK *work;
    s32 cur[3];
    s32 tgt[3];
    s16 step[3];
    s32 i;
    s32 from;
    s32 to;
    s32 count;
    s32 delta;

    if (wk == NULL) {
        return 1;
    }
    wk->mfh_mode = mode;
    wk->mfh_counter = 0;
    wk->startFlg |= 0x10;
    wk->mfh_cmpcounter = frame;
    if (frame == 0) {
        wk->mfh_cmpcounter = 1;
    }
    work = wk_sbtl;
    work->mfh_abgr_s = abgr;
    cur[0] = work->mfh_abgr & 0xFF;
    cur[1] = (work->mfh_abgr >> 8) & 0xFF;
    cur[2] = (work->mfh_abgr & 0xFF0000) >> 16;
    tgt[0] = work->mfh_abgr_s & 0xFF;
    tgt[1] = (work->mfh_abgr_s >> 8) & 0xFF;
    tgt[2] = (work->mfh_abgr_s & 0xFF0000) >> 16;
    for (i = 0; i < 3; i++) {
        from = cur[i];
        to = tgt[i];
        count = work->mfh_cmpcounter;
        if (mode != 0 ? from < to : to >= from) {
            step[i] = (to - from) / count;
            if (step[i] <= 0) {
                step[i] = 1;
            }
        } else {
            delta = from - to;
            step[i] = delta / count;
            if (step[i] <= 0) {
                step[i] = 1;
            }
            step[i] = -step[i];
        }
    }
    work->mfh_r = step[0];
    work->mfh_g = step[1];
    work->mfh_b = step[2];
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", sbtlSetMapFade_H);
#endif

/* NON_MATCHING: 33 of 63 words match. Retail keeps a dead lh of mfh_mode before each colour term and reloads wk_sbtl after the clamp store; vector checked (rows only cover the startFlg-clear return). */
#ifdef NON_MATCHING
s32 sbtlMapFadeProc_H(void)
{
    if (wk_sbtl == NULL) {
        return 1;
    }
    if (!(wk_sbtl->startFlg & 0x10)) {
        return 0;
    }
    if (wk_sbtl->mfh_counter >= wk_sbtl->mfh_cmpcounter) {
        wk_sbtl->mfh_abgr = wk_sbtl->mfh_abgr_s;
        wk_sbtl->startFlg &= ~0x10;
    } else {
        wk_sbtl->mfh_counter++;
        wk_sbtl->mfh_counter = (wk_sbtl->mfh_counter <= 0) ? 0
                             : (wk_sbtl->mfh_counter < wk_sbtl->mfh_cmpcounter) ? wk_sbtl->mfh_counter
                                                                                 : wk_sbtl->mfh_cmpcounter;
        wk_sbtl->mfh_abgr = (wk_sbtl->mfh_abgr_s & 0xFF000000)
                          | ((wk_sbtl->mfh_abgr & 0xFF) + wk_sbtl->mfh_r)
                          | (((wk_sbtl->mfh_abgr >> 8 & 0xFF) + wk_sbtl->mfh_g) << 8)
                          | (((wk_sbtl->mfh_abgr >> 16 & 0xFF) + wk_sbtl->mfh_b) << 16);
    }
    fldHirukoYukaGo_Idx2_B(wk_sbtl->mfh_abgr);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", sbtlMapFadeProc_H);
#endif

s32 sbtlSearchPacData(s32 major, s32 minor, s32 devilid)
{
    s32 i;

    for (i = 0; i < SBTL_PACDATA_NUM; i++) {
        if (sbtlData84->pac[i].key.major == major && sbtlData84->pac[i].key.minor == minor
            && sbtlData84->pac[i].key.devilid == devilid) {
            return i;
        }
    }
    for (i = 0; i < SBTL_PACDATA_NUM; i++) {
        if (sbtlData84->pac[i].key.major == 0 && sbtlData84->pac[i].key.minor == 0
            && sbtlData84->pac[i].key.devilid == 0) {
            return i;
        }
    }
    return -2;
}

s32 sbtlSearchPacData2(s32 major, s32 minor, s32 devilid)
{
    s32 i;

    i = sbtlSearchPacData(major, minor, devilid);
    if (i == -1) {
        return 1;
    }
    sbtlData84->pac[i].key.major = major;
    sbtlData84->pac[i].key.minor = minor;
    sbtlData84->pac[i].key.devilid = devilid;
    return 1;
}

s32 sbtlCallBtlSearchPacData(s32 major, s32 minor, s32 devilid, SBTL_HIT_PACFILE *file, s32 size)
{
    SBTLSKILLHIT_TBL *entry;
    SBTL_HIT_PACDATA *pwk;
    sdfMemHandle_t *handle;
    s32 i;
    s32 j;

    i = sbtlSearchPacData(major, minor, devilid);
    if (i == -1) {
        return 1;
    }
    entry = file->entry;
    sbtlSearchPacData2(major, minor, devilid);
    sbtlData84->pac[i].hit.num = file->num;
    for (j = 0; j < sbtlData84->pac[i].hit.num; j++) {
        handle = sdfMemAllocDirect(sizeof(SBTL_HIT_PACDATA));
        pwk = sdfMemLock(handle);
        pwk->handle = handle;
        sbtlData84->pac[i].hit.pwk[j] = pwk;
        pwk->entry = *entry;
        sbtlData84->pac[i].hit.pwk[j]->handle08 = NULL;
        sbtlData84->pac[i].hit.pwk[j]->handle10 = NULL;
        entry++;
    }
    sbtlData84->pac[i].hit.status |= 1;
    return 1;
}

s32 sbtlSetPacData_MHT(s32 major, s32 minor, s32 devilid, void *data, s32 size)
{
    sdfMemHandle_t *handle;
    void *buf;
    s32 i;

    i = sbtlSearchPacData(major, minor, devilid);
    if (i == -1) {
        return 1;
    }
    {
        sbtlSearchPacData2(major, minor, devilid);
        handle = sdfMemAllocDirect(size);
        buf = sdfMemLock(handle);
        sbtlData84->pac[i].mht.data = buf;
        sbtlData84->pac[i].mht.handle = handle;
        sbtlData84->pac[i].mht.size = size;
        memcpy(buf, data, size);
    }
    return 1;
}

s32 sbtlCallBtlSearchPacData2(s32 major, s32 minor, s32 devilid, void *data, s32 size)
{
    sdfMemHandle_t *handle;
    void *buf;
    s32 i;

    i = sbtlSearchPacData(major, minor, devilid);
    if (i == -1) {
        return 1;
    }
    {
        sbtlSearchPacData2(major, minor, devilid);
        handle = sdfMemAllocDirect(size);
        buf = sdfMemLock(handle);
        sbtlData84->pac[i].motionSE.data = buf;
        sbtlData84->pac[i].motionSE.handle = handle;
        sbtlData84->pac[i].motionSE.size = size;
        memcpy(buf, data, size);
    }
    return 1;
}

/* body: matched in raidou @ 88f0d54: src/raidou1/code_r1_0024B788.c:224, near, same function 1.00 (words 53/53, calls 1.00, shape 1.00) */
s32 DrawProc2(s32 major, s32 minor, s32 devilid, void *data, s32 size)
{
    sdfMemHandle_t *handle;
    void *buf;
    s32 i;

    i = sbtlSearchPacData(major, minor, devilid);
    if (i == -1) {
        return 1;
    }
    {
        sbtlSearchPacData2(major, minor, devilid);
        handle = sdfMemAllocDirect(size);
        buf = sdfMemLock(handle);
        sbtlData84->pac[i].file20.data = buf;
        sbtlData84->pac[i].file20.handle = handle;
        sbtlData84->pac[i].file20.size = size;
        memcpy(buf, data, size);
    }
    return 1;
}

void sbtlPacData_Init(void)
{
    SBTL_PACDATA *pac;
    s32 i;

    memset(sbtlData84, 0, sizeof(sbtlData84->pac));
    pac = sbtlData84->pac;
    for (i = SBTL_PACDATA_NUM - 1; i >= 0; i--) {
        pac->mht.handle = NULL;
        pac->mht.data = NULL;
        pac->motionSE.handle = NULL;
        pac->motionSE.data = NULL;
        pac->file20.handle = NULL;
        pac->file20.data = NULL;
        pac++;
    }
}

void sbtlCallMemFreeDirect(void) {
    s32 i;
    s32 j;

    for (i = 0; i < SBTL_PACDATA_NUM; i++) {
        if (sbtlData84->pac[i].key.major || sbtlData84->pac[i].key.minor || sbtlData84->pac[i].key.devilid) {
            if (sbtlData84->pac[i].hit.status & 1) {
                for (j = 0; j < sbtlData84->pac[i].hit.num; j++) {
                    if (sbtlData84->pac[i].hit.pwk[j]->handle08) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle08);
                        sbtlData84->pac[i].hit.pwk[j]->handle08 = NULL;
                    }
                    if (sbtlData84->pac[i].hit.pwk[j]->handle10) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle10);
                        sbtlData84->pac[i].hit.pwk[j]->handle10 = NULL;
                    }
                    if (sbtlData84->pac[i].hit.pwk[j]->handle) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle);
                        sbtlData84->pac[i].hit.pwk[j]->handle = NULL;
                    }
                }
                sbtlData84->pac[i].hit.status = 0;
            }
            if (sbtlData84->pac[i].mht.handle) {
                sdfMemFreeDirect(sbtlData84->pac[i].mht.handle);
                sbtlData84->pac[i].mht.handle = NULL;
                sbtlData84->pac[i].mht.data = NULL;
            }
            if (sbtlData84->pac[i].motionSE.handle) {
                sdfMemFreeDirect(sbtlData84->pac[i].motionSE.handle);
                sbtlData84->pac[i].motionSE.handle = NULL;
                sbtlData84->pac[i].motionSE.data = NULL;
            }
            if (sbtlData84->pac[i].file20.handle) {
                sdfMemFreeDirect(sbtlData84->pac[i].file20.handle);
                sbtlData84->pac[i].file20.handle = NULL;
                sbtlData84->pac[i].file20.data = NULL;
            }
        }
    }
}

void sbtlPacData_FreeIndex(MODELTYPE major, s32 minor, s32 devilid) {
    s32 i;
    s32 j;

    for (i = 0; i < SBTL_PACDATA_NUM; i++) {
        if ((sbtlData84->pac[i].key.major || sbtlData84->pac[i].key.minor || sbtlData84->pac[i].key.devilid)
            && sbtlData84->pac[i].key.major == major && sbtlData84->pac[i].key.minor == minor
            && sbtlData84->pac[i].key.devilid == devilid) {
            if (sbtlData84->pac[i].hit.status & 1) {
                for (j = 0; j < sbtlData84->pac[i].hit.num; j++) {
                    if (sbtlData84->pac[i].hit.pwk[j]->handle08) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle08);
                        sbtlData84->pac[i].hit.pwk[j]->handle08 = NULL;
                    }
                    if (sbtlData84->pac[i].hit.pwk[j]->handle10) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle10);
                        sbtlData84->pac[i].hit.pwk[j]->handle10 = NULL;
                    }
                    if (sbtlData84->pac[i].hit.pwk[j]->handle) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle);
                        sbtlData84->pac[i].hit.pwk[j]->handle = NULL;
                    }
                }
                sbtlData84->pac[i].hit.status = 0;
            }
            if (sbtlData84->pac[i].mht.handle) {
                sdfMemFreeDirect(sbtlData84->pac[i].mht.handle);
                sbtlData84->pac[i].mht.handle = NULL;
                sbtlData84->pac[i].mht.data = NULL;
            }
            if (sbtlData84->pac[i].motionSE.handle) {
                sdfMemFreeDirect(sbtlData84->pac[i].motionSE.handle);
                sbtlData84->pac[i].motionSE.handle = NULL;
                sbtlData84->pac[i].motionSE.data = NULL;
            }
            if (sbtlData84->pac[i].file20.handle) {
                sdfMemFreeDirect(sbtlData84->pac[i].file20.handle);
                sbtlData84->pac[i].file20.handle = NULL;
                sbtlData84->pac[i].file20.data = NULL;
            }
            sbtlData84->pac[i].key.major = 0;
            sbtlData84->pac[i].key.minor = 0;
            sbtlData84->pac[i].key.devilid = 0;
        }
    }
}

void sbtlCallMemset3(s32 battleMode, s32 stageNo)
{
    memset(wk_sbtl, 0, sizeof(SBTL_WORK));
    VRF_DeltaTime = 0;
    pbData = 0;
    wk_sbtl->summonIndex[1] = -1;
    if (battleMode >= 4) {
        wk_sbtl->battleMode = 0;
    } else if (battleMode < 0) {
        wk_sbtl->battleMode = 0;
    } else {
        wk_sbtl->battleMode = battleMode;
    }
    wk_sbtl->mode = 1;
    wk_sbtl->mf_abgr = 0x80808080;
    wk_sbtl->mfh_abgr = 0x80808080;
    wk_sbtl->stageNo = stageNo;
    wk_sbtl->flg &= ~8;
    wk_sbtl->animList = sbtlAnimListProc();
    wk_sbtl->hitList = sbtlCreateHitList();
    wk_sbtl->effList = sbtlCreateEffList();
    wk_sbtl->modelBindings = effAllocateModelBindings();
    wk_sbtl->dvltkList = sbtlCallChipAllocZeroDirect();
    wk_sbtl->expList = InitTarSelProcessData();
    wk_sbtl->stp = evtAllocateContext();
    wk_sbtl->list2CC = NULL;
    wk_sbtl->field404 = 0;
    wk_sbtl->field408 = 0;
    wk_sbtl->field40C = 0;
    wk_sbtl->list3E8 = sbtlCreateUnitList(48);
    wk_sbtl->list2D4 = sbtlCreateUnitList(48);
    wk_sbtl->itemList = sbtlCreateItemList();
    wk_sbtl->bulletList = sbtlCreateItemList();
    wk_sbtl->sysmsgList = sbtlCreateSMList();
    wk_sbtl->effdelList = sbtlCreateDelList();
    sbtlEntryItemList(wk_sbtl->itemList);
    sbtlEntryItemList_Type(wk_sbtl->bulletList, 4);
    sbtlInitTutorial();
    sbtlRefSbtl45();
    sbtlClearGeneratorWork();
    sbtlRefSbtl7();
    sbtlAllInit();
    sbtlSbtlmenuMain();
    Init_sbtlDraw();
    sbtlPacData_Init();
}

s32 sbtlSbattle5(dds3ProcessID_t *id)
{
    s32 i;

    if (dds3SearchProcessName(sbtlData85) == NULL) {
        return 0;
    }
    sbtlRefSbtl4();
    if (wk_sbtl->flg & 8) {
        sbtlSbattle2(0);
        return 0;
    }
    if (dds3BgSetScrollSub()) {
        return 0;
    }
    GetRand(95);
    if (!(wk_sbtl->flg & 0x10000)) {
        sbtlNakamaSetUp();
    }
    sbtlCheckPad();
    sbtlMapFadeProc();
    sbtlMapFadeProc_H();
    Proc_sbtlEXP(wk_sbtl->modelBindings);
    sbtlCallMemFreeDirect6(wk_sbtl->modelBindings, 0);
    Proc_sbtlDvltk(wk_sbtl->dvltkList);
    sbtlReleaseDvltk_List(wk_sbtl->dvltkList, 0);
    sbtlRefSbtl60(wk_sbtl->expList);
    sbtlReleaseEXP_List(wk_sbtl->expList, 0);
    sbtlCallDeleteMessage(wk_sbtl->sysmsgList);
    sbtlReleaseSM_List(wk_sbtl->sysmsgList, 0);
    Proc_sbtlSTP(wk_sbtl->stp);
    sbtlReleaseSTP_List(wk_sbtl->stp, 0);
    sbtlCallBtlGetNumUnitGroupID();
    sbtlRefSbtl47();
    sbtlCallBtlCheckEnshutuSummonUnit();
    sbtlCalcVibe();
    sbtlUpdateVibe();
    wk_sbtl->syscnt++;
    if (!(wk_sbtl->flg & 0x40) || (wk_sbtl->scrFlag & 1)) {
        for (i = 0; i < 4; i++) {
            wk_sbtl->scrCnt[i]++;
        }
        for (i = 0; i < 8; i++) {
            wk_sbtl->enshutuCnt[i]++;
        }
    }
    GetRand(32);
    pbCallBtlCheckLoadFlg();
    return 0;
}

/* NON_MATCHING: 42 of 291 words match. Block layout: retail puts the field344<=0 path after the shared bad-status tail; vector checked (32 rows: field424<=0, mode 0->2, mode<3, mode=5 fall-through agree by reading; summon load path row not traced). */
#ifdef NON_MATCHING
s32 sbtlNakamaSetUp(void)
{
    struct SBTLUNIT_WORK *wk;
    SBTL_PLAYER_DATA *player;
    BST bst;
    s32 i;
    s32 set;

    if (wk_sbtl->field424 <= 0) {
        return 0;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 2) && (u16)(wk->mode - 1) < 2) {
            return 0;
        }
    }
    player = &DDS3_GBWK->playerData;
    if (wk_sbtl->flg & 0x10000) {
        return 1;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk == NULL) {
        return 0;
    }
    if (!(wk_sbtl->flg & 0x8000)) {
        if (wk->mode == 0) {
            wk->mode = 2;
            if (wk_sbtl->battleMode == 3) {
                sbtlRefSbtl24(wk, 0);
            }
            return 0;
        }
        if (wk->mode < 3) {
            return 0;
        }
        wk_sbtl->flg |= 0x8000;
        set = 0;
        if (wk_sbtl->battleMode == 1) {
            wk->color = 0;
            wk->mode = 5;
            wk->vstatus |= 0x200000;
            bst = sbtlGetBadStatus(&wk->unit);
            if (bst != BST_BOYCOTT) {
                sbtlSetBadStatus_No(wk, 0, bst, 1, -1);
            }
            set = 2;
        }
        if ((wk_sbtl->battleMode == 0 || wk_sbtl->battleMode == 2) && wk_sbtl->summonIndex[0] >= 0
            && wk_sbtl->stockNum > 0 && sbtlBtlNakamaSetUpSub(&player->stock[wk_sbtl->summonIndex[0]])) {
            if (sbtlCallBtlSearchUnitID34(&player->stock[wk_sbtl->summonIndex[0]],
                                          player->stock[wk_sbtl->summonIndex[0]].devilId, 0)) {
                set = 1;
                sbtlSetSummon_Unit(wk, wk_sbtl->summonIndex[0], 0, 0, 0, 2);
                sbtlSetBit(&wk_sbtl->summonBit, wk_sbtl->summonIndex[0]);
                sbtlSetDevilSkillID(wk_sbtl->stockDevilSkill[wk_sbtl->summonIndex[0]], 0);
                wk_sbtl->field630->num = 1;
            } else {
                sbtlCreateSM(wk_sbtl->sysmsgList, wk, 53, 0);
            }
        }
        if (set != 0) {
            return 0;
        }
        if (wk_sbtl->battleMode == 3) {
            return 0;
        }
        wk->color = 0;
        wk->mode = 5;
        wk->vstatus |= 0x200000;
    } else {
        wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (wk == NULL) {
            return 0;
        }
        if (wk->mode < 4) {
            return 0;
        }
        if (wk_sbtl->field344 > 0) {
            wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
            if (wk == NULL) {
                return 0;
            }
            if (wk->mode < 4) {
                return 0;
            }
            if (!sbtlLoadEffectData_Decode(NULL)) {
                return 0;
            }
            if (!sbtlLoadAttackHitData_Decode(NULL)) {
                return 0;
            }
            if (!sbtlRefSbtl62(NULL)) {
                return 0;
            }
            wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
            if (wk == NULL) {
                return 0;
            }
            if (!sbtlCheckDamageTarget(wk)) {
                return 0;
            }
            wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
            if (wk == NULL) {
                return 0;
            }
            if (!sbtlCheckDamageTarget(wk)) {
                return 0;
            }
            wk_sbtl->flg |= 0x10000;
            sbtlBattleStart(&sbtlData100[wk_sbtl->stageNo]);
        } else {
            if (!sbtlLoadEffectData_Decode(NULL)) {
                return 0;
            }
            if (!sbtlLoadAttackHitData_Decode(NULL)) {
                return 0;
            }
            if (!sbtlRefSbtl62(NULL)) {
                return 0;
            }
            wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
            if (wk == NULL) {
                return 0;
            }
            if (sbtlCheckDamageTarget(wk)) {
                wk_sbtl->flg |= 0x10000;
                sbtlBattleStart(&sbtlData100[wk_sbtl->stageNo]);
            }
            return 0;
        }
    }
    bst = sbtlGetBadStatus(&wk->unit);
    if (bst != BST_BOYCOTT) {
        sbtlSetBadStatus_No(wk, 0, bst, 1, -1);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", sbtlNakamaSetUp);
#endif

INCLUDE_ASM(const s32, "code_r1_0024B788", sbtlLeaf);

s32 DrawProc(dds3ProcessID_t *id)
{
    if (dds3SearchProcessName(sbtlData85) == NULL) {
        return 0;
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    if (wk_sbtl->flg & 8) {
        if (!(wk_sbtl->flg2 & 4)) {
            sbtlCallSetOt();
            sbtlDrawAttachEPL(wk_sbtl->hitList);
        }
        return 0;
    }
    Draw_sbtlScene();
    Draw_sbtlUnit();
    sbtlRefSbtl27();
    sbtlRefSbtl15(32, 384);
    sbtlCallBtlSearchUnitID7(32, 384);
    Draw_sbtlDvltk(wk_sbtl->dvltkList);
    Draw_sbtlSTP(wk_sbtl->stp);
    sbtlRefSbtl29();
    return 0;
}

s32 sbtlSbattle6(dds3ProcessID_t *id)
{
    if (dds3SearchProcessName(sbtlData85) == NULL) {
        return 0;
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    if (!(wk_sbtl->flg & 8)) {
        sbtlHitCProc_Idou(wk_sbtl->hitList);
        sbtlCallPadVibeFadeUpdate(wk_sbtl->effList);
        sbtlReleaseEff(wk_sbtl->effList, 0);
        sbtlCallSin2();
        sbtlHitCProc(wk_sbtl->hitList);
        sbtlReleaseHitC_List(wk_sbtl->hitList, 0);
        sbtlDelProc(wk_sbtl->effdelList);
        sbtlReleaseDelList(wk_sbtl->effdelList, 0);
        sbtlCallSetOt();
        sbtlDrawAttachEPL(wk_sbtl->hitList);
        sbtlRefSbtl23();
        sbtlCallSetVarSBtlSM(wk_sbtl->sysmsgList);
    }
    return 0;
}

s32 Proc_sbtl(void)
{
    if (wk_sbtl == NULL) {
        return 0;
    }
    return dds3SearchProcessName(sbtlData85) != NULL;
}

s32 pbRefSbtl(void) {
    SBTL_WORK *wk = wk_sbtl;

    wk->endcnt = 0;
    wk->flg |= 8;
    sbtlCallBtlSearchUnitID32();
    sbtlCallBtlSearchUnitID33();
    return sbtlSetEndData();
}

s32 pbProcSequenceSub(void) {
    if (VRF_DeltaTime == 1 || pbData == 1) {
        return 1;
    }
    return 0;
}

/* NON_MATCHING: 162 of 243 words match. Endcnt clamp and early-return layout differ; vector checked (32 rows, all ret 1, only early-return and endcnt=0 paths covered). */
#ifdef NON_MATCHING
s32 pbCallBtlCheckLoadFlg(void)
{
    struct SBTLUNIT_WORK *wk;
    BST bst;

    if (!sbtlCheckLoadFlg()) {
        return 1;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk->status == 34) {
        if (!(wk_sbtl->flg & 0x1000000) && wk_sbtl->mode != 7 && wk_sbtl->mode != 8 && wk_sbtl->mode != 16) {
            wk_sbtl->flg |= 0x40000000;
            if (!(wk->unit.badStatus & 0x8000) && sbtlCheckUnit_Exec()) {
                if (!sbtlCheckTutorialBattle()) {
                    wk_sbtl->flg |= 0x20;
                }
                wk->unit.badStatus |= 0x8000;
                wk_sbtl->flg |= 8;
                sbtlInitDropWork();
                sbtlSetEndData();
                campFreeStatusModel();
                if (wk_sbtl->battleMode == 3) {
                    pbData = 1;
                }
                return 1;
            }
        }
        if (wk->status == 34) {
            wk_sbtl->endcnt = 0;
            return 1;
        }
    }
    if (wk_sbtl->flg & 0x1000000) {
        wk_sbtl->endcnt = 0;
        return 1;
    }
    if (wk->status == 40 || wk->status == 10 || wk_sbtl->mode == 7 || wk_sbtl->mode == 8
        || wk_sbtl->mode == 16 || wk_sbtl->field424 != 0
        || (sbtlChkBossEventStart_EnemySummonWait() && sbtlCallBtlCheckGeneratorGroupID())) {
        wk_sbtl->endcnt = 0;
        return 1;
    }
    wk_sbtl->flg |= 0x40000000;
    wk_sbtl->endcnt++;
    if (wk_sbtl->endcnt <= 0) {
        wk_sbtl->endcnt = 0;
    } else if (wk_sbtl->endcnt > 0x7FFF) {
        wk_sbtl->endcnt = 0x7FFF;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk_sbtl->endcnt == 1) {
        wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        bst = sbtlGetBadStatus(&wk->unit);
        if (wk->major == 0) {
            if (wk_sbtl->battleMode != 1 && wk->status != 42) {
                wk->vstatus |= 0x8000000;
                if (bst != BST_STONE && bst != BST_MOUDOKU) {
                    sbtlResetBadStatus(wk, BSTF_DEAD | BSTF_GUN | BSTF_SLEEP | BSTF_MAHUU | BSTF_BOUDO | BSTF_PANIC
                                               | BSTF_CHARM | BSTF_KOUFUN | BSTF_KAEN | BSTF_APPAKU | BSTF_KANDEN
                                               | BSTF_HYOUKETU | BSTF_KANASHIB);
                }
                if (!(wk_sbtl->flg2 & 1)) {
                    sbtlSetMotion_sbtlUnit(wk, 12, 0, 0, 0);
                }
                sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
            }
            sbtlCallBtlGetItemtbl();
        } else {
            if (bst != BST_STONE && bst != BST_MOUDOKU) {
                sbtlResetBadStatus(wk, BSTF_DEAD | BSTF_GUN | BSTF_SLEEP | BSTF_MAHUU | BSTF_BOUDO | BSTF_PANIC
                                           | BSTF_CHARM | BSTF_KOUFUN | BSTF_KAEN | BSTF_APPAKU | BSTF_KANDEN
                                           | BSTF_HYOUKETU | BSTF_KANASHIB);
            }
            sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
            sbtlSetMotion_sbtlUnit(wk, 4, 0, 0, 0);
        }
        sbtlSetAction(wk, 42, 0);
        sbtlSetEndBadStatus();
    }
    if (wk_sbtl->endcnt < 180) {
        if (!sbtlCheckNextAction(wk) || wk->field3DE <= 0) {
            return 1;
        }
    }
    if ((wk_sbtl->flg2 & 1) && wk_sbtl->endcnt < 180) {
        return 0;
    }
    if (sbtlCheckUnit_Exec()) {
        wk_sbtl->endcnt = 0;
        wk_sbtl->flg |= 8;
        sbtlCallBtlCheckTutorialBattle2();
        sbtlCallBtlSearchUnitID32();
        sbtlCallBtlSearchUnitID33();
        sbtlSetEndData();
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B788", pbCallBtlCheckLoadFlg);
#endif

void pbSetGameCamera0(void)
{
    SDF_VU0_COPYVECTOR(&pbWork3, &aViewDown);
    SDF_VU0_COPYVECTOR(&pbWork2, &aViewEye);
    SDF_VU0_COPYVECTOR(&pbWork, &aViewTarget);
}

void pbLeaf19(void)
{
    SDF_VU0_COPYVECTOR(&aViewDown, &pbWork3);
    SDF_VU0_COPYVECTOR(&aViewEye, &pbWork2);
    SDF_VU0_COPYVECTOR(&aViewTarget, &pbWork);
}

s32 Init_sbtlScene(void) {
    dds3Basic_t *scene;

    if (wk_sbtl == NULL || !(wk_sbtl->flg & 1)) {
        return 0;
    }
    scene = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
    if (scene == NULL) {
        return 0;
    }
    dds3ObjectBaseSetPadControl(scene, NULL);
    return 1;
}

void pbCallBtlGetRndEncJoken(s32 id) {
    datRndEncTbl_t *tbl;

    pbCallBtlGetRndEncJokenData = id;
    tbl = &sbtlData101[id][sbtlGetRndEncJoken(id)];
    sbtlSceneLoadData = tbl->mapNo[0];
    sbtlSceneLoadData2 = tbl->mapNo[1];
    sbtlSceneLoadData3 = 1;
}
