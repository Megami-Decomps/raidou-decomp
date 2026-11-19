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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlReLoadMap);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlModelFieldLb);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlModelFieldLb2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallMemGetPtr);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLoadMapFromMemorySub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLoadMapFromMemorySub2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallSync);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlModelFieldLb3);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallMemGetPtr3);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallPackDecode);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallSync2);

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

INCLUDE_ASM(const s32, "code_0026B0A8", cmpProcSequenceSub);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpSetColor);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpRefSbtl);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpRefSbtl2);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpLeaf3);

void sbtlSummonDvTaskSub(SBTLSOUND_FOOTWORK *fwk)
{
    fwk->prevSMGID = 0;
    fwk->prevFrame = 100000.0f;
    fwk->played = 0;
}

INCLUDE_ASM(const s32, "code_0026B0A8", cmpCallMemFreeDirect);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSEVolAngleCalc_sbtlSound);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlayViceSE);

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
        sbtlLeaf58(seid);
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

void sbtlLeaf58(u32 seid) {
    p2sdStopSe(seid);
}

INCLUDE_ASM(const s32, "code_0026B0A8", Proc_sbtlSound);

u32 sbtlGetDevilSESub(s32 devilID, s32 motionID) {
    return GetDevilSE(devilID, motionID);
}

INCLUDE_ASM(const s32, "code_0026B0A8", GetDevilSE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLoadDevilSE);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBstse);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlSetRirituNakamaHDSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallLoad);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallEffectShutdown);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSoundatBossAt);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlModelDataLoadSBtlUnitSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlayDeadVoice);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallGetSkillSE);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallBtlSEVolAngleCalc);

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

INCLUDE_ASM(const s32, "code_0026B0A8", Draw_sbtlUnit);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateUnit);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlDeleteUnit);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf61);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetMotionNo);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallIsSceanStop);

INCLUDE_ASM(const s32, "code_0026B0A8", RGBFade);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_DeadOn);

INCLUDE_ASM(const s32, "code_0026B0A8", ChangeTarget_BSHelper);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_Talk);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_Analyze);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlUpdateMiusinai);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckDamageTargetTemp);

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

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckTalkSelectTarget);

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
