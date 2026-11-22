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
#include "dds3obj.h"
#include "dds3UnitObjectBasic.h"
#include "dds3KernelDraw.h"
#include "code_r1_00128378.h"
#include "evtStage.h"
#include "evtMoon.h"
#include "fld.h"
#include "mapData.h"
#include "dds3Basic.h"
#include "dds3WorldBasic.h"
#include "sdfDelayedResrc.h"

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

/* NON_MATCHING: 518 of 527 words match.  */
#ifdef NON_MATCHING
s32 LoadMap_FromMemory(s32 fldno, s32 floorno) {
    sdfVector4_t rotBase = { 0.0f, 0.0f, 0.0f, 1.0f };
    s32 aMapMdl[23] = {
        0x2B, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
        0x3C, 0x3D, 0x3E, 0x3F, 0x65, 0x66, 0x3F, 0x82, 0xA0, 0x97, 0x98,
    };
    s32 aMapMdl2[23] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0x7E, 0x7F, 0, 0x7E, 0x7E, 0, 0, 0,
    };
    sdfVector4_t aPos = { 0.0f, 0.0f, 0.0f, 1.0f };
    sdfVector4_t aRot = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 x, y, z, r;
    dds3Basic_t *unit;
    SBTL_STAGE_DATA *stage;
    struct SBTLUNIT_WORK *pchara;
    s32 i;

    sbtlData156 = 0;
    sbtlCallMemset4Data4 = 0;
    sbtlCallMemset4Data5 = 0.0f;
    sbtlCallMemset4Data6 = 0.0f;
    sbtlCallMemset4Data7 = 0.0f;
    sbtlData90 = 0;
    sbtlData91 = 0;
    sdfLoadMapFromMemorySub();
    sbtlRefDDS3GBWK();
    sdfBtlReLoadMapSBtlSceneSub();
    dds3BistaClear();
    BasicSceneFree();
    mapBtlReLoadMapSBtlSceneSub2();
    evtLoadArea(fldno, floorno);
    if (wk_sbtl->mapmdl == 0) {
        wk_sbtl->mapmdl = floorno;
    }
    if (wk_sbtl->mapmdl >= 23) {
        wk_sbtl->mapmdl = 0;
    }
    if (aMapMdl2[wk_sbtl->mapmdl] != 0) {
        aPos.z = 1.0f;
        unit = dds3AddUnitObjectBasic(dds3AllocObjectId(), &aPos, &aRot);
        dds3SetActiveUnitSceneBasic(dds3GetControlScene(), unit);
        LoadMap_FromMemoryData3 = aMapMdl2[wk_sbtl->mapmdl];
        sbtlCallSync2();
        dds3ObjectBaseMakeUnit(unit, MODELTYPE_FIELD, aMapMdl2[wk_sbtl->mapmdl]);
        wk_sbtl->hMapMdl2 = dds3ObjectBaseGetModelHandle(unit);
        wk_sbtl->hMapMdl2->first->speed = 0.5f;
        mdlLoopMotion(wk_sbtl->hMapMdl2, 0, 0);
        dds3ObjectBaseOffFlag(unit, 1);
        dds3UnitObjectBasicSetOt(unit, 3);
        SetMapLight(unit, 0, 0x80808080);
        wk_sbtl->mapclr = 0x80808080;
        mdlSetColor(wk_sbtl->hMapMdl2, 0x80808080);
    }
    if (aMapMdl[wk_sbtl->mapmdl] != 0) {
        aPos.z = 0.0f;
        unit = dds3AddUnitObjectBasic(dds3AllocObjectId(), &aPos, &aRot);
        dds3SetActiveUnitSceneBasic(dds3GetControlScene(), unit);
        LoadMap_FromMemoryData3 = aMapMdl[wk_sbtl->mapmdl];
        sbtlCallSync2();
        dds3ObjectBaseMakeUnit(unit, MODELTYPE_FIELD, aMapMdl[wk_sbtl->mapmdl]);
        wk_sbtl->hMapMdl = dds3ObjectBaseGetModelHandle(unit);
        wk_sbtl->hMapMdl->first->speed = 0.5f;
        mdlLoopMotion(wk_sbtl->hMapMdl, 0, 0);
        dds3ObjectBaseOffFlag(unit, 1);
        dds3UnitObjectBasicSetOt(unit, 4);
        SetMapLight(unit, 0, 0x80808080);
        wk_sbtl->mapclr = 0x80808080;
        mdlSetColor(wk_sbtl->hMapMdl, 0x80808080);
    }
    for (i = 0; i < 8; i++) {
        wk_sbtl->apMarkerUnit[i] = dds3AddUnitObjectBasic(dds3AllocObjectId(), &aPos, &aRot);
        if (i == 0) {
            sbtlLoadMapFromMemorySub();
        }
        dds3ObjectBaseMakeUnit(wk_sbtl->apMarkerUnit[i], MODELTYPE_FIELD, 45);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnit[i], 1);
        dds3ObjectBaseOffFlag(wk_sbtl->apMarkerUnit[i], 0x400);
        dds3UnitObjectBasicSetOt(wk_sbtl->apMarkerUnit[i], 1);
    }
    for (i = 0; i < 8; i++) {
        wk_sbtl->apMarkerUnitR[i] = dds3AddUnitObjectBasic(dds3AllocObjectId(), &aPos, &aRot);
        if (i == 0) {
            sbtlLoadMapFromMemorySub2();
        }
        dds3ObjectBaseMakeUnit(wk_sbtl->apMarkerUnitR[i], MODELTYPE_FIELD, 159);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnitR[i], 1);
        dds3ObjectBaseOffFlag(wk_sbtl->apMarkerUnitR[i], 0x400);
        dds3UnitObjectBasicSetOt(wk_sbtl->apMarkerUnitR[i], 1);
    }
    evtEnableMoonDraw();
    SetTestCamera();
    sbtlSbtlCameraproc();
    sbtlResetRndEncCount();
    mapMapSetEnc(wk_sbtl->stageNo);
    stage = &sbtlData100[wk_sbtl->stageNo];
    if (stage->form != 0) {
        mapMapSetForm(stage->form);
    }
    mapCutSetCam(dds3GetActiveCameraSceneBasic(dds3GetActiveScene()), 0);
    pchara = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (main_data.mode == 1 && pchara != NULL) {
        mapEncPointGet(sbtlData100[wk_sbtl->stageNo].encPoint, &x, &y, &z, &r);
        pchara->pos.x = x;
        pchara->pos.y = y;
        pchara->pos.z = z;
        FromEuler(0.0f, r * 0.0174532925f, 0.0f);
        SDF_VU0_STOREVECTOR(&pchara->rot);
        SDF_VU0_LOADVECTOR(&rotBase);
        SDF_VU0_STOREVECTOR(&pchara->rotBase);
        pchara->angleY = r;
        if (sbtlCheckSummon(0)) {
            wk_sbtl->field3BC |= 0x20;
            LoadMap_FromMemoryData.y = y;
            LoadMap_FromMemoryData.z = z;
            LoadMap_FromMemoryData.x = x;
            if (r > -45.0f && r < 45.0f) {
                LoadMap_FromMemoryData.x = x - 200.0f;
            } else if (r < -45.0f && r > -135.0f) {
                LoadMap_FromMemoryData.z -= 200.0f;
            } else if (r > 45.0f && r < 135.0f) {
                LoadMap_FromMemoryData.z += 200.0f;
            } else {
                LoadMap_FromMemoryData.x += 200.0f;
            }
            FromEuler(0.0f, r * 0.0174532925f, 0.0f);
            SDF_VU0_STOREVECTOR(&LoadMap_FromMemoryData2);
            sbtlData161 = r;
        }
    }
    if (pchara != NULL) {
        sbtlCallMemset4Data4 = 0;
        sbtlCallMemset4Data5 = pchara->pos.x;
        sbtlCallMemset4Data6 = pchara->pos.y;
        sbtlCallMemset4Data7 = pchara->pos.z;
        mapCutSet_From_Pos(pchara->pos.x, pchara->pos.y - 100.0f, pchara->pos.z);
    }
    mapCutSetLight();
    sbtlRefSbtl31();
    while (sdfDelayedResrcCheck()) {
    }
    pbSetGameCamera0();
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0026B0A8", LoadMap_FromMemory);
#endif

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

void sbtlPlayViceSE(struct SBTLUNIT_WORK *wk, s32 motionID, s32 frame)
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

void Proc_sbtlSound(void) {
    s32 i;
    struct SBTLUNIT_WORK *wk;
    f32 prev;
    f32 frame;
    u32 se;
    s32 vol;
    s16 id;
    s32 cnt;
    s32 motion;

    if (wk_sbtl == NULL || wk_sbtl->unitList == NULL) {
        return;
    }
    for (i = 0; i < 256; i++) {
        sbtlWork10[i].count++;
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
        if (wk->hModel != NULL) {
            frame = wk->hModel->first->frame;
        } else {
            frame = 0.0f;
        }
        prev = wk->se.footwk.prevFrame;
        wk->se.footwk.prevFrame = frame;
        if ((prev < 25.0f && frame >= 25.0f) || frame < prev) {
            se = sbtlCallGetRandIntA4(wk);
            sbtlSEVolAngleCalc(se, sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96));
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
    sbtlCallBtlSEVolAngleCalc();
    if (IsSceanStop()) {
        return;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk == NULL) {
            continue;
        }
        if (wk->unit.devilId == 473) {
            se = 0;
            vol = sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96);
            if (wk->motionno == 18) {
                if (wk->se.mot_nowcnt == 50) se = 0x1000E;
                if (wk->se.mot_nowcnt == 85) se = 0x1000F;
                if (wk->se.mot_nowcnt == 104) se = 0x10010;
                if (wk->se.mot_nowcnt == 100) se = 0x5D90006;
            }
            if (wk->motionno == 21) {
                if (wk->se.mot_nowcnt == 80) se = 0x10017;
                if (wk->se.mot_nowcnt == 88) se = 0x10017;
                if (wk->se.mot_nowcnt == 96) se = 0x10017;
                if (wk->se.mot_nowcnt == 104) se = 0x10017;
            }
            if (se != 0) {
                sbtlSEVolAngleCalc(se, vol);
            }
        }
        if (wk->unit.devilId == 493) {
            se = 0;
            vol = sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96);
            if (wk->motionno == 21) {
                if (wk->se.mot_nowcnt == 80) se = 0x10017;
                if (wk->se.mot_nowcnt == 88) se = 0x10017;
                if (wk->se.mot_nowcnt == 96) se = 0x10017;
                if (wk->se.mot_nowcnt == 104) se = 0x10017;
            }
            if (se != 0) {
                sbtlSEVolAngleCalc(se, vol);
            }
        }
        if (wk->unit.devilId == 494) {
            se = 0;
            vol = sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96);
            if (wk->motionno == 21) {
                if (wk->se.mot_nowcnt == 80) se = 0x10017;
                if (wk->se.mot_nowcnt == 88) se = 0x10017;
                if (wk->se.mot_nowcnt == 96) se = 0x10017;
                if (wk->se.mot_nowcnt == 104) se = 0x10017;
            }
            if (wk->motionno == 18) {
                if (wk->se.mot_nowcnt == 20) se = 0x5EE0011;
                if (wk->se.mot_nowcnt == 90) se = 0x1002C;
                if (wk->se.mot_nowcnt == 150) se = 0x9AE0001;
            }
            if (se != 0) {
                sbtlSEVolAngleCalc(se, vol);
            }
        }
        if (wk->unit.devilId == 451) {
            se = 0;
            vol = sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96);
            if (wk->motionno == 16) {
                if (wk->se.mot_nowcnt == 15) se = 0x1002C;
            }
            if (wk->motionno == 17) {
                if (wk->se.mot_nowcnt == 45) se = 0x1000E;
                if (wk->se.mot_nowcnt == 65) se = 0x1000F;
                if (wk->se.mot_nowcnt == 85) se = 0x10010;
            }
            if (wk->motionno == 20) {
                if (wk->se.mot_nowcnt == 62) se = 0x10017;
            }
            if (wk->motionno == 22) {
                if (wk->se.mot_nowcnt == 82) se = 0x10017;
                if (wk->se.mot_nowcnt == 88) se = 0x10017;
                if (wk->se.mot_nowcnt == 94) se = 0x10017;
            }
            if (se != 0) {
                sbtlSEVolAngleCalc(se, vol);
            }
        }
        if (wk->unit.devilId == 452) {
            se = 0;
            vol = sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96);
            if (wk->motionno == 18) {
                if (wk->se.mot_nowcnt == 45) se = 0x1000E;
                if (wk->se.mot_nowcnt == 65) se = 0x1000F;
                if (wk->se.mot_nowcnt == 85) se = 0x10010;
                if (wk->se.mot_nowcnt == 80) se = 0x5C40011;
            }
            if (wk->motionno == 19) {
                if (wk->se.mot_nowcnt == 4) se = 0x5C40011;
            }
            if (wk->motionno == 21) {
                if (wk->se.mot_nowcnt == 4) se = 0x5C40011;
                if (wk->se.mot_nowcnt == 90) se = 0x1002C;
                if (wk->se.mot_nowcnt == 150) se = 0x9BC0001;
            }
            if (se != 0) {
                sbtlSEVolAngleCalc(se, vol);
            }
        }
        id = wk->unit.devilId;
        if (id == 67 || id == 274) {
            sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96);
            if ((wk->motionno == 7 || wk->motionno == 8) && wk->se.mot_nowcnt == 12) {
                sbtlPlaySE(sbtlGetDevilSESub(wk->unit.devilId, 13));
            }
        } else if (id == 192 || id == 399 || id == 195 || id == 402 || id == 194 || id == 401) {
            sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96);
            if (wk->motionno == 7 && wk->se.mot_nowcnt == 10) {
                sbtlPlaySE(sbtlGetDevilSESub(wk->unit.devilId, 13));
            }
        }
        id = wk->unit.devilId;
        if (id == 197 || id == 404 || id == 542 || id == 198 || id == 405 || id == 543 || id == 199 || id == 406 ||
            id == 544) {
            sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96);
            motion = wk->motionno;
            if (motion == 7) {
                if (wk->se.mot_nowcnt == 55) {
                    sbtlPlaySE(sbtlGetDevilSESub(wk->unit.devilId, 6));
                }
            } else if (motion == 9) {
                cnt = wk->se.mot_nowcnt;
                if (cnt == 60 || cnt == 104 || cnt == 150) {
                    sbtlPlaySE(sbtlGetDevilSESub(wk->unit.devilId, 6));
                }
            } else if (motion == 14) {
                if (wk->se.mot_nowcnt == 8) {
                    sbtlPlaySE(sbtlGetDevilSESub(wk->unit.devilId, 13));
                }
            } else if ((wk->unit.devilId == 197 || wk->unit.devilId == 404) && motion == 15) {
                if (wk->se.mot_nowcnt == 30 || wk->se.mot_nowcnt == 60 || wk->se.mot_nowcnt == 98) {
                    sbtlPlaySE(sbtlGetDevilSESub(wk->unit.devilId, 6));
                }
            }
        } else if (id == 77 || id == 284 || id == 483) {
            sbtlSEVolAngleCalc_sbtlSound(&wk->pos, 2000.0f, 1000.0f, 127, 96);
            if (wk->motionno == 15 && wk->se.mot_nowcnt == 24) {
                sbtlPlaySE(sbtlGetDevilSESub(wk->unit.devilId, 6));
            }
        }
    }
}

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

u32 GetR1CombSklSmg(SKLID skillID) {
    switch (skillID) {
    case 512:
        return 0x9000000;
    case 532:
        return 0x9010000;
    case 533:
        return 0x9020000;
    case 548:
        return 0x9030000;
    case 549:
        return 0x9040000;
    case 550:
        return 0x9050000;
    case 551:
        return 0x9060000;
    case 564:
        return 0x9070000;
    case 565:
        return 0x9080000;
    case 580:
        return 0x9090000;
    case 581:
        return 0x90A0000;
    case 610:
        return 0x90B0000;
    case 656:
        return 0x90C0000;
    case 657:
        return 0x90D0000;
    case 658:
        return 0x90E0000;
    case 659:
        return 0x90F0000;
    case 660:
        return 0x9100000;
    case 661:
        return 0x9110000;
    case 685:
        return 0x9120000;
    case 686:
        return 0x9130000;
    case 687:
        return 0x9140000;
    case 688:
        return 0x9150000;
    case 689:
        return 0x9160000;
    case 690:
        return 0x9170000;
    case 691:
        return 0x9180000;
    case 692:
        return 0x9190000;
    case 693:
        return 0x91A0000;
    case 694:
        return 0x91B0000;
    case 695:
        return 0x91C0000;
    case 696:
        return 0x91D0000;
    case 697:
        return 0x91E0000;
    case 698:
        return 0x91F0000;
    case 699:
        return 0x9200000;
    case 700:
        return 0x9210000;
    case 701:
        return 0x9220000;
    case 736:
        return 0x9230000;
    case 737:
        return 0x9240000;
    case 738:
        return 0x9250000;
    case 739:
        return 0x9260000;
    case 740:
        return 0x9270000;
    case 741:
        return 0x9280000;
    case 742:
        return 0x9290000;
    case 743:
        return 0x92A0000;
    case 744:
        return 0x92B0000;
    case 745:
        return 0x92C0000;
    case 746:
        return 0x92D0000;
    case 747:
        return 0x92E0000;
    case 748:
        return 0x92F0000;
    case 749:
        return 0x9300000;
    case 750:
        return 0x9310000;
    case 751:
        return 0x9320000;
    case 768:
        return 0x9330000;
    case 769:
        return 0x9340000;
    case 770:
        return 0x9350000;
    case 771:
        return 0x9360000;
    case 772:
        return 0x9370000;
    case 773:
        return 0x9380000;
    case 774:
        return 0x9390000;
    case 775:
        return 0x93A0000;
    case 776:
        return 0x93B0000;
    case 777:
        return 0x93C0000;
    case 778:
        return 0x93D0000;
    case 865:
        return 0x93E0000;
    case 866:
        return 0x93F0000;
    case 867:
        return 0x9400000;
    case 873:
        return 0x9410000;
    case 874:
        return 0x9420000;
    case 880:
        return 0x9430000;
    case 881:
        return 0x9440000;
    case 882:
        return 0x9450000;
    case 884:
        return 0x9460000;
    case 885:
        return 0x9470000;
    case 886:
        return 0x9480000;
    case 890:
        return 0x9490000;
    case 897:
        return 0x94A0000;
    case 898:
        return 0x94B0000;
    case 902:
        return 0x94C0000;
    case 903:
        return 0x94D0000;
    case 908:
        return 0x94E0000;
    case 913:
        return 0x94F0000;
    case 914:
        return 0x9500000;
    case 915:
        return 0x9510000;
    case 916:
        return 0x9520000;
    case 993:
        return 0x9530000;
    case 994:
        return 0x9540000;
    case 995:
        return 0x9550000;
    case 1001:
        return 0x9560000;
    case 1002:
        return 0x9570000;
    case 1006:
        return 0x9580000;
    case 1008:
        return 0x9590000;
    case 1010:
        return 0x95A0000;
    case 1011:
        return 0x95B0000;
    case 1012:
        return 0x95C0000;
    case 1091:
        return 0x95D0000;
    case 1093:
        return 0x95E0000;
    case 1095:
        return 0x95F0000;
    case 1096:
        return 0x9600000;
    case 1097:
        return 0x9610000;
    case 1098:
        return 0x9620000;
    case 1099:
        return 0x9630000;
    case 1101:
        return 0x9640000;
    case 1103:
        return 0x9650000;
    case 1104:
        return 0x9660000;
    case 1105:
        return 0x9670000;
    case 1106:
        return 0x9680000;
    case 1107:
        return 0x9690000;
    case 1108:
        return 0x96A0000;
    case 1110:
        return 0x96B0000;
    case 1112:
        return 0x96C0000;
    case 1113:
        return 0x96D0000;
    case 1114:
        return 0x96E0000;
    case 1120:
        return 0x96F0000;
    case 1121:
        return 0x9700000;
    case 1122:
        return 0x9710000;
    case 1123:
        return 0x9720000;
    case 1124:
        return 0x9730000;
    case 1125:
        return 0x9740000;
    case 1126:
        return 0x9750000;
    case 1127:
        return 0x9760000;
    case 1128:
        return 0x9770000;
    case 1129:
        return 0x9780000;
    case 1136:
        return 0x9790000;
    case 1137:
        return 0x97A0000;
    case 1138:
        return 0x97B0000;
    case 1139:
        return 0x97C0000;
    case 1140:
        return 0x97D0000;
    case 1141:
        return 0x97E0000;
    case 1142:
        return 0x97F0000;
    case 1143:
        return 0x9800000;
    case 1144:
        return 0x9810000;
    case 1145:
        return 0x9820000;
    case 1146:
        return 0x9830000;
    case 1148:
        return 0x9840000;
    case 1149:
        return 0x9850000;
    case 1150:
        return 0x9860000;
    case 1151:
        return 0x9870000;
    case 1152:
        return 0x9880000;
    case 1153:
        return 0x9890000;
    case 1154:
        return 0x98A0000;
    case 1155:
        return 0x98B0000;
    case 1156:
        return 0x98C0000;
    case 1157:
        return 0x98D0000;
    case 1158:
        return 0x98E0000;
    case 1159:
        return 0x98F0000;
    case 1160:
        return 0x9900000;
    case 1161:
        return 0x9910000;
    case 1164:
        return 0x9920000;
    case 1165:
        return 0x9930000;
    case 1166:
        return 0x9940000;
    case 1167:
        return 0x9950000;
    case 1168:
        return 0x9960000;
    case 1169:
        return 0x9970000;
    case 1170:
        return 0x9980000;
    case 1171:
        return 0x9990000;
    case 1172:
        return 0x99A0000;
    case 1175:
        return 0x99B0000;
    case 1178:
        return 0x99C0000;
    case 1179:
        return 0x99D0000;
    case 1180:
        return 0x99E0000;
    case 1181:
        return 0x99F0000;
    case 1182:
        return 0x9A00000;
    case 1183:
        return 0x9A10000;
    case 1184:
        return 0x9A20000;
    case 1185:
        return 0x9A30000;
    case 1186:
        return 0x9A40000;
    case 1188:
        return 0x9A50000;
    case 1190:
        return 0x9A60000;
    case 1191:
        return 0x9A70000;
    case 1192:
        return 0x9A80000;
    case 1193:
        return 0x9A90000;
    case 1194:
        return 0x9AA0000;
    case 1199:
        return 0x9AB0000;
    case 1200:
        return 0x9AC0000;
    case 1201:
        return 0x9AD0000;
    case 1204:
        return 0x9AE0000;
    case 1205:
        return 0x9AF0000;
    case 1206:
        return 0x9B00000;
    case 1210:
        return 0x9B10000;
    case 1211:
        return 0x9B20000;
    case 1212:
        return 0x9B30000;
    case 1213:
        return 0x9B40000;
    case 1214:
        return 0x9B50000;
    case 1215:
        return 0x9B60000;
    case 1216:
        return 0x9B70000;
    case 1217:
        return 0x9B80000;
    case 1218:
        return 0x9B90000;
    case 1219:
        return 0x9BA0000;
    case 1220:
        return 0x9BB0000;
    case 1221:
        return 0x9BC0000;
    case 1223:
        return 0x9BD0000;
    case 1224:
        return 0x9BE0000;
    case 1225:
        return 0x9BF0000;
    case 1226:
        return 0x9C00000;
    case 1227:
        return 0x9C10000;
    case 1228:
        return 0x9C20000;
    case 1229:
        return 0x9C30000;
    case 1232:
        return 0x9C40000;
    case 1233:
        return 0x9C50000;
    case 1234:
        return 0x9C60000;
    case 1235:
        return 0x9C70000;
    case 1236:
        return 0x9C80000;
    case 1237:
        return 0x9C90000;
    case 1240:
        return 0x9CA0000;
    case 1242:
        return 0x9CB0000;
    case 1243:
        return 0x9CC0000;
    case 1246:
        return 0x9CD0000;
    case 1247:
        return 0x9CE0000;
    case 1248:
        return 0x9CF0000;
    case 1249:
        return 0x9D00000;
    case 1250:
        return 0x9D10000;
    case 1251:
        return 0x9D20000;
    case 1252:
        return 0x9D30000;
    case 1254:
        return 0x9D40000;
    case 1255:
        return 0x9D50000;
    case 1256:
        return 0x9D60000;
    case 1257:
        return 0x9D70000;
    case 1258:
        return 0x9D80000;
    case 1259:
        return 0x9D90000;
    case 1260:
        return 0x9DA0000;
    case 1261:
        return 0x9DB0000;
    case 1264:
        return 0x9DC0000;
    case 1265:
        return 0x9DD0000;
    case 1266:
        return 0x9DE0000;
    case 1267:
        return 0x9DF0000;
    case 1268:
        return 0x9E00000;
    case 1269:
        return 0x9E10000;
    default:
        return 0;
    }
}

/* NON_MATCHING: 192 of 198 words match.  */
#ifdef NON_MATCHING
u32 GetSkillSE(s32 skillID, s32 type)
{
    struct SBTLSKILLP_TBL *tbl;
    u32 se;
    u32 ret;

    ret = 0;
    switch (skillID) {
    case SKLID_NOUSE_IAITATE:
        switch (type) {
        case -1:
            break;
        case 1:
            return 0x9E40000;
        case 2:
            return 0x9E40001;
        }
        break;
    case SKLID_NOUSE_MAGTUKI:
        switch (type) {
        case -1:
            break;
        case 1:
            return 0x9E40002;
        case 2:
            return 0x9E40003;
        }
        break;
    case SKLID_NOUSE_DORIRUPIKE:
        switch (type) {
        case -1:
            break;
        case 1:
            return 0x9E30000;
        case 2:
            return 0x9E30001;
        }
        break;
    case SKLID_NOUSE_RAINPIKE:
        switch (type) {
        case -1:
            break;
        case 1:
            return 0x9E30002;
        case 2:
            return 0x9E30003;
        }
        break;
    case SKLID_NOUSE_SHORTPIKE:
        switch (type) {
        case -1:
            break;
        case 1:
            return 0x9E30004;
        case 2:
            return 0x9E30005;
        }
        break;
    case SKLID_NOUSE_MEGATONWARI:
        switch (type) {
        case -1:
            break;
        case 1:
            return 0x9E20000;
        case 2:
            return 0x10003;
        }
        break;
    case SKLID_NOUSE_MIZOOCHITUKI:
        switch (type) {
        case -1:
            break;
        case 1:
            return 0x9E20002;
        case 2:
            return 0x9E20003;
        }
        break;
    case SKLID_MAWASIKERI:
        switch (type) {
        case -1:
            break;
        case 1:
            return 0x1002B;
        case 2:
            return 0x1002C;
        }
        break;
    }
    tbl = sbtlGetSkilltbl(skillID);
    if (tbl->s_type & 4) {
        switch (type) {
        case -1:
        case 1:
            ret = 0x10017;
            break;
        case 2:
            ret = 0x10017;
            break;
        }
        return ret;
    }
    if ((tbl->type & 0xFFF) == 8) {
        if (skillID != SKLID_ITM_STUNGREN) {
            se = (skillID << 16) + 0x8C00000;
            return se;
        }
        se = 0xA120000;
        return se;
    }
    if (skillID < SKLID_MENETUKE) {
        se = (skillID << 16) + 0x7000000;
    } else {
        se = GetR1CombSklSmg(skillID);
        if (se == 0) {
            return se;
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
INCLUDE_ASM(const s32, "code_0026B0A8", GetSkillSE);
#endif

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
    wk->next_motion_not_hokan = 0;
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

s32 UnitMV_sbtlUnit(struct SBTLUNIT_WORK *wk)
{
    sdfVector4_t mv;
    sdfVector4_t rot;
    f32 y;

    memset(&mv, 0, sizeof(mv));
    if (IsSceanStop() && !(wk->flg & 0x10)) {
        return 0;
    }
    if (wk->flg & 0x80000) {
        wk->model->flags = (wk->model->flags | 0x20) & ~0x40;
        SDF_VU0_LOADVECTOR(&wk->posSaved);
        SDF_VU0_STOREVECTOR(&wk->pos);
    }
    mv.x = -wk->mv_spd.x;
    mv.z = -wk->mv_spd.z;
    if (wk_sbtl->battleMode == 3 ? wk->field414 & 1 : wk->field414 & 0x80) {
        SDF_VU0_LOADVECTOR(&wk->rotbk);
        SDF_VU0_STOREVECTOR(&rot);
    } else {
        SDF_VU0_LOADVECTOR(&wk->rot);
        SDF_VU0_STOREVECTOR(&rot);
    }
    SDF_VU0_LOADVECTOR_B(&mv);
    SDF_VU0_LOADVECTOR(&rot);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&wk->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&wk->pos);
    if (wk->field424 != 0.0f) {
        FromRotation(-wk->field424 * 0.017453293f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(&wk->rot);
        Mult();
        SDF_VU0_STOREVECTOR(&wk->rot);
    }
    if (wk->field68C == 1) {
        dds3UnitMVSBtlUnitSub(wk->pBasic, 12.0f);
        wk->field68C = 0;
    } else if (wk->field68C == 2) {
        dds3UnitMVSBtlUnitSub(wk->pBasic, 20.0f);
        wk->field68C = 0;
    }
    switch (wk->unit.devilId) {
    case 444:
    case 459:
    case 461:
        y = wk->field790;
        if (y > -250.0f) {
            wk->field790 = y + 5.0f;
        } else if (y > -500.0f) {
            wk->field790 = y + 10.0f;
        } else {
            wk->field790 = y + 15.0f;
        }
        if (wk->field790 < -3400.0f && wk->field790 > -3416.0f) {
            sbtlPlaySE(sbtlGetDevilSESub(wk->unit.devilId, 6));
        }
        if (wk->field790 > -3500.0f && !(wk->field790 > -300.0f)) {
            if (wk->field790 > -500.0f) {
                dds3SetQuake(90, 300);
            } else {
                dds3SetQuake(90, 200);
            }
        }
        if (wk->field790 > 0.0f) {
            wk->field790 = 0.0f;
            wk_sbtl->status4 |= 8;
        }
        wk->pos.y = wk->field790;
        break;
    }
    if (!(wk->vstatus & 0x2000)) {
        dds3SetPosPoseSet(wk->pBasic, &wk->pos);
        if (!(wk->vstatus & 8)) {
            dds3SetRotPoseSet(wk->pBasic, &wk->rot);
        }
    }
    return 1;
}

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

/* NON_MATCHING: 232 of 332 words match.  */
#ifdef NON_MATCHING
void sbtlResetAtkSklEff(struct SBTLUNIT_WORK *wk)
{
    SKLID tbl[7];
    SKLID tbl2[3];
    SKLID tbl3[4];
    SKLID tbl4[4];
    SKLID tbl5[5];
    SKLID tbl6[4];
    SKLID tbl7[4];
    SKLID tbl8[4];
    SKLID tbl9[3];
    SKLID tbl10[3];
    SKLID tbl11[3];
    SKLID tbl12[4];
    SKLID tbl13[4];
    SKLID tbl14[8];
    SKLID tbl15[9];
    SKLID tbl16[5];
    SKLID tbl17[3];
    SKLID tbl18[4];
    SKLID tbl19[6];
    SKLID tbl20[2];
    SKLID *list;

    memcpy(tbl, sbtlResetAtkSklEffTable, sizeof(tbl));
    memcpy(tbl2, sbtlResetAtkSklEffTable2, sizeof(tbl2));
    memcpy(tbl3, sbtlResetAtkSklEffTable3, sizeof(tbl3));
    memcpy(tbl4, sbtlResetAtkSklEffTable4, sizeof(tbl4));
    memcpy(tbl5, sbtlResetAtkSklEffTable5, sizeof(tbl5));
    memcpy(tbl7, sbtlResetAtkSklEffTable7, sizeof(tbl7));
    memcpy(tbl8, sbtlResetAtkSklEffTable8, sizeof(tbl8));
    memcpy(tbl6, sbtlResetAtkSklEffTable6, sizeof(tbl6));
    memcpy(tbl9, sbtlResetAtkSklEffTable9, sizeof(tbl9));
    memcpy(tbl10, sbtlResetAtkSklEffTable10, sizeof(tbl10));
    memcpy(tbl11, sbtlResetAtkSklEffTable11, sizeof(tbl11));
    memcpy(tbl12, sbtlResetAtkSklEffTable12, sizeof(tbl12));
    memcpy(tbl13, sbtlResetAtkSklEffTable13, sizeof(tbl13));
    memcpy(tbl14, sbtlResetAtkSklEffTable14, sizeof(tbl14));
    memcpy(tbl15, sbtlResetAtkSklEffTable15, sizeof(tbl15));
    memcpy(tbl16, sbtlResetAtkSklEffTable16, sizeof(tbl16));
    memcpy(tbl17, sbtlResetAtkSklEffTable17, sizeof(tbl17));
    memcpy(tbl18, sbtlResetAtkSklEffTable18, sizeof(tbl18));
    memcpy(tbl19, sbtlResetAtkSklEffTable19, sizeof(tbl19));
    memcpy(tbl20, sbtlResetAtkSklEffData, sizeof(tbl20));

    switch (wk->unit.devilId) {
    case 437:
    case 438:
    case 446:
        list = tbl2;
        break;
    case 435:
    case 445:
    case 447:
        list = tbl3;
        break;
    case 436:
    case 522:
        list = tbl4;
        break;
    case 439:
        list = tbl5;
        break;
    case 473:
        list = tbl6;
        break;
    case 493:
        list = tbl7;
        break;
    case 494:
        list = tbl8;
        break;
    case 434:
    case 448:
        list = tbl9;
        break;
    case 440:
        list = tbl10;
        break;
    case 484:
        list = tbl11;
        break;
    case 449:
    case 450:
    case 454:
        list = tbl12;
        break;
    case 453:
    case 523:
        list = tbl13;
        break;
    case 451:
        list = tbl14;
        break;
    case 452:
    case 457:
        list = tbl15;
        break;
    case 441:
        list = tbl16;
        break;
    case 442:
        list = tbl17;
        break;
    case 455:
        list = tbl18;
        break;
    case 456:
        list = tbl19;
        break;
    case 502:
        list = tbl20;
        break;
    default:
        list = tbl;
        break;
    }
    while (*list != 0) {
        sbtlDeleteEffectData_SkillIDTypeUnit(wk_sbtl->effList, *list, 1, wk->field1A0);
        sbtlDltHit_by_UnitSkl(wk_sbtl->hitList, wk->field1A0, *list++);
    }
}
#else
INCLUDE_ASM(const s32, "code_0026B0A8", sbtlResetAtkSklEff);
#endif

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetMotion_sbtlUnit);

void sbtlplayertblSetPlayerMotionData_Hokan(struct SBTLUNIT_WORK *wk)
{
    if (wk->next_motion_not_hokan) {
        wk->beforeframe = 0;
        return;
    }
    wk->beforeframe = 4;
    switch (wk->next_motionno) {
    case 33:
        if (sbtlDat()->kind == 3 && wk_sbtl->fieldCA2 == 0) {
            wk->beforeframe = 0;
        }
        break;
    case 23:
        switch (wk->motionno) {
        case 0:
        case 1:
        case 2:
        case 27:
        case 28:
            wk->beforeframe = 3;
            break;
        case 23:
            wk->beforeframe = 0;
            break;
        case 24:
        case 29:
            wk->beforeframe = 8;
            break;
        default:
            wk->beforeframe = 5;
            break;
        }
        break;
    case 0:
    case 2:
        switch (wk->motionno) {
        case 15:
            wk->beforeframe = 10;
            break;
        case 27:
            wk->beforeframe = 6;
            break;
        case 1:
            wk->beforeframe = 8;
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
    case 35:
    case 36:
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
            wk->beforeframe = 1;
            break;
        case 15:
        case 16:
            wk->beforeframe = 3;
            break;
        case 7:
            wk->beforeframe = 3;
            break;
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        default:
            wk->beforeframe = 2;
            break;
        }
        if (wk_sbtl->fieldCA2) {
            wk->beforeframe = 0;
        }
        break;
    case 27:
        switch (wk->motionno) {
        case 0:
        case 2:
        case 15:
        case 16:
        case 19:
        case 28:
        case 29:
            wk->beforeframe = 2;
            break;
        case 42:
            wk->beforeframe = 6;
            break;
        }
        break;
    case 28:
        if (wk->motionno == 27) {
            wk->beforeframe = 0;
        }
        break;
    case 29:
        switch (wk->motionno) {
        case 27:
            wk->beforeframe = 2;
            break;
        case 28:
            wk->beforeframe = 0;
            break;
        }
        break;
    case 24:
        switch (wk->motionno) {
        case 27:
        case 28:
        case 30:
            wk->beforeframe = 3;
            break;
        case 29:
            wk->beforeframe = 2;
            break;
        }
        break;
    case 21:
        wk->beforeframe = 2;
        break;
    case 7:
    case 17:
    case 38:
        if (wk_sbtl->fieldCA2) {
            wk->beforeframe = 0;
        }
        break;
    case 41:
    case 42:
        wk->beforeframe = 0;
        break;
    }
}

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
