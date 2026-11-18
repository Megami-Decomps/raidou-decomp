#include "common.h"
#include "dds3Basic.h"
#include "dds3obj.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"
#include "sBtlEff.h"
#include "sBtlSound.h"
#include "sBtlScript.h"
#include "sbtlDMG.h"
#include "sBtlCalc.h"
#include "sBtlSM.h"
#include "sBtlItem.h"
#include "sdatUnit.h"
#include "dat.h"
#include "sBtlEXP.h"
#include "sBtlEncount.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "mdlManager.h"
#include "sdfDelayedResrc.h"
#include "libsdf.h"
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
#include "sdk_libc.h"
#include "fpu.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "dds3KernelMain.h"
#include "fld.h"
#include "sBtlPlayer.h"
#include "dds3PadManager.h"
#include "scrScriptProcess.h"
#include "itfMesManager.h"
#include "sBtlTalk.h"
#include "sdfVu0.h"
#include "newdata.h"
#include "code_r1_0027CBE8.h"
#include "sBtlPanel.h"
#include "AW_XAtkPad.h"
#include "EventBit.h"
#include "fldProcess.h"
#include "sdfMath.h"
#include "scrTraceCode.h"
#include "dds3KernelDraw.h"

/* NON_MATCHING: 0 of 67 words match. Retail copies sbtlSetVibeTable onto the stack and indexes the copy; this reads the table in place, vector checked (6 rows, table reads). */
#ifdef NON_MATCHING
void sbtlSetVibe(SBTLPLAYER_VTYPE mode)
{
    const SBTLPLAYER_VIBE *vibe = &sbtlSetVibeTable[mode];

    SetVibe(vibe->in, vibe->life, vibe->out, vibe->power);
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlSetVibe);
#endif

s32 sbtlGetPlayerID(void) {
    s32 id;
    struct SBTLUNIT_WORK *unit;

    if (wk_sbtl->flg & 0x4000000) {
        id = wk_sbtl->unitPadID;
    } else {
        id = wk_sbtl->targetUnitID;
    }
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, id);
    if (unit != NULL && sbtlCheckDamageTarget(unit) != 0) {
        if (wk_sbtl->mode != 6) {
            return id;
        }
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->scrWarp[0]);
        if (unit != NULL && sbtlCheckDamageTarget(unit) != 0) {
            return wk_sbtl->scrWarp[0];
        }
    }
    return wk_sbtl->targetUnitID;
}

/* NON_MATCHING: 14 of 29 words match. retail keeps the first search result in v0 to the exit and copies it to s1 in a delay slot, reloading wk_sbtl into a2. Logic as retail. */
#ifdef NON_MATCHING
struct SBTLUNIT_WORK *sbtlGetPlayerUnit(s32 id) {
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, id);
    struct SBTLUNIT_WORK *target;

    if (wk_sbtl->mode == 6) {
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->scrWarp[0]);
        if (target != NULL && sbtlCheckDamageTarget(target) != 0) {
            unit = target;
        }
    }
    return unit;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlGetPlayerUnit);
#endif

s32 sbtlCheckPad(void)
{
    s32 i;
    struct SBTLUNIT_WORK *unit;

    if (wk_sbtl == NULL || wk_sbtl->unitList == NULL) {
        return 0;
    }
    if (wk_sbtl->flg & 2) {
        return 0;
    }
    if (wk_sbtl->flg & 0x4000000) {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->unitPadID);
        if (unit != NULL && sbtlCheckDamageTarget(unit)) {
            dds3ObjectBaseSetPadControl(unit->pBasic, nakamamode_UnitPad);
            wk_sbtl->flg |= 2;
            sbtlSetScrollID(unit->field1A0);
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
            if (unit != NULL && (unit->field414 & 0x80) && unit->mode == 8) {
                dds3ObjectBaseSetPadControl(unit->pBasic, NULL);
            }
        }
        return 1;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && (unit->field414 & 0x80) && unit->mode == 8) {
            dds3ObjectBaseSetPadControl(unit->pBasic, nakamamodeCallGetActiveScene);
            wk_sbtl->flg |= 2;
            wk_sbtl->targetUnitID = unit->field1A0;
            sbtlSetScrollID(unit->field1A0);
            return 1;
        }
    }
    return 1;
}

s32 sbtlResetAction(struct SBTLUNIT_WORK *wk)
{
    wk->vstatus &= 0xF67DFFFF;
    wk->flg &= 0xFFFFBEEE;
    wk->flg2 &= ~0x20;
    wk->model->flags &= ~0x4200;
    FalseHuuma(wk);
    wk_sbtl->mode = 1;
    wk_sbtl->gstat[0] = 0;
    wk_sbtl->gstat[1] = 0;
    wk_sbtl->field484 = 0;
    wk_sbtl->field494 = 0;
    wk_sbtl->field48C = 0;
    wk_sbtl->walk_counter = 0;
    wk_sbtl->field498 = 0;
    wk_sbtl->field50A = 0;
    wk_sbtl->field618 = 0;
    wk_sbtl->field620 = 0;
    wk->field4A0 = 0;
    sbtlRestartMotion(wk);
    wk->vstatus |= 0x80;
    wk->field474 = 0;
    sbtlResetCharge(wk);
    DeleteAll(wk);
    return 1;
}

s32 sbtlRefSbtl36(void) {
    wk_sbtl->modebk = wk_sbtl->mode;
    wk_sbtl->mode = 7;
    return 1;
}

s32 sbtlActiveAction(void) {
    wk_sbtl->mode = wk_sbtl->modebk;
    return 1;
}

void sbtlCallBtlGetItemtbl(void) {
    SBTLITEM_TBL *item;

    if (wk_sbtl->battleMode != 3) {
        sbtlProcSub(wk_sbtl->effList, 144, 1);
        sbtlProcSub(wk_sbtl->effList, 145, 1);
        sbtlProcSub(wk_sbtl->effList, 146, 1);
        sbtlProcSub(wk_sbtl->effList, 147, 1);
        sbtlProcSub(wk_sbtl->effList, 148, 1);
        item = sbtlGetItemtbl(wk_sbtl->equipBullet);
        sbtlProcSub(wk_sbtl->effList, item->sklid, 1);
    }
}

/* NON_MATCHING: 14 of 73 words match. frame layout: retail has the VU0 direction at sp+0 and the zeroed offset vector at sp+16 (reloaded with a fresh addiu a0,sp,16), mine swaps them whatever the declaration order (u128 dir via _MEMOPS, f32[4] and sdfVector4_t ofs tried). Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallMemset8(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *target) {
    sdfVector4_t ofs;
    u128 dir;

    memset(&ofs, 0, sizeof(ofs));
    sbtlGetRotateDeg(&wk->rot, 90.0f);
    SDF_VU0_STOREVECTOR_MEMOPS(dir);
    ofs.z = -(wk->model->field120 + target->model->field11C);
    SDF_VU0_LOADVECTOR_B(&ofs);
    SDF_VU0_LOADVECTOR_MEMOPS(dir);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&wk->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&target->pos);
    SDF_VU0_LOADVECTOR(&wk->rot);
    SDF_VU0_STOREVECTOR(&target->rot);
    if (!(target->flg & 0x20)) {
        sbtlSetMotion_sbtlUnit(target, 0, 0, 0, 0);
    }
    target->flg &= ~1;
    wk_sbtl->modebk = 1;
    wk->flg &= ~0x800000;
    wk->model->flags &= ~0x20;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallMemset8);
#endif

void sbtlResetCombine(void) {
    wk_sbtl->gunskillid = 0;
}

/* NON_MATCHING: 27 of 48 words match. retail sets v0 = 1 at a join after the mode 1 arm and the default reaches the epilogue with the compare constant. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallObjectBaseSetPadControl(s32 arg, struct SBTLUNIT_WORK *wk, s32 mode) {
    switch (mode) {
    case 0:
        wk_sbtl->unitPadID = wk->field1A0;
        wk_sbtl->flg = (wk_sbtl->flg | 0x4000000) & ~2;
        wk->flg &= ~6;
        wk->vstatus |= 0x4000;
        return 1;
    case 1:
        wk_sbtl->flg &= ~0x4000002;
        wk->flg |= 6;
        dds3ObjectBaseSetPadControl(wk->pBasic, NULL);
        wk->vstatus &= ~0x4000;
        return 1;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallObjectBaseSetPadControl);
#endif

s32 sbtlLeaf63(struct SBTLUNIT_WORK *wk) {
    u32 status = wk->status;

    if (status >= 7 && (status < 11 || status == 34)) {
        return 0;
    }
    return 1;
}

s32 sbtlNormalProcNakamaSub(struct SBTLUNIT_WORK *wk)
{
    return wk->status < 3;
}

INCLUDE_RODATA(const s32, "code_r1_00259A88", sbtlSetVibeTable);

s32 sbtlCallBtlResetCharge(struct SBTLUNIT_WORK *wk, s32 type)
{
    switch (type) {
    case 1:
        wk_sbtl->field614 = 2;
        if (wk_sbtl->battleMode != 1) {
            wk->skillid = SKLID_NOUSE_SORD1;
        } else {
            wk->skillid = SKLID_HADAKA1;
        }
        sbtlRefSbtl11(wk, 1);
        wk_sbtl->field494 = 0;
        wk_sbtl->field484 = 0;
        wk_sbtl->field488 = 0;
        wk_sbtl->field48C = 0;
        wk_sbtl->field490 = 0;
        wk_sbtl->mode = 3;
        wk->model->field104 = -1;
        sbtlCallBtlSearchUnitID11(wk, 1);
        break;
    case 2:
        wk_sbtl->field614 = 1;
        if (wk_sbtl->battleMode != 1) {
            wk->skillid = SKLID_NOUSE_SORD2;
        } else {
            wk->skillid = SKLID_HADAKA2;
        }
        sbtlRefSbtl11(wk, 2);
        wk_sbtl->field494 = 0;
        wk_sbtl->field484 = 0;
        wk_sbtl->field488 = 0;
        wk_sbtl->field48C = 0;
        wk_sbtl->field490 = 0;
        wk_sbtl->mode = 3;
        wk->model->field104 = -1;
        sbtlCallBtlSearchUnitID11(wk, 1);
        break;
    case 3:
        wk_sbtl->field614 = 0;
        if (wk_sbtl->battleMode != 1) {
            wk->skillid = SKLID_NOUSE_SORD3;
        } else {
            wk->skillid = SKLID_HADAKA3;
        }
        sbtlRefSbtl11(wk, 3);
        wk_sbtl->field494 = 0;
        wk_sbtl->field484 = 0;
        wk_sbtl->field488 = 0;
        wk_sbtl->field48C = 0;
        wk_sbtl->field490 = 0;
        wk_sbtl->mode = 3;
        wk->model->field104 = -1;
        sbtlCallBtlSearchUnitID11(wk, 1);
        break;
    case 5:
        if (wk_sbtl->battleMode == 1) {
            break;
        }
        wk_sbtl->field614 = 0;
        wk->skillid = SKLID_TUKI;
        sbtlRefSbtl11(wk, 5);
        wk_sbtl->field494 = 0;
        wk_sbtl->field484 = 0;
        wk_sbtl->field488 = 0;
        wk_sbtl->field48C = 0;
        wk_sbtl->field490 = 0;
        wk_sbtl->mode = 3;
        wk->model->field104 = -1;
        sbtlCallBtlSearchUnitID11(wk, 0);
        break;
    case 4:
        if (wk_sbtl->battleMode != 1) {
            sbtlCallMemset17(wk_sbtl->effList, wk, 38, 0, &wk->pos.x, 0x80808080, 0);
            sbtlResetAction(wk);
            sbtlSetAction(wk, 1, 0);
            sbtlResetCharge(wk);
            wk_sbtl->field4AC = 20;
            wk_sbtl->field4A8 = 2;
            wk->vstatus |= 0x1000000;
        }
        break;
    case 6:
        if (wk_sbtl->battleMode == 1) {
            break;
        }
        if (sbtlCallBtlGetPlayerData3() > 0) {
            wk_sbtl->mode = 4;
            wk_sbtl->field484 = 0;
            wk_sbtl->field488 = 0;
            wk_sbtl->field48C = 0;
            wk_sbtl->field494 = 0;
            sbtlRefSbtl11(wk, 6);
        } else if (sbtlCallBtlGetPlayerData3() <= 0) {
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, 3, 0);
        }
        break;
    case 7:
        wk_sbtl->mode = 13;
        wk_sbtl->field484 = 0;
        wk_sbtl->field488 = 0;
        wk_sbtl->field494 = 0;
        sbtlSetAction(wk, 15, 0);
        sbtlSetMotion_sbtlUnit(wk, 29, 0, 0, 0);
        sbtlResetCharge(wk);
        sbtlRefSbtl11(wk, 7);
        break;
    case 8:
        if (CheckAction_Nakama(wk) && BeginSetsuma(wk)) {
            sbtlRefSbtl11(wk, 8);
        }
        break;
    case 9:
        if (wk_sbtl->battleMode != 1) {
            sbtlRefSbtl11(wk, 9);
        }
        break;
    case 10:
    case 11:
        wk_sbtl->mode = 1;
        wk_sbtl->gstat[0] = 0;
        wk_sbtl->gstat[1] = 0;
        sbtlResetAction(wk);
        sbtlSetAction(wk, 1, 0);
        sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        sbtlRefSbtl11(wk, 10);
        break;
    }
    return 1;
}

s32 sbtlRefSbtl10(struct SBTLUNIT_WORK *wk)
{
    return wk_sbtl->field750;
}

s32 sbtlRefSbtl11(struct SBTLUNIT_WORK *wk, s32 type)
{
    SBTL_REF11_DATA *data;
    s32 i;

    data = NULL;
    switch (type) {
    case 1:
        data = &sbtlData3;
        break;
    case 2:
        data = &sbtlData4;
        break;
    case 3:
        data = &sbtlData5;
        break;
    case 4:
        data = &sbtlData6;
        break;
    case 5:
        data = &sbtlData7;
        break;
    case 6:
        data = &sbtlData8;
        break;
    case 7:
        data = &sbtlData9;
        break;
    case 8:
        data = &sbtlData10;
        break;
    case 9:
        data = &sbtlData11;
        break;
    case 10:
        data = &sbtlData12;
        break;
    case 11:
        data = &sbtlData13;
        break;
    }
    wk_sbtl->field760 = 0;
    wk_sbtl->field74C = type;
    wk_sbtl->field750 = 0;
    wk_sbtl->field762 = data->field00;
    wk_sbtl->field754 = 0;
    wk_sbtl->field764 = data->field04;
    wk_sbtl->field758 = 0;
    wk_sbtl->field766 = data->field06;
    wk_sbtl->field75C = data->field1C;
    wk_sbtl->field77C = 0;
    wk_sbtl->field768 = data->field02;
    for (i = 0; i < 9; i++) {
        wk_sbtl->field76A[i] = 0;
    }
    return 1;
}

s32 DeleteAll(struct SBTLUNIT_WORK *wk)
{
    s32 i;

    wk_sbtl->field760 = 0;
    wk_sbtl->field74C = 0;
    wk_sbtl->field750 = 0;
    wk_sbtl->field754 = 0;
    wk_sbtl->field758 = 0;
    wk_sbtl->field75C = 0;
    wk_sbtl->field762 = 0;
    wk_sbtl->field764 = 0;
    wk_sbtl->field766 = 0;
    wk_sbtl->field768 = 0;
    wk_sbtl->field77C = 0;
    for (i = 0; i < 9; i++) {
        wk_sbtl->field76A[i] = 0;
    }
    return 1;
}

s32 sbtlCallDeleteAll(struct SBTLUNIT_WORK *wk)
{
    SBTL_REF11_DATA *data;
    s32 i;
    s32 j;
    s32 type;
    s32 ok;
    s32 inRange;
    s32 fail;
    s32 cnt;
    s32 start;
    s32 flag;

    data = NULL;
    wk_sbtl->field760++;
    wk_sbtl->field760 = (wk_sbtl->field760 <= 0) ? 0 : (wk_sbtl->field760 > 3000) ? 3000 : wk_sbtl->field760;
    if (wk_sbtl->field760 >= wk_sbtl->field762 + wk_sbtl->field766) {
        DeleteAll(wk);
        wk_sbtl->field750 = -1;
        wk_sbtl->field754 = -1;
        wk_sbtl->field758 = -1;
        return 0;
    }
    wk_sbtl->field750 = 0;
    for (i = 0; sbtlCallDeleteAllData[wk_sbtl->field74C][i] != 12; i++) {
        type = sbtlCallDeleteAllData[wk_sbtl->field74C][i];
        ok = 0;
        switch (type) {
        case 1:
            data = &sbtlData3;
            break;
        case 2:
            data = &sbtlData4;
            break;
        case 3:
            data = &sbtlData5;
            break;
        case 4:
            data = &sbtlData6;
            break;
        case 5:
            data = &sbtlData7;
            break;
        case 6:
            data = &sbtlData8;
            break;
        case 7:
            data = &sbtlData9;
            break;
        case 8:
            data = &sbtlData10;
            break;
        case 9:
            data = &sbtlData11;
            break;
        case 10:
            data = &sbtlData12;
            break;
        case 11:
            data = &sbtlData13;
            break;
        }
        cnt = wk_sbtl->field760;
        start = wk_sbtl->field762;
        if (cnt >= start) {
            flag = wk_sbtl->field75C & 4;
            ok = 1;
            if (!flag) {
                ok = cnt < start + wk_sbtl->field764;
            }
            if (data->field08 & 0x10000) {
                ok = 0;
                if (flag && cnt >= start + wk_sbtl->field764 && cnt <= start + wk_sbtl->field766) {
                    inRange = (u32)(cnt - (start + wk_sbtl->field764)) < (data->field08 & 0xFFFF);
                    ok = inRange == 0;
                }
            }
        }
        if (ok != 1) {
            continue;
        }
        j = 0;
        fail = 0;
        for (; j < 4; j++) {
            if (data->key[j] & 0x20000000) {
                if (!GetAnlogKeyToVecAndDir(wk, data->key[j], 2)) {
                    fail = 1;
                    break;
                }
                wk_sbtl->field77C |= data->key[j] & 0x0FFFF000;
                wk_sbtl->field758 = type;
            }
            if ((data->key[j] & 0x80000000) && !GetAnlogKeyToVecAndDir(wk, data->key[j], 0)) {
                fail = 1;
                break;
            }
            if ((data->key[j] & 0x40000000) && !GetAnlogKeyToVecAndDir(wk, data->key[j], 1)) {
                fail = 1;
                break;
            }
        }
        if (!fail) {
            if (wk_sbtl->field75C & 1) {
                if (wk_sbtl->field760 >= wk_sbtl->field762 + wk_sbtl->field768) {
                    wk_sbtl->field750 = type;
                    return 1;
                }
                wk_sbtl->field754 = type;
            }
            if (wk_sbtl->field75C & 2) {
                if (sbtlCheckNextAction(wk)) {
                    wk_sbtl->field750 = type;
                    return 1;
                }
            } else if (wk_sbtl->field75C & 4) {
                if (wk_sbtl->field760 >= wk_sbtl->field762 + wk_sbtl->field764 &&
                    wk_sbtl->field760 <= wk_sbtl->field762 + wk_sbtl->field766) {
                    wk_sbtl->field750 = type;
                    return 1;
                }
            }
        } else if ((wk_sbtl->field75C & 1) && wk_sbtl->field760 >= wk_sbtl->field762 + wk_sbtl->field768) {
            if (wk_sbtl->field758 > 0) {
                wk_sbtl->field750 = wk_sbtl->field758;
                return 1;
            }
            if (wk_sbtl->field754 > 0) {
                wk_sbtl->field750 = wk_sbtl->field754;
                return 1;
            }
        }
    }
    return 0;
}

s32 GetAnlogKeyToVecAndDir(struct SBTLUNIT_WORK *wk, u32 key, s32 type)
{
    s32 ok;

    ok = 1;
    switch (type) {
    case 0:
        if ((key & 0x8000000) && GetAnlogKey(wk, NULL) < 48.0f) {
            ok = 0;
        }
        if ((key & 0x1000) && !dds3PadButtonState[38]) {
            ok = 0;
        }
        if ((key & 0x2000) && !dds3PadButtonState[39]) {
            ok = 0;
        }
        if ((key & 0x4000) && !dds3PadButtonState[37]) {
            ok = 0;
        }
        if ((key & 0x8000) && !dds3PadButtonState[36]) {
            ok = 0;
        }
        if ((key & 0x10000) && !dds3PadButtonState[34]) {
            ok = 0;
        }
        if ((key & 0x20000) && !dds3PadButtonState[35]) {
            ok = 0;
        }
        if ((key & 0x40000) && !dds3PadButtonState[33]) {
            ok = 0;
        }
        if ((key & 0x80000) && !dds3PadButtonState[32]) {
            ok = 0;
        }
        break;
    case 1:
        if ((key & 0x8000000) && !(GetAnlogKey(wk, NULL) > 48.0f && wk_sbtl->field480 < 40.0f)) {
            ok = 0;
        }
        if ((key & 0x1000) && dds3PadButtonState[38] >= 0) {
            ok = 0;
        }
        if ((key & 0x2000) && dds3PadButtonState[39] >= 0) {
            ok = 0;
        }
        if ((key & 0x4000) && dds3PadButtonState[37] >= 0) {
            ok = 0;
        }
        if ((key & 0x8000) && dds3PadButtonState[36] >= 0) {
            ok = 0;
        }
        if ((key & 0x10000) && dds3PadButtonState[34] >= 0) {
            ok = 0;
        }
        if ((key & 0x20000) && dds3PadButtonState[35] >= 0) {
            ok = 0;
        }
        if ((key & 0x40000) && dds3PadButtonState[33] >= 0) {
            ok = 0;
        }
        if ((key & 0x80000) && dds3PadButtonState[32] >= 0) {
            ok = 0;
        }
        break;
    case 2:
        if (key & 0x8000000) {
            ok = 0;
            if (!(GetAnlogKey(wk, NULL) < 48.0f)) {
                ok = 1;
                if (++wk_sbtl->field76A[0] < (key & 0xFFF)) {
                    ok = 0;
                }
            }
        }
        if (key & 0x1000) {
            if (!dds3PadButtonState[38]) {
                ok = 0;
            } else if (++wk_sbtl->field76A[1] < (key & 0xFFF)) {
                ok = 0;
            }
        }
        if (key & 0x2000) {
            if (!dds3PadButtonState[39]) {
                ok = 0;
            } else if (++wk_sbtl->field76A[2] < (key & 0xFFF)) {
                ok = 0;
            }
        }
        if (key & 0x4000) {
            if (!dds3PadButtonState[37]) {
                ok = 0;
            } else if (++wk_sbtl->field76A[3] < (key & 0xFFF)) {
                ok = 0;
            }
        }
        if (key & 0x8000) {
            if (!dds3PadButtonState[36]) {
                ok = 0;
            } else if (++wk_sbtl->field76A[4] < (key & 0xFFF)) {
                ok = 0;
            }
        }
        if (key & 0x10000) {
            if (!dds3PadButtonState[34]) {
                ok = 0;
            } else if (++wk_sbtl->field76A[5] < (key & 0xFFF)) {
                ok = 0;
            }
        }
        if (key & 0x20000) {
            if (!dds3PadButtonState[35]) {
                ok = 0;
            } else if (++wk_sbtl->field76A[6] < (key & 0xFFF)) {
                ok = 0;
            }
        }
        if (key & 0x40000) {
            if (!dds3PadButtonState[33]) {
                ok = 0;
            } else if (++wk_sbtl->field76A[7] < (key & 0xFFF)) {
                ok = 0;
            }
        }
        if (key & 0x80000) {
            if (!dds3PadButtonState[32]) {
                ok = 0;
            } else if (++wk_sbtl->field76A[8] < (key & 0xFFF)) {
                ok = 0;
            }
        }
        break;
    }
    return ok != 0;
}

s32 sbtlCallBtlGetBadStatus2(struct SBTLUNIT_WORK *wk)
{
    BST bst;

    if (dds3PadButtonState[42] >= 0 || wk_sbtl->mode == 7 || wk_sbtl->mode == 8 || wk_sbtl->mode == 16
        || (wk_sbtl->flg & 0x40000000) || wk_sbtl->battleMode == 1 || wk_sbtl->mode == 15) {
        return 1;
    }
    bst = sbtlGetBadStatus(&wk->unit);
    if (bst == BST_BOUDO || bst == BST_MAHUU) {
        return 0;
    }
    if (wk_sbtl->mode == 14) {
        return 0;
    }
    if (wk->model->flags & 0x4000) {
        return 0;
    }
    if (!sbtlLeaf63(wk)) {
        return 0;
    }
    sbtlCallBtlResetMenuPara();
    wk->vstatus |= 0x8000000;
    sbtlSetNSTOP();
    return 1;
}

void sbtlSetNSTOP(void) {
    s32 i;
    struct SBTLUNIT_WORK *unit;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && unit->hModel != NULL) {
            unit->vstatus |= 0x8000000;
        }
    }
}

void sbtlResetNSTOP(void) {
    s32 i;
    struct SBTLUNIT_WORK *unit;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && unit->hModel != NULL) {
            unit->vstatus &= ~0x8000000;
        }
    }
}

/* NON_MATCHING: 7 of 23 words match. retail keeps the result in v0 from the first delay slot and wk_sbtl in a1. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlRefSbtl12(void) {
    s32 result = 0;
    s32 no;

    if (wk_sbtl->mode == 14) {
        no = wk_sbtl->field4B6;
        result = 1;
        if (no != 4) {
            result = 1;
            if (no != 2) {
                result = 1;
                if (no != 8) {
                    if (no == 9) {
                        return 1;
                    }
                    result = 0;
                    if (no == 10) {
                        return 1;
                    }
                }
            }
        }
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlRefSbtl12);
#endif

/* NON_MATCHING: 186 of 254 words match. Retail returns 1 with bne+delay where cc1 uses bnel (return-0 reorg class), and keeps a redundant ret = 2 after RestartBattle in mode 16 that cc1 drops; mode switch case order and store order taken from the asm; pad control proc (dds3BasicControlProc_t), retail reads neither argument; the mode procs (sbtlCallBtlGetSkilltbl2, sbtlPlayerRKaitenInit, KaihiProc_Nakama, GuardProc_Nakama, sbtlLeaf3) get s16 *mode = &wk_sbtl->mode as a second, unused argument (a1 set in the call's fallthrough, also in nakamamode_UnitPad); SBTL_WORK field440/442/444/478; vector checked (32 rows: flag early returns, mode 1 with ret 0 (walk_counter reset), the 0x478 counter and the field480 store at >= 20); other modes checked against the asm only. */
#ifdef NON_MATCHING
s32 nakamamodeCallGetActiveScene(dds3PoseSet_t *pose, dds3Basic_t *basic)
{
    struct SBTLUNIT_WORK *wk;
    SBTLITEM_TBL *item;
    sdfVector4_t oldpos;
    s32 ret;
    s32 status;
    f32 length;

    if (!(wk_sbtl->flg & 2)) {
        return 0;
    }
    if (dds3GetActiveScene() == NULL) {
        return 0;
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    if (wk_sbtl->unitList == NULL) {
        return 0;
    }
    ret = 0;
    if (!(wk_sbtl->flg & 0x10000)) {
        return 0;
    }
    if (wk_sbtl->flg & 8) {
        return 0;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    wk->mv_spd.z = 0.0f;
    wk->mv_spd.x = 0.0f;
    wk->field424 = 0.0f;
    SDF_VU0_LOADVECTOR(&wk->pos);
    SDF_VU0_STOREVECTOR(&oldpos);
    SDF_VU0_STOREVECTOR(&wk->nextpos);
    if ((wk_sbtl->flg & 0x40) && wk_sbtl->mode != 7 && wk_sbtl->mode != 16 && wk_sbtl->mode != 8 &&
        !sbtlRefSbtl12()) {
        return 0;
    }
    sbtlCallBtlGetBadStatus4(wk);
    if (!sbtlCallBtlGetBadStatus3(wk)) {
        return 0;
    }
    sbtlCallBtlGetBadStatus2(wk);
    if (sbtlCallBtlCheckDamageTarget3(wk)) {
        return 1;
    }
    sbtlCallDeleteAll(wk);
    switch (wk_sbtl->mode) {
    case 1:
        sbtlCallCheckActionNakama(wk);
        if (wk_sbtl->mode == 1) {
            ret = sbtlNormalProc_Huuma(wk);
        } else {
            ret = 2;
        }
        break;
    case 8:
        ret = sbtlCallLightSet(wk);
        break;
    case 16:
        ret = 2;
        item = sbtlGetItemtbl(wk_sbtl->equipBullet);
        if (sbtlCheckUnit_Exec_R()) {
            if (sbtlLoadEffectData_IDMask(item->sklid, EFFPAC_ST_DECODE | EFFPAC_ST_ONMEM)) {
                wk_sbtl->flg2 &= ~2;
                RestartBattle();
                ret = 2;
                sbtlCallBtlGetScrollID();
                sbtlActiveAction();
                wk->vstatus &= ~0x8000000;
                sbtlResetNSTOP();
                wk_sbtl->field440 = 0;
                wk_sbtl->field442 = 0;
                sbtlResetMidashiAnim(0);
                wk_sbtl->field436 = 0;
                wk->status = 1;
            } else {
                ret = 2;
            }
        }
        break;
    case 7:
        switch (sbtlGetMenuStatus()) {
        case 1:
            sbtlCallBtlSetReturn(wk);
            break;
        case 2:
            RestartBattle();
            sbtlCallBtlGetScrollID();
            sbtlActiveAction();
            wk_sbtl->gstat[0] = 0;
            wk_sbtl->gstat[1] = 0;
            wk->vstatus &= ~0x8000000;
            sbtlResetNSTOP();
            wk_sbtl->flg2 &= ~2;
            sbtlResetMidashiAnim(0);
            sbtlResetSelectAnim(0);
            wk_sbtl->field444 = 0;
            wk_sbtl->field436 = 0;
            wk_sbtl->field440 = 0;
            wk_sbtl->field442 = 0;
            sbtlResetColor(0x80808080);
            break;
        }
        ret = 2;
        break;
    case 3:
        if (!(wk_sbtl->flg & 0x40)) {
            wk->vstatus |= 0x20000;
            ret = 2;
            sbtlCallBtlGetSkilltbl2(wk, &wk_sbtl->mode);
        }
        break;
    case 4:
        if (!(wk_sbtl->flg & 0x40)) {
            sbtlPlayerRKaitenInit(wk, &wk_sbtl->mode);
            ret = 2;
        }
        break;
    case 2:
        ret = 2;
        break;
    case 15:
        if (!(wk_sbtl->flg & 0x40)) {
            ret = sbtlLeaf3(wk, &wk_sbtl->mode);
        }
        break;
    case 13:
        if (!(wk_sbtl->flg & 0x40)) {
            ret = GuardProc_Nakama(wk, &wk_sbtl->mode);
        }
        break;
    case 11:
        if (!(wk_sbtl->flg & 0x40)) {
            ret = KaihiProc_Nakama(wk, &wk_sbtl->mode);
        }
        break;
    case 5:
        ret = sbtlCallBtlSearchUnitID8(wk);
        break;
    case 14:
        ret = sbtlCallMemset10(wk);
        break;
    }
    if (sbtlNormalProcNakamaSub(wk) || ret == 3) {
        switch (ret) {
        case 0:
            if (sbtlCallBtlGetMotionVisualData2(wk, wk->next_motionID)) {
                sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
            }
            wk_sbtl->walk_counter = 0;
            break;
        case 1:
            if (wk->vstatus & 0x1000000) {
                if (wk->next_motionno != 32) {
                    if (wk->degcnt <= 0) {
                        sbtlSetMotion_sbtlUnit(wk, 5, 0, 0, 0);
                    }
                } else if (wk->degcnt > 0) {
                    sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
                }
            } else if (wk->next_motionno != 1) {
                if (wk->degcnt <= 0) {
                    sbtlSetMotion_sbtlUnit(wk, 5, 0, 0, 0);
                }
            } else if (wk->degcnt > 0) {
                sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
            }
            break;
        }
    }
    if (wk != NULL) {
        CheckMapHit(wk, &oldpos);
    }
    length = GetAnlogKey(wk, NULL);
    wk_sbtl->field478++;
    wk_sbtl->field478 = (wk_sbtl->field478 <= 0) ? 0 : (wk_sbtl->field478 > 120) ? 120 : wk_sbtl->field478;
    if (!(wk_sbtl->field478 < 20 && !(length <= 40.0f))) {
        if (length > 40.0f) {
            if (wk_sbtl->field478 >= 20) {
                wk_sbtl->field480 = GetAnlogKey(wk, NULL);
            }
        } else if (wk_sbtl->field478 >= 20) {
            wk_sbtl->field478 = 0;
            wk_sbtl->field480 = GetAnlogKey(wk, NULL);
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", nakamamodeCallGetActiveScene);
#endif

/* NON_MATCHING: 16 of 175 words match. retail tests flg&0x10000 and flg&8 into separate temps (mine merges when written as one condition, nested ifs fix it) and keeps the flg load in a0 / mask in v1 where mine uses v1 / v0. Logic as retail. */
#ifdef NON_MATCHING
s32 nakamamode_UnitPad(dds3PoseSet_t *pose, dds3Basic_t *basic) {
    struct SBTLUNIT_WORK *unit;
    sdfVector4_t oldpos;
    s32 ret;
    s32 status;
    u32 flg;

    flg = wk_sbtl->flg;
    if ((flg & 0x10000) != 0) {
        if ((flg & 8) != 0) {
            return 0;
        }
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
        if (unit != NULL) {
            unit->mv_spd.z = 0;
            unit->mv_spd.x = 0;
            unit->field424 = 0;
            if ((wk_sbtl->flg & 0x40) == 0 || wk_sbtl->mode == 7 || wk_sbtl->mode == 16 || wk_sbtl->mode == 8) {
                SDF_VU0_LOADVECTOR(&unit->pos);
                SDF_VU0_STOREVECTOR(&oldpos);
                SDF_VU0_STOREVECTOR(&unit->nextpos);
                ret = 0;
                if (sbtlCallBtlGetBadStatus3(unit) != 0) {
                    sbtlCallBtlGetBadStatus2(unit);
                    if (sbtlCallBtlCheckDamageTarget3(unit) == 0) {
                        switch (wk_sbtl->mode) {
                        case 1:
                            ret = NormalProc_Nakama(unit);
                            break;
                        case 2:
                            ret = 2;
                            break;
                        case 3:
                            ret = AtkProc_Nakama(unit, &wk_sbtl->mode);
                            break;
                        case 7:
                            status = sbtlGetMenuStatus();
                            if (status == 1) {
                                sbtlCallBtlSetReturn(unit);
                                ret = 2;
                            } else {
                                ret = 2;
                                if (status == 2) {
                                    RestartBattle();
                                    sbtlCallBtlGetScrollID();
                                    sbtlActiveAction();
                                    wk_sbtl->gstat[1] = 0;
                                    wk_sbtl->gstat[0] = 0;
                                    sbtlResetNSTOP();
                                    wk_sbtl->flg2 &= ~2;
                                    sbtlResetMidashiAnim(0);
                                    sbtlResetSelectAnim(0);
                                    wk_sbtl->field442 = 0;
                                    wk_sbtl->field444 = 0;
                                    wk_sbtl->field436 = 0;
                                    wk_sbtl->field440 = 0;
                                    sbtlResetColor(0x80808080);
                                    ret = 2;
                                }
                            }
                            break;
                        case 8:
                            ret = sbtlCallLightSet(unit);
                            break;
                        case 11:
                            if ((wk_sbtl->flg & 0x40) == 0) {
                                ret = KaihiProc_Nakama(unit, &wk_sbtl->mode);
                            }
                            break;
                        case 13:
                            if ((wk_sbtl->flg & 0x40) == 0) {
                                ret = GuardProc_Nakama(unit, &wk_sbtl->mode);
                            }
                            break;
                        default:
                            break;
                        }
                        if (ret == 0) {
                            if (unit->next_motionID == 5) {
                                sbtlSetMotion_sbtlUnit(unit, 0, 0, 0, 0);
                            }
                        } else if (ret == 1) {
                            if (sbtlSetMotion(unit, SBTLUNIT_MOTION_WALK) != 0) {
                                sbtlSetMotion_sbtlUnit(unit, 5, 0, 0, 0);
                            }
                        }
                        if (unit != NULL) {
                            CheckMapHit(unit, &oldpos);
                        }
                        return 1;
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", nakamamode_UnitPad);
#endif

s32 CheckStop_BadStatus(struct SBTLUNIT_WORK *wk)
{
    s32 ret = 0;

    switch (sbtlGetBadStatus(&wk->unit)) {
    case BST_STONE:
    case BST_MOUDOKU:
    case BST_SLEEP:
    case BST_KIZETU:
    case BST_RAND0:
    case BST_BOYCOTT:
        ret = 1;
    case BST_CHARM:
    case BST_PANIC:
    case BST_BOUDO:
    case BST_MAHUU:
    case BST_GUN:
    case BST_KANSEN:
    case BST_HUUIN:
    case BST_DEAD:
        break;
    }
    return ret;
}

/* NON_MATCHING: x words match. 5 aligned lines: the three status tests fill their delay slots from the fallthrough where retail copies the return-0 from the target (branch-likely class), vector checked (32 rows). */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetBadStatus3(struct SBTLUNIT_WORK *wk)
{
    u32 status;

    if (wk_sbtl->mode == 8) {
        return 1;
    }
    status = wk->status;
    if (status == 34) {
        return 0;
    }
    if (status == 42) {
        return 0;
    }
    if (status == 10) {
        return 0;
    }
    switch (sbtlGetBadStatus(&wk->unit)) {
    case BST_STONE:
    case BST_BOUDO:
    case BST_MOUDOKU:
    case BST_SLEEP:
    case BST_DEAD:
    case BST_RAND0:
    case BST_BOYCOTT:
        return 1;
    case BST_CHARM:
    case BST_PANIC:
    case BST_MAHUU:
    case BST_KIZETU:
    case BST_GUN:
    case BST_KANSEN:
    case BST_HUUIN:
        return 0;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlGetBadStatus3);
#endif

/* NON_MATCHING: 42 of 46 words match. i and n swap s2/s3. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlGetRandomTarget(struct SBTLUNIT_WORK *wk, s32 *skill0, s32 *skill1) {
    s32 skills[2];
    s32 *skill;
    s32 n;
    s32 i;

    skills[0] = 0;
    skills[1] = 0;
    skill = &wk->haveskill[2];
    n = 0;
    for (i = 0; i < 10 && n < 2; i++) {
        if (!(sbtlGetSkilltbl(*skill)->type & 0x3000)) {
            skills[n++] = *skill;
        }
        skill++;
    }
    *skill0 = skills[0];
    *skill1 = skills[1];
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlGetRandomTarget);
#endif

s32 sbtlNormalProcNakamaSub2(s32 no) {
    if (no < 120) {
        return no < 118;
    }
    return 1;
}

/* NON_MATCHING: 164 of 176 words match. retail's local frame has an 8-byte hole before the two skill slots (sbtlGetRandomTarget out params at sp+40/44, mine 32/36) and its mode-13 block fills the jal slot with field488 = 0 where mine uses skillid = 0. Logic as retail. */
#ifdef NON_MATCHING
s32 NormalProc_Nakama(struct SBTLUNIT_WORK *wk) {
    SBTL_MOTION_VISUAL *visual;
    BST bst;
    f32 key;
    s32 skill0;
    s32 skill1;
    sdfVector4_t dir;
    sdfVector4_t pos;
    f32 deg;
    s32 ret;

    ret = 0;

    visual = sbtlGetVisualData(wk->major, wk->minor);
    bst = sbtlGetBadStatus(&wk->unit);
    sbtlGetRandomTarget(wk, &skill0, &skill1);
    if (sbtlNormalProcNakamaSub(wk) != 0) {
        if (dds3PadButtonState[35] != 0) {
            if (wk_sbtl->field3A8 == 0) {
                wk->skillid = SKLID_NONE;
                wk_sbtl->mode = 13;
                wk_sbtl->field494 = 0;
                wk_sbtl->field484 = 0;
                wk_sbtl->field488 = 0;
                sbtlSetAction(wk, 15, 0);
                sbtlSetMotion_sbtlUnit(wk, 29, 0, 0, 0);
                return 2;
            }
        } else if (dds3PadButtonState[0] < 0) {
            if (wk_sbtl->field484 == 0) {
                wk->skillid = SKLID_ATK;
                wk_sbtl->field484 = 1;
                wk_sbtl->mode = 3;
                return 2;
            }
        } else if (dds3PadButtonState[2] < 0) {
            sbtlGetSkilltbl(skill0);
            if (skill0 != 0) {
                if (bst != BST_MOUDOKU) {
                    wk->skillid = skill0;
                    if (sbtlCheckUseSkill(wk, skill0) == 0) {
                        return 2;
                    }
                    if (sbtlNormalProcNakamaSub2(wk->skillid) != 0) {
                        wk_sbtl->field484 = 1;
                        wk_sbtl->mode = 3;
                    }
                    return 2;
                }
            }
        } else if (dds3PadButtonState[1] < 0) {
            sbtlGetSkilltbl(skill1);
            if (skill1 != 0) {
                if (bst != BST_MOUDOKU) {
                    wk->skillid = skill1;
                    if (sbtlCheckUseSkill(wk, skill1) == 0) {
                        return 2;
                    }
                    if (sbtlNormalProcNakamaSub2(wk->skillid) != 0) {
                        wk_sbtl->field484 = 1;
                        wk_sbtl->mode = 3;
                    }
                    return 2;
                }
            }
        }
    }
    key = GetAnlogKey(wk, &dir);
    if ((wk->vstatus & 0x80) != 0) {
        key = 0.0f;
    }
    if (sbtlNormalProcNakamaSub(wk) != 0 && key >= 48.0f) {
        wk->mv_spd.z = visual->field04;
        deg = mapGetScreenDir(wk->pos.x, wk->pos.y, wk->pos.z, 0.0f);
        SetRotateMatrixY(-deg * 0.017453293f);
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_SCALEVECTOR(2.0f);
        SDF_VU0_LOADVECTOR_B(&wk->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(&pos);
        sbtlGetRotate(wk, &pos);
        ret = 1;
        UnitMV(wk, &pos);
        sbtlSetAction(wk, 2, 0);
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", NormalProc_Nakama);
#endif

/* NON_MATCHING: 156 of 163 words match. delay-slot choice: retail fills sbtlGetSkilltbl's jal slot with lw a0,skillid after the field484 store, mine hoists the lw above the lw/addiu/sw. Logic as retail. */
#ifdef NON_MATCHING
s32 AtkProc_Nakama(struct SBTLUNIT_WORK *wk, s16 *mode) {
    struct SBTLUNIT_WORK *target;
    f32 deg;

    wk_sbtl->field494++;
    wk_sbtl->field494 = (wk_sbtl->field494 <= 0) ? 0 : (wk_sbtl->field494 > 0x7FFF) ? 0x7FFF : wk_sbtl->field494;
    switch (wk_sbtl->field484) {
    case 1:
        wk->model->mode = 15;
        sbtlSetUseSkill_sbtlActAI(wk, wk->skillid, 0);
        sbtlSetAction(wk, 12, 0);
        wk_sbtl->field484++;
        if ((sbtlGetSkilltbl(wk->skillid)->area & 0x4000) != 0) {
            wk->model->field104 = wk->field1A0;
        } else {
            wk->model->field104 = sbtlCallMemset11(wk, &wk->rot, 4, 150.0f, 0.6f, 0.1f);
            if (wk->model->field104 != -1
                || (wk->model->field104 = sbtlCallMemset11(wk, &wk->rot, 3, 1800.0f, 0.6f, 0.1f), wk->model->field104 != -1)) {
                target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
                if (target != NULL) {
                    deg = sbtlGetChorikiDeg(&wk->pos, &wk->rot, &target->pos, 1.0f);
                    SDF_VU0_STOREVECTOR(&wk->rot);
                    sbtlAddUnitDeg(wk, deg);
                    FromEuler(0.0f, wk->angleY * 0.017453293f, 0.0f);
                    SDF_VU0_STOREVECTOR(&wk->rot);
                    SDF_VU0_STOREVECTOR(&wk->rotbk);
                }
            }
        }
        break;
    case 2:
        if (wk->hModel->first->frame >= 0.0f) {
            wk_sbtl->field494 = 0;
            wk_sbtl->field484 = 3;
            wk_sbtl->field48C = 0;
            sbtlCheckMHTData(wk, wk->skillid);
            sdfRemoveTaskItem(wk, 1);
        }
        break;
    case 3:
        if (sbtlCheckNextAction(wk)) {
            wk_sbtl->mode = 1;
            wk_sbtl->gstat[0] = 0;
            wk_sbtl->gstat[1] = 0;
            sbtlResetAction(wk);
            sbtlSetAction(wk, 1, 0);
            sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        }
        break;
    }
    return 2;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", AtkProc_Nakama);
#endif

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlSetReturn);

/* NON_MATCHING: 70 of 77 words match. register allocation of the field358[summonIndex[0]] == 2 test: retail hoists li a2,2 and loads wk_sbtl into a1/lbu a0, mine a0/a1. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlSetReturn(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
    f32 deg;

    if (sbtlCheckBit(&wk_sbtl->summonBit, wk_sbtl->summonIndex[0])) {
        if (wk_sbtl->field358[wk_sbtl->summonIndex[0]] == 2) {
            sbtlCreateDvltk_Priority(wk_sbtl->dvltkList, unit, SBTLDVLTK_MODE_RETURN);
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                deg = sbtlGetChorikiDeg(&wk->pos, &wk->rot, &unit->pos, 1.0f);
                SDF_VU0_STOREVECTOR(&wk->rot);
                sbtlAddUnitDeg(wk, deg);
            }
            sbtlForceDelete_SkillFlag_Acter(unit, 0x400);
            sbtlForceDelete_ID_Acter(wk_sbtl->effList, unit, EFFID_RENKEIOR);
            sbtlCallBtlSearchUnitID4(wk_sbtl->summonIndex[0], 0);
            sbtlResetBit(&wk_sbtl->summonBit, wk_sbtl->summonIndex[0]);
            if (unit->bst_effserialID2 != -1) {
                sbtlDeleteEffectData_SerialID2(wk_sbtl->effList, unit->bst_effserialID2);
                unit->bst_effserialID2 = -1;
            }
            return 1;
        }
        return 0;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlSetReturn);
#endif

s32 sbtlCallHirukoYukaGoIdx2B(u32 color) {
    fldHirukoYukaGo_Idx2_B(color);
    sbtlCallSetColor2(color);
    return 1;
}

s32 sbtlResetColor(u32 color) {
    fldHirukoYukaGo_Idx2_B(color);
    sbtlCallSetColor3(color);
    return 1;
}

/* NON_MATCHING: 57 of 111 words match. mine is 72 bytes shorter: retail reloads wk_sbtl after each sbtlSearchEffect_SAB call and keeps sbtlReleaseItemList in both arms of both tests, gcc shares them. Logic as retail. */
#ifdef NON_MATCHING
void sbtlCallBtlResetMenuPara(void) {
    SBTLITEM_TBL *item;

    sbtlSbattle3();
    sbtlRefSbtl36();
    sbtlResetMenuPara();
    sbtlCallHirukoYukaGoIdx2B(0x80303030);
    wk_sbtl->field436 = 0;
    wk_sbtl->field224 = 0;
    wk_sbtl->flg2 |= 2;
    wk_sbtl->field440 = 0;
    wk_sbtl->field442 = 0;
    sbtlCallGetProcessData43();
    sbtlSummonMenuFuncOpen00Sub(0);
    if (wk_sbtl->field230 != 0
        && (sbtlSearchEffect_SAB(wk_sbtl->effList, wk_sbtl->field230, 1) != 0
            || sbtlSearchEffect_SAB(wk_sbtl->effList, wk_sbtl->field230, 2) != 0
            || sbtlSearchEffect_SAB(wk_sbtl->effList, wk_sbtl->field230, 5) != 0)) {
        sbtlReleaseItemList(wk_sbtl->itemList);
    } else {
        sbtlReleaseItemList(wk_sbtl->itemList);
        sbtlEntryItemList(wk_sbtl->itemList);
    }
    if (wk_sbtl->equipBulletBk != 0
        && ((item = sbtlGetItemtbl(wk_sbtl->equipBulletBk), sbtlSearchEffect_SAB(wk_sbtl->effList, item->sklid, 1) != 0)
            || sbtlSearchEffect_SAB(wk_sbtl->effList, item->sklid, 2) != 0
            || sbtlSearchEffect_SAB(wk_sbtl->effList, item->sklid, 5) != 0)) {
        sbtlReleaseItemList(wk_sbtl->bulletList);
    } else {
        sbtlReleaseItemList(wk_sbtl->bulletList);
        sbtlEntryItemList_Type(wk_sbtl->bulletList, 4);
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlResetMenuPara);
#endif

f32 GetAnlogKey(struct SBTLUNIT_WORK *wk, sdfVector4_t *dir)
{
    sdfVector4_t key;
    sdfVector4_t norm;
    f32 length;
    BST bst;
    SBTL_WORK *w;

    memset(&key, 0, sizeof(key));
    key.w = 1.0f;
    bst = sbtlGetBadStatus(&wk->unit);
    w = wk_sbtl;
    if (!(w->flg & 0x40)) {
        if (((wk->field414 & 0x80) || (w->battleMode == 3 && (wk->field414 & 1))) && bst == BST_BOUDO) {
            key.x = -(dds3PadStickState[2] - 128.0f);
            key.z = dds3PadStickState[3] - 128.0f;
        } else {
            key.x = dds3PadStickState[2] - 128.0f;
            key.z = -(dds3PadStickState[3] - 128.0f);
        }
    }
    SDF_VU0_LOADVECTOR(&key);
    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&norm);
    if (length < 48.0f) {
        key.x = 0.0f;
        key.z = 0.0f;
        if (dds3PadButtonState[5]) {
            key.x = 128.0f;
        }
        if (dds3PadButtonState[4]) {
            key.x = -128.0f;
        }
        if (dds3PadButtonState[6]) {
            key.z = 128.0f;
        }
        if (dds3PadButtonState[7]) {
            key.z = -128.0f;
        }
        if (((wk->field414 & 0x80) || (w->battleMode == 3 && (wk->field414 & 1))) && bst == BST_BOUDO) {
            key.x = -key.x;
            key.z = -key.z;
        }
        SDF_VU0_LOADVECTOR(&key);
        SDF_VU0_GETLENGTH(length);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&norm);
    }
    if (dir != NULL) {
        SDF_VU0_LOADVECTOR(&norm);
        SDF_VU0_STOREVECTOR(dir);
    }
    return length;
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallLightSet);

/* NON_MATCHING: 65 of 222 words match. Same flow and calls; cc1 keeps &wk->pos, &enemy->pos and 1.0f in callee-saved registers across the sbtlGetChorikiDeg pairs where retail rematerializes them, which adds s4 and shifts every register; returns s8 (retail sign-extends the result), UnitMV takes the target vector (all five retail callers pass it), vector checked (32 rows: idle and move paths, returns 0 and 1). */
#ifdef NON_MATCHING
s8 sbtlNormalProc_Huuma(struct SBTLUNIT_WORK *wk)
{
    sdfVector4_t key;
    sdfVector4_t target;
    f32 length;
    s32 input;
    s32 id;
    s32 ret;
    s32 result;
    f32 deg;
    f32 speed;
    sdfVector4_t *rot;
    struct SBTLUNIT_WORK *enemy;

    result = 0;
    sbtlGetVisualData(wk->major, wk->minor);
    length = GetAnlogKey(wk, &key);
    input = ChkHuuma_PlayerInputOk(wk);
    if ((sbtlNormalProcNakamaSub(wk) || (wk->status == 15 && wk_sbtl->field484 == 4)) && length >= 48.0f) {
        speed = wk->max_mvspd.z;
        if (input == 4) {
            speed *= 0.14f;
        }
        wk->mv_spd.z = speed;
        SetRotateMatrixY(-mapGetScreenDir(wk->pos.x, wk->pos.y, wk->pos.z, 0.0f) * 0.017453293f);
        SDF_VU0_LOADVECTOR(&key);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_SCALEVECTOR(2.0f);
        SDF_VU0_LOADVECTOR_B(&wk->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&target);
        result = 1;
        sbtlGetRotate(wk, &target);
        UnitMV(wk, &target);
        sbtlSetAction(wk, 2, 0);
        wk_sbtl->wait_counter = 0;
    }
    if (input == 0 && sbtlNormalProcNakamaSub(wk) && !(wk->vstatus & 0x80) && dds3PadButtonState[35] != 0 &&
        (wk_sbtl->field344 <= 0 || dds3PadButtonState[42] == 0)) {
        wk_sbtl->field494 = 0;
        wk_sbtl->mode = 13;
        wk_sbtl->field484 = 0;
        wk_sbtl->field488 = 0;
        sbtlSetAction(wk, 15, 0);
        sbtlSetMotion_sbtlUnit(wk, 29, 0, 0, 0);
        sbtlResetCharge(wk);
        return 2;
    }
    if (sbtlNormalProcNakamaSub(wk) && dds3PadButtonState[33] != 0) {
        id = sbtlCallBtlCheckDamageTarget4(wk);
        if (id != -1) {
            wk_sbtl->huumaTargetID = id;
            if (CheckHuuma_Moon(wk) && CheckHuuma2(wk)) {
                wk_sbtl->mode = 14;
                wk->model->field104 = wk_sbtl->huumaTargetID;
                wk_sbtl->field4BE = 0;
                wk_sbtl->field4B6 = 0;
                wk_sbtl->field4BC = 0;
                sbtlCheckEffectData(wk, SKLID_HUUMAST, 1, 0, wk->field414, -1, 0);
                enemy = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
                if (enemy != NULL) {
                    if (sbtlCheckDamageTarget(enemy)) {
                        rot = &wk->rot;
                        deg = sbtlGetChorikiDeg(&wk->pos, rot, &enemy->pos, 1.0f);
                        SDF_VU0_STOREVECTOR(rot);
                        sbtlAddUnitDeg(wk, deg);
                        rot = &enemy->rot;
                        deg = sbtlGetChorikiDeg(&enemy->pos, rot, &wk->pos, 1.0f);
                        SDF_VU0_STOREVECTOR(rot);
                        sbtlAddUnitDeg(enemy, deg);
                        sbtlSetAction(enemy, 35, 0);
                        sbtlSetMotion_sbtlUnit(wk, 35, 0, 0, 0);
                        enemy->flg |= 0x4000;
                    }
                }
                return 2;
            }
        }
        if (wk_sbtl->flg & 0x400) {
            return 2;
        }
    }
    ret = sbtlCallGetAnlogKey(wk, input);
    if (ret != -1) {
        result = ret;
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlNormalProc_Huuma);
#endif

/* NON_MATCHING: 24 of 294 words match. Flow walked against the asm branch by branch (case 3 TUKI early exit returns 2); cc1 allocates s0/s1 swapped and lays out both switches differently; vector checked (32 rows, all input 0 without buttons, return -1). */
#ifdef NON_MATCHING
s32 sbtlCallGetAnlogKey(struct SBTLUNIT_WORK *wk, s32 input)
{
    s32 ret;
    s32 move;
    f32 length;

    ret = -1;
    length = GetAnlogKey(wk, NULL);
    move = 0;
    switch (input) {
    case 3:
    case 5:
        move = 1;
        break;
    case 0:
        if (dds3PadButtonState[32] & 0x80) {
            move = 1;
        }
        break;
    }
    if (move == 1 && wk_sbtl->battleMode != 1 && GetAnlogKey(wk, NULL) > 48.0f && wk_sbtl->field480 < 40.0f &&
        sbtlNormalProcNakamaSub(wk) && (dds3PadButtonState[32] & 0x80) && (wk_sbtl->mode != 3 || wk_sbtl->field614 != 0)) {
        wk_sbtl->mode = 3;
        wk_sbtl->field614 = 0;
        wk->skillid = SKLID_TUKI;
        wk_sbtl->field494 = 0;
        wk_sbtl->field484 = 0;
        wk_sbtl->field488 = 0;
        wk_sbtl->field48C = 0;
        wk_sbtl->field490 = 0;
        sbtlRefSbtl11(wk, 5);
        wk->model->field104 = -1;
        sbtlCallBtlSearchUnitID11(wk, 0);
        return 2;
    }
    switch (input) {
    case 1:
        sbtlCallMemset17(wk_sbtl->effList, wk, 38, 0, &wk->pos.x, 0x80808080, 0);
        break;
    case 0:
        if (wk_sbtl->field498 == 0 && (sbtlNormalProcNakamaSub(wk) || wk->status == 15)) {
            if (!(dds3PadButtonState[32] & 0x80) && (dds3PadButtonState[32] == 0 || wk_sbtl->battleMode == 1)) {
                if ((dds3PadButtonState[34] & 0x80) && wk_sbtl->battleMode != 1) {
                    if (sbtlCallBtlGetPlayerData3() > 0 && wk_sbtl->battleMode != 1) {
                        wk_sbtl->field494 = 0;
                        wk_sbtl->mode = 4;
                        wk_sbtl->field484 = 0;
                        wk_sbtl->field488 = 0;
                        wk_sbtl->field48C = 0;
                        ret = 2;
                        sbtlRefSbtl11(wk, 6);
                    } else if (sbtlCallBtlGetPlayerData3() <= 0) {
                        sbtlCreateSM(wk_sbtl->sysmsgList, wk, 3, 0);
                    }
                }
            } else {
                sbtlRefSbtl14(wk);
            }
        }
        break;
    case 2:
        wk_sbtl->field4A0 = 0;
        wk_sbtl->mode = 5;
        wk_sbtl->field484 = 0;
        wk_sbtl->field4A4 = 0;
        wk_sbtl->field488 = 0;
        wk_sbtl->field494 = 0;
        wk_sbtl->field624 = 0;
        wk_sbtl->field498 = 0;
        wk_sbtl->field4B4 = 4;
        ret = 2;
        sbtlRefSbtl11(wk, 4);
        break;
    case 3:
        if (wk_sbtl->battleMode == 1 || GetAnlogKey(wk, NULL) < 48.0f ||
            (GetAnlogKey(wk, NULL) >= 48.0f && wk_sbtl->field480 > 40.0f)) {
            if (wk_sbtl->mode == 3 && wk_sbtl->field614 == 2) {
                break;
            }
            wk_sbtl->field614 = 2;
            wk->skillid = (wk_sbtl->battleMode == 1) ? SKLID_HADAKA1 : SKLID_NOUSE_SORD1;
            sbtlRefSbtl11(wk, 1);
        } else {
            if (wk_sbtl->mode == 3 && wk_sbtl->field614 == 0) {
                ret = 2;
                break;
            }
            wk_sbtl->field614 = 0;
            wk->skillid = SKLID_TUKI;
            sbtlRefSbtl11(wk, 5);
        }
        wk_sbtl->mode = 3;
        wk_sbtl->field494 = 0;
        wk_sbtl->field484 = 0;
        wk_sbtl->field488 = 0;
        wk_sbtl->field48C = 0;
        wk_sbtl->field490 = 0;
        wk->model->field104 = -1;
        if (length >= 48.0f) {
            sbtlCallBtlSearchUnitID11(wk, 0);
        } else {
            sbtlCallBtlSearchUnitID11(wk, 1);
        }
        ret = 2;
        break;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallGetAnlogKey);
#endif

s32 sbtlRefSbtl13(void) {
    SBTL_WORK *work = wk_sbtl;
    SBTL_STAGE_DATA *stage = sbtlData100;
    u32 flg;

    stage += work->stageNo;
    if (stage->flags & 0x10) {
        return 2;
    }
    flg = work->flg & 0x400000;
    return flg != 0;
}

void sbtlRefSbtl14(struct SBTLUNIT_WORK *wk) {
    wk_sbtl->field4A8 = 1;
    wk_sbtl->field4AC = 0;
}

void sbtlResetCharge(struct SBTLUNIT_WORK *wk)
{
    wk_sbtl->field4A8 = 0;
    wk_sbtl->field4AC = 0;
    wk->vstatus &= ~SBTLUNIT_VST_DISPOFFPOS;
}

/* NON_MATCHING: 65 of 66 words match. One branch: retail uses beql to the return 5 where cc1 uses beq (branch-likely class); the counter is clamped through a wk_sbtl copy, vector checked (32 rows, only the state-0 return). */
s32 ChkHuuma_PlayerInputOk(struct SBTLUNIT_WORK *wk)
{
    s32 cnt;
    SBTL_WORK *w = wk_sbtl;

    switch (w->field4A8) {
    case 1:
        if (w->battleMode != 1) {
            if (dds3PadButtonState[32] != 0) {
                cnt = ++w->field4AC;
                w->field4AC = cnt <= 0 ? 0 : (cnt < 21 ? cnt : 20);
                if (wk_sbtl->field4AC >= 20) {
                    wk_sbtl->field4A8++;
                    wk->vstatus |= 0x1000000;
                    return 1;
                }
            }
            if (dds3PadButtonState[32] == 0 && wk_sbtl->field4AC < 20) {
                sbtlResetCharge(wk);
                return 3;
            }
            return 5;
        }
        sbtlResetCharge(wk);
        return 3;
    case 2:
        return dds3PadButtonState[32] ? 4 : 2;
    }
    return 0;
}

/* NON_MATCHING: 6 of 113 words match. mine keeps the local array addresses in an extra callee-saved register (s2, 8 bytes longer) where retail rematerializes addiu v1,sp,N before every lqc2/sqc2. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallMemset9(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *target, f32 frame) {
    sdfVector4_t posA;
    sdfVector4_t posB;
    f32 a[4];
    f32 b[4];
    f32 c[4];
    f32 dot;

    memset(a, 0, sizeof(a));
    a[0] = 100.0f;
    memset(b, 0, sizeof(b));
    b[0] = 100.0f;
    memset(c, 0, sizeof(c));
    sbtlGetUnitHelperPos(wk, SBTLUNIT_MARK__1SHOT);
    SDF_VU0_STOREVECTOR(&posA);
    sbtlGetUnitHelperPos(target, SBTLUNIT_MARK__1SHOT);
    SDF_VU0_STOREVECTOR(&posB);
    a[1] = posA.y;
    b[1] = posB.y;
    c[1] = posA.y;
    SDF_VU0_LOADVECTOR(b);
    SDF_VU0_LOADVECTOR_B(c);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(b);
    SDF_VU0_LOADVECTOR(a);
    SDF_VU0_LOADVECTOR_B(c);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(a);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_INNERPRODUCT(dot);
    if (dot > 0.5f || dot < 0.0f || posB.y > posA.y) {
        return sbtlSetMotion_sbtlUnit(wk, 6, (s32)frame, 0, 1);
    }
    return sbtlSetMotion_sbtlUnit(wk, 26, (s32)frame, 0, 1);
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallMemset9);
#endif

s32 NormalProc(struct SBTLUNIT_WORK *wk) {
    s32 id = sbtlCallMemset11(wk, &wk->rot, 3, 300.0f, 0.59999997f, 0.099999995f);
    s32 id2;

    wk->model->field104 = id;
    if (id == -1) {
        id2 = sbtlCallMemset11(wk, &wk->rot, 3, 1600.0f, 0.59999997f, 0.099999995f);
        wk->model->field104 = id2;
        if (id2 == -1) {
            wk->model->field104 = sbtlCallMemset11(wk, &wk->rot, 3, 3000.0f, -1.0f, -1.0f);
        }
    }
    return wk->model->field104;
}

/* NON_MATCHING: 184 of 254 words match. cc1 keeps &wk->rot in s0 across FromEuler in step 2 where retail rematerialises it, which shifts registers; sbtlRefSbtl10 takes the unit (retail passes it); new SBTLITEM_TBL (HD first six fields, 24 bytes) and sbtlGetItemtbl in sBtlItem.h; float literals 0.6f/0.1f give the retail bits (ee-gcc rounds them down); vector checked (32 rows, steps 1 and 2, stores match). */
#ifdef NON_MATCHING
s32 sbtlPlayerRKaitenInit(struct SBTLUNIT_WORK *wk, s16 *mode)
{
    SBTLITEM_TBL *item;
    struct SBTLUNIT_WORK *target;
    sdfVector4_t *rot;
    s32 type;
    f32 deg;

    wk_sbtl->field494++;
    wk_sbtl->field494 = (wk_sbtl->field494 <= 0) ? 0 : (wk_sbtl->field494 > 0x7FFF) ? 0x7FFF : wk_sbtl->field494;
    item = sbtlGetItemtbl(wk_sbtl->equipBullet);
    type = sbtlRefSbtl10(wk);
    switch (wk_sbtl->field484) {
    case 0:
        rot = &wk->rot;
        sbtlSetUseSkill_sbtlActAI(wk, item->sklid, 0);
        sbtlSetAction(wk, 12, 0);
        wk_sbtl->field484++;
        wk->model->field104 = sbtlCallMemset11(wk, rot, 2, 300.0f, 0.6f, 0.1f);
        if (wk->model->field104 == -1) {
            wk->model->field104 = sbtlCallMemset11(wk, rot, 3, 1600.0f, 0.6f, 0.1f);
            if (wk->model->field104 == -1) {
                break;
            }
        }
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
        if (target != NULL) {
            deg = sbtlGetChorikiDeg(&wk->pos, rot, &target->pos, 1.0f);
            SDF_VU0_STOREVECTOR(rot);
            sbtlAddUnitDeg(wk, deg);
            FromEuler(0.0f, wk->angleY * 0.017453293f, 0.0f);
            SDF_VU0_STOREVECTOR(rot);
            SDF_VU0_STOREVECTOR(&wk->rotbk);
        }
        break;
    case 1:
        if (wk->hModel->first->frame >= 0.0f) {
            wk_sbtl->field484 = 2;
            wk_sbtl->field494 = 0;
            wk_sbtl->field48C = 0;
            sbtlCheckMHTData(wk, wk->skillid);
            sdfRemoveTaskItem(wk, 1);
        }
        break;
    case 2:
        if (wk->model->field104 != -1) {
            target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
            if (target != NULL) {
                deg = sbtlGetChorikiDeg(&wk->pos, &wk->rot, &target->pos, 1.0f);
                SDF_VU0_STOREVECTOR(&wk->rot);
                sbtlAddUnitDeg(wk, deg);
                FromEuler(0.0f, wk->angleY * 0.017453293f, 0.0f);
                SDF_VU0_STOREVECTOR(&wk->rot);
                SDF_VU0_STOREVECTOR(&wk->rotbk);
            }
        }
        if (wk_sbtl->field494 >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 1) && wk_sbtl->field48C == 0) {
            wk_sbtl->field48C = 1;
            if (sbtlCallBtlGetPlayerData3() > 0 && wk_sbtl->field488 < 2) {
                wk_sbtl->field484 = 0;
                wk_sbtl->field488++;
            }
        }
        if (sbtlCheckNextAction(wk)) {
            if (sbtlCallBtlGetPlayerData3() <= 0 && wk_sbtl->field488 < 2) {
                sbtlCreateSM(wk_sbtl->sysmsgList, wk, 3, 0);
            }
            sbtlSetMotion_sbtlUnit(wk, 19, 0, 0, 0);
            wk_sbtl->field484 = 4;
            sbtlRefSbtl11(wk, 9);
        }
        break;
    case 4:
        if (type == -1) {
            if (sbtlCheckNextAction(wk)) {
                wk_sbtl->mode = 1;
                wk_sbtl->gstat[0] = 0;
                wk_sbtl->gstat[1] = 0;
                sbtlResetAction(wk);
                sbtlSetAction(wk, 1, 0);
                sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
            }
        } else {
            sbtlCallBtlResetCharge(wk, type);
        }
        break;
    }
    return 2;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlPlayerRKaitenInit);
#endif

s32 GuardProc_Nakama(struct SBTLUNIT_WORK *wk, s16 *mode)
{
    wk->flg |= 0x100;
    wk->vstatus |= SBTLUNIT_VST_GRAV;
    if (!dds3PadButtonState[3]) {
        sbtlResetAction(wk);
        sbtlSetUseSkill_sbtlActAI(wk, 0, 0);
        sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
    }
    return 2;
}

/* NON_MATCHING: 50 of 270 words match. early-return layout: retail jumps from both GetAnlogKey and map-hit checks to two out-of-line copies of the FalseHuuma/SetMotion/SetUseSkill tail (return-0 reorg class) and mine inlines them with branch-likely, shifting everything after +0x98. Logic as retail. */
#ifdef NON_MATCHING
s32 KaihiProc_Nakama(struct SBTLUNIT_WORK *wk, s16 *mode) {
    SBTL_WORK *w;
    f32 pos[4];
    sdfVector4_t dir;
    f32 speed;
    f32 sumLevelA;
    f32 sumLevelB;
    f32 cntA;
    f32 cntB;
    f32 rate;
    s32 i;
    struct SBTLUNIT_WORK *u;
    s32 level;
    s32 hit;

    wk_sbtl->field494++;
    wk_sbtl->field494 = (wk_sbtl->field494 <= 0) ? 0 : (wk_sbtl->field494 > 0x7FFF) ? 0x7FFF : wk_sbtl->field494;
    if (!(GetAnlogKey(wk, &dir) >= 48.0f) && wk_sbtl->field484 < 2) {
        sbtlResetAction(wk);
        sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        sbtlSetUseSkill_sbtlActAI(wk, SKLID_NONE, 0);
        return 2;
    }
    speed = wk->max_mvspd.z;
    SetRotateMatrixY(-mapGetScreenDir(wk->pos.x, wk->pos.y, wk->pos.z, 0.0f) * 0.017453293f);
    SDF_VU0_LOADVECTOR(&dir);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&dir);
    SDF_VU0_LOADVECTOR(&dir);
    SDF_VU0_SCALEVECTOR(speed);
    SDF_VU0_LOADVECTOR_B(&wk->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(pos);
    hit = sbtlCallBtlCheckMapHitData(pos[0], pos[2]);
    w = wk_sbtl;
    if (hit && w->field484 < 2) {
        sbtlResetAction(wk);
        sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        sbtlSetUseSkill_sbtlActAI(wk, SKLID_NONE, 0);
        return 2;
    }
    switch (w->field484) {
    case 0:
        i = 0;
        sumLevelA = 0.0f;
        sumLevelB = 0.0f;
        cntA = 0.0f;
        cntB = 0.0f;
        if (wk_sbtl->unitList->number > 0) {
            do {
                u = wk_sbtl->unitList->pUnitL[i++].pUnit;
                if (u != NULL && sbtlCheckDamageTarget(u)) {
                    level = u->unit.level;
                    if (u->field414 & 2) {
                        cntA += 1.0f;
                        sumLevelA += (f32)level;
                    } else {
                        cntB += 1.0f;
                        sumLevelB += (f32)level;
                    }
                }
            } while (i < wk_sbtl->unitList->number);
        }
        rate = (sumLevelA / cntA - sumLevelB / cntB + 1.0f + (f32)(41 - wk->unit.now.luck) * 0.1f) * 0.5f + 2.0f;
        if (rate <= 1.0f) {
            rate = 1.0f;
        }
        wk_sbtl->field50C = 128;
        wk_sbtl->field510 = 0.0f;
        wk_sbtl->field484++;
        wk_sbtl->field514 = 128.0f / (rate * 60.0f);
        break;
    case 1:
        wk_sbtl->field510 += wk_sbtl->field514;
        if ((f32)wk_sbtl->field50C <= wk_sbtl->field510) {
            w->field510 = (f32)wk_sbtl->field50C;
            w->field484 = 2;
            sbtlCreateSM(w->sysmsgList, wk, SBTLSM_MODE_ESCAPE, SKLID_NONE);
            wk_sbtl->field494 = 0;
            wk->flg |= 0x4000;
        }
        break;
    case 2:
        if (wk_sbtl->field494 == 1) {
            dds3FadeOut(255, 255, 255, 60);
        }
        if (wk_sbtl->field494 >= 60) {
            wk_sbtl->field484++;
        }
        break;
    case 3:
        if (sbtlCheckUnit_Exec()) {
            wk_sbtl->flg |= 0x40000008;
            sbtlInitDropWork();
            sbtlSetEndData();
        }
        break;
    }
    return 2;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", KaihiProc_Nakama);
#endif

/* NON_MATCHING: 32 of 320 words match. 310 of 320 words emitted, 92 aligned diff lines: retail puts return 0 in each early branch's delay slot (reorg class, accounts for the size), keeps the 0x50A clamp as branches where cc1 uses movz, and rematerializes 1.0f around sbtlCallBtlSearchUnitID6 where cc1 keeps it in a saved FP register; the 32767 clamp only survives with an s32 copy of the counter; constant 0x3DCCCCCC written 0.1f (ee-gcc rounds it down); sdatUnitWork_t.level is s8 (lb, HD s8); VRF_DeltaTime s8 (lb; HD name looks wrong, it is a one-shot flag); SBTL_WORK field508/514/518; sbtlCallBtlSearchUnitID6 is f32 (f32); vector checked (32 rows, all take the flg & 0x1400000 early return 0); states 0-3 walked against the asm only. */
#ifdef NON_MATCHING
s32 sbtlCallBtlCheckDamageTarget3(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    s32 ret;
    s32 i;
    s32 cnt;
    f32 levelA;
    f32 levelB;
    f32 cntA;
    f32 cntB;
    f32 rate;
    f32 step;

    ret = 0;
    if (wk_sbtl == NULL) {
        return 0;
    }
    if (!(wk_sbtl->flg & 0x1400000)) {
        return 0;
    }
    if (wk_sbtl->flg & 0x40) {
        return 0;
    }
    if (wk->status == 34) {
        return 0;
    }
    if (wk_sbtl->mode == 7) {
        return 0;
    }
    if (wk_sbtl->mode == 8) {
        return 0;
    }
    if (wk_sbtl->mode == 16) {
        return 0;
    }
    if (wk_sbtl->mode == 11) {
        return 0;
    }
    wk_sbtl->field50A++;
    cnt = wk_sbtl->field50A;
    wk_sbtl->field50A = (cnt <= 0) ? 0 : (cnt > 32767) ? 32767 : cnt;
    switch (wk_sbtl->field508) {
    case 0:
        levelA = 0.0f;
        levelB = 0.0f;
        cntA = 0.0f;
        cntB = 0.0f;
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if (unit->field414 & 2) {
                    cntA += 1.0f;
                    levelA += unit->unit.level;
                } else {
                    cntB += 1.0f;
                    levelB += unit->unit.level;
                }
            }
        }
        if (cntA > 0.0f) {
            levelA = levelA / cntA;
        } else {
            levelA = 10.0f;
        }
        if (cntB > 0.0f) {
            levelB = levelB / cntB;
        } else {
            levelB = 10.0f;
        }
        if (evtBitCheck(2961)) {
            levelA = levelA + 50.0f - levelB + 1.0f;
            rate = ((levelA + (41 - wk->unit.now.luck * 10) * 0.1f) * 0.5f + 2.0f) * 5.0f;
        } else {
            levelA = levelA - levelB + 1.0f;
            rate = (levelA + (41 - wk->unit.now.luck) * 0.1f) * 0.5f + 2.0f;
        }
        wk_sbtl->field518 = rate;
        rate = rate * sbtlCallBtlSearchUnitID6(1.0f);
        if (rate <= 1.0f) {
            rate = 1.0f;
        }
        wk_sbtl->field50C = 128;
        wk_sbtl->field508++;
        wk_sbtl->field510 = 0.0f;
        wk_sbtl->field514 = 128.0f / (rate * 60.0f);
        break;
    case 1:
        rate = wk_sbtl->field518 * sbtlCallBtlSearchUnitID6(1.0f);
        if (rate <= 1.0f) {
            rate = 1.0f;
        }
        step = 128.0f / (rate * 60.0f);
        wk_sbtl->field514 = step;
        wk_sbtl->field510 += step;
        if (wk_sbtl->field50C <= wk_sbtl->field510) {
            if (VRF_DeltaTime) {
                break;
            }
            wk_sbtl->field510 = wk_sbtl->field50C;
            wk_sbtl->field508++;
            ret = 1;
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, 1, 0);
            wk_sbtl->field50A = 0;
            wk->flg |= 0x4000;
            wk_sbtl->flg |= 0x1000000;
            VRF_DeltaTime = 1;
            wk_sbtl->flg |= 0x40000000;
        }
        break;
    case 2:
        if (wk_sbtl->field50A == 1) {
            dds3FadeOut(255, 255, 255, 60);
        }
        ret = 1;
        if (wk_sbtl->field50A >= 60) {
            wk_sbtl->field508++;
        }
        break;
    case 3:
        if (sbtlCheckUnit_Exec()) {
            wk_sbtl->flg |= 0x40000008;
            sbtlInitDropWork();
            sbtlSetEndData();
        }
        ret = 1;
        break;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlCheckDamageTarget3);
#endif

/* NON_MATCHING: 59 of 62 words match. second quotient lands in f1 where retail divides in f20 (rate's register). Logic as retail. */
#ifdef NON_MATCHING
f32 sbtlCallBtlSearchUnitID6(f32 rate) {
    struct SBTLUNIT_WORK *unit;

    if (wk_sbtl->field344 > 0) {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
        if (unit != NULL && CheckLiveTarget(unit)) {
            if (sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&unit->unit, 0), &unit->unit.autost_cnt, &unit->unit.autost_cnt2) == 5) {
                rate = (f32)unit->unit.autost_cnt2 / 100.0f;
            } else if (sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&unit->unit, 1), &unit->unit.autost_cnt, &unit->unit.autost_cnt2) == 5) {
                rate = (f32)unit->unit.autost_cnt2 / 100.0f;
            }
        }
    }
    return rate;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlSearchUnitID6);
#endif

/* NON_MATCHING: x words match. Same tests in retail order; retail sets return 0 in each delay slot and keeps mode == 16 as its own block (branch-likely class). sbtlEscapeGauge gets (s16) of the f32 count as retail truncates and sign-extends, vector checked (32 rows, flg test path). */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchUnitID7(s32 x, s32 y)
{
    struct SBTLUNIT_WORK *unit;

    if (wk_sbtl == NULL) {
        return 0;
    }
    if (!(wk_sbtl->flg & 0x1400000) || (wk_sbtl->flg & 0x40)) {
        return 0;
    }
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (unit == NULL || unit->status == 34 || wk_sbtl->mode == 7 || wk_sbtl->mode == 8 || wk_sbtl->mode == 16) {
        return 0;
    }
    if (wk_sbtl->mode == 11 || (wk_sbtl->flg & 8)) {
        return 0;
    }
    sbtlEscapeGauge((s16)wk_sbtl->field510, wk_sbtl->field50C);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlSearchUnitID7);
#endif

s32 sbtlLeaf3(struct SBTLUNIT_WORK *wk, s16 *mode)
{
    return 2;
}

s32 sbtlRefSbtl15(s32 x, s32 y)
{
    if (wk_sbtl == NULL) {
        return 0;
    }
    if (wk_sbtl->mode != 14) {
        return 0;
    }
    if (wk_sbtl->flg & 8) {
        return 0;
    }
    if (wk_sbtl->field4B6 <= 0) {
        return 0;
    }
    if (wk_sbtl->field4B6 == 8 && wk_sbtl->field4BC < 26) {
        return 0;
    }
    sbtlCallBtlSprite(wk_sbtl->field4BA, wk_sbtl->field4B8);
    return 1;
}

s32 SetColor(struct SBTLUNIT_WORK *wk) {
    s32 i;

    for (i = 0; i < 12; i++) {
        if (wk_sbtl->huumaDevilID[i] == -1) {
            wk_sbtl->huumaDevilID[i] = sbtlGetDevilData_sbtlSub(wk->unit.devilId)->huumaMesNo;
            return 1;
        }
    }
    return 0;
}

/* NON_MATCHING: 48 of 69 words match. retail keeps wk in s1, unit in s0, ret in s2. Logic as retail. */
#ifdef NON_MATCHING
s32 CheckHuuma_Moon(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    s32 ret;

    ret = 1;
    if (unit != NULL) {
        if (sbtlGetDevilData_sbtlSub(unit->unit.devilId)->field0E == 7) {
            if (!sbtlCheckEvtBit_Table(8)) {
                ret = 0;
                sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_HUUMAMISS_GIKEI, SKLID_NONE);
                wk_sbtl->field4CE = 42;
            }
        } else if (sbtlCheckEvtBit_Table(8)) {
            ret = 0;
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_HUUMAFULLMOON, SKLID_NONE);
            wk_sbtl->field4CE = 39;
        }
    } else {
        ret = 0;
    }
    if (ret != 0) {
        return ret;
    }
    sbtlSbattle3();
    wk_sbtl->nego_gstat = 0;
    wk_sbtl->eventno = 3;
    wk_sbtl->eventgstat = 0;
    wk_sbtl->flg |= 0x400;
    wk_sbtl->eventListID = unit->field1A0;
    sbtlSetNSTOP();
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", CheckHuuma_Moon);
#endif

/* NON_MATCHING: 34 of 123 words match. retail keeps wk in s1, stage/target in s2, player in s3. Logic as retail. */
#ifdef NON_MATCHING
s32 CheckHuuma2(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *target;
    SBTL_PLAYER_DATA *player;
    SBTL_STAGE_DATA *stage;
    s32 *p;
    s32 n;
    s32 i;
    s32 id;
    s16 msgNo;

    n = 0;
    stage = &sbtlData100[wk_sbtl->stageNo];
    if (wk_sbtl->battleMode != 0 && wk_sbtl->battleMode != 2) {
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_HUUMAMISS_NORMAL, SKLID_NONE);
        return 0;
    }
    if (sbtlGetBadStatus(&wk->unit) == BST_MOUDOKU || (stage->flags & 6) != 0) {
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_HUUMAMISS_NORMAL, SKLID_NONE);
        return 0;
    }
    player = sbtlGetPlayerData();
    i = 11;
    p = &wk_sbtl->huumaDevilID[11];
    do {
        id = *p--;
        i--;
        if (id != -1) {
            n++;
        }
    } while (i >= 0);
    target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    if (wk_sbtl->stockNum + n >= player->partyNum) {
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_HUUMAKUDA, SKLID_NONE);
        msgNo = 41;
    } else {
        id = sbtlGetDevilData_sbtlSub(target->unit.devilId)->huumaMesNo;
        i = 11;
        p = &wk_sbtl->huumaDevilID[11];
        do {
            if (*p-- == id) {
                break;
            }
        } while (--i >= 0);
        if (i < 0) {
            return 1;
        }
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_HUUMANONE, SKLID_NONE);
        msgNo = 40;
    }
    sbtlSbattle3();
    wk_sbtl->nego_gstat = 0;
    wk_sbtl->eventno = 3;
    wk_sbtl->eventgstat = 0;
    wk_sbtl->flg |= 0x400;
    wk_sbtl->field4CE = msgNo;
    wk_sbtl->eventListID = target->field1A0;
    sbtlSetNSTOP();
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", CheckHuuma2);
#endif

s32 sbtlCallBtlCheckDamageTarget4(struct SBTLUNIT_WORK *wk)
{
    s32 i;
    s32 j;
    s32 blocked;
    struct SBTLUNIT_WORK *unit;
    SBTL_DEVIL_DATA *devil;
    sdatUnitWork_t *stock;

    blocked = 0;
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit == NULL || !sbtlCheckDamageTarget(unit) || !(unit->field414 & 2) || unit->status != 9 || !(unit->flg2 & 0x10) || (unit->unit.badStatus & 0x20000) || !(unit->m_rate <= 0.0f)) {
            continue;
        }
        if (!sbtlCheckCrossCircle_Rot(&wk->pos, &wk->rot, 200.0f, &unit->pos, unit->model->field11C, 0.0f)) {
            continue;
        }
        if (!(unit->flg & 0x10000)) {
            devil = sbtlGetDevilData_sbtlSub(unit->unit.devilId);
            for (j = 0; j < wk_sbtl->stockNum; j++) {
                stock = sbtlGetStockUnitData(j);
                if (stock != NULL && stock->devilId == devil->huumaMesNo) {
                    break;
                }
            }
            if (j >= wk_sbtl->stockNum) {
                return unit->field1A0;
            }
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, 7, 0);
            sbtlSbattle3();
            wk_sbtl->nego_gstat = 0;
            wk_sbtl->field4CE = 40;
            wk_sbtl->eventListID = unit->field1A0;
            wk_sbtl->eventno = 3;
            wk_sbtl->eventgstat = 0;
            wk_sbtl->huumaTargetID = unit->field1A0;
            wk_sbtl->flg |= 0x400;
            sbtlSetNSTOP();
            return -1;
        } else {
            blocked = 1;
        }
    }
    if (blocked == 1) {
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, 74, 0);
    }
    return -1;
}

INCLUDE_RODATA(const s32, "code_r1_00259A88", sbtlCallMemset10Table);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallMemset10);

void SetHuumaMiss(struct SBTLUNIT_WORK *wk, s16 flg) {
    wk_sbtl->field4B6 = 2;
    wk_sbtl->huumamagnone = flg;
    wk_sbtl->field4BC = 0;
    sbtlPlaySpecialSE(NULL, 3);
    sbtlDeleteEffectData_SkillID(wk_sbtl->effList, SKLID_HUUMAST);
    wk->model->field104 = -1;
    sbtlCallBtlGetSklVisualTbl2(wk, SKLID_HUUMAMIS);
    wk->flg |= 0x4000;
}

s32 FalseHuuma(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *target;

    if (wk_sbtl->field4B6 != 0 || wk_sbtl->mode == 14) {
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
        if (target != NULL) {
            sbtlSetAction(target, 9, 0);
            target->flg &= ~0x4000;
            sbtlCheckBadStatus(target);
            if (wk_sbtl->field4B6 == 1) {
                sbtlSbattle3();
                wk_sbtl->nego_gstat = 0;
                wk_sbtl->eventno = 2;
                wk_sbtl->eventListID = wk->field1A0;
                wk_sbtl->eventgstat = 0;
                wk_sbtl->flg |= 0x400;
                sbtlSetNSTOP();
            }
        }
        wk_sbtl->field4B6 = 0;
    }
    sbtlDeleteEffectData_SkillID(wk_sbtl->effList, SKLID_HUUMAST);
    return 1;
}

/* NON_MATCHING: 42 of 56 words match. last switch arm (KAITEN) jal plus shared epilogue where retail's other two arms sibcall with j. Logic as retail. */
#ifdef NON_MATCHING
void FakeRndHit(struct SBTLUNIT_WORK *wk, s32 pow) {
    switch (wk->skillid) {
    case SKLID_NOUSE_SORD3:
        if (pow == 14 && !sbtlResetJump()) {
            dds3SetQuake(8, 60);
        }
        break;
    case SKLID_OFS_BUTURI:
        if (pow == 6 && !sbtlResetJump()) {
            dds3SetQuake(9, 60);
        }
        break;
    case SKLID_NOUSE_KAITEN:
        if (pow == 4 && !sbtlResetJump()) {
            dds3SetQuake(13, 60);
        }
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", FakeRndHit);
#endif

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlGetSkilltbl2);

/* NON_MATCHING: 137 of 158 words match. retail's default arm (mode = 1) reloads wk_sbtl into v1 in a stub before the body where mine reuses the cached t0, 4 words and branch offsets. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchUnitID8(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit;

    switch (wk_sbtl->field4A4) {
    case 0:
        switch (wk_sbtl->field4B4) {
        case 0:
            wk_sbtl->field4A4 = 1;
            wk_sbtl->field498 = 0;
            wk_sbtl->field4B0 = 0;
            wk->model->field104 = -1;
            sbtlSetAction(wk, 15, 0);
            wk->skillid = SKLID_NOUSE_SORD3;
            break;
        case 1:
            wk_sbtl->field4A4 = 1;
            wk_sbtl->field4B0 = 4;
            wk_sbtl->field498 = 0;
            wk->model->field104 = -1;
            sbtlSetAction(wk, 15, 0);
            wk->skillid = SKLID_NOUSE_SORD2;
            break;
        case 3:
            wk_sbtl->field498 = 0;
            wk->model->field104 = sbtlCallMemset11(wk, &wk->rot, 0, 1600.0f, 0.75f, 0.1f);
            wk_sbtl->field4B0 = 1;
            wk_sbtl->field4A4++;
            sbtlSetAction(wk, 12, 0);
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                wk->skillid = unit->order_skillid;
            } else {
                wk->skillid = SKLID_NONE;
            }
            wk_sbtl->gunskillid = 0;
            if (wk->model->field104 != -1) {
                unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
                if (unit != NULL) {
                    sbtlCallMemset9(wk, unit, 4.0f);
                    break;
                }
            }
            sbtlSetMotion_sbtlUnit(wk, 6, 4, 0, 0);
            break;
        case 4:
            wk_sbtl->field4A4 = 1;
            wk_sbtl->field4B0 = 6;
            wk_sbtl->field498 = 0;
            sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
            wk->skillid = SKLID_NOUSE_KAITEN;
            break;
        }
        break;
    case 1:
        sbtlCallBtlSearchUnitID8Data[wk_sbtl->field4B0](wk);
        break;
    default:
        wk_sbtl->mode = 1;
        wk->m_rate = 0.5f;
        sbtlSetAction(wk, 0, 0);
        wk_sbtl->gstat[1] = 0;
        wk_sbtl->field4A4 = 0;
        wk_sbtl->gstat[0] = 0;
        break;
    }
    return 2;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlSearchUnitID8);
#endif

INCLUDE_RODATA(const s32, "code_r1_00259A88", sbtlCallMemset10Table3);

INCLUDE_RODATA(const s32, "code_r1_00259A88", sbtlCallMemset10Table4);

INCLUDE_RODATA(const s32, "code_r1_00259A88", jtbl_00457F50);

INCLUDE_RODATA(const s32, "code_r1_00259A88", sbtlCallBtlGetSkilltbl2Table);

INCLUDE_RODATA(const s32, "code_r1_00259A88", sbtlCallBtlGetSkilltbl2Table2);

INCLUDE_RODATA(const s32, "code_r1_00259A88", sbtlCallBtlGetSkilltbl2Table3);

s32 sbtlCallBtlResetCharge2(struct SBTLUNIT_WORK *wk) {
    wk_sbtl->field494++;
    wk_sbtl->field494 = (wk_sbtl->field494 <= 0) ? 0 : (wk_sbtl->field494 > 0x7FFF) ? 0x7FFF : wk_sbtl->field494;
    wk->flg2 |= 0x20;
    switch (wk_sbtl->field484) {
    case 0:
        if (wk_sbtl->field494 > 0) {
            wk_sbtl->field494 = 0;
            wk_sbtl->field484++;
            sbtlResetCharge(wk);
        }
        break;
    case 1:
        sbtlSetUseSkill_sbtlActAI(wk, wk->skillid, 0);
        sbtlSetAction(wk, 12, 0);
        wk_sbtl->field484++;
        break;
    case 2:
        if (wk->hModel->first->frame >= 0.0f) {
            wk_sbtl->field484++;
            wk_sbtl->field494 = 0;
            wk_sbtl->field490 = 0;
            wk_sbtl->field48C = 0;
            sbtlCheckMHTData(wk, wk->skillid);
        }
        break;
    case 3:
        FakeRndHit(wk, wk_sbtl->field494);
        if (sbtlCheckNextAction(wk)) {
            wk->flg2 &= ~0x20;
            wk_sbtl->mode = 1;
            wk_sbtl->gstat[0] = 0;
            wk_sbtl->gstat[1] = 0;
            sbtlResetAction(wk);
            sbtlSetAction(wk, 1, 0);
            sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        }
        break;
    case 4:
        if (sbtlCheckNextAction(wk)) {
            wk_sbtl->mode = 1;
            wk_sbtl->gstat[0] = 0;
            wk_sbtl->gstat[1] = 0;
            sbtlResetAction(wk);
            sbtlSetAction(wk, 1, 0);
            sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        }
        break;
    }
    return 2;
}

/* NON_MATCHING: 45 of 217 words match. block layout: retail emits the case 0 gstat else-branch (frame==0.5 check) out of line after the jump table and reloads model->field104 after storing it (sw/lw through a1) where mine keeps the id in a register. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchUnitID9(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_WORK *target;
    s32 id;
    f32 deg;
    SBTL_WORK *sbtl;
    SBTL_WORK *cur;

    cur = wk_sbtl;
    cur->field494++;
    cur->field494 = (cur->field494 <= 0) ? 0 : (cur->field494 > 0x7FFF) ? 0x7FFF : cur->field494;
    sbtl = wk_sbtl;
    switch (wk_sbtl->field484) {
    case 0:
        if (wk_sbtl->gstat[0] != 0) {
            wk->m_rate = 0.5f;
            if (wk_sbtl->field494 == 18) {
                sbtlCheckEffectData(wk, wk->skillid, 1, 0, wk->field414, -1, 0);
            }
            if (wk_sbtl->field494 == 20) {
                unit = sbtlSearchUnit_ID(wk_sbtl->list3E8, wk_sbtl->field3EC);
                if (unit != NULL && sbtlSearchUnit_ID(wk_sbtl->unitList, unit->field1A0) != NULL
                    && (unit->field414 & 2) != 0 && sbtlCheckDamageTarget(unit)) {
                    id = unit->field1A0;
                } else {
                    id = sbtlCallMemset11(wk, &wk->rot, 0, 1600.0f, 0.75f, 0.1f);
                }
                wk->model->field104 = id;
                if (wk->model->field104 != -1) {
                    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
                    if (unit != NULL) {
                        deg = sbtlGetChorikiDeg(&wk->pos, &wk->rot, &unit->pos, 0.5f);
                        SDF_VU0_STOREVECTOR(&wk->rot);
                        sbtlAddUnitDeg(wk, deg);
                    }
                }
            }
            if (wk_sbtl->field494 >= 26 && wk_sbtl->field488 < 5) {
                wk_sbtl->field484++;
            }
            if (wk_sbtl->field494 >= 40) {
                sbtlSetMotion_sbtlUnit(wk, 19, 4, 0, 0);
                sbtlSetAction(wk, 1, 0);
                wk_sbtl->field484 = 4;
            }
        } else if (wk->hModel->first->frame == 0.5f) {
            wk_sbtl->field484 = 3;
            sbtl->gstat[0] = 1;
            sbtl->field494 = 0;
        }
        break;
    case 1:
        if (wk->model->field104 == -1 || (target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104)) == NULL) {
            sbtlSetMotion_sbtlUnit(wk, 6, 0, 0, 1);
        } else {
            sbtlCallMemset9(wk, target, 0.0f);
        }
        wk_sbtl->field488++;
        wk_sbtl->field484++;
        break;
    case 2:
        wk_sbtl->field484++;
        break;
    case 3:
        sbtlCutMotion(wk, 4.0f);
        wk_sbtl->field494 = 17;
        wk_sbtl->field484 = 0;
        break;
    case 4:
        wk_sbtl->mode = 1;
        wk->m_rate = 0.5f;
        wk_sbtl->gstat[1] = 0;
        wk_sbtl->gstat[0] = 0;
        break;
    }
    return 2;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlSearchUnitID9);
#endif

s32 sbtlCallBtlSearchUnitID10(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit;

    wk_sbtl->field494++;
    wk_sbtl->field494 = (wk_sbtl->field494 <= 0) ? 0 : (wk_sbtl->field494 > 0x7FFF) ? 0x7FFF : wk_sbtl->field494;
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
    switch (wk_sbtl->field484) {
    case 0:
        if (unit != NULL && sbtlCheckDamageTarget(unit)) {
            wk_sbtl->field358[wk_sbtl->summonIndex[0]] = 3;
            sbtlRefSbtl24(sbtlCallMemset6(wk_sbtl->summonIndex[0], 0), 0);
            wk_sbtl->field484++;
        } else {
            wk_sbtl->field484 = 4;
        }
        break;
    case 1:
        if (sbtlCheckStatus_Move(unit)) {
            wk_sbtl->field484 = 4;
        }
        break;
    case 4:
        wk_sbtl->mode = 1;
        wk->m_rate = 0.5f;
        wk_sbtl->gstat[0] = 0;
        wk_sbtl->gstat[1] = 0;
        break;
    }
    return 2;
}

s32 sbtlCheckBit(u32 *bits, s32 bit)
{
    return (*bits >> bit) & 1;
}

void sbtlSetBit(u32 *bits, s32 no) {
    *bits |= 1 << no;
}

void sbtlResetBit(u32 *bits, s32 no) {
    *bits &= ~(1 << no);
}

s32 sbtlSetSummon_Unit(struct SBTLUNIT_WORK *wk, s32 stock, s32 flg, s8 usePos, const sdfVector4_t *pos, s8 mode) {
    SBTL_DEVIL_DATA *devil = sbtlGetDevilData_sbtlSub(wk_sbtl->stockDevilSkill[stock]);
    SBTL_MOTION_VISUAL *visual = sbtlGetVisualData(2, wk_sbtl->stockMinor[stock]);
    SBTL_UNIT_SUB *motion = sbtlGetMotionVisualData(2, wk_sbtl->stockMinor[stock], 0);
    struct SBTLUNIT_WORK *unit;
    SBTL_WORK *sbtl;
    s32 mag;

    unit = sbtlCallMemset7(1, devil->motionId, stock, mode);
    if (usePos != 0) {
        SDF_VU0_LOADVECTOR(pos);
        SDF_VU0_STOREVECTOR(&unit->pos);
    }
    sbtl = wk_sbtl;
    if (sbtl->battleMode == 3) {
        SDF_VU0_LOADVECTOR(&main_data.pos);
        SDF_VU0_STOREVECTOR(&unit->pos);
        SDF_VU0_LOADVECTOR(&main_data.rot);
        SDF_VU0_STOREVECTOR(&unit->rot);
    }
    unit->visual[0] = visual->scale;
    unit->unit.devilId = sbtl->stockDevilSkill[stock];
    unit->visual[1] = visual->scale;
    unit->field400 = devil->field0A;
    unit->visual[2] = visual->scale;
    unit->field404 = devil->aisyoId;
    unit->flg |= devil->flag | flg;
    unit->visual[3] = visual->scale;
    unit->visualKind = visual->kind;
    SDF_VU0_LOADVECTOR(&unit->pos);
    SDF_VU0_STOREVECTOR(&unit->model->vec20);
    unit->model->field12C = 1000.0f;
    unit->actMax = devil->field14;
    unit->model->field11C = visual->field10;
    unit->model->field120 = motion->field04;
    unit->model->field124 = devil->field60;
    unit->model->field128 = devil->field64;
    unit->mv_spd.z = 0;
    unit->field424 = 0;
    unit->max_mvspd.z = visual->field04;
    unit->field428 = visual->field08;
    sbtlSetactcounter(unit);
    unit->actCounter = unit->actMax;
    if (mode == 0 || mode == 2) {
        sbtlRefSbtl24(unit, 0);
    }
    mag = 0;
    if (wk_sbtl->battleMode != 3) {
        mag = sbtlCallBtlGetDevilDataSBtlSub5(&wk->unit, &unit->unit, unit->unit.devilId);
    }
    sbtlRefSbtl64(mag);
    sbtlCheckBattlePara(unit);
    return 1;
}

/* NON_MATCHING: 18 of 83 words match. retail has one sbtlCallMemset11 call with type, range and width chosen in two arms (flg 0: 200.0f, 0.4f. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchUnitID11(struct SBTLUNIT_WORK *wk, s32 flg) {
    struct SBTLUNIT_WORK *unit;
    f32 deg;

    if (wk->model->field104 == -1) {
        if (flg == 0) {
            wk->model->field104 = sbtlCallMemset11(wk, &wk->rot, 0, 200.0f, 0.39999998f, 1.0f);
        } else if (flg == 1) {
            wk->model->field104 = sbtlCallMemset11(wk, &wk->rot, 3, 400.0f, 0.78f, 1.0f);
        }
        if (wk->model->field104 == -1) {
            return 1;
        }
    }
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
    if (unit == NULL) {
        return 0;
    }
    if (!sbtlCheckDamageTarget(unit)) {
        return 0;
    }
    if (unit->vstatus & 0x10000) {
        return 0;
    }
    deg = sbtlGetChorikiDeg(&wk->pos, &wk->rot, &unit->pos, 1.0f);
    SDF_VU0_STOREVECTOR(&wk->rot);
    sbtlAddUnitDeg(wk, deg);
    FromEuler(0.0f, wk->angleY * 0.017453293f, 0.0f);
    SDF_VU0_STOREVECTOR(&wk->rot);
    SDF_VU0_STOREVECTOR(&wk->rotbk);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlSearchUnitID11);
#endif

/* NON_MATCHING: 159 of 238 words match. Only FP registers differ: cc1 hoists the 1.0f out of the target loop, retail loads it per pass; type is s8 (retail sign-extends it); para is s16[48] with a 48-byte memset (retail frame); SBTL_WORK.field40C is the temporary SBTLUNIT_LISTWORK *, SBTLUNIT_WORK.field4A2 s16; unit gains sbtlSub_0027ED08.h; vector checked (16 rows, types 2 and 3 reach sbtlCheckVerticalArea and sbtlCheckCrossCircle_Rot respectively, return -1). */
#ifdef NON_MATCHING
s32 sbtlCallMemset11(struct SBTLUNIT_WORK *wk, const sdfVector4_t *rot, s8 type, f32 range, f32 width, f32 width2)
{
    sdfVector4_t pos;
    sdfVector4_t tpos;
    sdfVector4_t pos2;
    sdfVector4_t tpos2;
    s16 para[48];
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 num;
    s32 nearest;
    s32 ok;
    s32 hit;
    s32 id;
    f32 len;
    f32 nearLen;

    memset(para, 0, 48);
    if (wk_sbtl->field40C != NULL) {
        sbtlDeleteUnitList(wk_sbtl->field40C);
        wk_sbtl->field40C = NULL;
    }
    nearLen = range;
    wk_sbtl->field40C = sbtlCreateTargetList_MASK(2, 0, 0x10000);
    nearest = -1;
    num = 0;
    for (i = 0; i < wk_sbtl->field40C->count; i++) {
        unit = wk_sbtl->field40C->pUnitL[i].pUnit;
        if (unit == NULL) {
            continue;
        }
        if (!sbtlCheckLiveTarget_NotHitDmg(wk)) {
            continue;
        }
        if (unit->vstatus & 0x10000) {
            continue;
        }
        ok = 0;
        if (type != 1) {
            if (type == 2) {
                if (unit->status != 9 && unit->field4A2 == 0) {
                    ok = 1;
                }
            } else {
                ok = 1;
            }
        }
        if (ok != 1) {
            continue;
        }
        hit = 0;
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&pos);
        SDF_VU0_STOREVECTOR(&pos2);
        SDF_VU0_LOADVECTOR(&unit->pos);
        SDF_VU0_STOREVECTOR(&tpos);
        SDF_VU0_STOREVECTOR(&tpos2);
        pos.y = 0.0f;
        tpos.y = 0.0f;
        pos2.z = 0.0f;
        tpos2.z = 0.0f;
        if (type != 2 && type != 4) {
            if (width < 1.0f && sbtlCheckCrossCircle_Rot(&pos, rot, range, &tpos, unit->model->field11C, width)) {
                hit = 1;
            } else if (width2 < 1.0f &&
                       sbtlCheckCrossCircle_Rot(&pos2, rot, range, &tpos2, unit->model->field11C, width2)) {
                hit = 1;
            }
        } else if (sbtlCheckVerticalArea(&pos, rot, &tpos, range)) {
            hit = 1;
        }
        if (hit != 1) {
            continue;
        }
        if (type >= 2 && type <= 4) {
            SDF_VU0_LOADVECTOR(&wk->pos);
            SDF_VU0_LOADVECTOR_B(&unit->pos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_GETLENGTH(len);
            if (len <= nearLen) {
                nearest = i;
                nearLen = len;
            }
        }
        para[i] = GetIntervalRand(0, 15) + 20;
        num++;
    }
    if (num == 0) {
        sbtlDeleteUnitList(wk_sbtl->field40C);
        wk_sbtl->field40C = NULL;
        return -1;
    }
    if (type >= 2 && type <= 4 && nearest != -1) {
        para[nearest] = 60;
    }
    i = sbtlGetLargePara(para, wk_sbtl->field40C->count);
    id = wk_sbtl->field40C->pUnitL[i].pUnit->field1A0;
    sbtlDeleteUnitList(wk_sbtl->field40C);
    wk_sbtl->field40C = NULL;
    return id;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallMemset11);
#endif

/* NON_MATCHING: 6 of 55 words match. retail branches bgtz over an unconditional b with li v0,-1 in its delay slot and sinks li s3,-1 into that bgtz delay slot. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchUnitID12(void) {
    s32 ret = -1;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 *haveskill;

    if (wk_sbtl->field344 <= 0) {
        return -1;
    }
    if (wk_sbtl->expList->field10 >= 300) {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
        if (unit == NULL) {
            return -1;
        }
        i = 0;
        if (sbtlCheckDamageTarget(unit)) {
            haveskill = unit->haveskill;
            do {
                if (sbtlGetSkilltbl(*haveskill)->type & 0x1000) {
                    if (datCheckSkillEnable(&unit->unit, *haveskill)) {
                        return *haveskill;
                    }
                    ret = -2;
                }
                i++;
                haveskill++;
            } while (i < 10);
        }
        return ret;
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlSearchUnitID12);
#endif

/* NON_MATCHING: 65 of 66 words match. One branch: retail tests the button byte with beq and loads wk_sbtl in the slot, cc1 uses beql; field358 (u8[12] at 0x358) is a per-stock state, vector checked (32 rows, no flag 0x40000 path). */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetBadStatus4(struct SBTLUNIT_WORK *wk)
{
    BST bst;
    s32 i;
    struct SBTLUNIT_WORK *unit;

    bst = sbtlGetBadStatus(&wk->unit);
    if (!(dds3PadButtonState[40] != 0 && sbtlCallBtlGetBadStatus3(wk) && bst != BST_BOUDO) && (wk_sbtl->flg & 0x40000)) {
        wk_sbtl->flg &= ~0x40000;
        for (i = 0; i < 12; i++) {
            if (sbtlCheckBit(&wk_sbtl->summonBit, i) && wk_sbtl->field358[i] == 2) {
                unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[i]);
                if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                    unit->actCounter = unit->actMax;
                }
            }
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlGetBadStatus4);
#endif

s32 CheckAction_Nakama(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;

    if (wk->status == 9) {
        return 0;
    }
    if (!CheckStop_BadStatus(wk)) {
        return 0;
    }
    if (wk_sbtl->field344 <= 0) {
        return 0;
    }
    sbtlGetDevilData_sbtlSub(wk_sbtl->stockDevilSkill[wk_sbtl->summonIndex[0]]);
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
    if (unit == NULL || !sbtlCheckDamageTarget(unit)) {
        return 0;
    }
    if (unit->aiwk->mode == 19 || unit->aiwk->mode == 20) {
        return 0;
    }
    return CheckStop_BadStatus(unit) != 0;
}

s32 BeginSetsuma(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
    SKLID sklid = sbtlCallBtlSearchUnitID12();

    if (sklid >= SKLID_NONE) {
        sbtlSetAction(wk, 1, 0);
        wk_sbtl->mode = 15;
        wk->model->flags |= 0x4000;
        sbtlSetMotion_sbtlUnit(wk, 35, 0, 0, 0);
        unit->actCounter = unit->actMax;
        unit->aiwk->mode = 19;
        wk_sbtl->flg &= ~0x40000;
        unit->model->stat = 1;
        unit->skillid = SKLID_NONE;
        unit->aiwk->stat = 2;
        unit->order_sklidbk = sklid;
        sbtlForceDelete_SkillFlag_Acter(unit, 0x400);
        sbtlClearHitC(wk_sbtl->hitList, unit);
        sbtlForceReset(wk_sbtl->effList, unit);
        sbtlSetAction(unit, 2, 0);
        sbtlCheckCost_SkillID(unit, sklid, 1);
        wk->flg |= 0x4010;
        unit->flg |= 0x4010;
        sbtlCreateDvltk_Priority(wk_sbtl->dvltkList, unit, SBTLDVLTK_MODE_COMBINATION);
        sbtlResetCharge(wk);
        sbtlSetMapFade_H(10, sbtlRefSbtl37(), 1);
        sbtlSbattle3();
        sbtlBeginSetsumaSub(wk);
        sbtlBeginSetsumaSub(unit);
        return 1;
    }
    if (sklid == -2) {
        sbtlCreateDvltk_Priority(wk_sbtl->dvltkList, unit, SBTLDVLTK_MODE_MAGGET);
    }
    return 0;
}

void sbtlBeginSetsumaSub(struct SBTLUNIT_WORK *wk) {
    wk->model->flags &= ~0x20;
    wk->flg &= ~0x20;
    wk->vstatus &= ~0x800;
}

s32 sbtlCallCheckActionNakama(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_WORK *devil;
    s32 i;
    BST bst;

    if (!CheckAction_Nakama(wk)) {
        return 0;
    }
    if (dds3PadButtonState[32] && dds3PadButtonState[35] && BeginSetsuma(wk)) {
        return 1;
    }
    if (!sbtlLeaf63(wk)) {
        return 0;
    }
    bst = sbtlGetBadStatus(&wk->unit);
    if (dds3PadButtonState[40] && bst != BST_BOUDO) {
        if (wk_sbtl->field64C >= 16) {
            if (!(wk_sbtl->flg & 0x40000)) {
                sbtlSetMotion_sbtlUnit(wk, 38, 0, 0, 0);
                for (i = 0; i < 12; i++) {
                    if (!sbtlCheckBit(&wk_sbtl->summonBit, i) || wk_sbtl->field358[i] != 2) {
                        continue;
                    }
                    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[i]);
                    if (unit == NULL || !sbtlCheckDamageTarget(unit) || (unit->flg & 0x800000)) {
                        continue;
                    }
                    wk_sbtl->field64C = 0;
                    sbtlForceDelete_ID_SummonFR(wk_sbtl->effList, unit);
                    wk_sbtl->field358[i] = 3;
                    devil = sbtlCallMemset6(i, 0);
                    sbtlForceDelete_SkillFlag_Acter(devil, 0x400);
                    sbtlForceDelete_ID_Acter(wk_sbtl->effList, devil, EFFID_RENKEIOR);
                    sbtlClearHitC(wk_sbtl->hitList, devil);
                    sbtlForceReset(wk_sbtl->effList, devil);
                    sbtlCallMemset17(wk_sbtl->effList, devil, 3, 0, &devil->pos.x, 0x80808080, 0);
                    devil->actCounter = devil->actMax;
                    if (devil->bst_effserialID2 != -1) {
                        sbtlDeleteEffectData_SerialID2(wk_sbtl->effList, devil->bst_effserialID2);
                        devil->bst_effserialID2 = -1;
                    }
                    bst = sbtlGetBadStatus(&devil->unit);
                    if (bst != BST_BOYCOTT) {
                        sbtlCallSetClutColor(devil, bst);
                    }
                    if (devil->field370 > 0 && !(devil->flg & 0x1000)) {
                        sbtlForceDelete_ID_Acter(wk_sbtl->effList, devil, EFFID_RENKEIOR);
                        sbtlCallMemset17(wk_sbtl->effList, devil, 55, 0, &devil->pos.x, 0x80808080, 0);
                    }
                }
            }
            wk_sbtl->flg |= 0x40000;
        }
        wk_sbtl->field64C++;
        wk_sbtl->field64C = (wk_sbtl->field64C <= 0) ? 0 : (wk_sbtl->field64C > 40) ? 40 : wk_sbtl->field64C;
    } else {
        wk_sbtl->field64C = 0;
    }
    return 1;
}

u32 sbtlRefSbtl37(void) {
    return !(wk_sbtl->scrFlag & 0x20000) ? 0x80202020 : 0x80181818;
}

/* NON_MATCHING: 28 of 181 words match. register allocation: retail holds wk s5, adr s1, skillid s6, steps s2, i s3, 255 in s4, table s7 and saves s6,s1,s5,s0,s2,s3,s4,s7. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCheckSkillEquation(struct SBTLUNIT_WORK *wk, s16 *adr, s16 skillid) {
    SBTL_SKILLEQ_STEP *steps;
    SBTLUNIT_SIDE side;
    SBTLUNIT_SIDE other;
    s32 ret;
    s32 i;

    if (skillid == SKLID_OFS_PROGRAM) {
        steps = sbtlCheckSkillEquationData;
    } else {
        steps = sbtlCheckSkillEquationData2;
    }
    side = wk->field414 & 0x3F;
    if (wk_sbtl->field404 != NULL) {
        sbtlDeleteUnitList(wk_sbtl->field404);
        wk_sbtl->field404 = NULL;
    }
    if (wk_sbtl->field408 != NULL) {
        sbtlDeleteUnitList(wk_sbtl->field408);
        wk_sbtl->field408 = NULL;
    }
    if (sbtlGetBadStatus(&wk->unit) == BST_MAHUU) {
        side = SBTLUNIT_SIDE_ENEMY;
    }
    wk_sbtl->field404 = sbtlCreateTargetList(side);
    other = SBTLUNIT_SIDE_PLAYER;
    if (side == SBTLUNIT_SIDE_PLAYER) {
        other = SBTLUNIT_SIDE_ENEMY;
    }
    wk_sbtl->field408 = sbtlCreateTargetList(other);
    if ((wk_sbtl->flg & 0x2000000) != 0) {
        wk_sbtl->field404->count = 0;
    }
    for (i = 0; steps[i].id != -1; i++) {
        SBTL_SKILLEQ_STEP *step = &steps[i];

        ret = sbtlCheckSkillEquationData3[step->id](wk, skillid);
        if (ret != 0) {
            if (step->op == 255) {
                if (step->flag == 0) {
                    goto fail;
                }
            } else if (step->op == 1) {
                if (step->flag != 0) {
                    *adr += step->value;
                }
            } else if (step->op == 2) {
                if (step->flag != 0) {
                    *adr -= step->value;
                }
            }
        } else {
            switch (step->op) {
            case 255:
                if (step->flag != 0) {
                    goto fail;
                }
                break;
            case 1:
                if (step->flag == 0) {
                    *adr += step->value;
                }
                break;
            case 2:
                if (step->flag == 0) {
                    *adr -= step->value;
                }
                break;
            }
        }
    }
    if (*adr <= 0) {
        *adr = 0;
    }
    sbtlDeleteUnitList(wk_sbtl->field404);
    sbtlDeleteUnitList(wk_sbtl->field408);
    wk_sbtl->field408 = NULL;
    wk_sbtl->field404 = NULL;
    return 1;
fail:
    *adr = -1;
    sbtlDeleteUnitList(wk_sbtl->field404);
    sbtlDeleteUnitList(wk_sbtl->field408);
    wk_sbtl->field408 = NULL;
    wk_sbtl->field404 = NULL;
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCheckSkillEquation);
#endif

/* NON_MATCHING: 34 of 90 words match. retail branches bnel s0,1 with li side in the delay slot and stores aiwk->bstside in both percentage arms. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCheckTargetEquation(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_SIDE side = wk->field414 & 0x3F;
    s32 bst = sbtlGetBadStatus(&wk->unit);
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 id;

    if ((u32)(bst - 9) < 2) {
        if (side != SBTLUNIT_SIDE_PLAYER) {
            side = SBTLUNIT_SIDE_PLAYER;
        } else {
            if (bst == BST_MAHUU || GetPercentage() >= 40) {
                side = SBTLUNIT_SIDE_ENEMY;
            }
            wk->aiwk->bstside = side;
        }
    }
    wk_sbtl->field404 = sbtlCreateTargetList(side);
    if (side == SBTLUNIT_SIDE_PLAYER) {
        id = wk->model->field108;
        if (id == -1) {
            list = sbtlCreateTargetList_PForward(SBTLUNIT_SIDE_ENEMY);
        } else {
            unit = sbtlSearchUnit_GroupID(wk_sbtl->unitList, id);
            if (unit != NULL) {
                list = sbtlCreateTargetList_Group(SBTLUNIT_SIDE_ENEMY, unit->GroupID);
            } else {
                list = sbtlCreateTargetList(SBTLUNIT_SIDE_ENEMY);
            }
        }
    } else {
        list = sbtlCreateTargetList(SBTLUNIT_SIDE_PLAYER);
    }
    wk_sbtl->field408 = list;
    if (!sbtlGetTargetUnit(wk)) {
        sbtlDeleteUnitList(wk_sbtl->field408);
        if (side == SBTLUNIT_SIDE_PLAYER) {
            wk_sbtl->field408 = sbtlCreateTargetList(SBTLUNIT_SIDE_ENEMY);
        } else {
            wk_sbtl->field408 = sbtlCreateTargetList(SBTLUNIT_SIDE_PLAYER);
        }
        sbtlGetTargetUnit(wk);
    }
    sbtlDeleteUnitList(wk_sbtl->field404);
    sbtlDeleteUnitList(wk_sbtl->field408);
    wk_sbtl->field408 = NULL;
    wk_sbtl->field404 = NULL;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCheckTargetEquation);
#endif

s32 SEquationno_00(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if (tbl->area & 0x4000) {
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                return 1;
            }
        }
    } else {
        for (i = 0; i < wk_sbtl->field408->count; i++) {
            unit = wk_sbtl->field408->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                return 1;
            }
        }
    }
    return 0;
}

s32 SEquationno_01(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    s32 ok = 0;
    u32 bit;
    s32 i;

    if ((tbl->type & 0xFFF) == 1) {
        bit = tbl->flg & 1;
        ok = bit != 0;
    }
    switch (skillid) {
    case SKLID_MHRAGI:
    case SKLID_MHRAGON:
    case SKLID_MHBUHU:
    case SKLID_MHBUHUR:
    case SKLID_MHJIO:
    case SKLID_MHJIONGA:
    case SKLID_MHZAN:
    case SKLID_MHZANMA:
    case SKLID_MARINKA:
    case SKLID_DORUMIN:
        ok = 1;
        break;
    default:
        break;
    }
    if (ok == 1) {
        for (i = 0; i < wk_sbtl->field408->count; i++) {
            unit = wk_sbtl->field408->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if (sbtlCheckCrossCircle(&wk->pos, wk->model->field11C * 2.5f, &unit->pos, unit->model->field11C)) {
                    return 1;
                }
            }
        }
        return 0;
    }
    return 0;
}

s32 sbtlCallBtlCheckDamageTarget5(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLUNIT_WORK *unit;
    s32 ok;
    s32 i;

    switch (skillid) {
    case SKLID_MHRAGI:
    case SKLID_MHRAGON:
    case SKLID_MHBUHU:
    case SKLID_MHBUHUR:
    case SKLID_MHJIO:
    case SKLID_MHJIONGA:
    case SKLID_MHZAN:
    case SKLID_MHZANMA:
    case SKLID_MARINKA:
    case SKLID_DORUMIN:
    case 149:
        ok = 0;
        break;
    default:
        ok = 1;
        break;
    }
    if (ok == 1) {
        for (i = 0; i < wk_sbtl->field408->count; i++) {
            unit = wk_sbtl->field408->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if (sbtlCheckCrossCircle(&wk->pos, wk->model->field11C * 8.0f, &unit->pos, unit->model->field11C)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* NON_MATCHING: 54 of 86 words match. tail: retail compares meirei_no with 3 and 4 by branching (bnel/beq into shared li 1 / li 0) where mine folds the final test to xori/sltiu (4 words longer). Logic as retail. */
#ifdef NON_MATCHING
s32 SEquationno_03(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    u32 aisyo;
    s32 count;
    s32 type;
    s32 i;

    if (!(tbl->area & 0x4000)) {
        count = 0;
        for (i = 0; i < wk_sbtl->field408->count; i++) {
            unit = wk_sbtl->field408->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                aisyo = sbtlSEquationno03Sub(&unit->unit.aisyo, tbl->attr);
                if (aisyo != 0x1000 && aisyo != 0x2000) {
                    count++;
                }
            }
        }
        if (count == 0) {
            type = tbl->type & 0xFFF;
            if (type != 3) {
                if (type < 4) {
                    if (type != 0 && wk_sbtl->panel->meirei_no == 3) {
                        return 1;
                    }
                    return 0;
                }
                if (type != 5) {
                    return 0;
                }
            }
            if (wk_sbtl->panel->meirei_no != 4) {
                return 0;
            }
            return 1;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", SEquationno_03);
#endif

s32 sbtlCallBtlGetSkilltbl3(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    s32 ret = 0;
    s32 i;

    for (i = 0; i < wk_sbtl->field404->count; i++) {
        unit = wk_sbtl->field404->pUnitL[i].pUnit;
        if (unit == NULL || !sbtlCheckDamageTarget(unit)) {
            continue;
        }
        if (tbl->flg & 0x20) {
            if (tbl->flg & 0x400000) {
                if (unit->field348 == 0) {
                    ret = 1;
                }
            } else if (unit->field360 <= 0) {
                ret = 1;
            }
        } else {
            ret = 1;
        }
    }
    return ret;
}

s32 SEquationno_05(struct SBTLUNIT_WORK *wk, s16 skillid) {
    u32 flg = sbtlGetSkilltbl(skillid)->flg;
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if ((flg & 4) && (flg & 0x81F000)) {
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if (unit->unit.now.maxhp != 0 && 100 * unit->unit.now.hp / unit->unit.now.maxhp < 40) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 sbtlCallBtlGetSkilltbl4(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if ((tbl->flg & 0x10004) == 4) {
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if (unit->unit.now.maxhp != 0 && unit->unit.now.hp < unit->unit.now.maxhp) {
                    if (!(tbl->flg & 0x800000)) {
                        return 1;
                    }
                    if (unit->field362 <= 0) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
    return 1;
}

s32 SEquationno_07(struct SBTLUNIT_WORK *wk, s16 skillid) {
    s32 type = sbtlGetSkilltbl(skillid)->type & 0x2000;

    return type != 0;
}

s32 SEquationno_08(struct SBTLUNIT_WORK *wk, s16 skillid) {
    u32 flg = sbtlGetSkilltbl(skillid)->flg;
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if ((flg & 4) && (flg & 0x81F000)) {
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if (unit->unit.now.maxhp != 0 && 100 * unit->unit.now.hp / unit->unit.now.maxhp < 70) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 SEquationno_09(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if ((tbl->attr & 1) && (tbl->flg & 1)) {
        for (i = 0; i < wk_sbtl->field408->count; i++) {
            unit = wk_sbtl->field408->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit) && (unit->flg2 & 0x10)) {
                return 1;
            }
        }
    }
    return 0;
}

s32 SEquationno_0a(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if (tbl->flg & 1) {
        for (i = 0; i < wk_sbtl->field408->count; i++) {
            unit = wk_sbtl->field408->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit) && sbtlSEquationno03Sub(&unit->unit.aisyo, tbl->attr) == 0x4000) {
                return 1;
            }
        }
    }
    return 0;
}

/* NON_MATCHING: 17 of 28 words match. retail sets result=1 with bnel after slt where mine if-converts to slt s1. Logic as retail. */
#ifdef NON_MATCHING
s32 SEquationno_0b(struct SBTLUNIT_WORK *wk, s16 skillid) {
    s32 result = 0;
    s32 type = sbtlGetSkilltbl(skillid)->type & 0xFFF;

    if (type < 3 && type != 0) {
        if (wk->unit.now.str < wk->unit.now.tec) {
            result = 1;
        }
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", SEquationno_0b);
#endif

s32 SEquationno_0c(struct SBTLUNIT_WORK *wk, s16 skillid) {
    s32 result = 0;
    s32 type = sbtlGetSkilltbl(skillid)->type & 0xFFF;

    if (type == 3 || type == 5) {
        result = wk->unit.now.str > wk->unit.now.tec;
    }
    return result;
}

s32 SEquationno_0d(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);

    if (SetNextTarget(wk, tbl, sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&wk->unit, 0), &wk->unit.autost_cnt, &wk->unit.autost_cnt2))) {
        return 1;
    }
    if (SetNextTarget(wk, tbl, sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&wk->unit, 1), &wk->unit.autost_cnt, &wk->unit.autost_cnt2))) {
        return 1;
    }
    if ((wk->field358 & 0x1000000) && (tbl->type & 0xFFF) == 1) {
        return 1;
    }
    return 0;
}

s32 SEquationno_0e(struct SBTLUNIT_WORK *wk, s16 skillid) {
    s32 type = sbtlGetSkilltbl(skillid)->type & 0x1000;

    return type != 0;
}

s32 SEquationno_0f(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);

    if (wk_sbtl->panel->meirei_no != 6) {
        return 0;
    }
    if (tbl->s_type && tbl->para != 0) {
        return 1;
    }
    return 0;
}

/* NON_MATCHING: 16 of 24 words match. retail branches with bne to a field424 reload where mine uses bnel. Logic as retail. */
#ifdef NON_MATCHING
s32 SEquationno_10(struct SBTLUNIT_WORK *wk, s16 skillid) {
    if (skillid != SKLID_OFS_PROGRAM) {
        return 0;
    }
    if (wk_sbtl->field424 == 1 && wk_sbtl->field408->count == 0) {
        return 1;
    }
    if (wk_sbtl->field424 <= 0) {
        return 1;
    }
    return wk_sbtl->field408->count == 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", SEquationno_10);
#endif

/* NON_MATCHING: 20 of 33 words match. prologue matches with a result variable. Logic as retail. */
#ifdef NON_MATCHING
s32 SEquationno_11(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    BST bst = sbtlGetBadStatus(&wk->unit);
    s32 result = 0;

    if (tbl->s_type && tbl->para) {
        result = 0;
        if (bst == BST_MOUDOKU) {
            if (skillid != 149) {
                return 1;
            }
            return 0;
        }
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", SEquationno_11);
#endif

s32 SEquationno_12(struct SBTLUNIT_WORK *wk, s16 skillid) {
    return sbtlCheckCost_SkillID(wk, skillid, 0);
}

s32 SEquationno_13(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    s32 i;

    for (i = 0; i < 6; i++) {
        if (((tbl->s_type >> i) & 1) && tbl->para > 0 && wk_sbtl->panel->meirei_no == 6) {
            return 1;
        }
    }
    return 0;
}

s32 SEquationno_14(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    u32 badst;
    s32 i;

    if ((tbl->flg & 0x10004) == 0x10004) {
        badst = tbl->badst;
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if ((unit->unit.badStatus & badst) != 0 && (badst & 0x8000) == 0) {
                    return 1;
                }
            }
        }
        return 0;
    }
    return 1;
}

/* NON_MATCHING: 31 of 75 words match. retail compares (type&0xFFF) against a register holding 1 or 3 (shared tail, beq) where mine emits xori/sltiu. Logic as retail. */
#ifdef NON_MATCHING
s32 SEquationno_15(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    u32 flg;

    switch (wk_sbtl->panel->meirei_no) {
    case 3:
        if (tbl->flg & 1) {
            return (tbl->type & 0xFFF) == 1;
        }
        return 0;
    case 4:
        if (tbl->flg & 1) {
            return (tbl->type & 0xFFF) == 3;
        }
        return 0;
    case 5:
        flg = tbl->flg;
        if (!(flg & 4) || !(flg & 0x81F000)) {
            if ((flg & 0x10004) == 0x10004) {
                return 1;
            }
            if (flg & 0x20) {
                if (flg & 0x400000) {
                    return wk->field348 != 1;
                }
                return wk->field360 < 1;
            }
            return 0;
        }
        return 1;
    default:
        return 1;
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", SEquationno_15);
#endif

s32 SEquationno_16(struct SBTLUNIT_WORK *wk, s16 skillid) {
    u32 flg = sbtlGetSkilltbl(skillid)->flg;
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if ((flg & 4) && (flg & 0x81F000)) {
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if (unit->unit.now.maxhp != 0 && 100 * unit->unit.now.hp / unit->unit.now.maxhp < 20) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* NON_MATCHING: 32 of 75 words match. same shared (type&0xFFF)==kind tail as SEquationno_15 (beq against a0 holding 1 or 3, mine xori/sltiu). Logic as retail. */
#ifdef NON_MATCHING
s32 SEquationno_17(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    u32 flg;

    switch (wk_sbtl->panel->meirei_no) {
    case 3:
        if (tbl->flg & 1) {
            return (tbl->type & 0xFFF) == 1;
        }
        return 0;
    case 4:
        if (tbl->flg & 1) {
            return (tbl->type & 0xFFF) == 3;
        }
        return 0;
    case 5:
        flg = tbl->flg;
        if (!(flg & 4) || !(flg & 0x81F000)) {
            if ((flg & 0x10004) != 0x10004 && (flg & 0x20)) {
                if (flg & 0x400000) {
                    return wk->field348 != 1;
                }
                return wk->field360 <= 0;
            }
        }
        return 1;
    default:
        return 0;
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", SEquationno_17);
#endif

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlAI_SetSeikaku_Action);

s32 sbtlCheckArea(struct SBTLUNIT_WORK *wk, f32 r, const sdfVector4_t *pos)
{
    s32 i;
    struct SBTLUNIT_WORK *unit;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && sbtlCheckDamageTarget(unit) && (unit->field414 & 7) && (unit->field414 & 7) != (wk->field414 & 7)) {
            if (sbtlCheckCrossCircle(pos, r, &unit->pos, unit->model->field11C)) {
                return 1;
            }
        }
    }
    return 0;
}

s32 sbtlCheckAreaCount(struct SBTLUNIT_WORK *wk, f32 r, const sdfVector4_t *pos)
{
    s32 i;
    s32 count = 0;
    struct SBTLUNIT_WORK *unit;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && sbtlCheckDamageTarget(unit) && (unit->field414 & 7) && (unit->field414 & 7) != (wk->field414 & 7)) {
            if (sbtlCheckCrossCircle(pos, r, &unit->pos, unit->model->field11C)) {
                count++;
            }
        }
    }
    return count;
}

s32 sbtlCheckArea_Atk(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos, f32 area) {
    struct SBTLUNIT_WORK *unit;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && sbtlCheckDamageTarget(unit)) {
            if ((unit->field414 & 7) != 0 && (unit->field414 & 7) != (wk->field414 & 7)
                && sbtlCheckCrossCircle(pos, area, &unit->pos, unit->model->field11C)) {
                if (unit->model->mode >= 15 && unit->model->field104 == wk->field1A0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 sbtlCheckArea_Masic(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos, f32 area, s32 flg) {
    struct SBTLUNIT_WORK *unit;
    sdfVector4_t vertex;
    sdfVector4_t dir;
    sdfVector4_t pos2;
    f32 len;
    s32 ok;
    s32 i;

    memset(&dir, 0, sizeof(dir));
    dir.z = -1.0f;
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&pos2);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && sbtlCheckDamageTarget(unit)) {
            if ((unit->field414 & 7) != 0 && (unit->field414 & 7) != (wk->field414 & 7)
                && sbtlCheckCrossCircle(&pos2, area, &unit->pos, unit->model->field11C)) {
                if (unit->model->mode >= 15 && unit->status == 6) {
                    ok = TRUE;
                    if (flg != 0) {
                        ok = unit->model->field104 == wk->field1A0;
                    }
                    if (ok == 1) {
                        SDF_VU0_LOADVECTOR(&unit->rot);
                        ToMatrix();
                        SDF_VU0_LOADVECTOR(&dir);
                        SDF_VU0_APPLYMATRIX();
                        SDF_VU0_NORMALIZE();
                        SDF_VU0_SCALEVECTOR(2000.0f);
                        SDF_VU0_LOADVECTOR_B(&unit->pos);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR_NOMEM(&vertex);
                        sbtlGetVerticalLength_Point(&unit->pos.x, &vertex.x, &wk->pos.x);
                        SDF_VU0_STOREVECTOR(&dir);
                        SDF_VU0_LOADVECTOR(&pos2);
                        SDF_VU0_LOADVECTOR_B(&dir);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(len);
                        if (len <= wk->model->field11C + wk->model->field11C) {
                            return unit->field1A0;
                        }
                    }
                }
            }
        }
    }
    return -1;
}

/* NON_MATCHING: 10 of 74 words match. retail keeps a result in v0 and branches with beq plus delay-slot li on the early exits (aiwk status and field414 tests), mine uses beql with ld in the slot. Logic as retail. */
#ifdef NON_MATCHING
s32 SetOrderTarget(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *target;
    s32 id;
    u16 area;

    if ((u32)(sbtlGetBadStatus(&wk->unit) - 9) < 2) {
        return 0;
    }
    if (!(wk->aiwk->status & 1)) {
        return 0;
    }
    if (!(wk->field414 & 1)) {
        return 0;
    }
    target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->order_targetID);
    if (target != NULL && sbtlCheckDamageTarget(target)) {
        wk->model->field104 = target->field1A0;
        return 1;
    }
    id = wk->model->field104;
    if (id == -1) {
        return 0;
    }
    target = sbtlSearchUnit_ID(wk_sbtl->unitList, id);
    if (target == NULL || !sbtlCheckDamageTarget(target)) {
        return 0;
    }
    area = sbtlGetSkilltbl(wk->skillid)->area;
    if ((area & 0x4000) && (target->field414 & 1)) {
        return 1;
    }
    if ((area & 0x8000) && (target->field414 & 2)) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", SetOrderTarget);
#endif

s32 SetNextTarget(struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *tbl, s32 no) {
    switch (no) {
    case 12:
        if ((tbl->type & 0xFFF) != 1) {
            break;
        }
        return 1;
    case 13:
        if ((tbl->type & 0xFFF) != 3) {
            break;
        }
        return 1;
    case 14:
        if (!(tbl->attr & (1 << wk->unit.autost_cnt) & 0xFFF)) {
            break;
        }
        return 1;
    }
    return 0;
}

/* NON_MATCHING: 60 of 85 words match. tail only: retail scales the pick index (sll 8*pick) in the delay slot before testing pick == -1 and compares low against pick (CSE of -1), mine tests first and reloads. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallBtlCheckDamageTarget6(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit;
    u32 aisyo;
    s32 hp;
    s32 low;
    s32 pick;
    s32 i;

    if (!(wk->field414 & 1)) {
        return 0;
    }
    low = -1;
    pick = -1;
    hp = 9999;
    for (i = 0; i < wk_sbtl->field408->count; i++) {
        unit = wk_sbtl->field408->pUnitL[i].pUnit;
        if (unit != NULL && sbtlCheckDamageTarget(unit)) {
            if (hp >= unit->unit.now.hp) {
                hp = unit->unit.now.hp;
                low = i;
                aisyo = sbtlSEquationno03Sub(&unit->unit.aisyo, sbtlGetSkilltbl(wk->skillid)->attr);
                if (aisyo != 0x1000 && aisyo != 0x2000) {
                    pick = i;
                }
            }
        }
    }
    if (pick == -1) {
        if (low == -1) {
            return 0;
        }
        pick = low;
    }
    wk->model->field104 = wk_sbtl->field408->pUnitL[pick].pUnit->field1A0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlCheckDamageTarget6);
#endif

s32 CheckEnemyTarget(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *target;

    if (!(wk->field414 & 2)) {
        return 0;
    }
    target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
    if (target != NULL && sbtlCheckDamageTarget(target) != 0) {
        return 1;
    }
    return 0;
}

s32 sbtlGetTargetUnit(struct SBTLUNIT_WORK *wk) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(wk->skillid);
    u32 flg;

    if (CheckBSTTarget(wk, tbl)) {
        return 1;
    }
    flg = tbl->flg;
    if (flg & 1) {
        return TEquationno_01(wk);
    }
    if (flg & 4) {
        if (flg & 0x10000) {
            return TEquationno_0d(wk);
        }
        return TEquationno_0c(wk);
    }
    if (flg & 0x200) {
        return TEquationno_0e(wk);
    }
    return TEquationno_01(wk);
}

s32 CheckBSTTarget(struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *tbl) {
    BST bst = sbtlGetBadStatus(&wk->unit);

    if (!(tbl->area & 0x4000) && (bst == BST_BOUDO || bst == BST_MAHUU)) {
        if (wk->aiwk->mode == 18) {
            wk->model->field104 = wk->field1A0;
        } else {
            if (wk->aiwk->bstside & 1) {
                return 0;
            }
            wk->model->field104 = wk_sbtl->targetUnitID;
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_r1_00259A88", TEquationno_01);

INCLUDE_ASM(const s32, "code_r1_00259A88", TEquationno_0c);

INCLUDE_ASM(const s32, "code_r1_00259A88", TEquationno_0d);

INCLUDE_ASM(const s32, "code_r1_00259A88", TEquationno_0e);

s32 sbtlCheckScriptData(s32 devilId)
{
    return sbtlGetAITBL(devilId)->scr_no != 0;
}

s8 CheckAIScript(struct SBTLUNIT_WORK *wk)
{
    AITBLDATA_WORK *aitbl = sbtlGetAITBL(wk->unit.devilId);

    if (aitbl->scr_no == 0) {
        return 0;
    }
    if (wk_sbtl->hAIScript == NULL) {
        return 0;
    }
    if (wk->model->mode == 9 || wk->model->mode == 10) {
        return 0;
    }
    if (wk_sbtl->hAIScript->MesHandle >= 0) {
        itfMesMngSetVar_itf(wk_sbtl->hAIScript->MesHandle, 0, wk->unit.devilId, 0);
    }
    wk->aiwk->scr_actcheckflg = 0;
    scrStartProcessProcedure(wk_sbtl->hAIScript, aitbl->scr_no);
    wk_sbtl->scrUnit = wk;
    scrRunScript(wk_sbtl->hAIScript);
    return 1;
}

struct SBTLAI_WORK *sbtlLeaf64(void)
{
    return NULL;
}

SBTLAI_WORK *Create_sbtlAI(void)
{
    sdfMemHandle_t *handle;
    SBTLAI_WORK *aiwk;

    handle = sdfMemAllocDirect(sizeof(SBTLAI_WORK));
    aiwk = sdfMemLock(handle);
    memset(aiwk, 0, sizeof(SBTLAI_WORK));
    aiwk->handle = handle;
    aiwk->mode = 0;
    aiwk->stat = 1;
    aiwk->field4C = 0;
    return aiwk;
}

s32 Proc_sbtlAI(SBTLAI_WORK *aiwk)
{
    if (aiwk != NULL) {
        sdfMemFreeDirect(aiwk->handle);
    }
    return 1;
}

/* NON_MATCHING: 176 of 239 words match. Retail jumps to one epilogue with the return value in the delay slot and lays the stat switch out as a tree (==2, >2, !=1), cc1 emits ==1/==2 and duplicates the epilogue; tried case order, default/case 0, enum and narrower casts on the index; flow walked against the asm; SBTL_WORK 0x1C4 is flg; SBTLAI_WORK gains backmode/status (HD order, shifted by the R1 handle); SBTLUNIT_WORK.field30 vector; vector checked (32 rows, all take the early return 0). */
#ifdef NON_MATCHING
s32 sbtlCallMemset12(struct SBTLUNIT_WORK *wk)
{
    sdfVector4_t zero;
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    BST bst;

    memset(&zero, 0, sizeof(zero));
    if (!(wk_sbtl->flg & 0x10000) || !(wk->flg & 4) || (wk->model->flags & 0x1000)) {
        return 0;
    }
    switch (wk->aiwk->stat) {
    case 1:
        if (!(wk->field414 & 1)) {
            break;
        }
        list = sbtlCreateTargetList_AI(SBTLUNIT_SIDE_ENEMY);
        bst = sbtlGetBadStatus(&wk->unit);
        if (wk_sbtl->field424 > 0 && list->count != 0 && !(wk_sbtl->flg & 0x40000)) {
            if (wk->aiwk->status & 8) {
                wk->aiwk->status |= 1;
            }
        } else if ((u32)(bst - BST_RAND0) >= 2 && bst != BST_SLEEP && bst != BST_MOUDOKU && bst != BST_STONE) {
            wk_sbtl->flg &= ~0x40000;
        } else {
            unit = sbtlGetPlayerUnit(wk_sbtl->targetUnitID);
            if (sbtlCheckCrossCircle(&wk->pos, wk->model->field120 + wk->max_mvspd.z * 4.0f, &unit->pos,
                                     unit->model->field11C) &&
                !(wk->flg & 0x800000) && (wk->next_motionID == 0 || wk->next_motionID == 2)) {
                sbtlDeleteUnitList(list);
                return 1;
            }
            wk->actCounter = wk->actMax;
            wk->aiwk->status &= ~1;
        }
        sbtlDeleteUnitList(list);
        if (bst == BST_CHARM) {
            wk->actCounter = 0;
        }
        if (wk->actCounter < wk->actMax) {
            wk->actCounter++;
            wk->actCounter = (wk->actCounter <= 0) ? 0 : (wk->actCounter < wk->actMax) ? wk->actCounter : wk->actMax;
        } else {
            wk->actCounter = 0;
        }
        if (wk->actCounter != 0) {
            break;
        }
        if (!sbtlCallBtlGetBadStatus5(wk)) {
            break;
        }
        sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
        sbtlCheckTargetEquation(wk);
        wk->aiwk->stat = 2;
        sbtlRefSbtl16(wk);
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
        if (unit == NULL) {
            break;
        }
        if (unit->unit.devilId == 314 && (unit->flg & 0x800)) {
            sbtlCallBtlCreateUnitList2(wk);
        }
        break;
    case 2:
        if (!(wk->field414 & 1)) {
            break;
        }
        if (wk_sbtl->mode == 6) {
            break;
        }
        list = sbtlCreateTargetList(SBTLUNIT_SIDE_ENEMY);
        if (wk_sbtl->flg & 0x40000) {
            if (wk->aiwk->mode != 0) {
                SDF_VU0_LOADVECTOR(&zero);
                SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
                wk->aiwk->stat = 1;
                wk->actCounter = wk->actMax;
            }
        } else if (wk->flg & 0x40000) {
            if (wk->aiwk->mode == 0 && wk_sbtl->field424 > 0 && list->count != 0) {
                SDF_VU0_LOADVECTOR(&zero);
                SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
                wk->aiwk->stat = 1;
                wk->aiwk->status &= ~1;
                wk->actCounter = wk->actMax;
            }
        }
        sbtlDeleteUnitList(list);
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallMemset12);
#endif

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlRefSbtl16);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlCreateUnitList2);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlGetBadStatus5);

s32 sbtlSetAIMode(struct SBTLUNIT_WORK *wk)
{
    struct SBTLSKILLP_TBL *skill = sbtlGetSkilltbl(wk->skillid);

    if (wk->skillid == 288 || wk->skillid == 0) {
        wk->aiwk->mode = 0;
        return 1;
    }
    switch (skill->type & 0xFFF) {
    case 1:
    case 4:
    case 12:
    case 13:
        wk->aiwk->mode = 1;
        wk->aiwk->scrflag |= 0x10000;
        break;
    case 2:
    case 3:
    case 5:
    case 9:
    case 10:
    case 11:
    case 14:
    case 15:
    case 16:
        wk->aiwk->mode = 8;
        wk->aiwk->scrflag |= 0x10000;
        break;
    case 6:
    case 7:
    case 8:
    default:
        wk->aiwk->mode = 0;
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlLeaf4);

s32 sbtlProcScript(struct SBTLUNIT_WORK *wk)
{
    if (wk == NULL || (wk->status == 10 && !(wk->aiwk->scrflag & 0x120000)) || wk->status == 11
        || wk->status == 14 || wk->model->mode == 9 || wk->status == 30 || wk->mode < 5) {
        return 0;
    }
    return CheckAIScript(wk);
}

s32 sbtlCreateAIScript(void)
{
    wk_sbtl->hAIScript = scrRegistScript2(ai_scr);
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallReleaseScript);

s32 sbtlCallRegistScript2(void)
{
    wk_sbtl->hDEADScript = scrRegistScript2(sbtlCallRegistScript2Data);
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallReleaseScript2);

void sbtlAllInit(void)
{
    s32 i;

    wk_sbtl->field674 = 0;
    wk_sbtl->scrEncountNo = wk_sbtl->stageNo;
    wk_sbtl->field67C = 0;
    wk_sbtl->field678 = 0;
    for (i = 0; i < 4; i++) {
        wk_sbtl->scrCnt[i] = 0;
    }
    wk_sbtl->scrCmpCnt = 0x7FFFFFFF;
    for (i = 0; i < 16; i++) {
        wk_sbtl->scrUnitList[i] = NULL;
    }
    for (i = 0; i < 8; i++) {
        wk_sbtl->scrData[i] = 0;
    }
    wk_sbtl->scrFlag = 0;
}

/* NON_MATCHING: 28 of 33 words match. One store slot: retail stores scrListID after the scrStep zeros, best of 112 source orders leaves it one slot early, vector checked (3 rows). */
#ifdef NON_MATCHING
void sbtlRefSbtl17(struct SBTLUNIT_WORK *wk)
{
    SBTLAI_WORK *aiwk = wk->aiwk;
    u8 side;

    aiwk->scrListID = -1;
    aiwk->scrSkillID = -1;
    aiwk->scrTargetCode = 0x20000;
    aiwk->scrflag = 0;
    aiwk->scrStep = 0;
    aiwk->scrStep2 = 0;
    aiwk->scrStep3 = 0;
    aiwk->act_p = 0;
    if (!(wk_sbtl->scrFlag & 0x40000)) {
        side = wk->field414;
        if ((u8)(side & 1) && !(side & 0x80) && wk_sbtl->battleMode != 3) {
            wk->vstatus |= SBTLUNIT_VST_WORLDMOT;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlRefSbtl17);
#endif

void sbtlRefSbtl18(struct SBTLUNIT_WORK *wk, s32 mode)
{

    if (mode == 2) {
        wk->flg |= 0x4001;
        if (wk->unit.devilId == 288 || wk->unit.devilId == 309) {
            wk->flg2 |= 8;
        }
    }
    switch (wk->unit.devilId) {
    case 182:
    case 272:
    case 278:
    case 279:
    case 292:
    case 294:
        wk->flg2 |= 8;
        break;
    }
    if (wk->unit.devilId == 221 || wk->unit.devilId == 223 || wk->unit.devilId == 228) {
        if (wk_sbtl->field674 == 0) {
            wk->flg |= 0x10;
            wk_sbtl->field674 = 1;
        }
    }
    if (wk->unit.devilId == 168 && wk_sbtl->field674 == 0) {
        wk->flg |= 0x10;
    }
    if ((wk->unit.devilId == 263 || wk->unit.devilId == 302) && wk_sbtl->field674 == 0) {
        wk->flg |= 0x10;
    }
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlDeath);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlRefSbtl19);

void sbtlBattleStart(SBTL_STAGE_DATA *stage)
{
    if (stage->flags & 0x20) {
        sbtlSupport_BattleStop(1, 1, 0);
    }
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlSetAttr);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlLeaf5);

INCLUDE_ASM(const s32, "code_r1_00259A88", CheckBadStatus);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCheckSkillCancel);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCheckEnshutuSummonUnit);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlDamage);

void sbtlEndAction(struct SBTLUNIT_WORK *wk)
{
    SBTLAI_WORK *aiwk = wk->aiwk;
    s16 devilId = wk->unit.devilId;

    aiwk->scrAutoCnt = 0;
    aiwk->scrflag &= ~0x10000;
    if (devilId == 285 || devilId == 308) {
        aiwk->scrflag &= ~0x200;
    }
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlStartDeadEvent);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlGetPointLast);

s32 GetUnitHelperMatrix(struct SBTLUNIT_WORK *wk, SBTLUNIT_MARK mark)
{
    sdfVector4_t vec;
    f32 angle;

    if (wk->unit.devilId == 259) {
        if (mark == 300) {
            sbtlGetUnitHelperPos(wk, 101);
            SDF_VU0_LOADVECTOR_B(&wk->model->scrPos3);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(&vec);
            angle = sdfAtan2(vec.x, vec.y);
            SDF_VU0_LOADIDENTITY();
            SetRotateMatrixZ(-angle);
            return 1;
        }
        if (mark == 302) {
            sbtlGetUnitHelperPos(wk, 102);
            SDF_VU0_LOADVECTOR_B(&wk->model->scrPos3);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(&vec);
            angle = sdfAtan2(vec.x, vec.y);
            SDF_VU0_LOADIDENTITY();
            SetRotateMatrixZ(-angle);
            return 1;
        }
        if (mark == 301) {
            SDF_VU0_LOADIDENTITY();
            return 1;
        }
    }
    return 0;
}

/* NON_MATCHING: 28 of 44 words match. Same tests and jump table; retail keeps return 0 in v0 and copies it into delay slots from the shared jr ra block, cc1 fills them with nops (branch-likely class), vector checked (9 rows, skillID != 465 path). */
#ifdef NON_MATCHING
s32 sbtlCheckEffectCancel(struct SBTLUNIT_WORK *wk, SKLID skillID, struct EFFPDATA_WORK *effpdwork)
{
    if (wk != NULL && skillID == 465 && (effpdwork->iti & 0xF0) && wk->unit.devilId == 259) {
        switch (effpdwork->iti_no) {
        case 101:
        case 103:
        case 104:
        case 108:
        case 109:
        case 110:
            if (wk_sbtl->field674 == 1 || wk_sbtl->field674 == 3) {
                return 1;
            }
            break;
        case 102:
        case 105:
        case 106:
        case 111:
        case 112:
        case 113:
            return wk_sbtl->field674 == 2 || wk_sbtl->field674 == 3;
        default:
            return 0;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCheckEffectCancel);
#endif

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlRefSbtl38);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlGetUseTableTargetSBtlActA);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlRefSbtl20);

INCLUDE_ASM(const s32, "code_r1_00259A88", Support_RunWaitProcess);

INCLUDE_ASM(const s32, "code_r1_00259A88", Support_RunWarpProcess);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlSupport_BattleStop);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallEncGetPointNums);

INCLUDE_ASM(const s32, "code_r1_00259A88", Support_RunDownProcess);

INCLUDE_ASM(const s32, "code_r1_00259A88", mapFunc12);

INCLUDE_ASM(const s32, "code_r1_00259A88", Support_GetNakamaData);

INCLUDE_ASM(const s32, "code_r1_00259A88", mapEncGetPointNums);

INCLUDE_ASM(const s32, "code_r1_00259A88", Support_SetAisyo);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlSupport_StopAction);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlSetMotionSBtlUnit);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlGetAITBL);

s32 sbtlCallGetIntPara31(void)
{
    s32 motionFlag;
    s32 effectFlag;

    motionFlag = scrGetIntPara(0);
    effectFlag = scrGetIntPara(1);
    sbtlSupport_BattleStop(motionFlag, effectFlag, scrGetIntPara(2));
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallEncGetPointNums4);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallSetIntReturnValue9);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallGetIntPara);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlSetEndData);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlDeath);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallGetIntPara32);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallBtlCheckDamageTarget7);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallGetIntPara33);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallGetIntPara34);

s32 sbtlCallGetIntPara35(void) {
    wk_sbtl->scrCutinID = scrGetIntPara(0);
    return 1;
}

s32 sbtlCallGetIntPara36(void) {
    wk_sbtl->scrMsgID = scrGetIntPara(0);
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallSetIntReturnValue10);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallSetIntReturnValue11);

INCLUDE_ASM(const s32, "code_r1_00259A88", Support_RunZoomProcess);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallGetIntPara2);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallGetFloatPara);

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallSetFloatReturnValue);

s32 sbtlCallGetIntPara3(void) {
    wk_sbtl->scrZoomFrame = scrGetIntPara(0);
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallSetIntReturnValue);

s32 sbtlCallGetIntPara4(void) {
    wk_sbtl->scrZoomOutFrame = scrGetIntPara(0);
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_00259A88", sbtlCallSetIntReturnValue2);
