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

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlReLoadMap_sbtlScene);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetCutScroll);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlDefaultCutScroll);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetZoom2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf55);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID19);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf56);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlProcSequenceSub);

void sbtlRefSbtl4(void)
{
    p2sdLoadEntry_t *midi;
    s32 i;

    midi = sdfLeaf35();
    for (i = 0; i < 30; i++) {
        wk_sbtl->midiLoad[i].unk0 = midi[i].unk0;
        wk_sbtl->midiLoad[i].unk4 = midi[i].unk4;
        wk_sbtl->midiLoad[i].unk5 = midi[i].unk5;
        wk_sbtl->midiLoad[i].unk6 = midi[i].unk6;
    }
    wk_sbtl->midiSkillCount = 0;
    for (i = 11; i < 29; i++) {
        if (wk_sbtl->midiLoad[i].unk6 != 0) {
            wk_sbtl->midiSkillCount++;
        }
    }
    wk_sbtl->midiCount8 = 0;
    for (i = 8; i < 11; i++) {
        if (wk_sbtl->midiLoad[i].unk6 != 0) {
            wk_sbtl->midiCount8++;
        }
    }
}

/* NON_MATCHING: 34 of 38 words match. Store scheduling: retail computes the footwork pointer first and loads the unit ID before the first zero stores; vector checked (menu). */
#ifdef NON_MATCHING
void sbtlRefSbtl5(struct SBTLUNIT_WORK *wk)
{
    wk->se.field10 = 0;
    wk->se.bossAt3gstat = 0;
    wk->se.loadgstat = 0;
    wk->se.at3Handle = NULL;
    wk->se.flag = 0;
    wk->se.pBossAt3Data = NULL;
    wk->se.loadHandle = NULL;
    wk->se.loadFile = NULL;
    wk->se.loadData = NULL;
    if (wk->field1A0 == wk_sbtl->targetUnitID && wk_sbtl->battleMode != 3 && wk_sbtl->battleMode != 1) {
        wk->se.mot_status = 2;
    } else {
        wk->se.mot_status = 0;
    }
    wk->se.mot_SMGID = 0;
    wk->se.mot_SMGID2 = 0;
    wk->se.mot_SMGID3 = 0;
    wk->se.mot_nowcnt = 0;
    wk->se.mot_startcnt = 0;
    wk->se.mot_seid = 0;
    wk->se.mot_vol = 0;
    wk->se.mot_step = 0;
    sbtlSummonDvTaskSub(&wk->se.footwk);
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl5);
#endif

void sbtlSummonDvTaskSub(SBTLSOUND_FOOTWORK *fwk)
{
    fwk->prevSMGID = 0;
    fwk->prevFrame = 100000.0f;
    fwk->played = 0;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallMemFreeDirect2);

DVLFOOTSE_WORK *sbtlDevilFootSEProcSub(s32 devilID)
{
    return &sbtlDevilFootSEProcSubData[devilID];
}

void sbtlBtlLoadSkillSESub(s32 arg)
{
    sdfCallSndSendCmd(arg);
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlBtlForceDeleteEffectWorkIDSub);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf57);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSummonMenuFuncOpen00Sub);

s32 sbtlBtlPlay3DSEBaseVolSub(const sdfVector4_t *pos, f32 maxlen, f32 minlen, s32 basevol, s32 minvol)
{
    sdfVector4_t eye;
    f32 len;
    s32 vol;

    if (pos == NULL) {
        return basevol;
    }
    SDF_VU0_LOADVECTOR(&aViewEye);
    SDF_VU0_STOREVECTOR(&eye);
    SDF_VU0_LOADVECTOR_B(pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(len);
    if (maxlen < len) {
        return minvol;
    }
    len -= minlen;
    if (maxlen < len) {
        len = maxlen;
    }
    if (len < 0.0f) {
        len = 0.0f;
    }
    vol = (basevol - minvol) * (s32)(maxlen - len) / (s32)maxlen + minvol;
    if (vol >= 128) {
        vol = 127;
    }
    if (vol < 0) {
        vol = 0;
    }
    return vol;
}

void sbtlSEVolAngleCalc(u32 no, s32 vol)
{
    p2sdPlaySE(no, vol, 63);
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlPlay3DSE_BaseVol);

void DevilFootSEProc(struct SBTLUNIT_WORK *wk)
{
    s32 devilID;
    DVLFOOTSE_WORK *foot;
    SBTLSOUND_FOOTWORK *fwk;
    f32 frame;
    u32 se;
    s32 i;

    if (wk->field1A0 == wk_sbtl->targetUnitID && wk_sbtl->battleMode == 1) {
        devilID = 321;
    } else {
        devilID = wk->unit.devilId;
    }
    foot = sbtlDevilFootSEProcSub(devilID);
    fwk = &wk->se.footwk;
    frame = wk->hModel->first->frame;
    if (frame < fwk->prevFrame) {
        fwk->prevFrame = frame;
        fwk->played = 0;
    } else {
        fwk->prevFrame = frame;
    }
    for (i = 0; i < foot->frameNum; i++) {
        if (i >= fwk->played && foot->frame[i] <= frame) {
            se = sbtlCallGetRandIntA5(fwk, devilID);
            sbtlSEVolAngleCalc(se, sbtlBtlPlay3DSEBaseVolSub(&wk->pos, 2000.0f, 1000.0f, 127, 58));
            fwk->prevSMGID = se;
            fwk->played = i + 1;
        }
    }
}

INCLUDE_ASM(const s32, "code_r1_0024F220", NakamaMotion3DSE);

void PlayMotionSENow(struct SBTLUNIT_WORK *wk, s32 motionID, s32 startcnt)
{
    u32 se;
    s32 start;
    s32 stop;
    s32 i;
    struct SBTLUNIT_WORK *other;
    s32 vol;

    if (wk->field1A0 == wk_sbtl->targetUnitID && wk_sbtl->battleMode != 3) {
        if (wk_sbtl->battleMode == 1) {
            se = sbtlPlayMotionSENowSub2(321, motionID);
        } else {
            se = sbtlGetBossSE(wk, motionID);
        }
    } else if (wk->unit.devilId == 290 || wk->unit.devilId == 297) {
        se = sbtlPlayMotionSENowSub3(wk->unit.devilId, motionID);
    } else {
        se = sbtlPlayMotionSENowSub2(wk->unit.devilId, motionID);
    }
    start = sbtlPlayMotionSENowSub(wk, motionID);
    stop = 1;
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        other = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (other != NULL && other->field1A0 != wk->field1A0 && other->unit.devilId == wk->unit.devilId &&
            other->mode >= 5 && other->se.mot_seid == wk->se.mot_seid) {
            stop = 0;
            break;
        }
    }
    if (stop && wk->se.mot_seid != 0 && (wk->unit.devilId != 260 || motionID != 17)) {
        p2sdStopSe(wk->se.mot_seid);
    }
    if (se != 0) {
        vol = sbtlBtlPlay3DSEBaseVolSub(&wk->pos, 2000.0f, 1000.0f, 127, 58);
        if (wk->se.mot_status >= 2) {
            wk->se.mot_status = 3;
        }
        wk->se.mot_motionID = motionID;
        wk->se.mot_startcnt = start;
        wk->se.mot_seid = se;
        wk->se.mot_vol = vol;
        wk->se.mot_nowcnt = 0;
    } else {
        wk->se.mot_motionID = motionID;
        wk->se.mot_status = 4;
        wk->se.mot_startcnt = 0;
        wk->se.mot_seid = 0;
        wk->se.mot_vol = 0;
        wk->se.mot_nowcnt = 0;
    }
}

void sbtlRefSbtl6(struct SBTLUNIT_WORK *wk, u32 seid, s32 vol, s32 motionID)
{
    switch (wk->unit.devilId) {
    case 259:
    case 260:
    case 263:
    case 302:
        break;
    case 262:
    case 300:
        if (motionID == 22) {
            sbtlCallEffectShutdown2(wk);
        }
        break;
    case 268:
    case 269:
    case 304:
    case 305:
        if (motionID == 12) {
            if (wk_sbtl->field674 == 4) {
                sbtlCallEffectShutdown(wk);
            } else if ((u32)(wk_sbtl->field674 - 5) < 2) {
                sbtlCallEffectShutdown2(wk);
            }
        }
        break;
    case 285:
    case 286:
    case 288:
    case 307:
    case 308:
    case 309:
        if (motionID == 13) {
            sbtlCallEffectShutdown(wk);
        }
        if (motionID == 12) {
            sbtlCallEffectShutdown2(wk);
        }
        break;
    default:
        if (motionID == 12) {
            if (wk->field1A0 != wk_sbtl->targetUnitID || wk_sbtl->battleMode == 3 || wk_sbtl->battleMode == 1) {
                if (!(wk->se.field10 & 1)) {
                    sbtlCallEffectShutdown2(wk);
                }
            }
        }
        if ((u32)(motionID - 13) < 2 && !(wk->field414 & 0x80) && (wk->field414 & 1)) {
            sbtlLeaf58(seid);
        }
        break;
    }
    sbtlSEVolAngleCalc(seid, vol);
}

f32 sbtlPlayMotionSENowSub(struct SBTLUNIT_WORK *wk, s32 motionID)
{
    return sbtlCallBtlGetMotionVisualData5(wk, motionID);
}

u32 sbtlGetBossSE(struct SBTLUNIT_WORK *wk, s32 motionID)
{
    u32 se;

    se = 0;
    switch (motionID) {
    case 4:
        se = dds3GetRandIntA(NULL, 1) ? 0x1000A : 0x1000B;
        break;
    case 5:
    case 12:
        se = 0x1000C;
        break;
    case 11:
        se = 0x1000D;
        break;
    case 27:
        se = 0x1000E;
        break;
    case 28:
        se = 0x1000F;
        break;
    case 29:
        se = 0x10010;
        break;
    case 30:
        se = 0x10011;
        break;
    case 31:
        se = 0x10012;
        break;
    case 33:
        se = 0x10013;
        break;
    case 22:
        se = 0x10017;
        break;
    case 26:
        se = 0x10018;
        break;
    case 13:
        se = 0x1001D;
        break;
    }
    return se;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlPlayDamageSE);

u32 sbtlCallGetRandIntA4(struct SBTLUNIT_WORK *wk)
{
    u32 base;
    u32 n;

    base = 0x10006;
    if (wk->se.footwk.prevSMGID == 0) {
        return dds3GetRandIntA(NULL, 4) + 0x10006;
    }
    n = dds3GetRandIntA(NULL, 3) + 1;
    return (wk->se.footwk.prevSMGID - base + n) % 4 + 0x10006;
}

/* NON_MATCHING: 34 of 60 words match. Branch layout: retail joins every result in one register and tests the SE count with separate signed compares; vector checked (menu): only sedataNum 1 covered. */
#ifdef NON_MATCHING
u32 sbtlCallGetRandIntA5(SBTLSOUND_FOOTWORK *fwk, s32 devilID)
{
    u32 base;
    DVLFOOTSE_WORK *foot;
    s32 num;
    u32 n;

    base = sbtlPlayMotionSENowSub2(devilID, -1);
    foot = sbtlDevilFootSEProcSub(devilID);
    if (foot->flg & 2) {
        if (fwk->prevSMGID == 0) {
            return base + dds3GetRandIntA(NULL, foot->sedataNum);
        }
        num = foot->sedataNum;
        if (num == 1) {
            return base;
        }
        if (num == 0) {
            return 0;
        }
        if (num < 5) {
            n = dds3GetRandIntA(NULL, num - 1) + 1;
            return (fwk->prevSMGID - base + n) % foot->sedataNum + base;
        }
        return 0;
    }
    if (foot->flg & 4) {
        return base + fwk->played;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetRandIntA5);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlPlaySpecialSE);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf58);

/* NON_MATCHING: 176 of 178 words match. Two branches are branch-likely in this build where retail uses plain branches with the same delay slot; vector checked (menu): count loop and mot_nowcnt paths only. */
void sbtlCallBtlCheckEnshutuSummonUnit(void)
{
    s32 i;
    struct SBTLUNIT_WORK *wk;
    f32 prev;
    f32 frame;
    u32 se;

    if (wk_sbtl == NULL || wk_sbtl->unitList == NULL) {
        return;
    }
    for (i = 0; i < 256; i++) {
        sbtlWork[i].count++;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk == NULL || wk->hModel == NULL || wk->mode < 3 || wk->mode == 9) {
            continue;
        }
        if ((wk_sbtl->flg & 0x40) && !(wk->flg & 0x10) && !(wk->vstatus & 0x2000000) &&
            !sbtlCheckEnshutuSummonUnit(wk)) {
            continue;
        }
        if (wk->se.mot_status == 3 && wk->se.mot_nowcnt >= wk->se.mot_startcnt) {
            sbtlRefSbtl6(wk, wk->se.mot_seid, wk->se.mot_vol, wk->se.mot_motionID);
            wk->se.mot_status = 4;
        }
        wk->se.mot_nowcnt++;
        if (wk->field1A0 != wk_sbtl->targetUnitID || wk_sbtl->battleMode == 3 || wk_sbtl->battleMode == 1 ||
            wk->motionno != 1) {
            continue;
        }
        prev = wk->se.footwk.prevFrame;
        frame = wk->hModel->first->frame;
        wk->se.footwk.prevFrame = frame;
        if ((prev < 25.0f && frame >= 25.0f) || frame < prev) {
            se = sbtlCallGetRandIntA4(wk);
            sbtlSEVolAngleCalc(se, sbtlBtlPlay3DSEBaseVolSub(&wk->pos, 2000.0f, 1000.0f, 127, 58));
            wk->se.footwk.prevSMGID = se;
        }
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && sbtlCheckDamageTarget(wk) && wk->motionno == 1 &&
            (wk->field1A0 != wk_sbtl->targetUnitID || wk_sbtl->battleMode == 3 || wk_sbtl->battleMode == 1)) {
            DevilFootSEProc(wk);
        }
    }
}

/* NON_MATCHING: 5 of 18 words match. Retail computes the devil SE base twice, once for IDs from 80 up (an inlined special devil ID lookup that folds to the identity); vector checked (menu). */
#ifdef NON_MATCHING
u32 sbtlPlayMotionSENowSub2(s32 devilID, s32 motionID)
{
    u32 se;

    se = (devilID << 16) + 0x4000000;
    if (devilID >= 80) {
        se = (devilID << 16) + 0x4000000;
    }
    if (motionID == -1) {
        return se;
    }
    if ((u32)(motionID - 4) < 28) {
        return se + motionID;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlPlayMotionSENowSub2);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlPlayMotionSENowSub3);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetDevilSESub);

/* NON_MATCHING: 87 of 131 words match. Second load-check switch and the pac index arithmetic are laid out differently; vector checked (menu). */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetDevilSESub(struct SBTLUNIT_WORK *wk)
{
    s32 devilID;
    s32 idx;
    SBTL_PACDATA *pac;
    u32 se;

    devilID = wk->unit.devilId;
    if (wk->field1A0 == wk_sbtl->targetUnitID && wk_sbtl->battleMode != 3) {
        devilID = 321;
        if (wk_sbtl->battleMode != 1) {
            return 1;
        }
    }
    if (devilID == 290 || devilID == 297) {
        return sbtlCallBtlSearchPacData3(wk);
    }
    if ((wk->field414 & 1) && (wk->field1A0 != wk_sbtl->targetUnitID || wk_sbtl->battleMode == 3)) {
        switch (p2sdLoadCheckMidi(sbtlGetDevilSESub(devilID))) {
        case 0:
            idx = sbtlSearchPacData(wk->major, wk->minor, devilID);
            pac = &sbtlData84->pac[idx];
            p2sdSetPcmBuffer(EE_ADDR_S(pac->file20.data), pac->file20.size);
            return 0;
        case 1:
            break;
        case 2:
            return 0;
        }
    }
    se = sbtlPlayMotionSENowSub2(devilID, -1);
    switch (p2sdLoadCheckMidi(se)) {
    case 0:
        if (wk_sbtl->midiCount8 == 3 && devilID >= 80) {
            break;
        }
        if (devilID == 321) {
            p2sdLoadSMGFile(se);
        } else {
            idx = sbtlSearchPacData(wk->major, wk->minor, devilID);
            pac = &sbtlData84->pac[idx];
            p2sdSetPcmBuffer(EE_ADDR_S(pac->motionSE.data), pac->motionSE.size);
        }
        wk->se.mot_status = 1;
        break;
    case 1:
        wk->se.mot_SMGID = se;
        wk->se.mot_status = 2;
        sbtlBtlLoadSkillSESub(se);
        return 1;
    case 2:
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetDevilSESub);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchPacData3);

/* NON_MATCHING: 21 of 176 words match. Same blocks and jump table; retail recomputes skillID<<16 per branch where cc1 hoists it into a separate register, so allocation differs throughout; retail also compares type with xor against a register 1 and joins the 357 return through v1; vector checked (menu). */
#ifdef NON_MATCHING
u32 sbtlCallBtlGetSkilltbl(s32 skillID, s32 type)
{
    struct SBTLSKILLP_TBL *tbl;
    u32 se;
    u32 ret;

    ret = 0;
    if (skillID == 458 || skillID == 460 || skillID == 459) {
        if (type == 2) {
            skillID = 459;
        }
        if (type == 1) {
            skillID = 458;
        }
    }
    tbl = sbtlGetSkilltbl(skillID);
    if (tbl->s_type & 4) {
        se = (skillID << 16) - 0xAC0000;
        switch (type) {
        case 1:
            ret = 0x10017;
            break;
        case -1:
        case 2:
            ret = se;
            break;
        }
        return ret;
    }
    if (tbl->type & 0x1000) {
        switch (skillID) {
        case 7:
            se = 0x3000000;
            break;
        case 8:
            se = 0x3010000;
            break;
        case 22:
            se = 0x3020000;
            break;
        case 23:
            se = 0x3030000;
            break;
        case 38:
            se = 0x3040000;
            break;
        case 39:
            se = 0x3050000;
            break;
        case 54:
            se = 0x3060000;
            break;
        case 55:
            se = 0x3070000;
            break;
        case 64:
            se = 0x3080000;
            break;
        case 156:
            se = 0x3090000;
            break;
        case 157:
            se = 0x30A0000;
            break;
        case 158:
            se = 0x30B0000;
            break;
        case 159:
            se = 0x30C0000;
            break;
        case 160:
            se = 0x30D0000;
            break;
        case 161:
            se = 0x30E0000;
            break;
        case 162:
            se = 0x30F0000;
            break;
        case 163:
            se = 0x3100000;
            break;
        case 191:
            se = 0x3110000;
            break;
        case 192:
            se = 0x3120000;
            break;
        case 193:
            se = 0x3130000;
            break;
        case 194:
            se = 0x3140000;
            break;
        case 195:
            se = 0x3150000;
            break;
        case 208:
            se = 0x3160000;
            break;
        case 209:
            se = 0x3170000;
            break;
        case 210:
            se = 0x3180000;
            break;
        case 211:
            se = 0x3190000;
            break;
        case 212:
            se = 0x31A0000;
            break;
        case 213:
            se = 0x31B0000;
            break;
        case 214:
            se = 0x31C0000;
            break;
        case 215:
            se = 0x31D0000;
            break;
        case 216:
            se = 0x31E0000;
            break;
        case 217:
            se = 0x31F0000;
            break;
        case 218:
            se = 0x3200000;
            break;
        case 219:
            se = 0x3210000;
            break;
        case 220:
            se = 0x3220000;
            break;
        default:
            se = 0;
            break;
        }
    } else {
        se = (skillID + 0x700) << 16;
        if ((tbl->type & 0xFFF) == 8) {
            if (skillID == 357) {
                return 0xA120000;
            }
            return (skillID << 16) + 0x8C00000;
        }
    }
    switch (type) {
    case -1:
    case 1:
        ret = se;
        break;
    case 2:
        ret = se + 1;
        break;
    case 5:
        ret = se + 2;
        break;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetSkilltbl);
#endif

/* NON_MATCHING: 28 of 69 words match. Retail fills the case 1 branch delay slot with the SMGID store as branch-likely; this build leaves a nop and the rest shifts by one word; vector checked (menu). */
s32 sbtlLoadSkillSE(struct SBTLEFF_PACLIST *ewk)
{
    s32 skillID;
    struct SBTLSKILLP_TBL *tbl;
    u32 se;

    skillID = ewk->skillID;
    tbl = sbtlGetSkilltbl(skillID);
    if (tbl->flg & 0x80000000) {
        return 1;
    }
    if (skillID == 287) {
        return 1;
    }
    if (skillID < 149) {
        if (skillID >= 144) {
            return 1;
        }
    }
    if (tbl->flg & 0x100) {
        return 1;
    }
    se = sbtlCallBtlGetSkilltbl(skillID, -1);
    switch (p2sdLoadCheckMidi(se)) {
    case 0:
        if (wk_sbtl->midiSkillCount != 12 || (tbl->type & 0x1000) || (tbl->type & 0xFFF) == 8 || (tbl->s_type & 4)) {
            p2sdLoadSMGFile(se);
        }
        return 0;
    case 1:
        ewk->SMGID = se;
        sbtlBtlLoadSkillSESub(se);
        return 1;
    case 2:
        break;
    }
    return 0;
}

void sbtlPlaySkillSE(const sdfVector4_t *pos, SKLID skillid, s32 type) {
    u32 flg;
    u32 no;

    flg = sbtlGetSkilltbl(skillid)->flg;
    if ((flg & 0x80000000) || skillid == 287 || (flg & 0x100)) {
        return;
    }
    switch (skillid) {
        case 144:
        case 145:
        case 146:
        case 147:
        case 148:
            return;
        case 236:
        case 498:
            sbtlCallEffectShutdown2(wk_sbtl->scrUnitList[1]);
            break;
        case 237:
        case 499:
            sbtlCallEffectShutdown2(wk_sbtl->scrUnitList[2]);
            break;
        case 239:
            sbtlStopSkillSE(238, -1);
            sbtlCallEffectShutdown2(wk_sbtl->scrUnitList[0]);
            break;
        case 230:
            sbtlCallEffectShutdown2(wk_sbtl->scrUnitList[0]);
            break;
    }
    no = sbtlCallBtlGetSkilltbl(skillid, type);
    if (no != 0) {
        sbtlBtlPlay3DSEBaseVolSub(pos, 2000.0f, 1000.0f, 127, 58);
        sbtlSEVolAngleCalc(no, 127);
    }
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetDevilSESub2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallLoad);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallEffectShutdown);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSoundatBossAt);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlBtlModelDataLoadSBtlUnitSub);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallEffectShutdown2);

void sbtlRefSbtl7(void)
{
    s32 i;

    wk_sbtl->field878 = 1;
    wk_sbtl->field879 = 1;
    wk_sbtl->field87A = 0;
    for (i = 0; i < 256; i++) {
        sbtlWork[i].count = 0x0FFFFFFF;
        sbtlWork[i].field04 = 4;
    }
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallEffectShutdown3);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlStopSkillSE);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlFunc10);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlENMY);

s32 Init_sbtlUnit(s8 flg)
{
    wk_sbtl->unitList = sbtlCreateUnitList(48);
    dds3LinkProcess(wk_sbtl->process, dds3StartProcess("sbtlunit_proc", 1015, 1, 1, sbtlCallBtlCreateEnemyList, sbtlModelDataLoad, NULL));
    sbtlCallBtlCreateUnit(flg);
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetEndBadStatus);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetEndData);

INCLUDE_ASM(const s32, "code_r1_0024F220", Exit_sbtlUnit);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetPlayerData2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl32);

s32 Draw_sbtlUnit(void) {
    struct SBTLUNIT_WORK *wk;
    s32 i;

    if (wk_sbtl == NULL || wk_sbtl->unitList == NULL) {
        return 0;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk == NULL || (wk->vstatus & 0x2000)) {
            continue;
        }
        switch (wk->mode) {
            case 5:
            case 6:
            case 7:
            case 8:
            case 10:
                SDF_VU0_LOADVECTOR(wk->visual);
                mdlSetScale(wk->hModel);
                if (wk->vstatus & 0x1000) {
                    wk->hModel->flags |= 1;
                    dds3ObjectBaseOffFlag(wk->pBasic, 0x400);
                } else {
                    wk->hModel->flags &= ~1;
                    if (wk->status == 10 || wk->status == 11) {
                        dds3ObjectBaseOffFlag(wk->pBasic, 0x400);
                    } else {
                        sbtlCallBtlGetMotionVisualData3(wk);
                    }
                }
                if (wk->flg & 0x80000000) {
                    sbtlCallPreCalcStrWidth(wk);
                }
                break;
            case 4:
                SDF_VU0_LOADVECTOR(wk->visual);
                mdlSetScale(wk->hModel);
                wk->hModel->flags |= 1;
                dds3ObjectBaseOffFlag(wk->pBasic, 0x400);
                break;
            case 9:
            case 11:
            case 12:
                break;
        }
    }
    return 1;
}

struct SBTLUNIT_LISTWORK *sbtlCreateUnitList(s32 number)
{
    struct SBTLUNIT_LISTWORK *lwk;
    sdfMemHandle_t *handle;
    SBTLUNIT_LISTWK *list;
    s32 i;

    lwk = sdfChipAllocZeroDirect(sizeof(struct SBTLUNIT_LISTWORK));
    lwk->number = number;
    lwk->count = 0;
    handle = sdfMemAllocDirect((number + 1) * sizeof(SBTLUNIT_LISTWK));
    list = sdfMemLock(handle);
    lwk->pUnitL = list;
    for (i = 0; i <= number; i++) {
        list[i].ID = 0;
        list[i].pUnit = NULL;
    }
    lwk->handle = handle;
    return lwk;
}

struct SBTLUNIT_WORK *sbtlSearchUnit_ID(struct SBTLUNIT_LISTWORK *lwk, s32 ID)
{
    if (wk_sbtl == NULL || wk_sbtl->unitList == NULL) {
        return NULL;
    }
    if (ID == -1) {
        return NULL;
    }
    return lwk->pUnitL[ID].pUnit;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSearchUnit_GroupID);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSearchUnit_Nakama);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetNumUnit_GroupID);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetNumUnit_GroupID2);

s32 sbtlClearUnitList(struct SBTLUNIT_LISTWORK *wk)
{
    s32 i;

    if (wk == NULL) {
        return 0;
    }
    for (i = 0; i < wk->number; i++) {
        wk->pUnitL[i].pUnit = NULL;
    }
    wk->count = 0;
    return 1;
}

s32 sbtlDeleteUnitList(struct SBTLUNIT_LISTWORK *lwk)
{
    sdfMemFreeDirect(lwk->handle);
    sdfChipFreeDirect(lwk);
    return 1;
}

s32 sbtlEntryUnitList(struct SBTLUNIT_LISTWORK *lwk, struct SBTLUNIT_WORK *wk)
{
    s32 i;

    for (i = 0; i < lwk->number; i++) {
        if (lwk->pUnitL[i].pUnit == NULL) {
            break;
        }
    }
    lwk->pUnitL[i].ID = i;
    lwk->pUnitL[i].pUnit = wk;
    lwk->count++;
    return lwk->pUnitL[i].ID;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlReleaseUnitList);

struct SBTLUNIT_WORK *sbtlCreateUnit(s32 major, s32 minor, s16 mode) {
    sdfMemHandle_t *handle;
    struct SBTLUNIT_WORK *wk;

    handle = sdfMemAllocDirect(sizeof(struct SBTLUNIT_WORK));
    wk = sdfMemLock(handle);
    memset(wk, 0, sizeof(struct SBTLUNIT_WORK));
    wk->handle = handle;
    wk->major = major;
    wk->minor = minor;
    wk->pBasic = NULL;
    wk->hEveUnit = NULL;
    wk->hModel = NULL;
    wk->pBasic_d = NULL;
    wk->hEveUnit_d = NULL;
    wk->hModel_d = NULL;
    wk->pacHandle = NULL;
    wk->msgHandle = NULL;
    wk->msgData = NULL;
    wk->msgPos = 0;
    wk->motionno = 0;
    sbtlRestartMotion(wk);
    wk->fadergb = 8;
    wk->field4F3 = 1;
    wk->damage_serialID = -1;
    wk->damage_workID = -1;
    wk->bst_effserialID2 = -1;
    wk->color = 0x808080;
    wk->model = sbtlCreate();
    wk->aiwk = Create_sbtlAI();
    wk->mode = mode;
    wk->unit.haveskill[0] = 149;
    wk->unit.haveskill[1] = 288;
    wk->haveskill[0] = 149;
    wk->haveskill[1] = 288;
    sbtlRefSbtl17(wk);
    sbtlRefSbtl5(wk);
    return wk;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallDeleteSceneBasicData);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckReturn_Unit);

/* NON_MATCHING: 23 of 47 words match. Retail copies wk_sbtl into a second register and inverts the loop's found test into a branch-likely; register allocation differs from there; vector checked (menu). */
#ifdef NON_MATCHING
s32 sbtlSearchReturn_Unit(struct SBTLUNIT_WORK *wk)
{
    s32 i;
    struct SBTLUNIT_WORK *unit;

    if (wk_sbtl->battleMode == 3) {
        return 0;
    }
    if ((wk->field414 & 1) && !(wk->field414 & 0x80)) {
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL) {
                if (sbtlCheckReturn_Unit(unit) != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSearchReturn_Unit);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlDeleteUnit);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl33);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSin);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetColor);

INCLUDE_ASM(const s32, "code_r1_0024F220", LightSet);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetColor2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetColor3);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlBtlFadeOutBadStatusEffectSub);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf59);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf60);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlChangeModelColor);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlResetMutekiColor);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlBtlFadeOutBadStatusEffectSub2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf61);

s32 sbtlCallBtlCheckMapHitData(f32 posx, f32 posz)
{
    f32 px;
    f32 pz;
    s32 ret;

    ret = sbtlCheckMapHit_Data(posx, posz, &px, &pz);
    return ret != -1 ? ret == 0 : 0;
}

/* NON_MATCHING: 25 of 34 words match. f0 and f1 swap for x and z and one load moves into the delay slot; vector checked (menu): rows cover only the no-hit return. */
#ifdef NON_MATCHING
s32 CheckMapHit(struct SBTLUNIT_WORK *wk, const sdfVector4_t *oldpos)
{
    f32 px;
    f32 pz;
    s32 ret;

    if (wk->flg & 0x80) {
        return 1;
    }
    ret = sbtlCheckMapHit_Data(wk->pos.x, wk->pos.z, &px, &pz);
    if (ret == -1) {
        wk->pos.x = oldpos->x;
        /* Retail reads 32 bytes into oldpos, past the vector the callers pass. */
        wk->pos.z = oldpos[2].x;
    } else if (ret != 0) {
        wk->pos.x = px;
        wk->pos.z = pz;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", CheckMapHit);
#endif

s32 sbtlCheckMapHit_Data(f32 posx, f32 posz, f32 *px, f32 *pz)
{
    s32 ret;
    f32 getx, getz;

    ret = mapHitCalc(1, posx, posz, 40.0f, &getx, &getz);
    *px = getx;
    *pz = getz;
    return ret;
}

/* NON_MATCHING: 44 of 69 words match. s3 and s4 swap for the position pointer and the count, and retail keeps the found test as branches where this build emits sltiu; vector checked (menu): rows return 1 only. */
#ifdef NON_MATCHING
s32 sbtlCheckMapHit_Cross(const sdfVector4_t *pos0, const sdfVector4_t *ppos, f32 dis)
{
    sdfVector4_t dir;
    sdfVector4_t pos;
    s32 num;
    s32 i;
    f32 len;
    f32 step;

    SDF_VU0_LOADVECTOR(ppos);
    SDF_VU0_STOREVECTOR(&pos);
    if (sbtlCallBtlCheckMapHitData(pos.x, pos.z) == 0) {
        return 0;
    }
    num = dis / 33.3f;
    SDF_VU0_LOADVECTOR(ppos);
    SDF_VU0_LOADVECTOR_B(pos0);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    len = 33.3f;
    step = len;
    for (i = 0; i < num; i++) {
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_SCALEVECTOR(len);
        SDF_VU0_LOADVECTOR_B(pos0);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&pos);
        if (sbtlCallBtlCheckMapHitData(pos.x, pos.z) == 0) {
            break;
        }
        len += step;
    }
    if (i < num) {
        return 0;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckMapHit_Cross);
#endif

/* NON_MATCHING: 22 of 108 words match. cc1 keeps the FromEuler 0.0f in callee-saved $f20 across both calls where retail rematerialises it; tried -fno-gcse, the other -fno-cse flags, -O1, int zero; rest of the body lines up; vector checked (menu): small-angle branch only. */
#ifdef NON_MATCHING
void sbtlGetRotate(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos) {
    SBTL_MOTION_VISUAL *mv;
    f32 deg;
    f32 spd;
    f32 step;

    mv = sbtlGetMotionVisualData(wk->major, wk->minor);
    FromEuler(0.0f, wk->angleY * 0.017453293f, 0.0f);
    SDF_VU0_STOREVECTOR(&wk->rot);
    deg = sbtlGetChorikiDeg(&wk->pos, &wk->rot, pos, 1.0f);
    SDF_VU0_STOREVECTOR(&wk->rotbk);
    spd = mv->field08 * 57.295776f;
    if (deg <= -180.0f) {
        deg += 360.0f;
    }
    if (deg > 180.0f) {
        deg = 360.0f - deg;
    }
    if (deg <= 0.0f) {
        spd = -spd;
    }
    wk->degcnt = ffabsf((s32)(deg / spd));
    step = deg * mv->field08;
    if (spd <= 0.0f ? step <= spd : spd <= step) {
        step = spd;
    }
    wk->deg = step;
    if ((deg >= -120.0f && deg <= 0.0f) || (deg <= 120.0f && deg >= 0.0f)) {
        wk->degcnt = 0;
    }
    if (deg <= 0.0f ? spd <= deg : deg <= spd) {
        wk->deg = deg;
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetRotate);
#endif

void UnitMV(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos)
{
    sbtlAddUnitDeg(wk, wk->deg);
    FromEuler(0.0f, wk->angleY * 0.0174532925f, 0.0f);
    VU0_STORE_VF(vf10, &wk->rot);
    wk->degcnt--;
    if (wk->degcnt <= 0) {
        wk->degcnt = 0;
    } else {
        wk->field18 *= 0.5f;
    }
}

/* NON_MATCHING: 22 of 162 words match. Retail tests the 0x10 flag with bnel around an unconditional jump to the return-0 exit and sets the return 1 in bnel delay slots (branch-likely class); devil ID if-chain and rot source pick line up; aligned diff is 15 lines (five words shorter than retail); vector checked (menu). */
#ifdef NON_MATCHING
s32 sbtlCallMemset5(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t ofs = { 0.0f, 0.0f, 0.0f, 0.0f };
    sdfVector4_t rot;
    SBTLUNIT_FLG flg;
    f32 angle;

    flg = wk->flg;
    if ((wk_sbtl->flg & 0x40) && !(flg & 0x10)) {
        return 0;
    }
    if (wk->unit.devilId == 259) {
            wk->pos.x = 50.0f;
            wk->pos.y = 600.0f;
            wk->pos.z = -150.0f;
            wk->pos.w = 1.0f;
    } else if (wk->unit.devilId == 258) {
            wk->pos.y = 50.0f;
            wk->pos.z = -50.0f;
            wk->rot.w = 1.0f;
            wk->pos.x = 50.0f;
            wk->pos.w = 1.0f;
            wk->rot.x = 0.0f;
            wk->rot.y = 0.0f;
            wk->rot.z = 0.0f;
    } else if (wk->unit.devilId == 260) {
            wk->pos.y = 600.0f;
            wk->pos.z = 2000.0f;
            wk->pos.x = 0.0f;
    }
    if (flg & 0x80000) {
        wk->model->flags = (wk->model->flags | 0x20) & ~0x40;
        SDF_VU0_LOADVECTOR(&wk->posSaved);
        SDF_VU0_STOREVECTOR(&wk->pos);
    }
    ofs.x = -wk->field10;
    ofs.z = -wk->field18;
    if (wk_sbtl->battleMode == 3 ? (wk->field414 & 1) : (wk->field414 & 0x80)) {
        SDF_VU0_LOADVECTOR(&wk->rotbk);
        SDF_VU0_STOREVECTOR(&rot);
    } else {
        SDF_VU0_LOADVECTOR(&wk->rot);
        SDF_VU0_STOREVECTOR(&rot);
    }
    SDF_VU0_LOADVECTOR_B(&ofs);
    SDF_VU0_LOADVECTOR(&rot);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&wk->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&wk->pos);
    angle = wk->field424;
    if (angle != 0.0f) {
        FromRotation(-angle * 0.017453292f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(&wk->rot);
        Mult();
        SDF_VU0_STOREVECTOR(&wk->rot);
    }
    if (wk->vstatus & 0x2000) {
        return 1;
    }
    dds3SetPosPoseSet_dds3Basic(wk->pBasic, &wk->pos);
    if (wk->vstatus & 8) {
        return 1;
    }
    dds3SetRotPoseSet(wk->pBasic, &wk->rot);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallMemset5);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckMotionNo);

s32 sbtlCheckMotionEnd(struct SBTLUNIT_WORK *wk)
{
    if (sbtlCallBtlGetMotionVisualData2(wk, wk->motionID) != 0) {
        return 0;
    }
    return wk->hModel->first->stat == 5;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckGuardMotion);

s32 sbtlRestartMotion(struct SBTLUNIT_WORK *wk)
{
    SBTL_MOTION_VISUAL *visual;
    SBTL_UNIT_SUB *sub;

    if (wk->vstatus & 0x10) {
        return 0;
    }
    visual = sbtlGetMotionVisualData(wk->major, wk->minor);
    sub = sbtlSetUnitSub(wk->major, wk->minor, wk->next_motionno);
    if (sub->flags & 1) {
        wk->m_rate = sub->field10;
    } else {
        wk->m_rate = visual->field0C;
    }
    return 1;
}

s32 sbtlSetMotion(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no)
{
    if (wk->next_motionID != no && wk->motionID != no) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetMotionVisualData);

s32 sbtlBtlCheckMotionNoSub(struct SBTLUNIT_WORK *wk, u8 flg, s32 no)
{
    if (wk->flg & 0x8000000) {
        return sbtlBtlCheckMotionNoSubData[wk->motion_setno][no];
    }
    if (flg & 0x80) {
        if (wk->vstatus & 0x1000000) {
            return sbtlBtlCheckMotionNoSubData[0][no];
        }
        return sbtlBtlCheckMotionNoSubData[1][no];
    }
    if (wk->minor > 0) {
        return sbtlBtlCheckMotionNoSubData[3][no];
    }
    if (flg & 0x20) {
        return sbtlBtlCheckMotionNoSubData[2][no];
    }
    return sbtlBtlCheckMotionNoSubData[0][no];
}

/* NON_MATCHING: 118 of 123 words match. Id selection: reorg picks plain vs likely branches differently (bne s1/beq bst); the frame params are s32 in retail (no sign extension), prototype changed; vector checked (menu): bst 18/19 hp-ratio branch confirmed (sbtlGetBadStatus gives 19 for no status). */
#ifdef NON_MATCHING
s32 sbtlSetMotion_sbtlUnit(struct SBTLUNIT_WORK *wk, s32 motion, s32 bframe, s32 aframe, s8 flg) {
    SBTL_UNIT_SUB *next;
    SBTL_UNIT_SUB *now;
    SBTL_MOTION_VISUAL *mv;
    BST bst;
    s32 id;
    f32 rate;

    if (wk->flg & 0x400000) {
        id = 0;
    } else if (motion != 0 || (wk->vstatus & 0x1000000)) {
        id = motion;
    } else {
        bst = sbtlGetBadStatus(&wk->unit);
        if (bst == BST_RAND0 || bst == BST_BOYCOTT) {
            if (wk->flg & 0x100000) {
                id = 0;
            } else if ((f32)wk->unit.now.hp / (f32)wk->unit.now.maxhp > 0.25) {
                id = motion;
            } else {
                id = 2;
            }
        } else {
            id = 2;
        }
    }
    wk->next_motionID = id;
    wk->next_motionno = sbtlBtlCheckMotionNoSub(wk, wk->field414, id);
    next = sbtlSetUnitSub(wk->major, wk->minor, wk->next_motionno);
    now = sbtlSetUnitSub(wk->major, wk->minor, wk->motionno);
    if (!flg) {
        mv = sbtlGetMotionVisualData(wk->major, wk->minor);
        if (next->flags & 1) {
            wk->m_rate = next->field10;
        } else {
            wk->m_rate = mv->field0C;
        }
        if (wk_sbtl->battleMode != 3 && wk_sbtl->battleMode != 1 && (wk->field414 & 0x80)) {
            wk->beforeframe = now->field14;
            sbtlBtlSetMotionSBtlUnitSub(wk);
        } else {
            wk->beforeframe = now->field14;
        }
    } else {
        wk->beforeframe = bframe;
    }
    rate = next->field04;
    wk->afterframe = aframe;
    wk->field3DE = 0;
    wk->field3DC = 0;
    wk->model->field120 = rate;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetMotion_sbtlUnit);
#endif

void sbtlBtlSetMotionSBtlUnitSub(struct SBTLUNIT_WORK *wk) {
    switch (wk->next_motionno) {
        case 0:
        case 2:
            switch (wk->motionno) {
                case 1:
                    wk->beforeframe = 8;
                    break;
                case 27:
                    wk->beforeframe = 6;
                    break;
            }
            break;
        case 30:
            switch (wk->motionno) {
                case 0:
                case 1:
                case 2:
                case 15:
                case 16:
                    wk->beforeframe = 1;
                    break;
            }
            break;
        case 31:
        case 32:
            switch (wk->motionno) {
                case 0:
                case 1:
                case 2:
                case 15:
                case 16:
                    wk->beforeframe = 4;
                    break;
            }
            break;
        case 1:
            switch (wk->motionno) {
                case 0:
                case 2:
                case 4:
                case 5:
                case 11:
                case 15:
                case 16:
                    wk->beforeframe = 8;
                    break;
            }
            break;
        case 11:
            switch (wk->motionno) {
                case 0:
                case 2:
                case 15:
                case 16:
                case 27:
                case 28:
                    wk->beforeframe = 4;
                    break;
            }
            break;
        case 27:
            switch (wk->motionno) {
                case 0:
                case 2:
                case 15:
                case 16:
                    wk->beforeframe = 4;
                    break;
            }
            break;
    }
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCutMotion);

s32 sbtlSetFade(struct SBTLUNIT_WORK *wk, SBTLUNIT_VST flg, s32 start, s32 count, s32 para)
{
    wk->fadepara = para;
    wk->fade_sttime = start;
    wk->vstatus |= flg;
    wk->cmpfade = count;
    wk->fadecnt = 0;
    return 1;
}

s32 sbtlFadeProc(struct SBTLUNIT_WORK *wk) {
    s16 cnt;
    u32 vstatus;
    u32 color;
    s32 alpha;

    if ((wk_sbtl->flg & 0x40) && !(wk->flg & 0x10) && !sbtlCheckEnshutuSummonUnit(wk)) {
        return 0;
    }
    if ((wk_sbtl->flg & 0x8000000) && !(wk->flg & 0x10)) {
        return 0;
    }
    cnt = ++wk->fadecnt;
    if (cnt < wk->fade_sttime) {
        return 0;
    }
    if (wk->cmpfade != 0 && cnt >= wk->fade_sttime + wk->cmpfade) {
        wk->vstatus &= ~1;
        wk->vstatus &= ~2;
    }
    vstatus = wk->vstatus;
    color = wk->color;
    if (vstatus & 1) {
        alpha = color >> 24;
        alpha += wk->fadepara;
        alpha = alpha > 0 ? (alpha < 129 ? alpha : 128) : 0;
        wk->color = (color & 0xFFFFFF) | (alpha << 24);
        if (alpha >= 128) {
            wk->vstatus = vstatus & ~1;
        }
    } else {
        alpha = color >> 24;
        alpha -= wk->fadepara;
        alpha = alpha > 0 ? (alpha < 129 ? alpha : 128) : 0;
        wk->color = (color & 0xFFFFFF) | (alpha << 24);
        if (alpha <= 0) {
            wk->vstatus = vstatus & ~2;
        }
    }
    return 1;
}

/* NON_MATCHING: 19 of 70 words match. Retail clamps each channel with branch-likely stores where this build if-converts to movz, and it extracts all three channels up front; vector checked (menu). */
#ifdef NON_MATCHING
s32 sbtlRefSbtl8(struct SBTLUNIT_WORK *wk)
{
    u32 color;
    s32 add;
    s32 r;
    s32 g;
    s32 b;

    if ((wk_sbtl->flg & 0x40) && !(wk->flg & 0x10)) {
        return 0;
    }
    if ((wk_sbtl->flg & 0x8000000) && !(wk->flg & 0x10)) {
        return 0;
    }
    color = wk->color;
    add = wk->fadergb;
    if (add == 0) {
        add = 8;
    }
    r = (color & 0xFF) + add;
    if (r <= 0) {
        r = 0;
    } else if (r > 128) {
        r = 128;
    }
    g = ((color & 0xFF00) >> 8) + add;
    if (g <= 0) {
        g = 0;
    } else if (g > 128) {
        g = 128;
    }
    b = ((color >> 16) & 0xFF) + add;
    if (b <= 0) {
        b = 0;
    } else if (b > 128) {
        b = 128;
    }
    wk->color = (color & 0xFF000000) | r | (g << 8) | (b << 16);
    if (r >= 128) {
        wk->vstatus &= ~0x200000;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl8);
#endif

struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_MASK(u8 side, u32 mask, u32 mask_v)
{
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLUNIT_WORK *wk;
    s32 i;
    s32 hit;

    lwk = sbtlCreateUnitList(48);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 0x3F) == side && sbtlCheckLiveTarget_NotHitDmg(wk) != 0) {
            hit = 0;
            if (mask != 0) {
                hit = (wk->flg & mask) != 0;
            }
            if (mask_v != 0 && (wk->vstatus & mask_v)) {
                hit = 1;
            }
            if (!hit) {
                sbtlEntryUnitList(lwk, wk);
            }
        }
    }
    return lwk;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCreateTargetList_AI);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCreateTargetList);

INCLUDE_ASM(const s32, "code_r1_0024F220", ChangeTarget_BSHelper);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCreateUnitList);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCreateTargetList_Group);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCreateTargetList_PForward);

s32 sbtlCheckDamageTarget(struct SBTLUNIT_WORK *wk)
{
    s32 status = wk->status;

    if (status != 10 && !(wk->flg & 0x5000) && status != 11 && status != 14 && wk->model->mode != 9 && status != 30 && wk->mode >= 5 && status != 40) {
        return 1;
    }
    return 0;
}

s32 CheckLiveTarget(struct SBTLUNIT_WORK *wk)
{
    s32 status = wk->status;

    if ((u32)(status - 10) >= 2 && status != 14 && wk->model->mode != 9 && wk->mode >= 5 && status != 40) {
        return 1;
    }
    return 0;
}

s32 sbtlCheckLiveTarget_NotHitDmg(struct SBTLUNIT_WORK *wk)
{
    s32 status = wk->status;

    if (status != 10 && wk->unit.devilId != 311 && !(wk->flg & 0x1000) && (!(wk->flg & 0x800) || (wk->flg & 0x4800) == 0x800) && status != 11 && status != 14 && wk->model->mode != 9 && wk->mode >= 5 && status != 40) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckTateSkillSelectTarget);

s32 sbtlCheckExist(struct SBTLUNIT_WORK *wk)
{
    s32 status = wk->status;

    if ((u32)(status - 10) >= 2 && !(wk->field414 & 0x18) && status != 14) {
        return 1;
    }
    return 0;
}

s32 sbtlCheckExist2_sbtlUnit(struct SBTLUNIT_WORK *wk)
{
    if ((u32)(wk->status - 10) >= 2 && !(wk->field414 & 0x18) && wk->mode >= 5) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetDevilDataSBtlSub);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSearchHitHelper_ID);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf62);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetDevilData);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl34);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl35);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetHelperPos);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlDeleteHitHelper);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetItem);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlItem_CheckSetItem);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckSummonUnit_Exist2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID3);

s32 EventBattleStopChack(void)
{
    struct SBTLUNIT_WORK *wk;

    if (wk_sbtl->field344 <= 0) {
        return 2;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
    if (wk != NULL && sbtlCheckDamageTarget(wk) != 0) {
        return 0;
    }
    if (wk != NULL) {
        return 1;
    }
    return 2;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl9);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetStockPoint);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlEntryStockData_Huuma);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlEntryStockData);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetHangekiFlag);

struct SBTLUNIT_WORK *sbtlSetSummon(s32 major, s32 minor, s32 index)
{
    struct SBTLUNIT_WORK *wk;
    s32 id;

    wk = sbtlCreateUnit(major, minor, 2);
    id = sbtlEntryUnitList(wk_sbtl->unitList, wk);
    wk->GroupID = index;
    wk->field1A0 = id;
    wk->model->mode = 9;
    wk->model->field11A = 1;
    wk->status = 0;
    sbtlResetAIPattern(wk, 0);
    wk->color = 0x808080;
    wk_sbtl->field424++;
    return wk;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallMemset6);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallMemset7);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID4);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlResetSummon_E);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCheckDamageTarget);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlResetAction_Flag);

INCLUDE_ASM(const s32, "code_r1_0024F220", ResetAction);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID20);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCheckDamageTarget2);

/* NON_MATCHING: 45 of 46 words match. Retail has a plain branch where this build emits branch-likely after sbtlCheckDamageTarget; vector checked (menu). */
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

/* NON_MATCHING: 70 of 72 words match. Retail has plain branches with the wk_sbtl reload in the delay slot where this build emits branch-likely; vector checked (menu). */
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

/* NON_MATCHING: 70 of 72 words match. Retail has plain branches with the wk_sbtl reload in the delay slot where this build emits branch-likely; vector checked (menu). */
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

s32 sbtlCallPlayMotionSENow(struct SBTLUNIT_WORK *wk)
{
    if (wk->motionno == wk->next_motionno && sbtlCallBtlGetMotionVisualData2(wk, wk->motionID) != 0) {
        wk->hModel->first->speed = wk->m_rate;
    } else {
        wk->motionno = wk->next_motionno;
        wk->motionID = wk->next_motionID;
        wk->hModel->first->speed = wk->m_rate;
        PlayMotionSENow(wk, wk->motionno, wk->beforeframe);
        if (sbtlCallBtlGetMotionVisualData2(wk, wk->motionID) != 0) {
            mdlSmoothLoopMotion(wk->hModel, 0, wk->motionno, wk->beforeframe, wk->beforeframe);
        } else {
            mdlSmoothSingleMotion(wk->hModel, 0, wk->motionno, wk->beforeframe, wk->beforeframe);
        }
    }
    return 1;
}

INCLUDE_RODATA(const s32, "code_r1_0024F220", sbtlGetHelperPosTable);

s32 sbtlCallBtlGetMotionVisualData2(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no)
{
    SBTL_UNIT_SUB *sub;

    if ((wk->flg & 0x40) && wk->changeflg != 0) {
        return 0;
    }
    sbtlGetMotionVisualData(wk->major, wk->minor);
    sub = sbtlSetUnitSub(wk->major, wk->minor, wk->next_motionno);
    if (sub->field18 == 1) {
        return 0;
    }
    if (sub->field18 == 3) {
        return 1;
    }
    switch (no) {
    case 0:
    case 2:
    case 5:
    case 29:
    case 35:
        return 1;
    default:
        return 0;
    }
}

/* NON_MATCHING: 99 of 226 words match. Aligned diff is 16 lines: plain vs likely branches (unit null test, loadListID compare, case 5 delay slots) shift the rest; same blocks and jump table targets. New fields SBTLUNIT_WORK.next_st 0x454 and SBTL_WORK.loadListID 0x1E0 (HD names); vector checked (menu). */
#ifdef NON_MATCHING
s32 sbtlCallBtlCreateEnemyList(dds3ProcessID_t *id)
{
    struct SBTLUNIT_WORK *wk;
    s32 i;

    if (wk_sbtl != NULL) {
    if (wk_sbtl->unitList == NULL) {
        return 0;
    }
    if (!(wk_sbtl->flg & 1)) {
        return 0;
    }
    if (wk_sbtl->panel != NULL && !(wk_sbtl->panel->flg & 2)) {
        return 0;
    }
    if (!(wk_sbtl->flg & 4)) {
        return 0;
    }
    if (wk_sbtl->flg & 8) {
        return 0;
    }
    sbtlCreateEnemyList();
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk == NULL) {
            continue;
        }
        if (wk->vstatus & 3) {
            sbtlFadeProc(wk);
        }
        if (wk->vstatus & 0x200000) {
            sbtlRefSbtl8(wk);
        }
        if (wk->hModel != NULL && !(wk->vstatus & 0x100)) {
            mdlSetColor(wk->hModel, wk->color);
        }
        switch (wk->mode) {
        case 1:
        case 2:
            if (!(wk_sbtl->flg & 0x100000) || wk_sbtl->loadListID == wk->field1A0) {
                if (sbtlSearchReturn_Unit(wk) == 0 && !(wk_sbtl->flg2 & 0x200)) {
                    wk_sbtl->flg |= 0x100000;
                    wk_sbtl->loadListID = wk->field1A0;
                    sbtlModelDataLoad_sbtlUnit(wk);
                }
            }
            break;
        case 4:
        case 12:
            Proc(wk);
            break;
        case 5:
            switch (wk->unit.devilId) {
            case 258:
            case 259:
            case 290:
            case 297:
                sbtlSetFade(wk, SBTLUNIT_VST_FADEIN, 0, 0, 32);
                break;
            default:
                sbtlSetFade(wk, SBTLUNIT_VST_FADEIN, 0, 0, 16);
                break;
            }
            wk->mode = 8;
            break;
        case 6:
            wk->mode = 7;
            wk->vstatus |= 2;
            sbtlCallBtlCheckEnshutuSummonUnit2(wk);
            break;
        case 7:
            if (!(wk->vstatus & 2)) {
                wk->mode = 4;
                wk->status = wk->next_st;
            }
            sbtlCallBtlCheckEnshutuSummonUnit2(wk);
            break;
        case 8:
            Proc(wk);
            sbtlCallBtlCheckEnshutuSummonUnit2(wk);
            sbtlSetLight_Map(wk, 10, 0);
            break;
        case 9:
            if (!(wk->flg & 0x20000) || AllDeleteEnemy(wk) != 0) {
                if (wk->field414 & 2) {
                    wk_sbtl->field424--;
                    sbtlCallBtlDeleteHitHelper(wk);
                    sbtlCallBtlGetDevilDataSBtlSub3(wk);
                }
                if (!(wk->field414 & 1) || sbtlSearchDvltk(wk_sbtl->dvltkList, wk) == 0) {
                    sbtlDeleteUnit(wk);
                }
            }
            break;
        }
    }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCreateEnemyList);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", AllDeleteEnemy);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckUnit_Exec_R);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckUnit_Exec);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlModelDataLoad);

/* NON_MATCHING: 93 of 257 words match. One word longer; aligned diff ~15 lines: case 0 memcpy arg scheduling, the return-0 exits share a block differently, flg2 |= 0x200 register. sbtlCallBtlSearchPacData now takes a 5th s32 size (retail caller sets t0; only caller), R01 draft needs the unused param; SBTLSOUND_DEVILWORK field00[11] split into named fields; pacHandle is struct fileRequest *; vector checked (menu). */
#ifdef NON_MATCHING
s32 sbtlCheckModelDataLoadEnd_sbtlUnit(struct SBTLUNIT_WORK *wk)
{
    char name[32];
    sdfPacDecodeResrc_t *res;
    SBTL_PACDATA *pac;
    sdfMemHandle_t *handle;
    void *data;
    s32 size;

    if (wk->flg2 & 0x200) {
        return sdfDelayedResrcCheck() == 0;
    }
    if (wk->pacHandle == NULL) {
        if (mdlSearchResrc(wk->major, wk->minor) == NULL) {
            if (wk_sbtl->battleMode != 3 && (wk->field414 & 0x80)) {
                sprintf(name, "/model_lb/battle/%03X.LB", sbtlCheckModelDataLoadEnd_sbtlUnitData[wk->minor]);
            } else {
                sprintf(name, "/model_lb/devil_b/%03X.LB", wk->minor);
            }
            if (sdfFileCheckExist(name) == 0) {
                if (mdlPrepare(wk->major, wk->minor, 0) != 0 && mdlPrepare(wk->major, wk->minor, 0) != -1) {
                    if (sdfDelayedResrcCheck() == 0) {
                        return 1;
                    }
                }
            } else {
                wk->pacHandle = pacLoad(name);
            }
            return 0;
        }
        if (mdlPrepare(wk->major, wk->minor, 0) == 0 || mdlPrepare(wk->major, wk->minor, 0) == -1 || sdfDelayedResrcCheck() != 0) {
            return 0;
        }
        pac = &sbtlData84->pac[sbtlSearchPacData(wk->major, wk->minor, wk->unit.devilId)];
        wk->se.at3Handle = sdfMemAllocDirect(pac->mht.size);
        wk->se.pBossAt3Data = sdfMemLock(wk->se.at3Handle);
        wk->se.bossAt3Size = pac->mht.size;
        memcpy(wk->se.pBossAt3Data, pac->mht.data, wk->se.bossAt3Size);
        wk->se.bossAt3gstat = 2;
        sbtlSearchAttackHitData_Copy(wk);
        return 1;
    }
    if (pacCheckEnd(wk->pacHandle) == 0) {
        return 0;
    }
    for (res = wk->pacHandle->pac.resrcHead; res != NULL; res = res->next) {
        switch (((sdfPacStdBlockHeader_t *)res->header)->kind) {
        case 0:
            handle = res->resrc;
            data = sdfMemGetPtr(handle);
            size = sdfMemGetSize(handle);
            mdlPackDecode(wk->major, wk->minor, 257, data, size);
            sdfMemFreeDirect(handle);
            break;
        case 1:
            handle = res->resrc;
            data = sdfMemGetPtr(handle);
            size = sdfMemGetSize(handle);
            sbtlSetPacData_MHT(wk->major, wk->minor, wk->unit.devilId, data, size);
            sdfMemFreeDirect(handle);
            pac = &sbtlData84->pac[sbtlSearchPacData(wk->major, wk->minor, wk->unit.devilId)];
            wk->se.at3Handle = sdfMemAllocDirect(pac->mht.size);
            wk->se.pBossAt3Data = sdfMemLock(wk->se.at3Handle);
            wk->se.bossAt3Size = pac->mht.size;
            memcpy(wk->se.pBossAt3Data, pac->mht.data, wk->se.bossAt3Size);
            wk->se.bossAt3gstat = 2;
            break;
        case 2:
            handle = res->resrc;
            data = sdfMemGetPtr(handle);
            size = sdfMemGetSize(handle);
            while (sbtlLoadAttackHitData(wk, wk->unit.devilId, data) == 0) {
            }
            sbtlCallBtlSearchPacData(wk->major, wk->minor, wk->unit.devilId, data, size);
            sdfMemFreeDirect(handle);
            break;
        case 3:
            handle = res->resrc;
            data = sdfMemGetPtr(handle);
            size = sdfMemGetSize(handle);
            sbtlCallBtlSearchPacData2(wk->major, wk->minor, wk->unit.devilId, data, size);
            sdfMemFreeDirect(handle);
            break;
        case 4:
            handle = res->resrc;
            data = sdfMemGetPtr(handle);
            size = sdfMemGetSize(handle);
            DrawProc2(wk->major, wk->minor, wk->unit.devilId, data, size);
            sdfMemFreeDirect(handle);
            break;
        default:
            sdfMemFreeDirect(res->resrc);
            break;
        }
    }
    pacFileHandleFree_(wk->pacHandle);
    wk->pacHandle = NULL;
    wk->flg2 |= 0x200;
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckModelDataLoadEnd_sbtlUnit);
#endif

s32 sbtlModelDataLoad_sbtlUnit(struct SBTLUNIT_WORK *wk)
{
    u32 serialID;

    switch (wk->gstat) {
    case 0:
        if (sbtlCheckModelDataLoadEnd_sbtlUnit(wk) == 0) {
            break;
        }
        serialID = UnitRegist(wk->major, wk->minor);
        wk->pBasic = dds3SearchSceneBasic(dds3GetActiveScene(), serialID, 5);
        wk->hEveUnit = ((dds3UnitObjectBasicWorkData_t *)wk->pBasic->workdata)->hUnit;
        wk->hModel = wk->hEveUnit->owner;
        wk->hEveUnit->flags |= 0x200000;
        dds3SetPosPoseSet_dds3Basic(wk->pBasic, &wk->pos);
        dds3SetRotPoseSet(wk->pBasic, &wk->rot);
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&wk->nextpos);
        wk->hModel->flags |= 1;
        SDF_VU0_LOADVECTOR(wk->visual);
        SDF_VU0_STOREVECTOR(&wk->pBasic->poseset->scl);
        sbtlSetLight_Map(wk, 0, 0);
        evtUnitLightOn(wk->hEveUnit);
        if (wk->flg & 0x40) {
            wk->gstat = 1;
            break;
        }
        if (wk->field414 & 3) {
            wk->gstat = 2;
            break;
        }
        if (wk->mode == 1) {
            wk->mode = 8;
        } else {
            wk->mode = 4;
        }
        wk_sbtl->flg &= ~0x100000;
        break;
    case 1:
        if (mdlPrepare(wk->major, wk->minor_d, 0) == 0 || mdlPrepare(wk->major, wk->minor_d, 0) == -1) {
            break;
        }
        if (sdfDelayedResrcCheck() != 0) {
            break;
        }
        serialID = UnitRegist(wk->major, wk->minor_d);
        wk->pBasic_d = dds3SearchSceneBasic(dds3GetActiveScene(), serialID, 5);
        wk->hEveUnit_d = ((dds3UnitObjectBasicWorkData_t *)wk->pBasic_d->workdata)->hUnit;
        wk->hModel_d = wk->hEveUnit_d->owner;
        wk->hEveUnit_d->flags |= 0x200000;
        dds3SetPosPoseSet_dds3Basic(wk->pBasic_d, &wk->pos);
        dds3SetRotPoseSet(wk->pBasic_d, &wk->rot);
        SDF_VU0_LOADVECTOR(wk->visual);
        SDF_VU0_STOREVECTOR(&wk->pBasic_d->poseset->scl);
        wk->hModel_d->flags |= 1;
        if (wk->field414 & 3) {
            wk->gstat = 2;
            break;
        }
        wk->gstat = 0;
        if (wk->mode == 1) {
            wk->mode = 8;
        } else {
            wk->mode = 4;
        }
        wk_sbtl->flg &= ~0x100000;
        break;
    case 2:
        if ((wk->field414 & 1) && !(wk->field414 & 0x80)) {
            if (sbtlSearchUnit_Nakama(wk) == 0 && sbtlCallBtlGetDevilSESub(wk) != 0) {
                sbtlCheckBossSE(wk);
            } else {
                wk_sbtl->flg |= 0x20000000;
            }
            break;
        }
        if (sbtlCallBtlGetDevilSESub(wk) == 0) {
            wk_sbtl->flg |= 0x20000000;
            break;
        }
        if (wk->field414 & 0x80) {
            if (wk_sbtl->battleMode == 3) {
                wk->gstat = 5;
            } else {
                wk->gstat = 5;
                sbtlCallBtlGetBadStatus(wk);
            }
        } else {
            sbtlCheckBossSE(wk);
        }
        wk_sbtl->flg &= ~0x20000000;
        break;
    case 3:
        if (sbtlBtlModelDataLoadSBtlUnitSub(wk) != 0) {
            sbtlCheckBossSE(wk);
        } else {
            wk_sbtl->flg |= 0x20000000;
        }
        break;
    case 4:
        if (sbtlCallLoad(wk) != 0) {
            wk->gstat = 5;
            wk_sbtl->flg &= ~0x20000000;
        } else {
            wk_sbtl->flg |= 0x20000000;
        }
        break;
    case 5:
        sbtlCallBtlGetBadStatus(wk);
        break;
    }
    return 1;
}

void sbtlCheckBossSE(struct SBTLUNIT_WORK *wk)
{
    switch (wk->unit.devilId) {
    case 262:
    case 268:
    case 269:
    case 285:
    case 286:
    case 288:
    case 300:
    case 304:
    case 305:
    case 307:
    case 308:
    case 309:
        wk->gstat = 4;
        break;
    default:
        wk->gstat = 5;
        if (!(wk_sbtl->flg & 0x10000)) {
            sbtlCallBtlGetBadStatus(wk);
        }
        break;
    }
    wk_sbtl->flg &= ~0x20000000;
}

/* NON_MATCHING: 187 of 190 words match. Epilogue order: retail sets the return 1 after ld s0, so the Nakama beql takes ld s0 into its delay slot; rest lines up. Return type changed void to s32 (retail returns 1); new prototypes sbtlLoadEffectData_Nakama, sbtlCallBtlCheckMsgMemType, sbtlCreateDvltk_Priority; vector checked (menu). */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetBadStatus(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *sumwk;
    BST bst;

    if (wk->field414 & 0x80) {
        wk->gstat = 0;
        if (wk->mode == 1) {
            wk->mode = 8;
        } else {
            wk->mode = 4;
        }
        wk_sbtl->flg &= ~0x100000;
    } else if (wk->field414 & 2) {
        if (sbtlCallBtlLoadAttackHitData(wk) == 0) {
            return 1;
        }
        wk->gstat = 0;
        if (wk->mode == 1) {
            wk->mode = 8;
        } else {
            wk->mode = 4;
        }
        if (wk_sbtl->flg & 0x10000) {
            bst = sbtlGetBadStatus(&wk->unit);
            if (bst != 19) {
                sbtlSetBadStatus_No(wk, SKLID_NONE, bst, 1, -1);
            }
        }
        wk_sbtl->flg &= ~0x100000;
    } else if (wk_sbtl->flg & 0x10000) {
        if (sbtlLoadEffectData_Nakama(wk) == 0) {
            return 1;
        }
        if (sbtlCallBtlCheckMsgMemType(wk) == 0) {
            return 1;
        }
        if (sbtlLoadAttackHitData(wk, wk->unit.devilId, NULL) == 0) {
            return 1;
        }
        wk->gstat = 0;
        if (wk->mode == 1) {
            wk->mode = 8;
        } else {
            wk->mode = 4;
        }
        if (wk_sbtl->flg & 0x10000) {
            bst = sbtlGetBadStatus(&wk->unit);
            if (bst != 19) {
                sbtlSetBadStatus_No(wk, SKLID_NONE, bst, 1, -1);
            }
        }
        if (wk_sbtl->battleMode != 3) {
            sbtlCreateDvltk_Priority(wk_sbtl->dvltkList, wk, SBTLDVLTK_MODE_SUMMON);
        }
        wk_sbtl->flg &= ~0x100000;
    } else {
        if (sbtlCallBtlCheckMsgMemType(wk) == 0) {
            return 1;
        }
        if (sbtlLoadAttackHitData(wk, wk->unit.devilId, NULL) == 0) {
            return 1;
        }
        wk->gstat = 0;
        if (wk->mode == 1) {
            wk->mode = 8;
        } else {
            wk->mode = 4;
        }
        if (wk_sbtl->flg & 0x10000) {
            bst = sbtlGetBadStatus(&wk->unit);
            if (bst != 19) {
                sbtlSetBadStatus_No(wk, SKLID_NONE, bst, 1, -1);
            }
        }
        if (wk_sbtl->battleMode != 3) {
            sbtlCreateDvltk_Priority(wk_sbtl->dvltkList, wk, SBTLDVLTK_MODE_SUMMON);
        }
        wk_sbtl->flg &= ~0x100000;
        if (wk_sbtl->battleMode != 3) {
            sumwk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
            sumwk->mode = 5;
            sumwk->color = 0;
            sumwk->vstatus |= 0x200000;
            bst = sbtlGetBadStatus(&sumwk->unit);
            if (bst != 19) {
                sbtlSetBadStatus_No(sumwk, SKLID_NONE, bst, 1, -1);
            }
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetBadStatus);
#endif

s32 sbtlCallBtlLoadAttackHitData(struct SBTLUNIT_WORK *wk)
{
    if (!(wk->field414 & 2)) {
        return 1;
    }
    if (!(wk_sbtl->flg & 0x10000)) {
        return 1;
    }
    if (sbtlCallBtlGetSkilltbl10(wk) != 0 && sbtlLoadAttackHitData(wk, wk->unit.devilId, NULL) != 0 && sbtlCallBtlGetDevilDataSBtlSub4(wk) != 0) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetLight_SetPos);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetLight_SetPos);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetUnitLight_Dir);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetUnitLight);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetLightParam);

/* NON_MATCHING: 103 of 142 words match. Retail hoists the return-0 v0 load to the entry and keeps it live over the first two tests; body after mapGetLightParam lines up (1-word shift); evtSetUnitLightDir2 returns s32 (retail tail-calls it); vector checked (menu). */
#ifdef NON_MATCHING
s32 sbtlSetLight_Map(struct SBTLUNIT_WORK *wk, s32 frame, s32 flg) {
    u32 vstatus;

    if (!(wk->vstatus & 0x10000000) && !(wk_sbtl->flg & 0x8000000)) {
        mapGetLightParam(&sbtlSetLight_MapData, wk->pos.x, wk->pos.y, wk->pos.z);
        if (flg == 1) {
            wk->field4EC = frame;
        } else if (--wk->field4EC > 0) {
            return 0;
        }
        if (wk->flg & 0x800) {
            return 0;
        }
        vstatus = wk->vstatus;
        if (!(vstatus & 0x40000)) {
            SDF_VU0_LOADVECTOR(&sbtlSetLight_MapData.dir0);
            SDF_VU0_STOREVECTOR(&wk->light.dir0);
            wk->light.col0[0] = sbtlSetLight_MapData.col0[0];
            wk->light.rgb0 = sbtlSetLight_MapData.rgb0;
            wk->light.col0[1] = sbtlSetLight_MapData.col0[1];
            wk->light.col0[2] = sbtlSetLight_MapData.col0[2];
        }
        if (!(vstatus & 0x80000)) {
            SDF_VU0_LOADVECTOR(&sbtlSetLight_MapData.dir1);
            SDF_VU0_STOREVECTOR(&wk->light.dir1);
            wk->light.col1[0] = sbtlSetLight_MapData.col1[0];
            wk->light.rgb1 = sbtlSetLight_MapData.rgb1;
            wk->light.col1[1] = sbtlSetLight_MapData.col1[1];
            wk->light.col1[2] = sbtlSetLight_MapData.col1[2];
        }
        if (!(vstatus & 0x100000)) {
            SDF_VU0_LOADVECTOR(&sbtlSetLight_MapData.dir2);
            SDF_VU0_STOREVECTOR(&wk->light.dir2);
            wk->light.col2[0] = sbtlSetLight_MapData.col2[0];
            wk->light.rgb2 = sbtlSetLight_MapData.rgb2;
            wk->light.col2[1] = sbtlSetLight_MapData.col2[1];
            wk->light.col2[2] = sbtlSetLight_MapData.col2[2];
        }
        if (!(vstatus & 0x1C0000)) {
            wk->light.rgba = sbtlSetLight_MapData.rgba;
        }
        evtSetUnitLightAndAmbient3(wk->hEveUnit, frame, wk->light.rgb0, wk->light.rgb1, wk->light.rgb2, wk->light.rgba);
        SDF_VU0_LOADVECTOR(&wk->light.dir0);
        evtSetUnitLightDir(wk->hEveUnit, 0);
        SDF_VU0_LOADVECTOR(&wk->light.dir1);
        evtSetUnitLightDir1(wk->hEveUnit, 1);
        SDF_VU0_LOADVECTOR(&wk->light.dir2);
        return evtSetUnitLightDir2(wk->hEveUnit, 2);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetLight_Map);
#endif

s32 sbtlCheckMotionMove(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t dir = { 0.0f, 0.0f, -1.0f, 0.0f };
    sdfVector4_t pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    f32 move;
    f32 frame;

    if (wk->flg & SBTLUNIT_FLG_KAIHIMUTEKI) {
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
        if (!(wk->flg & SBTLUNIT_FLG_NODMGROT) && !sbtlCheckMapHit_Cross(&wk->pos, &pos, move)) {
            return 0;
        }
        SDF_VU0_LOADVECTOR(&pos);
        SDF_VU0_STOREVECTOR(&wk->pos);
        wk->field415 = 1;
    }
    return 1;
}

/* NON_MATCHING: 133 of 134 words match. Hp test: retail bnel where this build emits bne (branch-likely class); vector checked (menu). */
#ifdef NON_MATCHING
s32 sbtlCallBtlCheckEnshutuSummonUnit2(struct SBTLUNIT_WORK *wk) {
    SBTL_UNIT_SUB *sub;
    f32 rate;

    if (wk->vstatus & 0x10) {
        return 0;
    }
    if (!(wk_sbtl->flg & 0x40) || (wk->flg & 0x10) || (wk->vstatus & 0x2000000)) {
        rate = wk->m_rate;
    } else {
        rate = sbtlCheckEnshutuSummonUnit(wk) ? wk->m_rate : 0.0f;
    }
    sub = sbtlSetUnitSub(wk->major, wk->minor, wk->motionno);
    switch (wk->field3DE) {
        case 0:
            sbtlCallPlayMotionSENow(wk);
            wk->field3DE = 1;
            wk->field3DC = 0;
            break;
        case 1:
            wk->hModel->first->speed = rate;
            if (!sbtlCheckMotionEnd(wk)) {
                return 1;
            }
            if (sub->field18 != 1 && !(wk->vstatus & 0x8000000)) {
                wk->field3DC = 0;
                wk->field3DE = 2;
            }
            break;
        case 2:
            if ((wk->vstatus & 0x8000000) || ++wk->field3DC < wk->afterframe) {
                break;
            }
            if (wk->vstatus & 0x2000000) {
                sbtlSetMotion_sbtlUnit(wk, wk->model->field224, 0, 0, 0);
            } else if (!wk->changeflg) {
                if (wk->field414 & 0x20) {
                    sbtlSetMotion_sbtlUnit(wk, 0, wk->beforeframe, 0, 0);
                } else if (wk->unit.now.hp == 0) {
                    sbtlSetMotion_sbtlUnit(wk, 2, wk->beforeframe, 0, 0);
                } else if (wk->field414 & 0x80) {
                    sbtlSetMotion_sbtlUnit(wk, 0, wk->beforeframe, 0, 0);
                } else {
                    sbtlSetMotion_sbtlUnit(wk, 0, wk->beforeframe, 0, 0);
                }
            }
            break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCheckEnshutuSummonUnit2);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlReleaseUnitList_ID);

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
    id = dds3SerializeID();
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

/* NON_MATCHING: 124 of 360 words match. Same control flow and stores; gcc folds player+52 to DDS3_GBWK+7380 and keeps DDS3_GBWK in an extra saved reg, so every register differs; aisyoBase copy scheduled before visual[2..3]; one word long. R1 sBtlSetPlayerData. SBTLUNIT_WORK.model at 0x484 is the ActAI work (flags 0xF8, 0x11C hit_area_r, 0x120 priv_area_r per HD). flg param unused in R1. New SBTL_WORK fields equipSword/equipGun/equipBullet/equipBulletBk/stockMinor/unitPadID/hitTargetID/field436/mag/hgunNum/huumaDevilID/field644; SBTL_START_DATA pos/rot/stockId; sbtlCallBtlGetPlayerData3 prototype.; vector checked (menu). */
#ifdef NON_MATCHING
s32 sbtlCallBtlCreateUnit(s8 flg) {
    s32 minorTbl[4] = { 1, 2, 3, 0 };
    SBTL_PLAYER_DATA *player;
    struct SBTLUNIT_WORK *wk;
    SBTL_MOTION_VISUAL *visual;
    SBTL_UNIT_SUB *sub;
    s32 i;

    wk = NULL;
    player = &DDS3_GBWK->playerData;
    if (wk_sbtl->battleMode != 3) {
        sdatUnitWork_t *pu;
        wk = sbtlCreateUnit(0, minorTbl[wk_sbtl->battleMode], 0);
        wk->field1A0 = sbtlEntryUnitList(wk_sbtl->unitList, wk);
        visual = sbtlGetMotionVisualData(wk->major, wk->minor);
        sub = sbtlSetUnitSub(wk->major, wk->minor, 0);
        wk->field414 = 0x81;
        wk->field18 = 0.0f;
        wk->field424 = 0.0f;
        wk->minor_d = 0;
        wk->visualA = visual->field04;
        wk->field428 = visual->field08;
        sbtlGetRotateDeg(&wk->rot, 180.0f);
        SDF_VU0_STOREVECTOR(&wk->rot);
        wk->model->field100 = 0;
        wk->model->field0FC = 0;
        wk->model->flags = (wk->model->flags | 0xC0) & ~0x121;
        wk->angleY = 0.0f;
        wk_sbtl->targetUnitID = wk->field1A0;
        pu = &player->unit;
        wk->unit = *pu;
        if (pu->now.hp == 0) {
            wk->status = 0;
            wk->unit.now.hp = pu->now.maxhp;
            wk->unit.badStatus = 0;
            wk_sbtl->flg &= ~8;
        }
        wk->model->field11C = visual->field10;
        wk->model->field120 = sub->field04;
        SDF_VU0_LOADVECTOR(&main_data.pos);
        SDF_VU0_STOREVECTOR(&wk->pos);
        SDF_VU0_LOADVECTOR(&main_data.rot);
        SDF_VU0_STOREVECTOR(&wk->rot);
        sbtlRestartMotion(wk);
        wk->visual[0] = visual->scale;
        wk->color = 0x80808080;
        wk->visual[1] = visual->scale;
        wk->visual[2] = visual->scale;
        wk->visual[3] = visual->scale;
        wk->aisyoBase = wk->unit.aisyo;
    }
    wk_sbtl->hitTargetID = -1;
    wk_sbtl->equipBullet = player->val154;
    wk_sbtl->equipBulletBk = player->val154;
    wk_sbtl->hgunNum = sbtlCallBtlGetPlayerData3();
    wk_sbtl->equipSword = DDS3_GBWK->playerData.val146;
    wk_sbtl->mag = DDS3_GBWK->playerData.val20;
    wk_sbtl->equipGun = 1;
    wk_sbtl->stockNum = 0;
    wk_sbtl->field644 = 0;
    wk_sbtl->field436 = 0;
    for (i = 0; i < SDAT_STOCK_NUM; i++) {
        if (player->stock[i].devilId != 0 && (player->stock[i].flag & 1)) {
            SBTL_DEVIL_DATA *devil;

            wk_sbtl->stockNum++;
            devil = sbtlGetDevilData_sbtlSub(player->stock[i].devilId);
            wk_sbtl->stockDevilSkill[i] = player->stock[i].devilId;
            wk_sbtl->stockMinor[i] = devil->motionId;
        }
    }
    for (i = 0; i < 12; i++) {
        wk_sbtl->huumaDevilID[i] = -1;
    }
    wk_sbtl->field344 = 0;
    switch (wk_sbtl->battleMode) {
    case 0:
    case 2:
        wk_sbtl->summonIndex[0] = player->unkF9A;
        break;
    case 3:
        if (main_data.stockId >= 0 && main_data.stockId < wk_sbtl->stockNum) {
            wk_sbtl->summonIndex[0] = main_data.stockId;
        } else {
            wk_sbtl->summonIndex[0] = player->unkF9A;
        }
        if (wk_sbtl->stockNum > 0 && (player->stock[wk_sbtl->summonIndex[0]].flag & 1)) {
            sbtlSetSummon_Unit(wk, wk_sbtl->summonIndex[0], 0, 0, 0, 1);
            sbtlSetBit(&wk_sbtl->summonBit, wk_sbtl->summonIndex[0]);
            sbtlSetDevilSkillID(wk_sbtl->stockDevilSkill[wk_sbtl->summonIndex[0]], 0);
            wk_sbtl->field630->num = 1;
            wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
            wk_sbtl->unitPadID = wk->field1A0;
            wk_sbtl->flg = (wk_sbtl->flg | 0x4000000) & ~2;
            wk_sbtl->targetUnitID = wk->field1A0;
            wk->flg &= ~6;
            wk->model->flags = (wk->model->flags | 0xC0) & ~0x121;
            wk->vstatus |= 0x4000;
        }
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCreateUnit);
#endif

/* NON_MATCHING: 3 of 40 words match. Same flow; retail keeps the redundant wk = NULL as beql where cc1 threads the jump to the return (R02a branch-likely class), vector checked (32 rows, only the id == -1 path). */
#ifdef NON_MATCHING
struct SBTLUNIT_WORK *sbtlGetPlayer_HitTarget(void)
{
    struct SBTLUNIT_WORK *wk;
    s32 id = wk_sbtl->hitTargetID;

    wk = NULL;
    if (id != -1) {
        wk = sbtlSearchUnit_ID(wk_sbtl->unitList, id);
        if (wk == NULL) {
            wk = NULL;
        } else if (sbtlCheckDamageTarget(wk)) {
            wk_sbtl->hitTargetID = wk->field1A0;
        } else {
            wk = NULL;
        }
    }
    if (wk != NULL) {
        if (!(wk->field414 & 2)) {
            wk = NULL;
            wk_sbtl->hitTargetID = -1;
        } else if ((wk->flg & 0x8800) == 0x8800) {
            wk = NULL;
        }
    }
    return wk;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetPlayer_HitTarget);
#endif

/* NON_MATCHING: 0 of 67 words match. Retail copies sbtlSetVibeTable onto the stack and indexes the copy; this reads the table in place, vector checked (6 rows, table reads). */
#ifdef NON_MATCHING
void sbtlSetVibe(SBTLPLAYER_VTYPE mode)
{
    const SBTLPLAYER_VIBE *vibe = &sbtlSetVibeTable[mode];

    SetVibe(vibe->in, vibe->life, vibe->out, vibe->power);
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetVibe);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID5);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetPlayerUnit);

/* NON_MATCHING: 91 of 94 words match. Same flow; retail loads wk_sbtl into v1 before the second sbtlSearchUnit_ID, cc1 picks v0 (allocation only), vector checked (32 rows, loop path set flg and targetUnitID). */
#ifdef NON_MATCHING
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
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckPad);
#endif

s32 sbtlCallFalseHuuma(struct SBTLUNIT_WORK *wk)
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

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl36);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlActiveAction);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetItemtbl);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallMemset8);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlResetCombine);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallObjectBaseSetPadControl);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf63);

s32 sbtlNormalProcNakamaSub(struct SBTLUNIT_WORK *wk)
{
    return wk->status < 3;
}

INCLUDE_RODATA(const s32, "code_r1_0024F220", sbtlCallBtlCreateUnitTable);

INCLUDE_RODATA(const s32, "code_r1_0024F220", sbtlSetVibeTable);

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
            sbtlCallFalseHuuma(wk);
            sbtlCallBtlEndAction(wk, 1, 0);
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
        sbtlCallBtlEndAction(wk, 15, 0);
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
        sbtlCallFalseHuuma(wk);
        sbtlCallBtlEndAction(wk, 1, 0);
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

/* NON_MATCHING: 289 of 295 words match. Retail sets the return 0 before the three -1 stores and fills the jump delay slot with the last one; cc1 writes 'ok' with xori where retail uses sltiu; SBTL_REF11_DATA 0x0C..0x1B is u32 key[4] (rodata 0x40080000 is a mask, not a float); sbtlCallDeleteAllData, GetAnlogKeyToVecAndDir and sbtlCallDeleteAll prototypes in sBtlPlayer.h; vector checked (32 rows, all the timeout path: counter 1, three -1 stores, return 0). */
#ifdef NON_MATCHING
s32 sbtlCallDeleteAll(struct SBTLUNIT_WORK *wk)
{
    SBTL_REF11_DATA *data;
    s32 i;
    s32 j;
    s32 type;
    s32 ok;
    s32 fail;
    s32 cnt;
    s32 start;
    s32 flag;

    data = NULL;
    wk_sbtl->field760++;
    wk_sbtl->field760 = (wk_sbtl->field760 <= 0) ? 0 : (wk_sbtl->field760 > 3000) ? 3000 : wk_sbtl->field760;
    if (wk_sbtl->field760 >= wk_sbtl->field762 + wk_sbtl->field766) {
        DeleteAll(wk);
        wk_sbtl->field758 = -1;
        wk_sbtl->field750 = -1;
        wk_sbtl->field754 = -1;
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
                    ok = ((u32)(cnt - (start + wk_sbtl->field764)) < (data->field08 & 0xFFFF)) == 0;
                }
            }
        }
        if (ok != 1) {
            continue;
        }
        fail = 0;
        for (j = 0; j < 4; j++) {
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
                if (sbtlCallBtlSearchEffectSABActer(wk)) {
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
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallDeleteAll);
#endif

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

/* NON_MATCHING: 1 of 58 words match. Same tests in retail order; cc1 hoists the shared return 1 into v0 at entry (branch-likely class), which shifts every register by one, vector checked (32 rows, only the button-state early return). */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetBadStatus2(struct SBTLUNIT_WORK *wk)
{
    s16 mode;
    BST bst;

    if (dds3PadButtonState[42] >= 0) {
        return 1;
    }
    mode = wk_sbtl->mode;
    if (mode == 7) {
        return 1;
    }
    if (mode == 8) {
        return 1;
    }
    if (mode == 16) {
        return 1;
    }
    if (wk_sbtl->flg & 0x40000000) {
        return 1;
    }
    if (wk_sbtl->battleMode == 1 || mode == 15) {
        return 1;
    }
    bst = sbtlGetBadStatus(&wk->unit);
    if (bst == BST_BOUDO || bst == BST_MAHUU) {
        return 0;
    }
    if (wk_sbtl->mode == 14 || (wk->model->flags & 0x4000)) {
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
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetBadStatus2);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetNSTOP);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlResetNSTOP);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl12);

/* NON_MATCHING: 347 of 353 words match. Retail returns 1 with bne+delay where cc1 uses bnel (return-0 reorg class), and keeps a redundant ret = 2 after RestartBattle in mode 16 that cc1 drops; mode switch case order and store order taken from the asm; pad control proc (dds3BasicControlProc_t), retail reads neither argument; the mode procs (sbtlCallBtlGetSkilltbl2, sbtlPlayerRKaitenInit, KaihiProc_Nakama, GuardProc_Nakama, sbtlLeaf3) get s16 *mode = &wk_sbtl->mode as a second, unused argument (a1 set in the call's fallthrough, also in nakamamode_UnitPad); SBTL_WORK field440/442/444/478; vector checked (32 rows: flag early returns, mode 1 with ret 0 (walk_counter reset), the 0x478 counter and the field480 store at >= 20); other modes checked against the asm only. */
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
    wk->field18 = 0.0f;
    wk->field10 = 0.0f;
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
INCLUDE_ASM(const s32, "code_r1_0024F220", nakamamodeCallGetActiveScene);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", nakamamode_UnitPad);

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

/* NON_MATCHING: 11 of 36 words match. 5 aligned lines: the three status tests fill their delay slots from the fallthrough where retail copies the return-0 from the target (branch-likely class), vector checked (32 rows). */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetBadStatus3(struct SBTLUNIT_WORK *wk)
{
    u32 status;

    if (wk_sbtl->mode == 8) {
        return 1;
    }
    status = wk->status;
    if (status == 34 || status == 42 || status == 10) {
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
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetBadStatus3);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetRandomTarget);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlNormalProcNakamaSub2);

INCLUDE_ASM(const s32, "code_r1_0024F220", NormalProc_Nakama);

INCLUDE_ASM(const s32, "code_r1_0024F220", AtkProc_Nakama);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSetReturn);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetReturn);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallHirukoYukaGoIdx2B);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlResetColor);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlResetMenuPara);

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

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallLightSet);

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
    sbtlGetMotionVisualData(wk->major, wk->minor);
    length = GetAnlogKey(wk, &key);
    input = ChkHuuma_PlayerInputOk(wk);
    if ((sbtlNormalProcNakamaSub(wk) || (wk->status == 15 && wk_sbtl->field484 == 4)) && length >= 48.0f) {
        speed = wk->visualA;
        if (input == 4) {
            speed *= 0.14f;
        }
        wk->field18 = speed;
        SetRotateMatrixY(-mapGetScreenDir(wk->pos.x, wk->pos.y, wk->pos.z, 0.0f) * 0.017453292f);
        SDF_VU0_LOADVECTOR(&key);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_SCALEVECTOR(2.0f);
        SDF_VU0_LOADVECTOR_B(&wk->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&target);
        result = 1;
        sbtlGetRotate(wk, &target);
        UnitMV(wk, &target);
        sbtlCallBtlEndAction(wk, 2, 0);
        wk_sbtl->wait_counter = 0;
    }
    if (input == 0 && sbtlNormalProcNakamaSub(wk) && !(wk->vstatus & 0x80) && dds3PadButtonState[35] != 0 &&
        (wk_sbtl->field344 <= 0 || dds3PadButtonState[42] == 0)) {
        wk_sbtl->field494 = 0;
        wk_sbtl->mode = 13;
        wk_sbtl->field484 = 0;
        wk_sbtl->field488 = 0;
        sbtlCallBtlEndAction(wk, 15, 0);
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
                sbtlCallBtlSearchEffectData(wk, SKLID_HUUMAST, 1, 0, wk->field414, -1, 0);
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
                        sbtlCallBtlEndAction(enemy, 35, 0);
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
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlNormalProc_Huuma);
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
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetAnlogKey);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl13);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl14);

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

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallMemset9);

INCLUDE_ASM(const s32, "code_r1_0024F220", NormalProc);

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
        sbtlCallBtlGetNumOfEffectSAB(wk, item->sklid, 0);
        sbtlCallBtlEndAction(wk, 12, 0);
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
            FromEuler(0.0f, wk->angleY * 0.017453292f, 0.0f);
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
                FromEuler(0.0f, wk->angleY * 0.017453292f, 0.0f);
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
        if (sbtlCallBtlSearchEffectSABActer(wk)) {
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
            if (sbtlCallBtlSearchEffectSABActer(wk)) {
                wk_sbtl->mode = 1;
                wk_sbtl->gstat[0] = 0;
                wk_sbtl->gstat[1] = 0;
                sbtlCallFalseHuuma(wk);
                sbtlCallBtlEndAction(wk, 1, 0);
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
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlPlayerRKaitenInit);
#endif

s32 GuardProc_Nakama(struct SBTLUNIT_WORK *wk, s16 *mode)
{
    wk->flg |= 0x100;
    wk->vstatus |= SBTLUNIT_VST_GRAV;
    if (!dds3PadButtonState[3]) {
        sbtlCallFalseHuuma(wk);
        sbtlCallBtlGetNumOfEffectSAB(wk, 0, 0);
        sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
    }
    return 2;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", KaihiProc_Nakama);

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
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCheckDamageTarget3);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID6);

/* NON_MATCHING: 7 of 53 words match. Same tests in retail order; retail sets return 0 in each delay slot and keeps mode == 16 as its own block (branch-likely class). sbtlEscapeGauge gets (s16) of the f32 count as retail truncates and sign-extends, vector checked (32 rows, flg test path). */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchUnitID7(s32 x, s32 y)
{
    struct SBTLUNIT_WORK *unit;
    s16 mode;

    if (wk_sbtl == NULL) {
        return 0;
    }
    if (!(wk_sbtl->flg & 0x1400000) || (wk_sbtl->flg & 0x40)) {
        return 0;
    }
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (unit == NULL) {
        return 0;
    }
    if (unit->status == 34) {
        return 0;
    }
    mode = wk_sbtl->mode;
    if (mode == 7) {
        return 0;
    }
    if (mode == 8) {
        return 0;
    }
    if (mode == 16) {
        return 0;
    }
    if (mode == 11) {
        return 0;
    }
    if (wk_sbtl->flg & 8) {
        return 0;
    }
    sbtlEscapeGauge((s16)wk_sbtl->field510, wk_sbtl->field50C);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID7);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf3);

/* NON_MATCHING: 15 of 30 words match. Same flow; retail keeps wk_sbtl in a2 and uses bnel to the epilogue where cc1 picks a0 and plain branches, vector checked (32 rows, only the mode != 14 path). */
#ifdef NON_MATCHING
s32 sbtlRefSbtl15(s32 x, s32 y)
{
    if (wk_sbtl == NULL) {
        return 0;
    }
    if (wk_sbtl->mode == 14 && !(wk_sbtl->flg & 8) && wk_sbtl->field4B6 > 0) {
        if (wk_sbtl->field4B6 == 8 && wk_sbtl->field4BC < 26) {
            return 0;
        }
        sbtlCallBtlSprite(wk_sbtl->field4BA, wk_sbtl->field4B8);
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl15);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", SetColor);

INCLUDE_ASM(const s32, "code_r1_0024F220", CheckHuuma_Moon);

INCLUDE_ASM(const s32, "code_r1_0024F220", CheckHuuma2);

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

INCLUDE_RODATA(const s32, "code_r1_0024F220", sbtlCallMemset10Table);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallMemset10);

INCLUDE_ASM(const s32, "code_r1_0024F220", SetHuumaMiss);

s32 FalseHuuma(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *target;

    if (wk_sbtl->field4B6 != 0 || wk_sbtl->mode == 14) {
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
        if (target != NULL) {
            sbtlCallBtlEndAction(target, 9, 0);
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

INCLUDE_ASM(const s32, "code_r1_0024F220", FakeRndHit);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetSkilltbl2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID8);

INCLUDE_RODATA(const s32, "code_r1_0024F220", sbtlCallMemset10Table3);

INCLUDE_RODATA(const s32, "code_r1_0024F220", sbtlCallMemset10Table4);

INCLUDE_RODATA(const s32, "code_r1_0024F220", jtbl_00457F50);

INCLUDE_RODATA(const s32, "code_r1_0024F220", sbtlCallBtlGetSkilltbl2Table);

INCLUDE_RODATA(const s32, "code_r1_0024F220", sbtlCallBtlGetSkilltbl2Table2);

INCLUDE_RODATA(const s32, "code_r1_0024F220", sbtlCallBtlGetSkilltbl2Table3);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlResetCharge2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID9);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID10);

s32 sbtlCheckBit(u32 *bits, s32 bit)
{
    return (*bits >> bit) & 1;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetBit);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetDamage);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetSummon_Unit);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID11);

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
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallMemset11);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID12);

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
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetBadStatus4);
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

INCLUDE_ASM(const s32, "code_r1_0024F220", BeginSetsuma);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlBeginSetsumaSub);

/* NON_MATCHING: 196 of 197 words match. Only the branch at +0x118 differs (beq here, beql in retail); tried local order and one shared unit variable; the stop-key path is if (pressed && bst != BST_BOUDO) {...} else field64C = 0 for the block order; sbtlCallMemset6 takes (s32 index, s32 flg) with flg unused (callers pass 0 in a1); sbtlDeleteEffectData_SerialID2 takes an s32 serial, not a unit; prototypes sbtlForceReset (sBtlEff.h), sbtlCallSetClutColor(wk, BST) (sBtlCalc.h); SBTL_WORK.field64C s8, SBTLUNIT_WORK.field370 s16; vector checked (32 rows, all take the CheckAction_Nakama == 0 return); the summon-cancel loop walked against the asm only. */
#ifdef NON_MATCHING
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
                    sbtlCallBtlGetSkilltbl8(devil, 0x400);
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
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallCheckActionNakama);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl37);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckSkillEquation);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckTargetEquation);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_00);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_01);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCheckDamageTarget5);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_03);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetSkilltbl3);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_05);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetSkilltbl4);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_07);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_08);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_09);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_0a);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_0b);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_0c);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_0d);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_0e);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetSkilltbl5);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_10);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_11);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_12);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetSkilltbl6);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_14);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_15);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_16);

INCLUDE_ASM(const s32, "code_r1_0024F220", SEquationno_17);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlAI_SetSeikaku_Action);

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

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckAreaCount);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckArea_Atk);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckArea_Masic);

INCLUDE_ASM(const s32, "code_r1_0024F220", SetOrderTarget);

INCLUDE_ASM(const s32, "code_r1_0024F220", SetNextTarget);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCheckDamageTarget6);

INCLUDE_ASM(const s32, "code_r1_0024F220", CheckEnemyTarget);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetTargetUnit);

INCLUDE_ASM(const s32, "code_r1_0024F220", CheckBSTTarget);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetOrderTarget);

INCLUDE_ASM(const s32, "code_r1_0024F220", TEquationno_0c);

INCLUDE_ASM(const s32, "code_r1_0024F220", TEquationno_0d);

INCLUDE_ASM(const s32, "code_r1_0024F220", TEquationno_0e);

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

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf64);

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

INCLUDE_ASM(const s32, "code_r1_0024F220", Proc_sbtlAI);

/* NON_MATCHING: 176 of 239 words match. Retail jumps to one epilogue with the return value in the delay slot and lays the stat switch out as a tree (==2, >2, !=1), cc1 emits ==1/==2 and duplicates the epilogue; tried case order, default/case 0, enum and narrower casts on the index; flow walked against the asm; SBTL_WORK 0x1C4 is flg; SBTLAI_WORK gains backmode/status (HD order, shifted by the R1 handle); SBTLUNIT_WORK.field30 vector; vector checked (32 rows, all take the early return 0). */
#ifdef NON_MATCHING
s32 sbtlCallMemset12(struct SBTLUNIT_WORK *wk)
{
    sdfVector4_t zero;
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    BST bst;

    memset(&zero, 0, sizeof(zero));
    if (!(wk_sbtl->flg & 0x10000)) {
        return 0;
    }
    if (!(wk->flg & 4)) {
        return 0;
    }
    if (wk->model->flags & 0x1000) {
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
            if (sbtlCheckCrossCircle(&wk->pos, wk->model->field120 + wk->visualA * 4.0f, &unit->pos,
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
        sbtlCallBtlGetSkilltbl8(wk, 0x400);
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
                SDF_VU0_STOREVECTOR(&wk->field30);
                wk->aiwk->stat = 1;
                wk->actCounter = wk->actMax;
            }
        } else if (wk->flg & 0x40000) {
            if (wk->aiwk->mode == 0 && wk_sbtl->field424 > 0 && list->count != 0) {
                SDF_VU0_LOADVECTOR(&zero);
                SDF_VU0_STOREVECTOR(&wk->field30);
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
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallMemset12);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl16);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCreateUnitList2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetBadStatus5);

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

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf4);

/* NON_MATCHING: 6 of 39 words match. Same tests in retail order; cc1 hoists the shared return 0 into v0 where retail sets it in each delay slot (branch-likely class). Retail does not tail-call CheckAIScript, so CheckAIScript returns s8 (HD bool), vector checked (32 rows: model mode 9 and the CheckAIScript path). */
#ifdef NON_MATCHING
s32 sbtlProcScript(struct SBTLUNIT_WORK *wk)
{
    u32 status;

    if (wk == NULL) {
        return 0;
    }
    status = wk->status;
    if (status == 10 && !(wk->aiwk->scrflag & 0x120000)) {
        return 0;
    }
    if (status == 11) {
        return 0;
    }
    if (status == 14) {
        return 0;
    }
    if (wk->model->mode == 9) {
        return 0;
    }
    if (status == 30) {
        return 0;
    }
    if (wk->mode < 5) {
        return 0;
    }
    return CheckAIScript(wk);
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlProcScript);
#endif

s32 sbtlCreateAIScript(void)
{
    wk_sbtl->hAIScript = scrRegistScript2(ai_scr);
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallReleaseScript);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallRegistScript2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallReleaseScript2);

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
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl17);
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

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlDeath);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl19);

void sbtlBattleStart(SBTL_STAGE_DATA *stage)
{
    if (stage->flags & 0x20) {
        sbtlSupport_BattleStop(1, 1, 0);
    }
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetAttr);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf5);

INCLUDE_ASM(const s32, "code_r1_0024F220", CheckBadStatus);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckSkillCancel);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckEnshutuSummonUnit);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlDamage);

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

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlStartDeadEvent);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetPointLast);

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
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckEffectCancel);
#endif

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl38);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetUseTableTargetSBtlActA);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl20);

INCLUDE_ASM(const s32, "code_r1_0024F220", Support_RunWaitProcess);

INCLUDE_ASM(const s32, "code_r1_0024F220", Support_RunWarpProcess);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSupport_BattleStop);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallEncGetPointNums);

INCLUDE_ASM(const s32, "code_r1_0024F220", Support_RunDownProcess);

INCLUDE_ASM(const s32, "code_r1_0024F220", mapFunc12);

INCLUDE_ASM(const s32, "code_r1_0024F220", Support_GetNakamaData);

INCLUDE_ASM(const s32, "code_r1_0024F220", mapEncGetPointNums);

INCLUDE_ASM(const s32, "code_r1_0024F220", Support_SetAisyo);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSupport_StopAction);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSetMotionSBtlUnit);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetAITBL);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetIntPara31);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallEncGetPointNums4);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetIntReturnValue9);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetIntPara);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSetEndData);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlDeath);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetIntPara32);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlCheckDamageTarget7);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetIntPara33);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetIntPara34);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetIntPara35);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetIntPara36);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetIntReturnValue10);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetIntReturnValue11);

INCLUDE_ASM(const s32, "code_r1_0024F220", Support_RunZoomProcess);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetIntPara2);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetFloatPara);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetFloatReturnValue);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetIntPara3);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetIntReturnValue);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetIntPara4);

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetIntReturnValue2);
