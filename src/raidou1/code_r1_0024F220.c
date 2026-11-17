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

s32 sbtlBtlForceDeleteEffectWorkIDSub(s32 arg) {
    return sdfCallSndSendCmd2(arg);
}

s32 sbtlLeaf57(void) {
    return sdfBtlReLoadMapSBtlSceneSub();
}

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

void sbtlLeaf58(u32 seid) {
    p2sdStopSe(seid);
}

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

s32 sbtlSearchReturn_Unit(struct SBTLUNIT_WORK *wk)
{
    s32 i;
    struct SBTLUNIT_WORK *unit;

    if (wk_sbtl->battleMode != 3 && (wk->field414 & 1) && !(wk->field414 & 0x80)) {
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckReturn_Unit(unit) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

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

/* NON_MATCHING: 21 of 21 words match. Fragile, see docs/matching/code_r1_0024F220.md. */
#ifdef NON_MATCHING
s32 sbtlCheckMapHit_Data(f32 posx, f32 posz, f32 *px, f32 *pz)
{
    s32 ret;
    f32 getx, getz;

    ret = mapHitCalc(1, posx, posz, 40.0f, &getx, &getz);
    *px = getx;
    *pz = getz;
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCheckMapHit_Data);
#endif

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

    mv = sbtlGetVisualData(wk->major, wk->minor);
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
        wk->mv_spd.z *= 0.5f;
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
    ofs.x = -wk->mv_spd.x;
    ofs.z = -wk->mv_spd.z;
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
    visual = sbtlGetVisualData(wk->major, wk->minor);
    sub = sbtlGetMotionVisualData(wk->major, wk->minor, wk->next_motionno);
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

/* NON_MATCHING: 122 of 123 words match. The flg & 0x100000 test takes bne where retail has bnel; the frame params are s32 in retail (no sign extension), prototype changed; vector checked (menu): bst 18/19 hp-ratio branch confirmed (sbtlGetBadStatus gives 19 for no status). */
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
    } else if (motion == 0 && !(wk->vstatus & 0x1000000)) {
        bst = sbtlGetBadStatus(&wk->unit);
        if (bst != BST_RAND0 && bst != BST_BOYCOTT) {
            id = 2;
        } else if (wk->flg & 0x100000) {
            id = 0;
        } else if ((f32)wk->unit.now.hp / (f32)wk->unit.now.maxhp > 0.25) {
            id = motion;
        } else {
            id = 2;
        }
    } else {
        id = motion;
    }
    wk->next_motionID = id;
    wk->next_motionno = sbtlBtlCheckMotionNoSub(wk, wk->field414, id);
    next = sbtlGetMotionVisualData(wk->major, wk->minor, wk->next_motionno);
    now = sbtlGetMotionVisualData(wk->major, wk->minor, wk->motionno);
    if (!flg) {
        mv = sbtlGetVisualData(wk->major, wk->minor);
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
    wk->model->stat = 1;
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
    sbtlGetVisualData(wk->major, wk->minor);
    sub = sbtlGetMotionVisualData(wk->major, wk->minor, wk->next_motionno);
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

s32 sbtlCallBtlCreateEnemyList(dds3ProcessID_t *id)
{
    struct SBTLUNIT_WORK *wk;
    s32 i;

    if (wk_sbtl == NULL || wk_sbtl->unitList == NULL) {
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
    return 0;
}

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
    sub = sbtlGetMotionVisualData(wk->major, wk->minor, wk->motionno);
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
        visual = sbtlGetVisualData(wk->major, wk->minor);
        sub = sbtlGetMotionVisualData(wk->major, wk->minor, 0);
        wk->field414 = 0x81;
        wk->mv_spd.z = 0.0f;
        wk->field424 = 0.0f;
        wk->minor_d = 0;
        wk->max_mvspd.z = visual->field04;
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
    wk_sbtl->mag = DDS3_GBWK->playerData.mag;
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

INCLUDE_RODATA(const s32, "code_r1_0024F220", sbtlCallBtlCreateUnitTable);

