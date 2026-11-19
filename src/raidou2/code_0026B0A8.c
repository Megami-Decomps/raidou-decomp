#include "common.h"
#include "sBtlUnit.h"
#include "sBtlSM.h"
#include "sdfChip.h"
#include "ps2asm.h"
#include "sdfQuat.h"
#include "dds3Randomize.h"
#include "sbtlWork.h"
#include "sBtlSound.h"
#include "sbtlSub.h"
#include "dds3KernelCore.h"
#include "sBtlSkill.h"
#include "sndAt3.h"
#include "mdlManager.h"
#include "AW_XAtkPad.h"
#include "sBtlCalc.h"
#include "sBtlScene.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "EventBit.h"
#include "sBtlCmndMenuNew.h"
#include "dds3SceneBasic.h"
#include "code_r1_002250F8.h"
#include "sBtlAI.h"
#include "dds3ObjectBase.h"
#include "sBtlPlayer.h"
#include "code_r1_0010DF68.h"
#include "sBtlScript.h"
#include "code_0027A1E8.h"

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlReLoadMap);

void sbtlModelFieldLb(void) {
    sbtlCallMemGetPtrData2 = pacLoad("/model/field_lb/FIELD02D.LB");
}

void sbtlModelFieldLb2(void) {
    sbtlCallMemGetPtr2Data2 = pacLoad("/model/field_lb/FIELD09F.LB");
}

void sbtlCallMemGetPtr(void) {
    sdfPacDecodeResrc_t *resrc;
    s32 n = 0;

    for (resrc = sbtlCallMemGetPtrData2->pac.resrcHead; resrc != NULL; resrc = resrc->next, n++) {
        if (n == 0) {
            sbtlCallMemGetPtrData = resrc->resrc;
            sbtlData157 = sdfMemGetPtr(sbtlCallMemGetPtrData);
            sbtlData159 = sdfMemGetSize(resrc->resrc);
        } else {
            sdfMemFreeDirect(resrc->resrc);
        }
    }
    pacFileHandleFree_(sbtlCallMemGetPtrData2);
}

void sbtlCallMemGetPtr2(void) {
    sdfPacDecodeResrc_t *resrc;
    s32 n = 0;

    for (resrc = sbtlCallMemGetPtr2Data2->pac.resrcHead; resrc != NULL; resrc = resrc->next, n++) {
        if (n == 0) {
            sbtlCallMemGetPtr2Data = resrc->resrc;
            sbtlData158 = sdfMemGetPtr(sbtlCallMemGetPtr2Data);
            sbtlData160 = sdfMemGetSize(resrc->resrc);
        } else {
            sdfMemFreeDirect(resrc->resrc);
        }
    }
    pacFileHandleFree_(sbtlCallMemGetPtr2Data2);
}

void sbtlLoadMapFromMemorySub(void) {
    mdlPackDecode(2, 0x2D, 0x101, sbtlData157, sbtlData159);
}

void sbtlLoadMapFromMemorySub2(void) {
    mdlPackDecode(2, 0x9F, 0x101, sbtlData158, sbtlData160);
}

void sbtlCallSync(void) {
    sbtlModelFieldLb();
    pacSync(sbtlCallMemGetPtrData2);
    sbtlCallMemGetPtr();
    sbtlModelFieldLb2();
    pacSync(sbtlCallMemGetPtr2Data2);
    sbtlCallMemGetPtr2();
}

void sbtlModelFieldLb3(void) {
    char name[32];

    sprintf(name, "/model/field_lb/FIELD%03X.LB", LoadMap_FromMemoryData3);
    sbtlCallMemGetPtr3Data = pacLoad(name);
}

void sbtlCallMemGetPtr3(void) {
    sdfPacDecodeResrc_t *resrc;
    s32 n = 0;

    for (resrc = sbtlCallMemGetPtr3Data->pac.resrcHead; resrc != NULL; resrc = resrc->next, n++) {
        if (n == 0) {
            sbtlCallPackDecodeData = resrc->resrc;
            sbtlCallPackDecodeData2 = sdfMemGetPtr(sbtlCallPackDecodeData);
            sbtlCallPackDecodeData3 = sdfMemGetSize(resrc->resrc);
        } else {
            sdfMemFreeDirect(resrc->resrc);
        }
    }
    pacFileHandleFree_(sbtlCallMemGetPtr3Data);
}

void sbtlCallPackDecode(void) {
    mdlPackDecode(2, LoadMap_FromMemoryData3, 0x101, sbtlCallPackDecodeData2, sbtlCallPackDecodeData3);
    sdfMemFreeDirect(sbtlCallPackDecodeData);
    sbtlCallPackDecodeData = NULL;
    sbtlCallPackDecodeData2 = NULL;
    sbtlCallPackDecodeData3 = 0;
}

void sbtlCallSync2(void) {
    sbtlModelFieldLb3();
    pacSync(sbtlCallMemGetPtr3Data);
    sbtlCallMemGetPtr3();
    sbtlCallPackDecode();
}

INCLUDE_ASM(const s32, "code_0026B0A8", LoadMap_FromMemory);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallMemset2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetCutScroll);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlDefaultCutScroll);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlDefaultZoom2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf55);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallBtlSearchUnitID19);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf56);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpLeaf2);

s16 cmpProcSequenceSub(s32 stage) {
    return sbtlData100[stage].bgmId;
}

void cmpSetColor(s32 rgba) {
    wk_sbtl->mapclr = rgba;
    mdlSetColor(wk_sbtl->hMapMdl, rgba);
    if (wk_sbtl->hMapMdl2 != NULL) {
        mdlSetColor(wk_sbtl->hMapMdl2, wk_sbtl->mapclr);
    }
}

INCLUDE_ASM(const s32, "code_0026B0A8", cmpRefSbtl);

void cmpRefSbtl2(void)
{
    p2sdLoadEntry_t *midi;
    s32 i;

    midi = sdfSndGetTrackSlotTable();
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
    if (wk_sbtl->midiLoad[5].unk6 != 0) {
        wk_sbtl->midiCount8++;
    }
    if (wk_sbtl->midiLoad[6].unk6 != 0) {
        wk_sbtl->midiCount8++;
    }
}

void cmpLeaf3(struct SBTLUNIT_WORK *wk) {
    wk->se.field10 = 0;
    wk->se.at3Handle = NULL;
    wk->se.flag = 0;
    wk->se.pBossAt3Data = NULL;
    wk->se.bossAt3gstat = 0;
    wk->se.loadHandle = NULL;
    wk->se.loadFile = NULL;
    wk->se.loadData = NULL;
    wk->se.loadgstat = 0;
    wk->se.mot_status = 0;
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

void sbtlSummonDvTaskSub(SBTLSOUND_FOOTWORK *fwk)
{
    fwk->prevSMGID = 0;
    fwk->prevFrame = 100000.0f;
    fwk->played = 0;
}

void cmpCallMemFreeDirect(struct SBTLUNIT_WORK *wk)
{
    if (wk->se.at3Handle != NULL) {
        sdfMemFreeDirect(wk->se.at3Handle);
        wk->se.at3Handle = NULL;
        wk->se.pBossAt3Data = NULL;
    }
    if (wk->se.loadData != NULL) {
        sdfMemFreeDirect(wk->se.loadHandle);
        wk->se.loadHandle = NULL;
        wk->se.loadData = NULL;
    }
    if (wk->se.mot_SMGID != 0) {
        sbtlBtlForceDeleteEffectWorkIDSub(wk->se.mot_SMGID);
    }
    if (wk->se.mot_SMGID2 != 0) {
        sbtlBtlForceDeleteEffectWorkIDSub(wk->se.mot_SMGID2);
    }
    if (wk->se.mot_SMGID3 != 0) {
        sbtlBtlForceDeleteEffectWorkIDSub(wk->se.mot_SMGID3);
    }
    if ((wk->field414 & 1) && !(wk->field414 & 0x80) && wk->se.mot_SMGID != 0) {
        sdfSndReleaseMidiTrack(wk->se.mot_SMGID);
    }
    if (wk->field1A0 == wk_sbtl->targetUnitID && wk_sbtl->battleMode != 3) {
        switch (sbtlDat()->kind) {
        case 1:
            sbtlBtlForceDeleteEffectWorkIDSub(0x9E40000);
            sdfSndReleaseMidiTrack(0x9E40000);
            return;
        case 2:
            sbtlBtlForceDeleteEffectWorkIDSub(0x9E30000);
            sdfSndReleaseMidiTrack(0x9E30000);
            return;
        case 3:
            sbtlBtlForceDeleteEffectWorkIDSub(0x9E20000);
            sdfSndReleaseMidiTrack(0x9E20000);
            break;
        }
    }
}

DVLFOOTSE_WORK *sbtlDevilFootSEProcSub(s32 devilID)
{
    if (devilID == 156 || devilID == 363) {
        return &sbtlDevilFootSEProcSubData2;
    }
    return &sbtlDevilFootSEProcSubData3[devilID];
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

void sbtlPlaySE(u32 frame)
{
    sbtlSEVolAngleCalc(frame, 0x7F);
}

void sbtlSEVolCalc(u32 frame)
{
    sbtlSEVolAngleCalc(frame, 0x7F);
}

s32 sbtlSEVolAngleCalc_sbtlSound(const sdfVector4_t *pos, f32 maxlen, f32 minlen, s32 basevol, s32 minvol)
{
    sdfVector4_t eye;
    f32 len;
    s32 vol;

    if (pos == NULL) {
        return basevol;
    }
    if (evtBitCheck(0x847)) {
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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlay3DSE_BaseVol);

void DevilFootSEProc(struct SBTLUNIT_WORK *wk)
{
    s32 devilID;
    SBTLSOUND_FOOTWORK *fwk;
    DVLFOOTSE_WORK *foot;
    f32 frame;
    f32 prev;
    s32 i;
    u32 se;

    if (wk->field1A0 == wk_sbtl->targetUnitID && wk_sbtl->battleMode == 1) {
        devilID = 321;
    } else {
        devilID = wk->unit.devilId;
    }
    fwk = &wk->se.footwk;
    foot = sbtlDevilFootSEProcSub(devilID);
    if (wk->hModel != NULL) {
        frame = wk->hModel->first->frame;
    } else {
        frame = 0.0f;
    }
    prev = fwk->prevFrame;
    fwk->prevFrame = frame;
    if (frame < prev) {
        fwk->played = 0;
    }
    for (i = 0; i < foot->frameNum; i++) {
        if (i >= fwk->played && foot->frame[i] <= frame) {
            se = sbtlCallGetRandIntA5(fwk, devilID);
            if (se != 0) {
                sbtlSEVolAngleCalc(se, sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96));
                fwk->prevSMGID = se;
                fwk->played = i + 1;
            }
        }
    }
}

s32 NakamaMotion3DSE(struct SBTLSOUND_FOOTWORK *fwk, s32 devilID, f32 frame)
{
    DVLFOOTSE_WORK *foot = sbtlDevilFootSEProcSub(devilID);
    f32 prev;
    u32 se;
    s32 i;

    prev = fwk->prevFrame;
    fwk->prevFrame = frame;
    if (frame < prev) {
        fwk->played = 0;
    }
    for (i = 0; i < foot->frameNum; i++) {
        if (i >= fwk->played && foot->frame[i] <= frame) {
            se = sbtlCallGetRandIntA5(fwk, devilID);
            fwk->played = i + 1;
            fwk->prevSMGID = se;
            return se;
        }
    }
    return -1;
}

void sbtlPlayViceSE(struct SBTLUNIT_WORK *wk, s32 motionID)
{
    u32 se;
    s32 start;
    s32 stop;
    s32 i;
    struct SBTLUNIT_WORK *other;
    s32 vol;
    s32 motion;

    motion = motionID;
    if (wk->field1A0 == wk_sbtl->targetUnitID && wk_sbtl->battleMode != 3) {
        if (wk_sbtl->battleMode == 1) {
            se = GetDevilSE(321, motionID);
        } else {
            se = sbtlGetBossSE(wk, motionID);
        }
    } else {
        if (wk_sbtl->field430 && motionID == 7) {
            motion = 13;
        }
        se = GetDevilSE(wk->unit.devilId, motion);
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
    if (stop && wk->se.mot_seid != 0 && (wk->unit.devilId != 628 || motionID != 17)) {
        p2sdStopSe(wk->se.mot_seid);
    }
    if (se != 0) {
        vol = sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96);
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
    if (motionID == 12) {
        if (wk->field1A0 != wk_sbtl->targetUnitID || wk_sbtl->battleMode == 3 || wk_sbtl->battleMode == 1) {
            if (!(wk->se.field10 & 1)) {
                if (!wk->no_dead_se) {
                    sbtlPlayDeadVoice(wk);
                }
                return;
            }
        }
    }
    if ((u32)(motionID - 13) < 2 && !(wk->field414 & 0x80) && (wk->field414 & 1)) {
        sbtlStopSE(seid);
    }
    sbtlSEVolAngleCalc(seid, vol);
}

f32 sbtlPlayMotionSENowSub(struct SBTLUNIT_WORK *wk, s32 motionID) {
    return sbtlGetSETime(wk, motionID);
}

u32 sbtlGetBossSE(struct SBTLUNIT_WORK *wk, s32 motionID)
{
    u32 se;

    se = 0;
    switch (motionID) {
    case 4:
        se = dds3GetRandIntA(NULL, 2) ? 0x1000A : 0x1000B;
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
    case 39:
        se = 0x10011;
        break;
    case 31:
        se = 0x10012;
        break;
    case 24:
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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlayDamageSE);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallGetRandIntA5);

void sbtlPlaySpecialSE(const sdfVector4_t *pos, s32 specialType)
{
    u32 se = 0;

    switch (specialType) {
    case 1:
        se = 0x10014;
        break;
    case 2:
        se = 0x10015;
        break;
    case 3:
        se = 0x10016;
        break;
    case 4:
        se = 0x10019;
        break;
    case 5:
        se = 0x1001C;
        break;
    }
    if (se != 0) {
        sbtlSEVolAngleCalc(se, sbtlSEVolAngleCalc_sbtlSound(pos, 2000.0f, 1000.0f, 127, 96));
    }
}

void sbtlStopSE(u32 seid) {
    p2sdStopSe(seid);
}

INCLUDE_ASM(const s32, "code_0026B0A8", Proc_sbtlSound);

u32 sbtlGetDevilSESub(s32 devilID, s32 motionID) {
    return GetDevilSE(devilID, motionID);
}

/* NON_MATCHING: 2 of 98 words match.  */
#ifdef NON_MATCHING
u32 GetDevilSE(s32 devilID, s32 motionID)
{
    u32 se = (devilID << 16) + 0x4000000;
    u32 ret = 0;

    if (motionID == -1) {
        ret = se;
    } else if (motionID >= 4 && motionID < 32) {
        if (motionID == 17) {
            switch (devilID) {
            case 11:
            case 15:
            case 16:
            case 17:
            case 34:
            case 36:
            case 54:
            case 76:
            case 77:
            case 79:
            case 98:
            case 123:
            case 124:
            case 150:
            case 151:
            case 155:
            case 164:
            case 165:
            case 166:
            case 167:
            case 184:
            case 202:
                motionID = 3;
                break;
            }
        }
        ret = se + motionID;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_0026B0A8", GetDevilSE);
#endif

/* NON_MATCHING: 65 of 77 words match.  */
#ifdef NON_MATCHING
s32 sbtlLoadDevilSE(struct SBTLUNIT_WORK *wk)
{
    s32 devilID;
    u32 se;
    s32 isPlayer = 0;
    s32 ret;

    devilID = wk->unit.devilId;
    if (wk->field1A0 == wk_sbtl->targetUnitID) {
        devilID = 0;
        se = 0;
        isPlayer = 1;
        switch (sbtlDat()->kind) {
        case 1:
            se = 0x9E40000;
            break;
        case 2:
            se = 0x9E30000;
            break;
        case 3:
            se = 0x9E20000;
            break;
        }
    } else {
        se = GetDevilSE(devilID, -1);
    }
    ret = p2sdLoadCheckMidi(se);
    if (ret != 1) {
        if (ret == 0 && (wk_sbtl->midiCount8 != 2 || devilID < 208)) {
            p2sdLoadSMGFile(se);
            wk->se.mot_status = 1;
        }
        return 0;
    }
    if (!isPlayer) {
        wk->se.mot_SMGID = se;
    }
    sbtlBtlLoadSkillSESub(se);
    wk->se.mot_status = 2;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLoadDevilSE);
#endif

INCLUDE_ASM(const s32, "code_0026B0A8", GetR1CombSklSmg);

INCLUDE_ASM(const s32, "code_0026B0A8", GetSkillSE);

/* NON_MATCHING: 61 of 69 words match.  */
#ifdef NON_MATCHING
s32 sbtlLoadSkillSE(struct SBTLEFF_PACLIST *ewk)
{
    s32 skillID;
    struct SBTLSKILLP_TBL *tbl;
    u32 se;
    u32 flg;

    skillID = ewk->skillID;
    tbl = sbtlGetSkilltbl(skillID);
    flg = tbl->flg;
    if ((s32)flg < 0) {
        return 1;
    }
    if (skillID == 287) {
        return 1;
    }
    if (tbl->flg2 & 0x10) {
        return 1;
    }
    if (skillID == 674) {
        return 1;
    }
    if (flg & 0x100) {
        return 1;
    }
    se = GetSkillSE(skillID, -1);
    switch (p2sdLoadCheckMidi(se)) {
    case 0:
        if (wk_sbtl->midiSkillCount != 17 || (tbl->type & 0x1000) || (tbl->type & 0xFFF) == 8 || (tbl->s_type & 4)) {
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
#else
INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLoadSkillSE);
#endif

void sbtlBstse(sdfVector4_t *pos, SKLID sklid, s32 type)
{
    struct SBTLSKILLP_TBL *skl;
    u32 se;

    if (type == 1 && wk_sbtl->stageNo == 523 && sklid == SKLID_DANDOKUSATU) {
        sbtlSEVolAngleCalc(0x9470000, 127);
        return;
    }
    skl = sbtlGetSkilltbl(sklid);
    if (sklid != SKLID_DAISHYOT && sklid != SKLID_TYUSHYOT && (skl->flg & 0x80000000)) {
        return;
    }
    if (sklid == SKLID_HUUMAMIS) {
        return;
    }
    if (skl->flg & 0x100) {
        datDebugPrintf(sbtlBstseData);
        return;
    }
    switch (sklid) {
    case SKLID_OFS_BUTURI:
    case SKLID_OFS_BUTURI + 1:
    case SKLID_OFS_BUTURI + 2:
    case SKLID_OFS_BUTURI + 3:
    case SKLID_OFS_BUTURI + 4:
        datDebugPrintf(sbtlBstseData2);
        return;
    case SKLID_BS14_DEAD_R2:
    case SKLID_BS14_DEAD_R:
        sbtlPlayDeadVoice(wk_sbtl->scrUnitList[1]);
        break;
    case SKLID_BS14_DEAD_L2:
    case SKLID_BS14_DEAD_L:
        sbtlPlayDeadVoice(wk_sbtl->scrUnitList[2]);
        break;
    case SKLID_BS14_CHANGE:
        sbtlStopSkillSE(SKLID_BS14_CHANGE_W, -1);
        sbtlPlayDeadVoice(wk_sbtl->scrUnitList[0]);
        break;
    case SKLID_BS14_DEAD_S:
        sbtlPlayDeadVoice(wk_sbtl->scrUnitList[0]);
        break;
    }
    if (sklid == SKLID_DAISHYOT || sklid == SKLID_TYUSHYOT) {
        se = 0x10017;
    } else {
        se = GetSkillSE(sklid, type);
    }
    if (se != 0) {
        sbtlSEVolAngleCalc_sbtlSound(pos, 2000.0f, 1000.0f, 127, 96);
        sbtlSEVolAngleCalc(se, 127);
    }
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlSetRirituNakamaHDSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallLoad);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallEffectShutdown);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSoundatBossAt);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlModelDataLoadSBtlUnitSub);

void sbtlPlayDeadVoice(struct SBTLUNIT_WORK *wk) {
    if (wk->se.bossAt3Size != 0) {
        nbEffectShutdown();
        mnuInstallTitleSoundBufferFromData(wk->se.pBossAt3Data, wk->se.bossAt3Size, 2);
        sndCallWaitSema();
    }
}

void sbtlRefSbtl7(void)
{
    s32 i;

    wk_sbtl->field878 = 1;
    wk_sbtl->field879 = 1;
    wk_sbtl->field87A = 0;
    for (i = 0; i < 256; i++) {
        sbtlWork10[i].count = 0x0FFFFFFF;
        sbtlWork10[i].field04 = 4;
    }
    sbtlLeaf2();
}

void sbtlCallEffectShutdown3(s32 mode)
{
    nbEffectShutdown();
    p2sdStopAllSe();
}

/* NON_MATCHING: 22 of 23 words match.  */
#ifdef NON_MATCHING
void sbtlStopSkillSE(SKLID skillid, s32 type) {
    s32 se;

    if ((u32)(type - 1) < 2 || type == 5) {
        se = GetSkillSE(skillid, type);
        if (se > 0x10035) {
            p2sdStopSe(se);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0026B0A8", sbtlStopSkillSE);
#endif

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlFunc10);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlENMY);

void sbtlLeaf2(void)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        sbtlWork11[i].active = 0;
        sbtlWork11[i].type = 0;
        sbtlWork11[i].seid = 0;
        sbtlWork11[i].vol = 127;
    }
    sbtlCallBtlSEVolAngleCalcData = 0;
}

void sbtlLeaf3(u32 seid, s32 type, s32 vol)
{
    SBTLSOUND_SEQUEUE *queue = sbtlWork11;
    s32 i;

    for (i = 0; i < 32; i++, queue++) {
        if (queue->active == 0) {
            queue->active = 1;
            queue->type = type;
            queue->seid = seid;
            queue->vol = vol;
            return;
        }
    }
}

void sbtlCallBtlSEVolAngleCalc(void)
{
    s32 i;

    if (sbtlCallBtlSEVolAngleCalcData > 0) {
        sbtlCallBtlSEVolAngleCalcData--;
    }
    for (i = 0; i < 32; i++) {
        if (sbtlWork11[i].active) {
            if (sbtlWork11[i].type > 0) {
                sbtlWork11[i].type--;
            }
            if (sbtlWork11[i].type == 0) {
                sbtlSEVolAngleCalc(sbtlWork11[i].seid, sbtlWork11[i].vol);
                sbtlWork11[i].active = 0;
                sbtlWork11[i].type = 0;
                sbtlWork11[i].seid = 0;
                sbtlWork11[i].vol = 127;
                sbtlCallBtlSEVolAngleCalcData = 2;
                return;
            }
        }
    }
}

s32 Init_sbtlUnit(s8 flg)
{
    wk_sbtl->unitList = sbtlCreateUnitList(48);
    dds3LinkProcess(wk_sbtl->process, dds3StartProcess(Init_sbtlUnitStrSbtlunitProc, 1015, 1, 1, Proc_sbtlUnit, sbtlModelDataLoad, NULL));
    sbtlSetPlayerData(flg);
    return 1;
}

void sbtlSetEndBadStatus(void)
{
    struct SBTLUNIT_WORK *wk;
    BST bst;
    s32 i;

    if (wk_sbtl->member_num <= 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        wk = sbtlGetNakama(i);
        if (wk == NULL || !CheckLiveTarget(wk)) {
            continue;
        }
        bst = sbtlGetBadStatus(&wk->unit);
        if (bst != BST_RAND0 && bst != BST_BOYCOTT && bst != BST_MOUDOKU && bst != BST_STONE) {
            sbtlResetBadStatus(wk, BSTF_DEAD | BSTF_GUN | BSTF_SLEEP | BSTF_MOUDOKU | BSTF_MAHUU | BSTF_BOUDO | BSTF_PANIC
                                       | BSTF_CHARM | BSTF_KOUFUN | BSTF_KAEN | BSTF_APPAKU | BSTF_KANDEN | BSTF_HYOUKETU
                                       | BSTF_KANASHIB);
        }
        sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
    }
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetEndData);

s32 Exit_sbtlUnit(void)
{
    struct SBTLUNIT_LISTWORK *lwk = wk_sbtl->unitList;
    struct SBTLUNIT_WORK *wk;
    s32 busy;
    s32 num;
    s32 i;

    if (lwk == NULL) {
        return 1;
    }
    num = lwk->number;
    busy = 0;
    for (i = 0; i < num; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL) {
            if ((u16)(wk->mode - 1) >= 2) {
                sbtlDeleteUnit(wk);
            } else {
                busy = 1;
            }
        }
    }
    if (busy == 1) {
        return 0;
    }
    if (wk_sbtl->unitList != NULL) {
        sbtlDeleteUnitList(wk_sbtl->unitList);
        wk_sbtl->unitList = NULL;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallBtlGetPlayerData2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlRefSbtl32);

INCLUDE_RODATA(const s32, "code_0026B0A8", Init_sbtlUnitStrSbtlunitProc);

s32 Draw_sbtlUnit(void) {
    struct SBTLUNIT_WORK *wk;
    struct SBTLUNIT_WORK *target;
    s32 i;

    if (wk_sbtl == NULL || wk_sbtl->unitList == NULL) {
        return 0;
    }
    wk_sbtl->field4308 = 0;
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
                        sbtlDrawShadow(wk);
                    }
                }
                if ((wk->flg & 0x80000000) && wk_sbtl->fieldE8C == 0) {
                    target = sbtlGetPlayer_HitTarget();
                    if (target == NULL) {
                        sbtlCallBtlGetRaceName(wk);
                        wk_sbtl->field4308++;
                    } else if (target == wk) {
                        sbtlCallBtlGetRaceName(wk);
                        wk_sbtl->field4308++;
                    }
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
    if (ID < 0) {
        return NULL;
    }
    if (wk_sbtl == NULL || wk_sbtl->unitList == NULL) {
        return NULL;
    }
    if (ID >= lwk->number) {
        return NULL;
    }
    return lwk->pUnitL[ID].pUnit;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSearchUnit_GroupID);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSearchUnit_Nakama);

s32 sbtlGetNumUnit_GroupID(struct SBTLUNIT_LISTWORK *lwk, s32 serialID)
{
    struct SBTLUNIT_WORK *wk;
    s32 num = 0;
    s32 i;

    if (lwk == NULL) {
        return 0;
    }
    for (i = 0; i < lwk->number; i++) {
        wk = lwk->pUnitL[i].pUnit;
        if (wk != NULL && !(wk->field414 & 0x18) && wk->GroupID == serialID) {
            num++;
        }
        if (sbtlCheckDelWork_Request(wk_sbtl->effdelList, serialID)) {
            num++;
        }
    }
    return num;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetNumUnit_GroupID2);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf4);

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
    wk->cacheHandle = NULL;
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
    wk->mag_on1 = -1;
    wk->mag_on2 = -1;
    wk->mag_on3 = -1;
    wk->dead_on = -1;
    wk->field5E0 = 0;
    wk->field5E4 = 0;
    wk->field704 = 0;
    wk->field708 = 0;
    wk->nakama_asimoto_anim = 0.0f;
    wk->field710 = 0;
    wk->field714 = 0;
    wk->field718 = 0;
    wk->field71C = 0;
    wk->field72C = 0;
    wk->asimoto_mode = 0;
    wk->asimoto_pos_x = 0.0f;
    wk->asimoto_pos_z = 0.0f;
    wk->asimoto_pos_x2 = 0.0f;
    wk->asimoto_pos_z2 = 0.0f;
    wk->asimoto_pos_x3 = 0.0f;
    wk->asimoto_pos_z3 = 0.0f;
    wk->asimoto_pos_x4 = 0.0f;
    wk->asimoto_pos_z4 = 0.0f;
    wk->field768 = 0;
    wk->field76C = 0;
    wk->field5DE = 0;
    wk->no_dead_se = 0;
    wk->miusinai = 0;
    wk->field790 = -4000.0f;
    wk->fieldFC = 1.0f;
    wk->kuramasi = 0;
    wk->field778 = 0;
    wk->field78C = 0;
    wk->field794 = 0;
    wk->L2_muteki = 0;
    wk->field79C = 0;
    wk->field780 = 0;
    wk->field7A0 = 0;
    wk->field7A4 = 0;
    wk->field7A8 = 0;
    wk->fieldF0 = 0;
    wk->fieldF4 = 0;
    wk->fieldF8 = 0;
    wk->field7AC = 0;
    wk->field7B0 = 0;
    wk->eff_wait_kaisin = 0;
    Init_sbtlScript(wk);
    cmpLeaf3(wk);
    return wk;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlDeleteUnit_UnWork);

s32 sbtlCheckReturn_Unit(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    s32 index;

    if (wk_sbtl->flg2 & 0x100) {
        index = wk_sbtl->returnindex[0];
        if (index != -1) {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[index]);
            if (unit != NULL && wk->field1A0 == unit->field1A0) {
                return 1;
            }
        }
    }
    return 0;
}

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

s32 sbtlDeleteUnit(struct SBTLUNIT_WORK *wk) {
    if (wk_sbtl->battleMode != 3 && (wk->field414 & 1) && !(wk->field414 & 0x80)) {
        wk_sbtl->returnindex[0] = -1;
        wk_sbtl->flg2 &= ~0x100;
        sbtlReleaseDvltk_List(wk_sbtl->dvltkList, 1);
    }
    sbtlReleaseUnitList_ID(wk_sbtl->unitList, wk->field1A0);
    if (wk->pBasic != NULL) {
        dds3DeleteSceneBasicData(wk->pBasic);
        wk->pBasic = NULL;
        wk->hEveUnit = NULL;
    }
    if (wk->pBasic_d != NULL) {
        dds3DeleteSceneBasicData(wk->pBasic_d);
        wk->pBasic_d = NULL;
        wk->hEveUnit_d = NULL;
    }
    if (wk->msgHandle != NULL) {
        if (wk->cacheHandle != NULL) {
            effReleaseSharedReference(wk->cacheRef);
            sdfMemFreeDirect(wk->cacheHandle);
            wk->field4E0 = 0;
            wk->cacheHandle = NULL;
        }
        if (wk->field61C != NULL) {
            effReleaseSharedReference(wk->field624);
            sdfMemFreeDirect(wk->field61C);
            wk->field620 = 0;
            wk->field61C = NULL;
            wk->field624 = NULL;
        }
        sdfMemFreeDirect(wk->msgHandle);
        wk->msgHandle = NULL;
        wk->msgData = NULL;
    }
    if (wk->hitwk.status & 1) {
        sbtlDeleteAttackHitData(wk);
    }
    sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
    sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
    if (!(wk->flg & 0x800)) {
        cmpCallMemFreeDirect(wk);
    }
    sbtlDelete(wk->model);
    Proc_sbtlAI(wk->aiwk);
    if (!(wk->field414 & 2)) {
        if (!(wk->flg & 0x800)) {
            sbtlCallMemFreeDirect(wk->major, wk->minor, wk->unit.devilId);
        }
    } else {
        if (!(wk->flg & 0x800)) {
            if (!sbtlRefSbtl33(wk)) {
                sbtlCallMemFreeDirect(wk->major, wk->minor, wk->unit.devilId);
            }
        }
    }
    sdfMemFreeDirect(wk->handle);
    return 1;
}

s32 sbtlRefSbtl33(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && wk->field1A0 != unit->field1A0 && unit->major == wk->major && unit->minor == wk->minor &&
            unit->unit.devilId == wk->unit.devilId) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallSin);

s32 sbtlUseItemProcSBtlPlayerSub(void)
{
    return 1;
}

s32 sbtlUseItemProcSBtlPlayerSub2(struct SBTLUNIT_WORK *wk, u32 rgba)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf5);

s32 sbtlResetColorSub(u32 color)
{
    return 1;
}

void sbtlBtlFadeOutBadStatusEffectSub(struct SBTLUNIT_WORK *wk)
{
    wk->chgcolor = 0;
    wk->color = 0x80808080;
}

void sbtlLeaf59(struct SBTLUNIT_WORK *wk, u32 color)
{
    wk->chgcolor = color;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf60);

s32 sbtlChangeModelColor(struct SBTLUNIT_WORK *wk)
{
    u32 color = wk->chgcolor;

    if (color != 0) {
        wk->color = color;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlResetMutekiColor);

void sbtlBtlFadeOutBadStatusEffectSub2(struct SBTLUNIT_WORK *wk)
{
    wk->dstcolor = 0;
    wk->color = 0x80808080;
    wk->field3D4 = 0x80808080;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlChangeModelData);

s32 sbtlCallBtlCheckMapHitData(f32 posx, f32 posz)
{
    f32 px;
    f32 pz;
    s32 ret;

    ret = sbtlCheckMapHit_Data(posx, posz, &px, &pz);
    return ret != -1 ? ret == 0 : 0;
}

INCLUDE_ASM(const s32, "code_0026B0A8", CheckMapHit);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckMapHit_Data);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckMapHit_Cross);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetRotate);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetRotate_Kaihi);

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

INCLUDE_ASM(const s32, "code_0026B0A8", UnitMV_sbtlUnit);

s32 sbtlCheckMotionNo(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no)
{
    return wk->motionno == sbtlGetMotionNo(wk, wk->field414, no);
}

s32 sbtlCheckMotionEnd(struct SBTLUNIT_WORK *wk)
{
    if (wk->hModel == NULL) {
        return 1;
    }
    if (sbtlCheckLoopMotion(wk, wk->motionID) != 0) {
        return 0;
    }
    return wk->hModel->first->stat == 5;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckGuardMotion);

s32 sbtlRestartMotion(struct SBTLUNIT_WORK *wk)
{
    SBTL_MOTION_VISUAL *visual;
    SBTL_UNIT_SUB *sub;

    if (wk->vstatus & 0x10) {
        return 0;
    }
    visual = sbtlGetMotionVisualData(wk->major, wk->minor);
    sub = sbtlGetMotionVisualData_sbtlVisual(wk->major, wk->minor, wk->next_motionno);
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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetMotion_ID);

s32 sbtlGetMotionNo(struct SBTLUNIT_WORK *wk, u8 side, s32 no)
{
    if (wk->field6A0 != 0 && no < SBTLUNIT_MOTION_ATTACK && no >= SBTLUNIT_MOTION_WAIT) {
        return sbtlGetMotionNoData[wk->motion_setno][wk->field6A0];
    }
    if (wk->flg & 0x8000000) {
        return sbtlGetMotionNoData[wk->motion_setno][no];
    }
    if (side & 0x80) {
        if (wk->vstatus & 0x1000000) {
            if (no == SBTLUNIT_MOTION_WAIT) {
                return 31;
            }
            if (no == SBTLUNIT_MOTION_WALK) {
                return 32;
            }
        }
        return sbtlGetMotionNoData2[no];
    }
    if (wk->minor > 0) {
        return sbtlGetMotionNoData3[no];
    }
    return sbtlGetMotionNoData[0][no];
}

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable2);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable3);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable4);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable5);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable6);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable7);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable8);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable9);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable10);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable11);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable12);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable13);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable14);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable15);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable16);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable17);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable18);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable19);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlResetAtkSklEff);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetMotion_sbtlUnit);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlplayertblSetPlayerMotionData_Hokan);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlplayertblLeaf);

s32 sbtlSetFade(struct SBTLUNIT_WORK *wk, SBTLUNIT_VST flg, s32 start, s32 count, s32 para)
{
    wk->fadepara = para;
    wk->fade_sttime = start;
    wk->vstatus |= flg;
    wk->cmpfade = count;
    wk->fadecnt = 0;
    return 1;
}

/* NON_MATCHING: 64 of 179 words match.  */
#ifdef NON_MATCHING
s32 sbtlFadeProc(struct SBTLUNIT_WORK *wk)
{
    s16 cnt;
    u32 vstatus;
    u32 color;
    u32 base;
    u32 max;
    u32 level;
    s32 alpha;

    if (!wk_sbtl->field42CA) {
        if (IsSceanStop() && !(wk->flg & 0x10) && !sbtlCheckEnshutuSummonUnit(wk)) {
            return 0;
        }
        if ((wk_sbtl->flg & 0x8000000) && !(wk->flg & 0x10)) {
            return 0;
        }
    }
    cnt = ++wk->fadecnt;
    if (cnt < wk->fade_sttime) {
        return 0;
    }
    if (wk->cmpfade != 0 && cnt >= wk->fade_sttime + wk->cmpfade) {
        wk->vstatus &= ~1;
        wk->vstatus &= ~2;
    }
    if ((wk->flg & 0x100000) && (wk->unit.badStatus & 0x1000)) {
        color = wk->color;
        level = color >> 24;
        if (wk->dstcolor == 0) {
            base = color;
            max = 128;
        } else {
            base = wk->dstcolor;
            max = (wk->dstcolor & 0xFF000000) >> 24;
        }
        if (wk->vstatus & 1) {
            level += wk->fadepara;
            level = level != 0 ? (level < max ? level : max) : 0;
            wk->color = (base & 0xFFFFFF) | (level << 24);
            if (level >= max) {
                wk->vstatus &= ~1;
            }
            return 1;
        }
        level -= wk->fadepara;
        level = level != 0 ? (level < max ? level : max) : 0;
        wk->color = (base & 0xFFFFFF) | (level << 24);
        if (level != 0) {
            return 1;
        }
        wk->vstatus &= ~2;
        if (wk->field6C4 & 1) {
            wk->mode = 9;
        }
        return 1;
    } else {
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
            return 1;
        }
        alpha = color >> 24;
        alpha -= wk->fadepara;
        alpha = alpha > 0 ? (alpha < 129 ? alpha : 128) : 0;
        wk->color = (color & 0xFFFFFF) | (alpha << 24);
        if (alpha > 0) {
            return 1;
        }
        wk->vstatus = vstatus & ~2;
    }
    if (wk->field6C4 & 1) {
        wk->mode = 9;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0026B0A8", sbtlFadeProc);
#endif

/* NON_MATCHING: 46 of 76 words match.  */
#ifdef NON_MATCHING
s32 RGBFade(struct SBTLUNIT_WORK *wk)
{
    s32 fadergb;
    s32 r;
    s32 g;
    s32 b;

    if (IsSceanStop() && !(wk->flg & 0x10)) {
        return 0;
    }
    if ((wk_sbtl->flg & 0x8000000) && !(wk->flg & 0x10)) {
        return 0;
    }
    if (wk->fadergb) {
        fadergb = wk->fadergb;
    } else {
        fadergb = 8;
    }
    r = (wk->color & 0xFF) + fadergb;
    g = (wk->color & 0xFF00) >> 8;
    b = (wk->color >> 16) & 0xFF;
    r = r <= 0 ? 0 : r < 129 ? r : 128;
    g += fadergb;
    g = g <= 0 ? 0 : g < 129 ? g : 128;
    b += fadergb;
    b = b <= 0 ? 0 : b < 129 ? b : 128;
    wk->color = (wk->color & 0xFF000000) | r | (g << 8) | (b << 16);
    if (r >= 128) {
        wk->vstatus &= ~0x200000;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0026B0A8", RGBFade);
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

struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_AI(u8 side)
{
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLUNIT_WORK *wk;
    s32 i;

    lwk = sbtlCreateUnitList(48);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 0x3F) == side && wk->status != 35 && sbtlCheckLiveTarget_NotHitDmg(wk) != 0) {
            sbtlEntryUnitList(lwk, wk);
        }
    }
    return lwk;
}

struct SBTLUNIT_LISTWORK *sbtlCreateTargetList(u8 side)
{
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLUNIT_WORK *wk;
    s32 i;

    lwk = sbtlCreateUnitList(48);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 0x3F) == side && wk->status != 35 && sbtlCheckDamageTarget(wk) != 0) {
            sbtlEntryUnitList(lwk, wk);
        }
    }
    return lwk;
}

struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_DeadOn(u8 side)
{
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLUNIT_WORK *wk;
    s32 i;

    lwk = sbtlCreateUnitList(48);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 0x3F) == side && wk->status != 35) {
            sbtlEntryUnitList(lwk, wk);
        }
    }
    return lwk;
}

INCLUDE_ASM(const s32, "code_0026B0A8", ChangeTarget_BSHelper);

struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Talk(u8 side)
{
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_LISTWORK *sorted;
    struct SBTLUNIT_WORK *wk;
    struct SBTLUNIT_WORK *front;
    s32 i;
    s32 frontIndex;
    f32 frontX;
    s32 count;

    list = sbtlCreateUnitList(48);
    sorted = sbtlCreateUnitList(48);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 0x3F) == side && !(wk->flg & 0x1000) && wk->status != 35
            && sbtlCheckDamageTarget(wk)) {
            sbtlEntryUnitList(list, wk);
        }
    }
    count = 0;
    if (list->count > 0) {
        frontIndex = 0;
        do {
            frontX = -10000.0f;
            front = NULL;
            for (i = 0; i < list->number; i++) {
                wk = list->pUnitL[i].pUnit;
                if (wk != NULL && CheckLiveTarget(wk)) {
                    SDF_VU0_LOADVECTOR(&wk->pos);
                    if (wk->pos.x >= frontX) {
                        frontX = wk->pos.x;
                        front = wk;
                        frontIndex = i;
                    }
                }
            }
            if (front == NULL) {
                continue;
            }
            if ((front->flg & 0x8800) == 0x8800) {
                count++;
                if (count == 6) {
                    sbtlEntryUnitList(sorted, front);
                }
                sbtlReleaseUnitList_ID(list, frontIndex);
            } else {
                sbtlEntryUnitList(sorted, front);
                sbtlReleaseUnitList_ID(list, frontIndex);
            }
        } while (list->count > 0);
    }
    sbtlDeleteUnitList(list);
    return sorted;
}

struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Analyze(u8 side)
{
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_LISTWORK *sorted;
    struct SBTLUNIT_WORK *wk;
    struct SBTLUNIT_WORK *front;
    s32 i;
    s32 frontIndex;
    f32 frontX;
    s32 count;

    list = sbtlCreateUnitList(48);
    sorted = sbtlCreateUnitList(48);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 0x3F) == side && !(wk->flg & 0x1000) && wk->status != 35
            && sbtlCheckDamageTarget(wk)) {
            sbtlEntryUnitList(list, wk);
        }
    }
    count = 0;
    if (list->count > 0) {
        frontIndex = 0;
        do {
            frontX = -10000.0f;
            front = NULL;
            for (i = 0; i < list->number; i++) {
                wk = list->pUnitL[i].pUnit;
                if (wk != NULL && CheckLiveTarget(wk)) {
                    SDF_VU0_LOADVECTOR(&wk->pos);
                    if (wk->pos.x >= frontX) {
                        frontX = wk->pos.x;
                        front = wk;
                        frontIndex = i;
                    }
                }
            }
            if (front == NULL) {
                continue;
            }
            if ((front->flg & 0x8800) == 0x8800) {
                count++;
                if (count == 6) {
                    sbtlEntryUnitList(sorted, front);
                }
                sbtlReleaseUnitList_ID(list, frontIndex);
            } else {
                sbtlEntryUnitList(sorted, front);
                sbtlReleaseUnitList_ID(list, frontIndex);
            }
        } while (list->count > 0);
    }
    sbtlDeleteUnitList(list);
    return sorted;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_Group);

struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_PForward(u8 side)
{
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLUNIT_WORK *wk;
    struct SBTLUNIT_WORK *target;
    s32 i;

    lwk = sbtlCreateUnitList(48);
    target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 0x3F) == side && wk->status != 35 && sbtlCheckDamageTarget(wk) != 0
            && target != NULL) {
            if (sbtlCheckCrossCircle_Rot(&target->pos, &target->rot, 1000.0f, &wk->pos, wk->model->field11C, 0.099999995f)) {
                sbtlEntryUnitList(lwk, wk);
            }
        }
    }
    return lwk;
}

void sbtlUpdateMiusinai(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);

    if (target != NULL) {
        if (sbtlCheckDamageTargetTemp(target)) {
            wk->miusinai = 0;
        } else {
            wk->miusinai = 1;
        }
    } else {
        wk->miusinai = 0;
    }
}

s32 sbtlCheckDamageTargetTemp(struct SBTLUNIT_WORK *wk) {
    s32 ret = 0;

    if (wk != NULL) {
        ret = wk->kuramasi == 0;
    }
    return ret;
}

s32 sbtlCheckDamageTarget(struct SBTLUNIT_WORK *wk)
{
    s32 status;

    if (wk == NULL) {
        return 0;
    }
    status = wk->status;
    if (status != 10 && !(wk->flg & 0x5000) && status != 11 && status != 14 && wk->model->mode != 9 && status != 30 && wk->mode >= 5 && status != 40) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf6);

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

    if (status != 10 && wk->unit.devilId != 681 && !(wk->flg & 0x1000) && (!(wk->flg & 0x800) || (wk->flg & 0x4800) == 0x800) && status != 11 && status != 14 && wk->model->mode != 9 && wk->mode >= 5 && status != 40) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckTateSkillSelectTarget);

/* NON_MATCHING: 33 of 69 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckTalkSelectTarget(struct SBTLUNIT_WORK *unit)
{
    s16 id;

    if (unit->flg & 0x100000) {
        id = unit->unit.devilId;
        if (id != 505 && id != 473 && id != 493 && id != 494 && id != 388 && id != 389 && id != 390 && id != 391) {
            return 0;
        }
    }
    switch (unit->status) {
    case 10:
    case 19:
        return 0;
    case 54:
        if (unit->field6C4 & 2) {
            return 0;
        }
        break;
    }
    if (unit->mode != 8) {
        return 0;
    }
    if (unit->vstatus & 0x1000) {
        return 0;
    }
    if (unit->vstatus & 1) {
        return 0;
    }
    if (unit->field6F4) {
        return 0;
    }
    if (!sbtlCheckDamageTargetTemp(unit)) {
        return 0;
    }
    return sbtlCheckDamageTarget(unit) != 0;
}
#else
INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckTalkSelectTarget);
#endif

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckTateSkillSelectTarget_sbtlUnit);

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
