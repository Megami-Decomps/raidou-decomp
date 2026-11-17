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
#include "code_r1_002250F8.h"
#include "dat.h"
#include "sBtlAction.h"
#include "sBtlEXP.h"
#include "gohMain.h"
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
#include "sndAt3.h"
#include "mapData.h"
#include "sbtlSub.h"
#include "sbtlSub_0027ED08.h"
#include "sdfQuat.h"
#include "sBtlSkill.h"
#include "sBtlAI.h"
#include "sBtlDvltk.h"
#include "sbtlActAI.h"
#include "evtUnitManager.h"
#include "dds3UnitObjectBasic.h"
#include "dds3KernelDraw.h"
#include "dds3KernelDraw_0010A750.h"
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

/* NON_MATCHING: 68 of 192 words match.  */
#ifdef NON_MATCHING
s32 sbtlReLoadMap_sbtlScene(void) {
    sdfVector4_t zero;
    f32 x, y, z, r;
    struct SBTLUNIT_WORK *target;
    struct SBTLUNIT_WORK *helper;
    SBTL_STAGE_DATA *stage;
    SBTL_WORK *work;

    memset(&zero, 0, sizeof(zero));
    zero.w = 1.0f;
    sdfBtlReLoadMapSBtlSceneSub();
    work = wk_sbtl;
    stage = &sbtlData100[work->stageNo];
    work->mapNo[0] = stage->mapNo[0];
    work->mapNo[1] = stage->mapNo[1];
    mapBtlReLoadMapSBtlSceneSub3();
    fldFreeMapData();
    mapBtlReLoadMapSBtlSceneSub(wk_sbtl->mapNo[0], wk_sbtl->mapNo[1]);
    mapBtlReLoadMapSBtlSceneSub2();
    mapFileLoad(wk_sbtl->mapNo[0]);
    mapMapSetEnc(wk_sbtl->stageNo);
    stage = &sbtlData100[wk_sbtl->stageNo];
    if (stage->form) {
        mapMapSetForm(stage->form);
    }
    mapCutSetCam(dds3GetActiveCameraSceneBasic(dds3GetActiveScene()), 0);
    target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (target) {
        mapEncPointGet(sbtlData100[wk_sbtl->stageNo].encPoint, &x, &y, &z, &r);
        target->pos.x = x;
        target->pos.y = y;
        target->pos.z = z;
        FromEuler(0.0f, r * 0.017453293f, 0.0f);
        SDF_VU0_STOREVECTOR(&target->rot);
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&target->rotBase);
        target->angleY = r;
        helper = sbtlRefSbtl();
        if (helper) {
            helper->pos.x = x;
            helper->pos.y = y;
            helper->pos.z = z;
            if (r > -45.0f && r < 45.0f) {
                helper->pos.x = x - 200.0f;
            } else if (r < -45.0f && r > -135.0f) {
                helper->pos.z -= 200.0f;
            } else if (r > 45.0f && r < 135.0f) {
                helper->pos.z += 200.0f;
            } else {
                helper->pos.x += 200.0f;
            }
            FromEuler(0.0f, r * 0.017453293f, 0.0f);
            SDF_VU0_STOREVECTOR(&helper->rot);
            SDF_VU0_LOADVECTOR(&zero);
            SDF_VU0_STOREVECTOR(&helper->rotBase);
            helper->angleY = r;
        }
        if (target) {
            sbtlCallMemset4Data4 = 0;
            sbtlCallMemset4Data5 = target->pos.x;
            sbtlCallMemset4Data6 = target->pos.y;
            sbtlCallMemset4Data7 = target->pos.z;
            mapCutSet_From_Pos(target->pos.x, target->pos.y, target->pos.z);
        }
    }
    mapCutSetCam(dds3GetActiveCameraSceneBasic(dds3GetActiveScene()), 1);
    mapCutSetLight();
    sbtlSetScrollID(sbtlGetScrollID());
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlReLoadMap_sbtlScene);
#endif

void sbtlSetCutScroll(sdfVector4_t *pos)
{
    sbtlCallMemset4Data5 = pos->x;
    sbtlCallMemset4Data6 = pos->y;
    sbtlCallMemset4Data7 = pos->z;
    sbtlCallMemset4Data4 = 1;
    mapCutSet_From_Pos(pos->x, pos->y, pos->z);
    mapCutSetCam(dds3GetActiveCameraSceneBasic(dds3GetActiveScene()), 1);
    mapCutSetLight();
}

s32 sbtlDefaultCutScroll(void)
{
    struct SBTLUNIT_WORK *wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);

    if (wk != NULL) {
        sbtlCallMemset4Data4 = 0;
        sbtlCallMemset4Data5 = wk->pos.x;
        sbtlCallMemset4Data6 = wk->pos.y;
        sbtlCallMemset4Data7 = wk->pos.z;
    }
#ifdef FIX_BUGS
    /* Retail returns the v0 sbtlSearchUnit_ID leaves. A script command:
     * CodeFunc_Comm reads 0 as still pending. */
    return wk != NULL;
#endif
}

void sbtlSetZoom2(const sdfVector4_t *pos, f32 zoom, s32 time)
{
    f32 x = pos->x;
    f32 y = pos->y;
    f32 z = pos->z;

    mapZoomSetGoal(x, y, z, zoom, time);
    sbtlData87 = x;
    sbtlData88 = y;
    sbtlData89 = z;
    sbtlData86 = 1;
}

void sbtlCallBtlSearchUnitID(s32 time)
{
    sdfVector4_t pos;
    struct SBTLUNIT_WORK *wk;

    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->zoomlockID);
    sbtlGetUnitHelperPos(wk, SBTLUNIT_MARK__1SHOT);
    SDF_VU0_STOREVECTOR(&pos);
    pos.y = wk->pos.y - 90.0f;
    mapZoomSetGoal(pos.x, pos.y, pos.z, 1.0f, time);
    sbtlData86 = 0;
}

/* NON_MATCHING: 2 of 9 words match.  */
#ifdef NON_MATCHING
s32 sbtlLeaf55(f32 *pos)
{
    sbtlCallMemset4Data4 = 1;
    sbtlCallMemset4Data5 = pos[0];
    sbtlCallMemset4Data6 = pos[1];
    sbtlCallMemset4Data7 = pos[2];
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf55);
#endif

void sbtlCallBtlSearchUnitID19(void)
{
    struct SBTLUNIT_WORK *wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);

    if (wk != NULL) {
        sbtlCallMemset4Data4 = 0;
        sbtlCallMemset4Data5 = wk->pos.x;
        sbtlCallMemset4Data6 = wk->pos.y;
        sbtlCallMemset4Data7 = wk->pos.z;
    }
}

s32 sbtlLeaf56(s32 id)
{
    sbtlData91 = id;
    sbtlData90 = 1;
    return 1;
}

void sbtlLeaf2(void)
{
    sbtlData90 = 0;
    sbtlData91 = 0;
}

s32 sbtlProcSequenceSub(s32 stageNo)
{
    return sbtlData100[stageNo].bgmId;
}

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

void sbtlCallMemFreeDirect2(struct SBTLUNIT_WORK *wk)
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
}

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

void sbtlSummonMenuFuncOpen00Sub(u32 no)
{
    sbtlSEVolAngleCalc(no, 127);
}

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

void sbtlPlay3DSE_BaseVol(u32 no, const sdfVector4_t *pos, s32 flg)
{
    s32 vol;

    if (flg & 1) {
        vol = sbtlBtlPlay3DSEBaseVolSub(pos, 2000.0f, 1000.0f, 127, 58);
    } else {
        vol = 127;
    }
    sbtlSEVolAngleCalc(no, vol);
}

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
    return sbtlGetSETime(wk, motionID);
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

/* NON_MATCHING: 48 of 95 words match.  */
#ifdef NON_MATCHING
void sbtlPlayDamageSE(const sdfVector4_t *pos, s32 damageType)
{
    u32 se = 0;
    s32 rnd;
    u32 rnd2;

    switch (damageType) {
    case 1:
        se = (wk_sbtl->battleMode == 1) ? 0x541000A : 0x10005;
        break;
    case 5:
        se = 0x1001A;
        break;
    case 4:
        se = 0x1001B;
        break;
    case 2:
        rnd = dds3GetRandIntA(NULL, 3);
        if (rnd == 1) {
            se = 0x10001;
        } else if (rnd >= 2) {
            if (rnd == 2) {
                se = 0x10002;
            }
        } else if (rnd == 0) {
            se = 0x10000;
        }
        break;
    case 3:
        rnd2 = dds3GetRandIntA(NULL, 2);
        if (rnd2 == 0) {
            se = 0x10003;
        } else if (rnd2 == 1) {
            se = 0x10004;
        }
        break;
    }
    if (se != 0) {
        sbtlSEVolAngleCalc(se, sbtlBtlPlay3DSEBaseVolSub(pos, 2000.0f, 1000.0f, 127, 58));
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlPlayDamageSE);
#endif

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

/* NON_MATCHING: 51 of 60 words match. Branch layout: retail joins every result in one register and tests the SE count with separate signed compares; vector checked (menu): only sedataNum 1 covered. */
#ifdef NON_MATCHING
u32 sbtlCallGetRandIntA5(SBTLSOUND_FOOTWORK *fwk, s32 devilID)
{
    u32 base;
    DVLFOOTSE_WORK *foot;
    s32 num;
    u32 n;
    u32 ret;

    base = sbtlPlayMotionSENowSub2(devilID, -1);
    foot = sbtlDevilFootSEProcSub(devilID);
    ret = 0;
    if (foot->flg & 2) {
        if (fwk->prevSMGID == 0) {
            ret = base + dds3GetRandIntA(NULL, foot->sedataNum);
        } else {
            num = foot->sedataNum;
            if (num == 1) {
                return base;
            }
            if (num == 0) {
                return 0;
            }
            if (num < 5) {
                n = dds3GetRandIntA(NULL, num - 1) + 1;
                ret = (fwk->prevSMGID - base + n) % foot->sedataNum + base;
            }
        }
    } else if (foot->flg & 4) {
        ret = base + fwk->played;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallGetRandIntA5);
#endif

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
        sbtlSEVolAngleCalc(se, sbtlBtlPlay3DSEBaseVolSub(pos, 2000.0f, 1000.0f, 127, 58));
    }
}

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

/* NON_MATCHING: 48 of 59 words match.  */
#ifdef NON_MATCHING
u32 sbtlPlayMotionSENowSub3(s32 devilID, s32 motionID)
{
    u32 se = 0;

    if (devilID == 290 || devilID == 297) {
        switch (motionID) {
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 26:
            if (devilID == 290) {
                se = sbtlPlayMotionSENowSub2(290, -1);
                se += motionID;
            } else {
                se = sbtlPlayMotionSENowSub2(297, -1);
                se += motionID;
            }
            break;
        case 12:
            se = sbtlPlayMotionSENowSub2(320, -1) + 1;
            break;
        case 13:
            se = sbtlPlayMotionSENowSub2(320, -1) + 2;
            break;
        case 15:
            se = sbtlPlayMotionSENowSub2(320, -1) + 4;
            break;
        case 16:
            se = sbtlPlayMotionSENowSub2(320, -1) + 5;
            break;
        }
    }
    return se;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlPlayMotionSENowSub3);
#endif

u32 sbtlGetDevilSESub(s32 devilID)
{
    return (devilID << 16) + 0x2000000;
}

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

/* NON_MATCHING: 5 of 90 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchPacData3(struct SBTLUNIT_WORK *wk) {
    s32 devilID;
    s32 idx;
    s32 key;
    SBTL_PACDATA *pac;
    u32 smg[2];

    devilID = wk->unit.devilId;
    if (devilID == 290) {
        key = 290;
    } else {
        if (devilID != 297) {
            return 1;
        }
        key = 297;
    }
    smg[0] = sbtlPlayMotionSENowSub2(key, -1);
    smg[1] = sbtlPlayMotionSENowSub2(320, -1);
    switch (wk->se.mot_step) {
    case 0:
        idx = sbtlSearchPacData(wk->major, wk->minor, devilID);
        pac = &sbtlData84->pac[idx];
        p2sdSetPcmBuffer(EE_ADDR_S(pac->motionSE.data), pac->motionSE.size);
        wk->se.mot_status = 1;
        wk->se.mot_step = 1;
        break;
    case 1:
        if (p2sdLoadCheckMidi(smg[0]) == 1) {
            wk->se.mot_SMGID = smg[0];
            sbtlBtlLoadSkillSESub(smg[0]);
            p2sdLoadSMGFile(smg[1]);
            wk->se.mot_step = 2;
        }
        break;
    case 2:
        if (p2sdLoadCheckMidi(smg[1]) == 1) {
            wk->se.mot_step = 3;
            wk->se.mot_status = 2;
            wk->se.mot_SMGID2 = smg[1];
            sbtlBtlLoadSkillSESub(smg[1]);
            return 1;
        }
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchPacData3);
#endif

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

void sbtlCallBtlGetDevilSESub2(struct SBTLUNIT_WORK *wk, s32 type)
{
    u32 se = 0;
    s32 devilID = wk->unit.devilId;

    switch (type) {
    case 1:
        se = sbtlPlayMotionSENowSub2(devilID, 13);
        sbtlLeaf58(se);
        break;
    case 2:
        se = sbtlPlayMotionSENowSub2(devilID, 14);
        sbtlLeaf58(se);
        break;
    case 3:
        se = sbtlGetDevilSESub(devilID);
        break;
    }
    if (se != 0) {
        sbtlBtlPlay3DSEBaseVolSub(&wk->pos, 2000.0f, 1000.0f, 127, 58);
        sbtlSEVolAngleCalc(se, 127);
    }
}

s32 sbtlCallLoad(struct SBTLUNIT_WORK *wk)
{
    char name[64];

    switch (wk->se.loadgstat) {
    case 0:
        sbtlSoundatBossAt(wk->unit.devilId, name);
        wk->se.loadFile = fileLoad(name);
        wk->se.loadgstat++;
        break;
    case 1:
        if (!pacFileCheckEnd(wk->se.loadFile)) {
            break;
        }
        wk->se.loadHandle = fileGetMemHandle(wk->se.loadFile);
        wk->se.loadData = sdfMemLock(wk->se.loadHandle);
        wk->se.loadSize = fileGetLoadSize(wk->se.loadFile);
        fileHandleFree(wk->se.loadFile);
        wk->se.loadFile = NULL;
        wk->se.loadgstat++;
        return 1;
    }
    return 0;
}

void sbtlCallEffectShutdown(struct SBTLUNIT_WORK *wk)
{
    nbEffectShutdown();
    mnuInstallTitleSoundBufferFromData(wk->se.loadData, wk->se.loadSize, 2);
    sndCallWaitSema();
}

/* NON_MATCHING: 25 of 30 words match.  */
#ifdef NON_MATCHING
void sbtlSoundatBossAt(s32 id, char *name)
{
    switch (id) {
    case 262:
    case 300:
        sprintf(name, sbtlStrSoundat3Boss00At3);
        break;
    case 268:
    case 269:
    case 304:
    case 305:
        sprintf(name, sbtlStrSoundat3Boss01At3);
        break;
    case 285:
    case 286:
    case 288:
    case 307:
    case 308:
    case 309:
        sprintf(name, sbtlStrSoundat3Boss02At3);
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSoundatBossAt);
#endif

s32 sbtlBtlModelDataLoadSBtlUnitSub(struct SBTLUNIT_WORK *wk)
{
    return 1;
}

void sbtlCallEffectShutdown2(struct SBTLUNIT_WORK *wk)
{
    nbEffectShutdown();
    mnuInstallTitleSoundBufferFromData(wk->se.pBossAt3Data, wk->se.bossAt3Size, 2);
    sndCallWaitSema();
}

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

void sbtlCallEffectShutdown3(s32 mode)
{
    nbEffectShutdown();
    p2sdStopAllSe();
}

/* NON_MATCHING: 17 of 18 words match.  */
#ifdef NON_MATCHING
void sbtlStopSkillSE(SKLID skillid, s32 type)
{
    if ((u32)(type - 1) < 2 || type == 5) {
        p2sdStopSe(sbtlCallBtlGetSkilltbl(skillid, type));
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlStopSkillSE);
#endif

void sbtlFunc10(s32 devilID, s32 motionID)
{
    p2sdStopSe(sbtlPlayMotionSENowSub2(devilID, motionID));
}

/* NON_MATCHING: 73 of 80 words match.  */
#ifdef NON_MATCHING
void sbtlENMY(void)
{
    char buf[128];
    s32 i;
    s32 y = 5;

    eglSetOt(0x67);
    eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
    eglLinkAlpha(0);
    evtSubmitDefaultDepthGradientRect(380, 5, 512, 408, 0x30000000, 0x30000000, 0x30000000, 0x30000000);
    for (i = 0; i < 30; i++) {
        sprintf(buf, sbtlStr0204, i, (u16)wk_sbtl->midiLoad[i].unk0, wk_sbtl->midiLoad[i].unk6);
        eglLinkPrint(380, y, 0, buf);
        y += 12;
    }
    sprintf(buf, sbtlENMYData, wk_sbtl->midiSkillCount);
    eglLinkPrint(380, 365, 0, buf);
    sprintf(buf, sbtlStrEnmy, wk_sbtl->midiCount8);
    eglLinkPrint(380, 377, 0, buf);
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlENMY);
#endif

s32 Init_sbtlUnit(s8 flg)
{
    wk_sbtl->unitList = sbtlCreateUnitList(48);
    dds3LinkProcess(wk_sbtl->process, dds3StartProcess("sbtlunit_proc", 1015, 1, 1, sbtlCallBtlCreateEnemyList, sbtlModelDataLoad, NULL));
    sbtlCallBtlCreateUnit(flg);
    return 1;
}

void sbtlSetEndBadStatus(void)
{
    struct SBTLUNIT_WORK *wk;
    BST bst;

    if (wk_sbtl->field344 <= 0) {
        return;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
    if (wk == NULL || !CheckLiveTarget(wk)) {
        return;
    }
    bst = sbtlGetBadStatus(&wk->unit);
    if (bst != BST_RAND0 && bst != BST_BOYCOTT && bst != BST_MOUDOKU && bst != BST_STONE) {
        sbtlResetBadStatus(wk, BSTF_DEAD | BSTF_GUN | BSTF_SLEEP | BSTF_MAHUU | BSTF_BOUDO | BSTF_PANIC | BSTF_CHARM
                                   | BSTF_KOUFUN | BSTF_KAEN | BSTF_APPAKU | BSTF_KANDEN | BSTF_HYOUKETU | BSTF_KANASHIB);
    }
    sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
}

/* NON_MATCHING: 218 of 237 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetEndData(void) {
    SBTL_PLAYER_DATA *player;
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_WORK *helper;
    BST bst;
    s32 skill;

    player = &DDS3_GBWK->playerData;
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (unit) {
        SDF_VU0_LOADVECTOR(&unit->pos);
        SDF_VU0_STOREVECTOR(&main_data);
        SDF_VU0_LOADVECTOR(&unit->rot);
        SDF_VU0_STOREVECTOR(&main_data.rot);
        sbtlResetItemFlg(&unit->unit);
        sbtlBtlSetEndDataSub(unit, -1);
        if (sbtlCheckTutorialBattle()) {
            sbtlEntryStockData_Huuma();
        }
        if (wk_sbtl->battleMode != 3) {
            player->unit = unit->unit;
        }
        player->val150 = sbtlCallBtlGetPlayerData3();
        player->val14C = wk_sbtl->equipGun;
        player->val146 = wk_sbtl->equipSword;
        player->val154 = wk_sbtl->equipBullet;
        player->mag = wk_sbtl->mag;
        bst = sbtlGetBadStatus(&unit->unit);
        if (bst != BST_RAND0 && bst != BST_BOYCOTT) {
            if (bst != BST_MOUDOKU && bst != BST_STONE) {
                sbtlResetBadStatus(unit, BSTF_DEAD | BSTF_GUN | BSTF_SLEEP | BSTF_MAHUU | BSTF_BOUDO | BSTF_PANIC | BSTF_CHARM
                                             | BSTF_KOUFUN | BSTF_KAEN | BSTF_APPAKU | BSTF_KANDEN | BSTF_HYOUKETU | BSTF_KANASHIB);
            }
        }
        sbtlForceDelete_ID_Acter(wk_sbtl->effList, unit, EFFID_RENKEIOR);
    }
    if (wk_sbtl->field344 > 0) {
        helper = sbtlRefSbtl();
        if (helper) {
            skill = sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&helper->unit, 0), &helper->unit.autost_cnt, &helper->unit.autost_cnt2);
            if (skill != 1) {
                skill = sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&helper->unit, 1), &helper->unit.autost_cnt, &helper->unit.autost_cnt2);
            }
            if (skill == 1) {
                if (wk_sbtl->battleMode == 3
                    || (sbtlCallBtlGetSkilltbl24(&DDS3_GBWK->playerData.unit, skill, 0, SKLID_NONE, helper->unit.autost_cnt,
                                                 helper->unit.autost_cnt2),
                        wk_sbtl->battleMode == 3)) {
                    if (sbtlGetBadStatus(&helper->unit) != BST_KANSEN) {
                        sbtlCallBtlGetSkilltbl24(&helper->unit, skill, 0, SKLID_NONE, helper->unit.autost_cnt, helper->unit.autost_cnt2);
                    }
                } else {
                    sbtlCallBtlGetSkilltbl24(&helper->unit, skill, 0, SKLID_NONE, helper->unit.autost_cnt, helper->unit.autost_cnt2);
                }
            }
            if (wk_sbtl->expList->field10 >= 250) {
                wk_sbtl->expList->field10 = 250;
            }
            helper->unit.vice = wk_sbtl->expList->field10;
            sbtlBtlSetEndDataSub(helper, wk_sbtl->summonIndex[0]);
            bst = sbtlGetBadStatus(&helper->unit);
            if (bst != BST_RAND0 && bst != BST_BOYCOTT) {
                if (bst != BST_MOUDOKU && bst != BST_STONE) {
                    sbtlResetBadStatus(helper, BSTF_DEAD | BSTF_GUN | BSTF_SLEEP | BSTF_MAHUU | BSTF_BOUDO | BSTF_PANIC | BSTF_CHARM
                                                   | BSTF_KOUFUN | BSTF_KAEN | BSTF_APPAKU | BSTF_KANDEN | BSTF_HYOUKETU | BSTF_KANASHIB);
                }
            }
            sbtlForceDelete_ID_Acter(wk_sbtl->effList, helper, EFFID_RENKEIOR);
        }
    }
    sbtlCallBtlGetPlayerData2();
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetEndData);
#endif

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

void sbtlCallBtlGetPlayerData2(void)
{
    SBTL_PLAYER_DATA *player = sbtlGetPlayerData();
    sdatUnitWork_t *unit;
    s32 i;

    unit = &player->unit;
    unit->badStatus &= 0xFFFDA840;
    for (i = 0; i < player->partyNum; i++) {
        unit = sbtlGetStockUnitData(i);
        if (unit != NULL) {
            unit->badStatus &= 0xFFFDA840;
        }
    }
}

/* NON_MATCHING: 34 of 59 words match.  */
#ifdef NON_MATCHING
s32 sbtlRefSbtl32(struct SBTLUNIT_WORK *src, s8 mode)
{
    struct SBTLUNIT_WORK *wk;
    s32 m;
    s32 i;

    if (wk_sbtl == NULL || wk_sbtl->unitList == NULL) {
        return 0;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk == NULL) {
            continue;
        }
        m = wk->mode;
        if (m < 5) {
            continue;
        }
        if (m >= 9 && m != 10) {
            continue;
        }
        if (mode == 0) {
            wk->vstatus &= ~0x1000;
        } else if (mode == 2 || src->field1A0 != wk->field1A0) {
            wk->vstatus |= 0x1000;
        } else {
            wk->vstatus &= ~0x1000;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlRefSbtl32);
#endif

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

struct SBTLUNIT_WORK *sbtlSearchUnit_GroupID(struct SBTLUNIT_LISTWORK *lwk, s32 serialID)
{
    struct SBTLUNIT_WORK *wk;
    s32 i;

    for (i = 0; i < lwk->number; i++) {
        wk = lwk->pUnitL[i].pUnit;
        if (wk != NULL && !(wk->field414 & 0x18) && wk->GroupID == serialID) {
            return wk;
        }
    }
    return NULL;
}

s32 sbtlSearchUnit_Nakama(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && unit->field1A0 != wk->field1A0 && (unit->field414 & 1) && !(unit->field414 & 0x80)) {
            return 1;
        }
    }
    return 0;
}

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

s32 sbtlGetNumUnit_GroupID2(struct SBTLUNIT_LISTWORK *lwk, s32 serialID, s32 flg)
{
    struct SBTLUNIT_WORK *wk;
    s32 num = 0;
    s32 i;

    if (lwk == NULL) {
        return 0;
    }
    for (i = 0; i < lwk->number; i++) {
        wk = lwk->pUnitL[i].pUnit;
        if (wk == NULL || (wk->field414 & 0x18) || wk->GroupID != serialID) {
            continue;
        }
        switch (flg) {
        case 0:
            if (wk->mode >= 4) {
                num++;
            }
            break;
        case 1:
            if (wk->mode >= 5) {
                num++;
            }
            break;
        }
    }
    return num;
}

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

s32 sbtlReleaseUnitList(struct SBTLUNIT_LISTWORK *lwk, struct SBTLUNIT_WORK *wk)
{
    s32 i;

    for (i = 0; i < lwk->number; i++) {
        if (lwk->pUnitL[i].pUnit != NULL && lwk->pUnitL[i].ID == wk->field1A0) {
            break;
        }
    }
    lwk->pUnitL[i].ID = 0;
    lwk->pUnitL[i].pUnit = NULL;
    lwk->count--;
    return lwk->pUnitL[i].ID;
}

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
    sbtlRefSbtl17(wk);
    sbtlRefSbtl5(wk);
    return wk;
}

s32 sbtlCallDeleteSceneBasicData(struct SBTLUNIT_WORK *wk) {
    if (wk->changeflg) {
        sbtlLeaf61(wk);
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
        }
        sdfMemFreeDirect(wk->msgHandle);
        wk->msgHandle = NULL;
        wk->msgData = NULL;
        wk->msgState = 0;
        wk->flg2 &= ~1;
    }
    if (wk->hitwk.status & 1) {
        sbtlCreateHitC_New(wk);
        wk->hitwk.status &= ~1;
        wk->hitwk.gstat = 0;
    }
    sbtlCallMemFreeDirect2(wk);
    sbtlRefSbtl19(wk);
    sbtlCallBtlCreateDelWork(wk);
    sbtlResetBadStatus(wk, wk->unit.badStatus);
    sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
    if (!(wk->field414 & 2)) {
        if (!(wk->flg & 0x800)) {
            sbtlPacData_FreeIndex(wk->major, wk->minor, wk->unit.devilId);
        }
    } else {
        if (!(wk->flg & 0x800)) {
            if (!sbtlRefSbtl33(wk)) {
                sbtlPacData_FreeIndex(wk->major, wk->minor, wk->unit.devilId);
            }
        }
    }
    wk->flg2 &= ~0x200;
    return 1;
}

s32 sbtlCheckReturn_Unit(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    s32 index;

    if (wk_sbtl->flg2 & 0x100) {
        index = wk_sbtl->summonIndex[1];
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
        wk_sbtl->summonIndex[1] = -1;
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
        }
        sdfMemFreeDirect(wk->msgHandle);
        wk->msgHandle = NULL;
        wk->msgData = NULL;
    }
    if (wk->hitwk.status & 1) {
        sbtlCreateHitC_New(wk);
    }
    sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
    sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
    if (!(wk->flg & 0x800)) {
        sbtlCallMemFreeDirect2(wk);
    }
    sbtlDelete(wk->model);
    Proc_sbtlAI(wk->aiwk);
    if (!(wk->field414 & 2)) {
        if (!(wk->flg & 0x800)) {
            sbtlPacData_FreeIndex(wk->major, wk->minor, wk->unit.devilId);
        }
    } else {
        if (!(wk->flg & 0x800)) {
            if (!sbtlRefSbtl33(wk)) {
                sbtlPacData_FreeIndex(wk->major, wk->minor, wk->unit.devilId);
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

/* NON_MATCHING: 28 of 228 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallSin(struct SBTLUNIT_WORK *wk) {
    s32 base;
    s32 low;
    s32 r;
    s32 g;
    s32 b;
    s32 c;
    s32 start;
    u32 color;
    f32 sine;

    if (wk->field4A2 < 65) {
        base = 64 - wk->field4A2;
    } else {
        base = 0;
    }
    wk->field4B4 = (wk->field4B4 + (base >> 2) + 32) % 360;
    if (wk->dstcolor != 0) {
        c = wk->dstcolor & 0xFF;
        start = c + base - 64;
        low = 0;
        if (start > 0) {
            low = start;
            if (start >= 256) {
                low = 255;
            }
        }
        sine = sdfSin((f32)((wk->field4B4 + 90) % 360) / 180.0f * 3.14159f);
        r = (s32)((f32)low + (f32)(c - low) * ((sine + 1.0f) * 0.5f));
        c = (wk->dstcolor >> 8) & 0xFF;
        start = c + base - 64;
        low = 0;
        if (start > 0) {
            low = start;
            if (start >= 256) {
                low = 255;
            }
        }
        sine = sdfSin((f32)((wk->field4B4 + 90) % 360) / 180.0f * 3.14159f);
        g = (s32)((f32)low + (f32)(c - low) * ((sine + 1.0f) * 0.5f));
        c = (wk->dstcolor >> 16) & 0xFF;
        start = c + base - 64;
        low = 0;
        if (start > 0) {
            low = start;
            if (start >= 256) {
                low = 255;
            }
        }
        sine = sdfSin((f32)((wk->field4B4 + 90) % 360) / 180.0f * 3.14159f);
        b = (s32)((f32)low + (f32)(c - low) * ((sine + 1.0f) * 0.5f));
        color = (b << 16) | r | (g << 8) | 0x80000000;
    } else {
        sine = sdfSin((f32)((wk->field4B4 + 90) % 360) / 180.0f * 3.14159f);
        c = (s32)((f32)(base + 64) + (f32)(64 - base) * ((sine + 1.0f) * 0.5f));
        wk->field4B8 = c;
        color = (c << 16) | c | (c << 8) | 0x80000000;
    }
    if (wk->field4A2 < 2) {
        color = 0x80808080;
        if (wk->dstcolor) {
            color = wk->dstcolor;
        }
    }
    sbtlLeaf59(wk, color);
    sbtlChangeModelColor(wk);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSin);
#endif

s32 sbtlCallSetColor(void)
{
    struct SBTLUNIT_WORK *wk;
    s32 mode;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number;) {
        wk = wk_sbtl->unitList->pUnitL[i++].pUnit;
        if (wk == NULL) {
            continue;
        }
        mode = wk->mode;
        if (mode < 10) {
            if (mode < 4) {
                continue;
            }
            if (wk->hModel != NULL) {
                if (wk->color & 0xFF000000) {
                    wk->vstatus &= ~0x10000000;
                    wk->color = wk->field3D8;
                    mdlSetColor(wk->hModel, wk->color);
                }
                if (wk->hModel != NULL) {
                    wk->vstatus &= ~0x10000000;
                }
            }
        }
    }
    return 1;
}

/* NON_MATCHING: 27 of 94 words match.  */
#ifdef NON_MATCHING
s32 LightSet(struct SBTLUNIT_WORK *wk, u32 color)
{
    s32 r;
    s32 t;
    s32 g;
    s32 b;

    if (wk->hModel != NULL) {
        wk->color = color;
        mdlSetColor(wk->hModel, color);
        wk->vstatus |= 0x10000000;
        mapGetLightParam(&LightSetData, wk->pos.x, wk->pos.y, wk->pos.z);
        t = (LightSetData.rgb0 & 0xFF) + 40;
        g = ((LightSetData.rgb0 & 0xFF00) >> 8);
        r = 0;
        if (t > 0) {
            r = t;
            if (t >= 256) {
                r = 255;
            }
        }
        t = g + 40;
        g = 0;
        if (t > 0) {
            g = t;
            if (t >= 256) {
                g = 255;
            }
        }
        b = ((LightSetData.rgb0 >> 16) & 0xFF) + 40;
        if (b > 0) {
            if (b >= 256) {
                b = 255;
            }
        } else {
            b = 0;
        }
        LightSetData.rgb0 = (LightSetData.rgb0 & 0xFF000000) | r | (g << 8) | (b << 16);
        evtSetUnitLightAndAmbient3(wk->hEveUnit, 0, LightSetData.rgb0, LightSetData.rgb1, LightSetData.rgb2, LightSetData.rgba);
        SDF_VU0_LOADVECTOR(&LightSetData.dir0);
        evtSetUnitLightDir(wk->hEveUnit, 0);
        SDF_VU0_LOADVECTOR(&LightSetData.dir1);
        evtSetUnitLightDir1(wk->hEveUnit, 1);
        SDF_VU0_LOADVECTOR(&LightSetData.dir2);
        evtSetUnitLightDir2(wk->hEveUnit, 2);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", LightSet);
#endif

/* NON_MATCHING: 63 of 94 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallSetColor2(u32 color)
{
    struct SBTLUNIT_WORK *wk;
    struct evtUnit_t *hEveUnit;
    u32 cur;
    u32 rgb;
    u32 res;
    s32 mode;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk == NULL) {
            continue;
        }
        mode = wk->mode;
        if (mode >= 10) {
            continue;
        }
        if (mode < 4) {
            continue;
        }
        if (wk->hModel != NULL) {
            cur = wk->color;
            wk->field3D4 = cur;
            wk->field3D8 = cur;
            if (cur & 0xFF000000) {
                wk->field3D8 = color;
                wk->color = (color & 0xFFFFFF) | (cur & 0xFF000000);
                mdlSetColor(wk->hModel, color);
                hEveUnit = wk->hEveUnit;
                if (hEveUnit->flags & 0x8000) {
                    wk->vstatus |= 0x20000000;
                    rgb = hEveUnit->color60 & 0xFFFFFF;
                    hEveUnit->flags &= ~0x8000;
                    if (rgb == 0x808080) {
                        cur = wk->color;
                        wk->field3D8 = color;
                        res = (color & 0xFFFFFF) | (cur & 0xFF000000);
                    } else {
                        res = rgb | (wk->color & 0xFF000000);
                        wk->field3D8 = res;
                    }
                    wk->color = res;
                    mdlSetColor(wk->hModel, wk->color);
                }
            }
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallSetColor2);
#endif

s32 sbtlCallSetColor3(u32 color)
{
    struct SBTLUNIT_WORK *wk;
    struct mdlObj_t *hModel;
    s32 mode;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk == NULL) {
            continue;
        }
        mode = wk->mode;
        if (mode < 10) {
            if (mode < 4) {
                continue;
            }
            hModel = wk->hModel;
            if ((hModel != NULL && (wk->color & 0xFF000000)) || (wk->vstatus & 0x20000000)) {
                wk->color = wk->field3D4;
                mdlSetColor(hModel, wk->color);
                if (wk->vstatus & 0x20000000) {
                    wk->vstatus &= ~0x20000000;
                    wk->hEveUnit->flags |= 0x8000;
                }
            }
            if (wk->hModel != NULL) {
                wk->vstatus &= ~0x10000000;
            }
        }
    }
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

/* NON_MATCHING: 53 of 60 words match.  */
#ifdef NON_MATCHING
void sbtlLeaf60(struct SBTLUNIT_WORK *wk, u32 from, u32 to, f32 rate)
{
    s32 dst[3];
    s32 src[4];
    u32 color;
    s32 v;
    s32 i;

    src[0] = from & 0xFF;
    src[1] = (from & 0xFF00) >> 8;
    src[2] = (from & 0xFF0000) >> 16;
    dst[0] = to & 0xFF;
    dst[1] = (to & 0xFF00) >> 8;
    dst[2] = (to & 0xFF0000) >> 16;
    i = 0;
    do {
        v = src[i] + (dst[i] - src[i]) * rate;
        if (v < 0) {
            v = 0;
        }
        dst[i] = v;
        if (v >= 255) {
            dst[i] = 255;
        }
        i++;
    } while (i < 3);
    color = (dst[2] << 16) | dst[0] | (dst[1] << 8) | 0x80000000;
    wk->dstcolor = wk->field3D4 = color;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlLeaf60);
#endif

s32 sbtlChangeModelColor(struct SBTLUNIT_WORK *wk)
{
    u32 color = wk->chgcolor;

    if (color != 0) {
        wk->color = color;
        return 1;
    }
    return 0;
}

s32 sbtlResetMutekiColor(struct SBTLUNIT_WORK *wk)
{
    u32 color = wk->dstcolor;

    if (color != 0) {
        wk->color = color;
        return 1;
    }
    return 0;
}

void sbtlBtlFadeOutBadStatusEffectSub2(struct SBTLUNIT_WORK *wk)
{
    wk->dstcolor = 0;
    wk->color = 0x80808080;
    wk->field3D4 = 0x80808080;
}

s32 sbtlLeaf61(struct SBTLUNIT_WORK *wk)
{
    struct dds3Basic_t *basic = wk->pBasic;
    struct evtUnit_t *eveUnit = wk->hEveUnit;
    struct mdlObj_t *model = wk->hModel;

    wk->pBasic = wk->pBasic_d;
    wk->hEveUnit = wk->hEveUnit_d;
    wk->hModel = wk->hModel_d;
    wk->pBasic_d = basic;
    wk->hEveUnit_d = eveUnit;
    wk->hModel_d = model;
    return 1;
}

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
s8 sbtlCheckMapHit_Cross(const sdfVector4_t *pos0, const sdfVector4_t *ppos, f32 dis)
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
    spd = mv->field08 * 57.295777f;
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

s32 sbtlCallMemset5(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t ofs = { 0.0f, 0.0f, 0.0f, 0.0f };
    sdfVector4_t rot;
    SBTLUNIT_FLG flg;
    f32 angle;

    if ((wk_sbtl->flg & 0x40) && !(wk->flg & 0x10)) {
        return 0;
    }
    flg = wk->flg;
    if (wk->unit.devilId == 259) {
        wk->pos.x = 50.0f;
        wk->pos.y = 600.0f;
        wk->pos.z = -150.0f;
        wk->pos.w = 1.0f;
    } else if (wk->unit.devilId == 258) {
        wk->pos.x = 50.0f;
        wk->pos.y = 50.0f;
        wk->pos.z = -50.0f;
        wk->pos.w = 1.0f;
        wk->rot.x = 0.0f;
        wk->rot.y = 0.0f;
        wk->rot.z = 0.0f;
        wk->rot.w = 1.0f;
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
        FromRotation(-angle * 0.017453293f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(&wk->rot);
        Mult();
        SDF_VU0_STOREVECTOR(&wk->rot);
    }
    if (!(wk->vstatus & 0x2000)) {
        dds3SetPosPoseSet_dds3Basic(wk->pBasic, &wk->pos);
        if (!(wk->vstatus & 8)) {
            dds3SetRotPoseSet(wk->pBasic, &wk->rot);
        }
    }
    return 1;
}

s32 sbtlCheckMotionNo(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no)
{
    return wk->motionno == sbtlBtlCheckMotionNoSub(wk, wk->field414, no);
}

s32 sbtlCheckMotionEnd(struct SBTLUNIT_WORK *wk)
{
    if (sbtlCallBtlGetMotionVisualData2(wk, wk->motionID) != 0) {
        return 0;
    }
    return wk->hModel->first->stat == 5;
}

void sbtlCheckGuardMotion(struct SBTLUNIT_WORK *wk)
{
    if (wk->hModel != NULL && !(wk->flg & 0x800) && wk->next_motionID == 29 && wk->hModel->first->frame < 0.0f) {
        sbtlSetMotion_sbtlUnit(wk, 29, 0, 0, 1);
        wk->motionno = 0;
    }
}

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

/* NON_MATCHING: 79 of 81 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetMotionVisualData(struct SBTLUNIT_WORK *wk, s32 motionID, s32 bframe, s32 aframe, s8 flg)
{
    SBTL_UNIT_SUB *cur;
    SBTL_UNIT_SUB *next;
    SBTL_MOTION_VISUAL *visual;
    f32 radius;

    wk->next_motionID = 48;
    wk->next_motionno = motionID;
    next = sbtlGetMotionVisualData(wk->major, wk->minor, motionID);
    cur = sbtlGetMotionVisualData(wk->major, wk->minor, wk->motionno);
    if (flg == 0) {
        visual = sbtlGetVisualData(wk->major, wk->minor);
        if (next->flags & 1) {
            wk->m_rate = next->field10;
        } else {
            wk->m_rate = visual->field0C;
        }
        if (wk_sbtl->battleMode != 3 && wk_sbtl->battleMode != 1 && (wk->field414 & 0x80)) {
            wk->beforeframe = cur->field14;
            sbtlBtlSetMotionSBtlUnitSub(wk);
        } else {
            wk->beforeframe = cur->field14;
        }
        radius = next->field04;
    } else {
        wk->beforeframe = bframe;
        radius = next->field04;
    }
    wk->afterframe = aframe;
    wk->field3DE = 0;
    wk->field3DC = 0;
    wk->model->field120 = radius;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlGetMotionVisualData);
#endif

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

/* NON_MATCHING: 1 of 12 words match.  */
#ifdef NON_MATCHING
s32 sbtlCutMotion(struct SBTLUNIT_WORK *wk, f32 frame)
{
    f32 now = wk->hModel->first->frame;

    if (frame > now) {
        wk->m_rate = frame - now;
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCutMotion);
#endif

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

/* NON_MATCHING: 123 of 138 words match.  */
#ifdef NON_MATCHING
s32 ChangeTarget_BSHelper(void) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_LISTWORK *sorted;
    struct SBTLUNIT_WORK *wk;
    struct SBTLUNIT_WORK *front;
    struct SBTLUNIT_WORK *entry;
    SBTLPROJECTEDSCREEN_POINT point;
    s32 i;
    s32 frontIndex;
    s32 frontX;

    list = sbtlCreateUnitList(48);
    sorted = sbtlCreateUnitList(48);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        entry = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (entry != NULL && (entry->flg & 0x8800) == 0x8800) {
            sbtlEntryUnitList(list, entry);
        }
    }
    frontIndex = 0;
    if (list->count > 0) {
        do {
            frontX = -512;
            front = NULL;
            for (i = 0; i < list->number; i++) {
                wk = list->pUnitL[i].pUnit;
                if (wk != NULL && CheckLiveTarget(wk)) {
                    SDF_VU0_LOADVECTOR(&wk->pos);
                    btlProjectForwardPositionToScreen(&point);
                    if (point.x >= frontX) {
                        frontX = point.x;
                        front = wk;
                        frontIndex = i;
                    }
                }
            }
            if (front != NULL) {
                sbtlEntryUnitList(sorted, front);
                sbtlReleaseUnitList_ID(list, frontIndex);
            }
        } while (list->count > 0);
        for (i = 0; i < sorted->number; i++) {
            wk = sorted->pUnitL[i].pUnit;
            if (wk != NULL && CheckLiveTarget(wk)) {
                SDF_VU0_LOADVECTOR(&wk->pos);
                btlProjectForwardPositionToScreen(&point);
                if (point.x >= 224) {
                    if (point.x < 353) {
                        frontIndex = wk->field1A0;
                        sbtlDeleteUnitList(list);
                        sbtlDeleteUnitList(sorted);
                        return frontIndex;
                    }
                }
            }
        }
    }
    sbtlDeleteUnitList(list);
    sbtlDeleteUnitList(sorted);
    return -1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", ChangeTarget_BSHelper);
#endif

struct SBTLUNIT_LISTWORK *sbtlCallBtlCreateUnitList(u8 side) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_LISTWORK *sorted;
    struct SBTLUNIT_WORK *wk;
    struct SBTLUNIT_WORK *front;
    SBTLPROJECTEDSCREEN_POINT point;
    s32 i;
    s32 frontIndex;
    s32 frontX;
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
            frontX = -512;
            front = NULL;
            for (i = 0; i < list->number; i++) {
                wk = list->pUnitL[i].pUnit;
                if (wk != NULL && CheckLiveTarget(wk)) {
                    SDF_VU0_LOADVECTOR(&wk->pos);
                    btlProjectForwardPositionToScreen(&point);
                    if (point.x >= frontX) {
                        frontX = point.x;
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

struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Group(u8 side, s32 GroupID)
{
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLUNIT_WORK *wk;
    s32 i;

    lwk = sbtlCreateUnitList(48);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 0x3F) == side && sbtlCheckDamageTarget(wk) != 0 && wk->status != 35
            && wk->GroupID == GroupID) {
            sbtlEntryUnitList(lwk, wk);
        }
    }
    return lwk;
}

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

s32 sbtlCheckTateSkillSelectTarget(struct SBTLUNIT_WORK *wk)
{
    if (wk->status == 10 || wk->status == 40) {
        return 0;
    }
    return 1;
}

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

s32 sbtlCallBtlGetDevilDataSBtlSub(struct SBTLUNIT_WORK *wk, s32 mode)
{
    SBTL_DEVIL_DATA *devil = sbtlGetDevilData_sbtlSub(wk->unit.devilId);
    SBTL_MOTION_VISUAL *visual = sbtlGetVisualData(wk->major, wk->minor);

    wk->flg &= ~0x4000;
    switch (mode) {
    case 0:
        wk->flg |= devil->flag;
        if (visual->field30 > 0) {
            sbtlLeaf62(wk, 0);
        }
        break;
    case 1:
        wk->flg |= sbtlLeaf10(visual->field32, 1)[1];
        if (visual->field30 > 0) {
            sbtlLeaf62(wk, 1);
        }
        break;
    }
    return 1;
}

struct SBTLUNIT_WORK *sbtlSearchHitHelper_ID(struct SBTLUNIT_WORK *wk, s32 id, s32 max)
{
    struct SBTLUNIT_WORK *unit;
    s32 mark;
    s32 i;

    for (mark = 301; mark < max + 301; mark++) {
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && unit->field492 == wk->field1A0 && unit->field490 == mark && (unit->flg & 0x800)) {
                return unit;
            }
        }
    }
    return NULL;
}

/* body: matched in raidou @ 61f09dc: src/raidou1/code_002929C8.c:124, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sbtlLeaf62(struct SBTLUNIT_WORK *wk, s32 mode) {
    return 1;
}

/* NON_MATCHING: 166 of 216 words match.  */
#ifdef NON_MATCHING
void sbtlGetDevilData(struct SBTLUNIT_WORK *wk, s32 devilId, s32 num) {
    s32 i;
    u8 level;
    SBTL_DEVIL_DATA *devil;
    struct SBTLUNIT_WORK *unit;
    SBTL_MOTION_VISUAL *visual;
    struct SBTLUNIT_MODEL *model;
    struct SBTL_AISYO_TBL *aisyo;

    devil = sbtlGetDevilData_sbtlSub(devilId);
    for (i = 0; i < num; i++) {
        unit = sbtlCreateUnit(1, devil->motionId, 12);
        unit->field1A0 = sbtlEntryUnitList(wk_sbtl->unitList, unit);
        visual = sbtlGetVisualData(unit->major, unit->minor);
        unit->field414 = wk->field414;
        unit->status = 20;
        unit->field490 = i;
        unit->model->flags = (unit->model->flags | 0x40) & ~0x20;
        wk_sbtl->field424++;
        unit->field492 = wk->field1A0;
        level = devil->level;
        unit->unit.devilId = devilId;
        unit->unit.level = level;
        unit->unit.raceId = devil->field0E;
        unit->field400 = devil->field0A;
        unit->field404 = devil->aisyoId;
        aisyo = sbtlGetAisyotbl(devil->aisyoId);
        unit->unit.aisyo = *aisyo;
        unit->aisyoBase = unit->unit.aisyo;
        unit->flg |= devil->flag | 0x800;
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
        sbtlCheckAutoSkill_Enemy(wk);
        sbtlCheckBattlePara(wk);
        model = unit->model;
        unit->unit.seikaku = devil->field0F;
        model->field12C = devil->field64;
        model->field130 = devil->field60;
        unit->visual[0] = visual->scale;
        unit->visual[1] = visual->scale;
        unit->visual[2] = visual->scale;
        unit->visual[3] = visual->scale;
        unit->visualKind = visual->kind;
        model->field11C = visual->field10;
        model->field120 = visual->field10 + 1.0f;
        model->field124 = devil->field60;
        model->field128 = devil->field64;
        sbtlGetHelperPos(unit, wk, wk->unit.devilId);
        if ((unit->flg & 0x8800) == 0x8800) {
            if (i == 6) {
                unit->field498 = -1;
            } else {
                unit->field498 = 0;
            }
        }
        SDF_VU0_LOADVECTOR(&unit->pos);
        SDF_VU0_STOREVECTOR(&unit->effpos);
    }
    wk->field494 = -1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetDevilData);
#endif

void sbtlRefSbtl34(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && (unit->flg & 0x800) && unit->field492 == wk->field1A0) {
            unit->flg |= 0x4000;
            unit->flg |= 0x4000000;
        }
    }
}

void sbtlRefSbtl35(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && (unit->flg & 0x800) && unit->field492 == wk->field1A0) {
            unit->flg &= ~0x4000;
            unit->flg &= ~0x4000000;
        }
    }
}

/* NON_MATCHING: 97 of 105 words match.  */
#ifdef NON_MATCHING
s32 sbtlGetHelperPos(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, s32 devilid) {
    sdfVector4_t rot = {0.0f, 0.0f, 0.0f, 1.0f};
    s32 offsetX[16] = {-1200, -1000, -800, -600, -400, -200, 0, 200, 400, 600, 800, 1000, 1200, 1200, 1200, 1200};

    f32 x;
    f32 y;
    f32 z;

    if (devilid == 258) {
        SDF_VU0_LOADVECTOR(&wklock->pos);
        SDF_VU0_STOREVECTOR(&wk->pos);
        x = wk->pos.x + offsetX[wk->field490];
        y = wk->pos.y - 120.0f;
        z = wk->pos.z + 908.0f;
        wk->pos.y = y;
        wk->pos.z = z;
        wk->pos.x = x;
        SDF_VU0_LOADVECTOR(&rot);
        SDF_VU0_STOREVECTOR(&wk->rot);
    } else if (devilid == 260) {
        SDF_VU0_LOADVECTOR(&wklock->pos);
        SDF_VU0_STOREVECTOR(&wk->pos);
        wk->pos.y -= 650.0f;
        wk->pos.z -= 1000.0f;
        SDF_VU0_LOADVECTOR(&rot);
        SDF_VU0_STOREVECTOR(&wk->rot);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlGetHelperPos);
#endif

s32 sbtlDeleteHitHelper(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && (unit->flg & 0x800) && unit->field492 == wk->field1A0) {
            unit->mode = 9;
        }
    }
    return 1;
}

s32 sbtlSetItem(struct SBTLUNIT_WORK *wk)
{
    SBTL_DEVIL_DATA *data = sbtlGetDevilData_sbtlSub(wk->unit.devilId);
    s32 i;

    for (i = 0; i < 4; i++) {
        sbtlItem_CheckSetItem(&data->item[i], &wk->pos);
    }
    return 1;
}

s32 sbtlItem_CheckSetItem(struct SBTLITEM_WORK *item, const sdfVector4_t *pos) {
    s32 chance = 0;
    s32 percent = (s16)GetPercentage();
    f32 per = 0.0f;
    struct SBTLUNIT_WORK *target;
    struct SBTLUNIT_WORK *stock;
    s32 autoskill;
    f32 luck;

    if (item->itemid && item->per) {
        autoskill = 18;
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (wk_sbtl->field344 > 0) {
            stock = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
            if (stock && sbtlCheckDamageTarget(stock)) {
                autoskill = sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&stock->unit, 0), &stock->unit.autost_cnt, &stock->unit.autost_cnt2);
                if (autoskill != 7) {
                    autoskill = sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&stock->unit, 1), &stock->unit.autost_cnt, &stock->unit.autost_cnt2);
                }
            }
        }
        if (target && sbtlCheckDamageTarget(target)) {
            per = item->per;
            luck = target->unit.now.luck;
            chance = (s16)((f64)per * ((f64)luck * 1.5 + 100.0) / 100.0);
            if (target->unit.itemstatus & 0x200) {
                chance = (s16)(chance + 10);
            }
            if (autoskill == 7) {
                chance = (s16)(chance * 1.5f);
            }
        }
        if (chance >= percent + 1) {
            if (per > 0.0f) {
                sbtlSetDropItem(item->itemid, item->para, chance);
            }
        }
    }
    return 1;
}

void sbtlCheckSummonUnit_Exist2(void)
{
    wk_sbtl->field428++;
}

s32 sbtlCallBtlSearchUnitID2(void)
{
    struct SBTLUNIT_WORK *wk;

    if (wk_sbtl->field344 <= 0) {
        return 2;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
    if (wk != NULL && CheckLiveTarget(wk) != 0) {
        return 0;
    }
    if (wk != NULL) {
        return 1;
    }
    return 2;
}

s32 sbtlCallBtlSearchUnitID3(void)
{
    struct SBTLUNIT_WORK *wk;

    if (wk_sbtl->field344 <= 0) {
        return 2;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
    if (wk == NULL) {
        return 2;
    }
    if (wk->status == 10 || wk->status == 40) {
        return 0;
    }
    return 2;
}

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

s32 sbtlRefSbtl9(void)
{
    s32 i;

    if (wk_sbtl->stockNum <= 0) {
        return 1;
    }
    if (wk_sbtl->field344 > 0) {
        if (wk_sbtl->field358[wk_sbtl->summonIndex[0]] != 2) {
            return 3;
        }
        return 2;
    }
    for (i = 0; i < 12; i++) {
        if (wk_sbtl->field358[i] != 0) {
            return 3;
        }
    }
    return 0;
}

s32 sbtlGetStockPoint(void)
{
    if (wk_sbtl->stockNum >= 12) {
        return -1;
    }
    return wk_sbtl->stockNum;
}

s32 sbtlEntryStockData_Huuma(void)
{
    SBTL_PLAYER_DATA *player = &DDS3_GBWK->playerData;
    SBTL_DEVIL_DATA *devil;
    s32 id;
    s32 i;

    for (i = 11; i >= 0; i--) {
        if (wk_sbtl->huumaDevilID[i] == -1) {
            continue;
        }
        id = sbtlEntryStockData(wk_sbtl->huumaDevilID[i]);
        wk_sbtl->huumaDevilID[i] = -1;
        if (id != -1 && player->stock[i].devilId != 0 && (player->stock[i].flag & 1)) {
            devil = sbtlGetDevilData_sbtlSub(player->stock[i].devilId);
            wk_sbtl->stockDevilSkill[i] = player->stock[i].devilId;
            wk_sbtl->stockMinor[i] = devil->motionId;
        }
    }
    return 1;
}

/* NON_MATCHING: 64 of 147 words match.  */
#ifdef NON_MATCHING
s32 sbtlEntryStockData(s32 devilid) {
    SBTL_DEVIL_DATA *data;
    s32 idx;
    sdatUnitWork_t *unit;
    s32 *skill;
    s16 *param;
    s32 n;
    SBTL_AISYO_TBL *aisyo;

    data = sbtlGetDevilData_sbtlSub(devilid);
    idx = sbtlGetStockPoint();
    if (idx == -1) {
        return idx;
    }
    unit = &DDS3_GBWK->playerData.stock[idx];
    memset(unit, 0, sizeof(sdatUnitWork_t));
    unit->devilId = devilid;
    unit->flag = 5;
    unit->level = gohData7[devilid].level;
    unit->seikaku = data->field0F;
    unit->raceId = datCreateDevilUnitExSub(devilid);
    unit->chuusei = 0;
    unit->exp = 0;
    unit->backup.str = data->resist[0];
    unit->backup.hp = data->stat1C;
    unit->backup.vit = data->resist[1];
    unit->backup.maxhp = data->stat1C;
    unit->backup.tec = data->resist[2];
    unit->backup.mp = data->stat24;
    unit->backup.luck = data->resist[3];
    unit->backup.maxmp = data->stat24;
    sbtlHuumaParamUp(unit);
    sbtlCheckStockPara(unit);
    unit->now.hp = unit->now.maxhp;
    unit->now.mp = unit->now.maxmp;
    skill = unit->haveskill;
    memset(skill, 0, 10);
    param = data->param;
    n = 3;
    do {
        if (*param != 0) {
            *skill = *param;
            skill++;
        }
        param++;
    } while (--n >= 0);
    sbtlCheckAutoSkill(unit, 0);
    sbtlSetAutoSkill_GB(unit);
    aisyo = sbtlGetAisyotbl(data->aisyoId);
    unit->aisyo = *aisyo;
    wk_sbtl->stockDevilSkill[idx] = unit->devilId;
    wk_sbtl->stockMinor[idx] = devilid;
    wk_sbtl->stockNum++;
    gohCommandDEVILOUTAFTERSub(unit, &DDS3_GBWK->unk48F0, 1);
    return idx;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlEntryStockData);
#endif

/* NON_MATCHING: 74 of 85 words match.  */
#ifdef NON_MATCHING
void sbtlHuumaParamUp(struct sdatUnitWork_t *unit) {
    u32 total;
    s32 roll;
    s16 i;
    s16 idx;
    u8 *p;

    total = 0;
    sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    GetPercentage();
    i = 0;
    do {
        total += sdatLvUpDevilParamUp[unit->devilId].Param[i];
        i++;
    } while (i < 4);
    roll = dds3GetRandA(NULL) % total;
    idx = 0;
    if (roll >= sdatLvUpDevilParamUp[unit->devilId].Param[0]) {
        p = sdatLvUpDevilParamUp[unit->devilId].Param;
        do {
            idx++;
        } while (idx < 4 && roll >= p[idx]);
    }
    switch (idx) {
    case 0:
        unit->huuma.str++;
        break;
    case 1:
        unit->huuma.tec++;
        break;
    case 2:
        unit->huuma.vit++;
        break;
    default:
        unit->huuma.luck++;
        break;
    }
    datCallGetPlayerMaxHP(unit, 0);
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlHuumaParamUp);
#endif

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

struct SBTLUNIT_WORK *sbtlCallMemset6(s32 index, s32 flg) {
    sdfVector4_t pos;
    struct SBTLUNIT_WORK *wk;
    struct SBTLUNIT_WORK *target;
    s16 saved;

    memset(&pos, 0, sizeof(pos));
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[index]);
    wk->aiwk->stat = 3;
    wk->aiwk->mode = 4;
    wk->model->stat = 1;
    wk->model->flags &= ~1;
    wk->mode = 4;
    wk->color = 0x808080;
    saved = wk->field384;
    wk->status = 0;
    sbtlBtlSetEndDataSub(wk, index);
    wk->field384 = saved;
    sbtlCheckBattlePara(wk);
    wk_sbtl->field34C[index] = 1;
    wk_sbtl->stockUnitID[index] = wk->field1A0;
    wk_sbtl->field358[index] = 1;
    target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (!sbtlGetMovePos_MapHit(target, wk, &pos, target->model->field11C + wk->model->field11C, 0.0f, 1)) {
        if (!sbtlGetMovePos_MapHit(target, wk, &pos, target->model->field11C + wk->model->field11C + wk->model->field11C, 0.0f, 1)) {
            SDF_VU0_LOADVECTOR(&wk->pos);
            SDF_VU0_STOREVECTOR(&pos);
        }
    }
    SDF_VU0_LOADVECTOR(&pos);
    SDF_VU0_STOREVECTOR(&wk->pos);
    SDF_VU0_LOADVECTOR(&target->rot);
    SDF_VU0_STOREVECTOR(&wk->rot);
    wk->angleY = target->angleY;
    sbtlSetMotion_sbtlUnit(wk, 9, 0, 0, 0);
    return wk;
}

INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallMemset7);

/* NON_MATCHING: 126 of 159 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchUnitID4(s32 index, s32 flg) {
    struct SBTLUNIT_WORK *wk;
    s32 kansen = 0;

    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[index]);
    if (wk) {
        wk->aiwk->stat = 3;
        wk->aiwk->mode = 5;
        wk->model->stat = 1;
        sbtlClearHitC(wk_sbtl->hitList, wk);
        sbtlForceReset(wk_sbtl->effList, wk);
        sbtlCallBtlCreateDelWork(wk);
        if (sbtlGetBadStatus(&wk->unit) != BST_KANSEN) {
            if (wk_sbtl->expList->field10 >= 250) {
                wk_sbtl->expList->field10 = 250;
            }
            wk->unit.vice = wk_sbtl->expList->field10;
            sbtlResetVicePara(wk_sbtl->expList);
        } else {
            kansen = 1;
            sbtlResetVicePara(wk_sbtl->expList);
            wk->unit.vice = wk_sbtl->expList->field10;
        }
        sbtlBtlSetEndDataSub(wk, index);
        sbtlResetBadStatus(wk, wk->unit.badStatus);
        wk_sbtl->summonIndex[1] = index;
        wk_sbtl->flg2 |= 0x100;
        wk_sbtl->field34C[index] = 0;
        wk_sbtl->field348--;
        if (--wk_sbtl->field344 <= 0) {
            wk_sbtl->field344 = 0;
        } else if (wk_sbtl->field344 >= 4) {
            wk_sbtl->field344 = 3;
        }
        sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
        sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
        if (wk->flg2 & 0x100) {
            sbtlCallMemset17(wk_sbtl->effList, wk, EFFID_RETURN_D, 0, &wk->pos.x, 0, 0);
            wk->color = 0;
        } else if (flg) {
            sbtlCallMemset17(wk_sbtl->effList, wk, EFFID_RETURN_D, 0, &wk->pos.x, 0x80808080, 0);
        } else {
            sbtlCallMemset17(wk_sbtl->effList, wk, EFFID_RETURN, 0, &wk->pos.x, 0x80808080, 0);
        }
        wk->flg |= 0x4000;
        if (kansen == 1 || flg == 1) {
            wk->m_rate = 0.0f;
        } else {
            sbtlSetAction(wk, 1, 0);
            sbtlSetMotion_sbtlUnit(wk, 7, 0, 0, 0);
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID4);
#endif

/* NON_MATCHING: 23 of 27 words match.  */
#ifdef NON_MATCHING
s32 sbtlResetSummon_E(struct SBTLUNIT_WORK *wk)
{
    wk->mode = 4;
    wk->color = 0x808080;
    wk->model->mode = 9;
    wk->model->stat = 1;
    wk->status = 0;
    sbtlCallMemset17(wk_sbtl->effList, wk, EFFID_SUMMON_E, 0, &wk->pos.x, 0x80808080, 0);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlResetSummon_E);
#endif

s32 sbtlCallBtlCheckDamageTarget(struct SBTLUNIT_WORK *wk)
{
    if (wk != NULL && sbtlCheckDamageTarget(wk) != 0) {
        sbtlCallMemset17(wk_sbtl->effList, wk, EFFID_RETURNE, 0, &wk->pos.x, 0x80808080, 0);
        sbtlClearHitC(wk_sbtl->hitList, wk);
        sbtlForceReset(wk_sbtl->effList, wk);
        wk->status = 0;
        wk->model->mode = 9;
        wk->model->stat = 1;
    }
    return 1;
}

void sbtlResetAction_Flag(struct SBTLUNIT_WORK *wk)
{
    if (wk_sbtl->battleMode != 3 && ((wk->field414 & 0x80) || (wk->field414 & 1))) {
        wk->model->flags &= ~0x4000;
        wk->flg &= ~0x4000;
    }
}

/* NON_MATCHING: 19 of 71 words match.  */
#ifdef NON_MATCHING
s32 ResetAction(struct SBTLUNIT_WORK *wk, s8 flg)
{
    sdfVector4_t zero;
    struct SBTLUNIT_MODEL *model;
    s32 type = flg;

    memset(&zero, 0, sizeof(zero));
    model = wk->model;
    model->flags = (model->flags | 0x44) & ~0x223B;
    SDF_VU0_LOADVECTOR(&zero);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    wk->field424 = 0.0f;
    if (type == 1) {
        model->field118 = flg;
        sbtlSetGroupAction(wk, -1, 12, 0);
        model = wk->model;
    } else {
        model->mode = 12;
    }
    model->stat = 1;
    model->frame = 0;
    model->cmpframe = 60;
    sbtlRestartMotion(wk);
    wk->flg &= ~0x100;
    wk->vstatus &= ~0x08020000;
    wk->model->flags &= ~0x4000;
    sbtlResetAIPattern(wk, 0);
    sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", ResetAction);
#endif

/* NON_MATCHING: 143 of 150 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchUnitID20(struct SBTLUNIT_LISTWORK *lwk, s32 *index) {
    sdfVector4_t posA;
    sdfVector4_t posB;
    sdfVector4_t vecA;
    sdfVector4_t vecB;
    sdfVector4_t vertexA;
    struct SBTLUNIT_WORK *target;
    struct SBTLUNIT_WORK *wk;
    f32 nearest;
    f32 length;
    s32 best;
    s32 i;

    target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (target == NULL) {
        return 0;
    }
    sbtlGetPosforDistance(target, -target->model->field11C);
    SDF_VU0_STOREVECTOR(&vertexA);
    if (lwk != NULL) {
        sbtlClearUnitList(lwk);
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 2)) {
            SDF_VU0_LOADVECTOR(&vertexA);
            SDF_VU0_STOREVECTOR(&posA);
            SDF_VU0_STOREVECTOR(&vecA);
            SDF_VU0_LOADVECTOR(&wk->pos);
            SDF_VU0_STOREVECTOR(&posB);
            SDF_VU0_STOREVECTOR(&vecB);
            posA.y = 0.0f;
            posB.y = 0.0f;
            vecA.z = 0.0f;
            vecB.z = 0.0f;
            if (sbtlCheckCrossCircle_Rot(&posA, &target->rot, 3000.0f, &posB, wk->model->field11C, 0.9f)
                || sbtlCheckCrossCircle_Rot(&vecA, &target->rot, 3000.0f, &vecB, wk->model->field11C, 0.29999998211860657f)) {
                sbtlEntryUnitList(lwk, wk);
            }
        }
    }
    if (lwk->count == 0) {
        return 0;
    }
    sbtlGetPosforDistance(target, 3000.0f);
    SDF_VU0_STOREVECTOR(&posA);
    nearest = 9999.8994140625f;
    best = 0;
    for (i = 0; i < lwk->number; i++) {
        wk = lwk->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 2) && sbtlCheckDamageTarget(wk)) {
            length = sbtlGetVerticalLength(&vertexA.x, &posA.x, &wk->pos.x);
            if (length <= nearest) {
                nearest = length;
                best = i;
            }
        }
    }
    *index = best;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlCallBtlSearchUnitID20);
#endif

s32 sbtlCallBtlCheckDamageTarget2(s32 id)
{
    struct SBTLUNIT_WORK *unit;
    s32 i;

    for (i = 0; i < wk_sbtl->list2D4->number; i++) {
        unit = wk_sbtl->list2D4->pUnitL[i].pUnit;
        if (unit != NULL && sbtlCheckDamageTarget(unit) && unit->field1A0 == id) {
            wk_sbtl->field2D0 = i;
            return 1;
        }
    }
    wk_sbtl->field2D0 = -1;
    return 0;
}

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

s32 AllDeleteEnemy(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    s32 allDeleted = 1;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && sbtlCheckDamageTarget(unit) && (unit->field414 & 2) && wk->field1A0 != unit->field1A0
            && !(wk_sbtl->flg & 0x2000000) && unit->mode != 9) {
            allDeleted = 0;
            unit->model->flags = (unit->model->flags | 0x40) & ~0x20;
            sbtlSetDeadDamage(unit, 0);
        } else if (unit != NULL && (unit->field414 & 2)) {
            if (wk->field1A0 != unit->field1A0) {
                allDeleted = 0;
            }
        }
    }
    return allDeleted;
}

s32 sbtlCheckUnit_Exec_R(void)
{
    struct SBTLUNIT_WORK *wk;
    s32 i;

    if (wk_sbtl->unitList == NULL) {
        return 1;
    }
    if (wk_sbtl->flg2 & 0x200) {
        return 0;
    }
    for (i = 0; i < wk_sbtl->unitList->number;) {
        wk = wk_sbtl->unitList->pUnitL[i++].pUnit;
        if (wk != NULL && (u16)(wk->mode - 1) < 2 && wk->gstat != 5) {
            if (wk->field414 & 2) {
                return 0;
            }
            if (wk->field414 & 1) {
                if (!sbtlSearchReturn_Unit(wk)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

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

/* NON_MATCHING: 66 of 89 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetUnitLight(struct SBTLUNIT_WORK *wk, s32 frame, u32 diffuse, u32 ambient, s32 flg, s32 flg2)
{
    s32 idx = 0;
    u32 vstatus;

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
    if (flg2 == 1) {
        wk->field4EC = frame;
    } else if (wk->field4EC > 0) {
        return 0;
    }
    if (wk->flg & 0x800) {
        return 0;
    }
    switch (idx) {
    case 1:
        wk->light.rgb0 = (diffuse & 0xFFFFFF) | 0x80000000;
        vstatus = wk->vstatus | 0x40000;
        break;
    case 2:
        wk->light.rgb1 = (diffuse & 0xFFFFFF) | 0x80000000;
        vstatus = wk->vstatus | 0x80000;
        break;
    case 3:
    default:
        wk->light.rgb2 = (diffuse & 0xFFFFFF) | 0x80000000;
        vstatus = wk->vstatus | 0x100000;
        break;
    }
    wk->vstatus = vstatus;
    wk->light.rgba = (ambient & 0xFFFFFF) | 0x80000000;
    evtSetUnitLightAndAmbient3(wk->hEveUnit, frame, wk->light.rgb0, wk->light.rgb1, wk->light.rgb2, wk->light.rgba);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024F220", sbtlSetUnitLight);
#endif

s32 sbtlCallGetLightParam(struct SBTLUNIT_WORK *wk, s32 frame)
{
    mapGetLightParam(&sbtlCallGetLightParamData, wk->pos.x, wk->pos.y, wk->pos.z);
    SDF_VU0_LOADVECTOR(&sbtlCallGetLightParamData.dir0);
    SDF_VU0_STOREVECTOR_NOMEM(&wk->light.dir2);
    wk->light.col2[0] = sbtlCallGetLightParamData.col0[0];
    wk->light.rgb2 = sbtlCallGetLightParamData.rgb0;
    wk->light.col2[1] = sbtlCallGetLightParamData.col0[1];
    wk->light.rgba = sbtlCallGetLightParamData.rgba;
    wk->light.col2[2] = sbtlCallGetLightParamData.col0[2];
    evtSetUnitLightAndAmbient3(wk->hEveUnit, frame, wk->light.rgb0, wk->light.rgb1, wk->light.rgb2, wk->light.rgba);
    SDF_VU0_LOADVECTOR(&wk->light.dir2);
    evtSetUnitLightDir2(wk->hEveUnit, 2);
    return 1;
}

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

