#include "common.h"
#include "dds3KernelCore.h"
#include "sdfMemory.h"
#include "fldProcess.h"
#include "cmpFile.h"
#include "code_00323280.h"
#include "code_0038AD80.h"
#include "cmpMain.h"
#include "code_00300578.h"
#include "code_r1_00323138.h"
#include "cmpMap.h"
#include "sdk_libc.h"
#include "code_0016A7D8.h"
#include "cmpStatus.h"
#include "code_r1_002CA3F0.h"
#include "EventBit.h"
#include "fclMsg.h"
#include "cmpItem.h"
#include "code_003648C0.h"
#include "dat.h"
#include "fclMisc.h"
#include "newdata.h"
#include "dds3KernelDraw.h"
#include "cfg.h"

void DrawTop(struct tagCMP_MAP_WORK *wk) {
    struct sprHandle_t *spr;
    s32 flg;

    spr = cmpGetSpriteHnd(CMP_SPR_MAP);
    flg = fldDrawTopSub();
    if (spr == NULL) {
        return;
    }
    itfDrawMiscSpriteList(0, 0, 0, 0xFF, spr, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL, DrawTopData);
    if (flg & 4) {
        itfDrawMiscSpriteAlpha(242, 62, 0, 0xFF, spr, 21, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 8) {
        itfDrawMiscSpriteAlpha(242, 375, 0, 0xFF, spr, 22, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 0x10) {
        itfDrawMiscSpriteAlpha(26, 218, 0, 0xFF, spr, 20, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 0x20) {
        itfDrawMiscSpriteAlpha(459, 219, 0, 0xFF, spr, 19, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 1) {
        itfIconDrawSprite(11, 17, 0, 0xFF, 53, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(43, 15, 0, 0xFF, spr, 2, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 2) {
        itfIconDrawSprite(435, 17, 0, 0xFF, 54, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(467, 15, 0, 0xFF, spr, 3, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    }
    itfIconDrawSprite(438, 419, 0, 0xFF, 13, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    itfDrawMiscSpriteAlpha(460, 420, 0, 0xFF, spr, 4, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
}

s32 DrawProcess(dds3ProcessID_t *PID) {
    tagCMP_MAP_WORK *wk = dds3GetProcessData(PID);

    if (!(wk->flag & 8)) {
        return 0;
    }
    cmpDrawBack(wk);
    cmpDrawBackPattern(wk);
    mapAutoMapDraw3();
    DrawTop(wk);
    return 0;
}

void Release_cmpMap(tagCMP_MAP_WORK *wk) {
    fldCallTexFreeDelayed();
    if (wk->flag & 2) {
        wk->flag &= ~2;
        cmpFreeSpriteMem(CMP_SPR_MAP);
    }
    if (wk->flag & 4) {
        wk->flag &= ~4;
        cmpFreeSpriteHnd(CMP_SPR_MAP);
    }
}

void Shutdown_cmpMap(dds3ProcessID_t *id) {
    tagCMP_MAP_WORK *wk = dds3GetProcessData(id);

    Release_cmpMap(wk);
    if (dds3ExistProcess(wk->drawPID)) {
        dds3KillProcess(wk->drawPID, 0);
    }
    sdfMemFreeDirect(wk->handle);
    g_mainPID_cmpMap = NULL;
}

void ProcessStart_cmpMap(void) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(sizeof(tagCMP_MAP_WORK));
    tagCMP_MAP_WORK *wk = sdfMemGetPtr(handle);

    memset(wk, 0, sizeof(tagCMP_MAP_WORK));
    wk->handle = handle;
    g_mainPID_cmpMap = dds3StartProcess(ProcessStart_cmpMapStrCampAutomapMain, 1007, 0, 0, Process_cmpMap,
                                        (dds3ProcessFunc_t)Shutdown_cmpMap, wk);
    wk->drawPID = dds3StartProcess(ProcessStart_cmpMapStrCampAutomapDraw, 11012, 0, 0, DrawProcess, NULL, wk);
    if (cmpGetSpriteHnd(CMP_SPR_MAP) == NULL) {
        if (cmpLoadSpriteHnd(CMP_SPR_MAP) == NULL) {
            cmpLoadSpriteMem(CMP_SPR_MAP);
            wk->flag |= 2;
        }
        cmpSetupCmpItemSub(CMP_SPR_MAP);
        wk->flag |= 4;
    }
    fldMapAllAmap();
}

void ProcessEnd_cmpMap(void) {
    tagCMP_MAP_WORK *wk;

    if (g_mainPID_cmpMap != NULL && dds3ExistProcess(g_mainPID_cmpMap)) {
        wk = dds3GetProcessData(g_mainPID_cmpMap);
        if (!(wk->flag & 1)) {
            wk->flag |= 1;
            wk->step = 4;
            wk->count = 0;
        }
    }
}

s32 ProcessCheck_cmpMap(void) {
    if (g_mainPID_cmpMap == NULL) {
        return 1;
    }
    if (dds3GetProcessStat(g_mainPID_cmpMap) == 3) {
        g_mainPID_cmpMap = NULL;
        return 1;
    }
    if (dds3GetProcessStat(g_mainPID_cmpMap) == 0) {
        g_mainPID_cmpMap = NULL;
        return 1;
    }
    return 0;
}

struct tagCMP_WORK *cmpRefMainPIDCmpMain(void) {
    return dds3GetProcessData(g_mainPID_cmpMain);
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpFunc7);

dds3ProcessID_t *cmpPartyDrawFuncSub(void)
{
    return cmpRefMainPIDCmpMain()->partyPID;
}

dds3ProcessID_t *cmpGetCharPID(void)
{
    return cmpRefMainPIDCmpMain()->charPID;
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpGetsubMenuPID);

struct tagCMP_WORK *cmpGetStockPID(void) {
    return dds3GetProcessData(g_mainPID_cmpMain);
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpUpdateSkillHDCmpKudaSub);

INCLUDE_ASM(const s32, "code_0038AD80", cmpUpdateSkillHDCmpKudaSub2);

INCLUDE_ASM(const s32, "code_0038AD80", cmpUpdateSkillHDCmpKudaSub3);

INCLUDE_ASM(const s32, "code_0038AD80", cmpUpdateSkillHDCmpKudaSub4);

const char *cmpDrawPartsOmoideSkillSub3(u32 id) {
    if (id < 36) {
        return cmpDrawPartsOmoideSkillSub3Data[id];
    }
    return cmpData49;
}

s32 cmpChkTabChg(void) {
    return cmpRefMainPIDCmpMain()->pageMax != 0;
}

void cmpCallMenuSetFlagMenu(void) {
    struct tagCMP_WORK *wk = cmpRefMainPIDCmpMain();

    if (!(wk->markID & 1)) {
        return;
    }
    if (wk->markID != CMP_MENU_ID_SKILLBT && wk->markID != CMP_MENU_ID_STATUS && wk->markID != CMP_MENU_ID_SWAP
        && wk->markID != CMP_MENU_ID_PART && wk->markID != CMP_MENU_ID_SUMMON) {
        return;
    }
    itfMenuSetFlagMenu(wk->menuPID, -1, ITFMENU_CB_GF_ENABLE_DECIDE, 0);
    itfMenuSetFlagMenu(wk->menuPID, wk->markID, ITFMENU_CB_GF_ENABLE_DECIDE, 1);
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallMenuGetSelectID3);

INCLUDE_RODATA(const s32, "code_0038AD80", ProcessStart_cmpMapStrCampAutomapMain);

INCLUDE_RODATA(const s32, "code_0038AD80", ProcessStart_cmpMapStrCampAutomapDraw);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrInheritedPassiveSkills);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrInvestigativeSkills);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrPassiveSkills);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrCombatSkills);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrKnownPassiveSkills);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrSelectPassiveSkill);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrCannotUseAutoSettings);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrCancelBattleSettings);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrCancelAutoSettings);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrBattleSetting);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrSettings);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrCaseFile);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrInsectCages);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrNotebook);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrCrystals);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrEquipSword);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrValuables);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrThrowAway);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrVibration);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrReference);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrMainMenu);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrSetWhetherToHaveTheSkill);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrSetUpToFiveSkillsForEach);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrUseDemonSkill);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrViewCurrentlyAccepted);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrViewInsectCages);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrViewKeyItems);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrViewEntriesInThe);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrViewCrystalsOnHand);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrViewGemsOnHand);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrEquipAnotherSword);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrMoneyOnHand);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrReleaseDemon);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrOrganizeYourDemons);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrChooseDemonTo);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrCheckValuables);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrThrowAwayItems);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrChangeTheEquippedBullets);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrUseItems);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrLoadSavedGame);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrVibrationSettings);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrCheckTheCurrentLocation);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrViewRaidouAndHisDemons);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrViewTheDetectiveNotebook);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrAdjustDemonSettings);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrConsumeItemsEquipSwords);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpStrSetAndUseDemonAbilities);

void ChkEnableSelectMenuBook(struct tagCMP_WORK *wk) {
    CMP_MENU_ID ids[3] = { CMP_MENU_ID_KEYWORD, CMP_MENU_ID_CHECK2, CMP_MENU_ID_MUSHI };
    u32 i;

    for (i = 0; i < 3; i++) {
        itfMenuSetFlagMenu(wk->menuPID, ids[i], ITFMENU_CB_GF_ENABLE_DECIDE, cmpCallMakeCateChk(ids[i]) > 0);
    }
}

void cmpChkEnableSelectMenuItem(struct tagCMP_WORK *wk) {
    CMP_MENU_ID ids[6] = { CMP_MENU_ID_USE,   CMP_MENU_ID_BULLET, CMP_MENU_ID_THROW,
                           CMP_MENU_ID_SWORD, CMP_MENU_ID_JEWEL,  CMP_MENU_ID_MATERIAL };
    u32 i;

    for (i = 0; i < 6; i++) {
        itfMenuSetFlagMenu(wk->menuPID, ids[i], ITFMENU_CB_GF_ENABLE_DECIDE, MakeList(ids[i]) > 0);
    }
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpProcess);

INCLUDE_ASM(const s32, "code_0038AD80", cmpChkEnableSelectMenu);

s32 cmpGetstrCmpMenuHD(void) {
    s32 i;

    for (i = 0x1F01; i < 0x1F65; i++) {
        if (evtBitCheck(i)) {
            return 1;
        }
    }
    for (i = 0x1E81; i < 0x1EE5; i++) {
        if (evtBitCheck(i)) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpSetMenu);

void Init_cmpMain(struct tagCMP_WORK *wk) {
    s32 num = datGetStockDevilNums();

    wk->markID = -1;
    wk->count = -1;
    wk->flag = 0;
    wk->unk24 = 0;
    wk->page = 0;
    wk->pageMax = num / 9;
    wk->unk2E = DDS3_GBWK->playerData.partyNum / 9;
    cmpUpdateBulletNum();
    cmpResetMotion(0);
    wk->unk28 = 0;
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallGetProcessData25);

void cmpDrawKeyHelp2(u8 a_alphaRate, DDS3_OT a_otNo) {
    itfDrawMiscSpriteList(-10, 379, 0, a_alphaRate, cmpGetSpriteHnd(CMP_SPR_COMMON), a_otNo, cmpDrawKeyHelp2Data);
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawHelpText);

void cmpDrawKeyReturnHelp(struct tagCMP_WORK *wk, u8 a_alphaRate1, u8 a_alphaRate2) {
    struct sprHandle_t *hSpr = cmpGetSpriteHnd(CMP_SPR_COMMON);

    if (fclChkMessageDraw() != 1 && cmpChkMessageDraw() != 1) {
        itfIconDrawSprite(438, 413, 0, a_alphaRate2, 13, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(460, 414, 0, a_alphaRate2, hSpr, 0, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
}

void cmpDrawKeyHelp(struct tagCMP_WORK *wk, u8 a_alphaRate1, u8 a_alphaRate2) {
    struct sprHandle_t *hSpr = cmpGetSpriteHnd(CMP_SPR_COMMON);

    if (fclChkMessageDraw() != 1 && cmpChkMessageDraw() != 1) {
        itfIconDrawSprite(365, 413, 0, a_alphaRate1, 12, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(387, 414, 0, a_alphaRate1, hSpr, 1, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        itfIconDrawSprite(438, 413, 0, a_alphaRate2, 13, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(460, 414, 0, a_alphaRate2, hSpr, 0, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
}

void cmpDrawKeyHelp2_cmpMain(struct tagCMP_WORK *wk, u8 a_alphaRate1, u8 a_alphaRate2) {
    struct sprHandle_t *hSpr = cmpGetSpriteHnd(CMP_SPR_COMMON);

    if (fclChkMessageDraw() != 1 && cmpChkMessageDraw() != 1) {
        itfIconDrawSprite(295, 413, 0, a_alphaRate2, 11, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(317, 414, 0, a_alphaRate2, hSpr, 188, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
}

void cmpDrawKeyHelp3(struct tagCMP_WORK *wk, u8 a_alphaRate, s32 a_x) {
    struct sprHandle_t *hSpr = cmpGetSpriteHnd(CMP_SPR_COMMON);
    s32 num = datGetStockDevilNums();

    if (fclChkMessageDraw() != 1 && cmpChkMessageDraw() != 1 && num >= 9) {
        itfIconDrawSprite(a_x + 247, 413, 0, a_alphaRate, 15, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(a_x + 269, 414, 0, a_alphaRate, hSpr, 185, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallGetSpriteHnd);

void cmpDrawMenuFunc(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_color, u8 a_alpha) {
    struct sprHandle_t *hSpr = cmpGetSpriteHnd(CMP_SPR_COMMON);

    if (itfMenuChkMenuTop(cmpRefMainPIDCmpMain()->menuPID, a_id)) {
        itfDrawMiscSpriteList(0, 0, a_z, a_alpha, hSpr, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL, cmpDrawMenuFuncData);
    }
    if (a_id & 0x10) {
        if (dds3RefDDS3GBWK(0)) {
            if (a_id == 0xF010) {
                itfDrawMiscSpriteAlpha(a_x - 8, a_y - 4, a_z, a_alpha, hSpr, 69, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
            }
        } else if (a_id == 0x10010) {
            itfDrawMiscSpriteAlpha(a_x - 8, a_y - 4, a_z, a_alpha, hSpr, 69, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        }
    }
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpGetEvtItemSprNo);

void cmpCallGetSpriteHnd2(struct tagCMP_WORK *wk, u8 a_alphaRate) {
    char buf[16];
    dds3GlobalWork_t *gbwk;
    struct sprHandle_t *hSpr = cmpGetSpriteHnd(CMP_SPR_COMMON);

    if (cmpCallGetProcessData25Data == 0) {
        itfDrawMiscSpriteList(0, 0, 0, a_alphaRate, hSpr, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL, cmpCallGetSpriteHnd7Data);
        gbwk = DDS3_GBWK;
        sprintf(buf, cmpCallGetSpriteHnd2Data2, gbwk->playerData.maka);
        itfDrawMiscFont(ITF_DM_STYLE_SNUM, 405, 357, 0, 0, 0xA09DC380, a_alphaRate, buf);
    }
}

void cmpLeaf49(s32 a_sprNo, s32 a_x, s32 a_y, u32 a_alphaRate, struct sprHandle_t *a_hSpr) {
    effDrawSpriteWithCornerFade(a_x * 16, a_y * 8, 0, a_alphaRate, 96, a_hSpr, a_sprNo, DDS3_OT_BACK5_NORMAL);
}

void cmpCallGetSpriteHnd9(s32 a_sprNo, s32 a_x, s32 a_y, s32 a_alphaRate) {
    cmpLeaf49(a_sprNo, a_x, a_y, a_alphaRate, cmpGetSpriteHnd(CMP_SPR_COMMON));
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallGetSpriteHnd10);

void cmpFunc8(s32 a_x, s32 a_y, s32 a_alphaRate) {
    cmpCallGetSpriteHnd9(127, a_x + 27, a_y + 28, a_alphaRate);
    cmpCallGetSpriteHnd9(127, a_x + 97, a_y + 67, a_alphaRate);
    cmpCallGetSpriteHnd9(128, a_x + 9, a_y + 42, a_alphaRate);
    cmpCallGetSpriteHnd9(126, a_x + 13, a_y + 54, a_alphaRate);
    cmpCallGetSpriteHnd9(129, a_x + 36, a_y + 97, a_alphaRate);
    cmpCallGetSpriteHnd9(128, a_x + 101, a_y + 12, a_alphaRate);
    cmpCallGetSpriteHnd9(126, a_x + 50, a_y + 75, a_alphaRate);
    cmpCallGetSpriteHnd9(129, a_x + 61, a_y + 69, a_alphaRate);
    cmpCallGetSpriteHnd9(126, a_x + 85, a_y + 32, a_alphaRate);
    cmpCallGetSpriteHnd9(126, a_x + 109, a_y + 44, a_alphaRate);
    cmpCallGetSpriteHnd9(128, a_x + 83, a_y + 85, a_alphaRate);
}

void cmpStatusDrawSub(s32 a_x, s32 a_y, s32 a_alphaRate) {
    cmpCallGetSpriteHnd9(100, a_x + 27, a_y + 28, a_alphaRate);
    cmpCallGetSpriteHnd9(100, a_x + 97, a_y + 67, a_alphaRate);
    cmpCallGetSpriteHnd9(101, a_x + 9, a_y + 42, a_alphaRate);
    cmpCallGetSpriteHnd9(99, a_x + 13, a_y + 54, a_alphaRate);
    cmpCallGetSpriteHnd9(102, a_x + 36, a_y + 97, a_alphaRate);
    cmpCallGetSpriteHnd9(101, a_x + 101, a_y + 12, a_alphaRate);
    cmpCallGetSpriteHnd9(99, a_x + 50, a_y + 75, a_alphaRate);
    cmpCallGetSpriteHnd9(102, a_x + 61, a_y + 69, a_alphaRate);
    cmpCallGetSpriteHnd9(99, a_x + 85, a_y + 32, a_alphaRate);
    cmpCallGetSpriteHnd9(99, a_x + 109, a_y + 44, a_alphaRate);
    cmpCallGetSpriteHnd9(101, a_x + 83, a_y + 85, a_alphaRate);
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallGetSpriteHnd11);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawRaidouSkill);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallSetOt3);

void cmpCallSetOt4(s32 a_alphaRate) {
    cmpCallGetSpriteHnd11();
    eglSetOt(28);
    eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
    eglLinkAlpha(0);
    cmpCallGetSpriteHnd9(122, 171, 82, a_alphaRate);
    cmpCallGetSpriteHnd9(123, 171, 82, a_alphaRate);
    cmpCallGetSpriteHnd9(124, 171, 235, a_alphaRate);
    cmpFunc8(269, 64, a_alphaRate);
    cmpFunc8(265, 81, a_alphaRate);
    cmpFunc8(404, 96, a_alphaRate);
    cmpFunc8(103, 192, a_alphaRate);
    cmpFunc8(243, 185, a_alphaRate);
    cmpFunc8(352, 192, a_alphaRate);
    cmpFunc8(345, 228, a_alphaRate);
    cmpCallGetSpriteHnd9(105, 386, 193, a_alphaRate);
    cmpCallGetSpriteHnd9(107, 351, 232, a_alphaRate);
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallSetOt5);

void CmpStatusDraw(s32 a_alphaRate) {
    eglSetOt(28);
    eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
    eglLinkAlpha(0);
    cmpStatusDrawSub(84, -39, a_alphaRate);
    cmpStatusDrawSub(179, -22, a_alphaRate);
    cmpStatusDrawSub(319, -62, a_alphaRate);
    cmpStatusDrawSub(7, 142, a_alphaRate);
    cmpStatusDrawSub(-20, 277, a_alphaRate);
    cmpStatusDrawSub(25, 320, a_alphaRate);
    cmpStatusDrawSub(32, 378, a_alphaRate);
    cmpStatusDrawSub(168, 379, a_alphaRate);
    cmpStatusDrawSub(362, 378, a_alphaRate);
    cmpStatusDrawSub(-52, 148, a_alphaRate);
    cmpStatusDrawSub(180, -41, a_alphaRate);
    cmpStatusDrawSub(71, -37, a_alphaRate);
    cmpStatusDrawSub(321, -56, a_alphaRate);
    cmpCallGetSpriteHnd9(103, 94, 0, a_alphaRate);
    cmpCallGetSpriteHnd9(104, -25, -17, a_alphaRate);
    cmpCallGetSpriteHnd9(106, 287, -35, a_alphaRate);
    cmpCallGetSpriteHnd9(104, 392, 2, a_alphaRate);
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawProcess);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawProcess_cmpMain);

CMP_MODE cmpGetEndMode(void)
{
    return cmpCallFuncData;
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpSetMarkID);

s32 cmpCallGetProcessData26(void)
{
    struct tagCMP_WORK *wk = dds3GetProcessData(g_mainPID_cmpMain);

    return wk->count;
}

void cmpCallProcEnd(struct tagCMP_WORK *wk) {
    ProcessEnd_cmpMap();
    itfMenuProcessEnd(wk->menuPID);
    itfMenuProcessEnd(wk->partyPID);
    itfMenuProcessEnd(wk->charPID);
    itfMenuProcessEnd(wk->submPID);
    Delete_fclMsg();
    ProcessEnd_cmpStatus();
}

u8 cmpCheckProcEnd(struct tagCMP_WORK *wk) {
    u8 cnt = ProcessCheck_cmpMap() == 0;

    if (!itfMenuProcessCheck(wk->menuPID)) {
        wk->menuPID = NULL;
    } else {
        cnt++;
    }
    if (!itfMenuProcessCheck(wk->partyPID)) {
        wk->partyPID = NULL;
    } else {
        cnt++;
    }
    if (!itfMenuProcessCheck(wk->charPID)) {
        wk->charPID = NULL;
    } else {
        cnt++;
    }
    if (!itfMenuProcessCheck(wk->submPID)) {
        wk->submPID = NULL;
    } else {
        cnt++;
    }
    if (pbCheckPreviousRecord()) {
        cnt++;
    }
    if (ProcessCheck_cmpStatus()) {
        cnt++;
    }
    return cnt;
}

void cmpRelease(struct tagCMP_WORK *wk) {
    End_cmpItem();
    cmpFreeSpriteToGS();
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpProcessStartEx);

INCLUDE_RODATA(const s32, "code_0038AD80", cmpDrawHelpTextTable);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCampMain);

INCLUDE_ASM(const s32, "code_0038AD80", cmpProcessStart);

INCLUDE_ASM(const s32, "code_0038AD80", cmpProcessEnd);

s32 cmpProcessCheck(void) {
    if (g_mainPID_cmpMain == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(g_mainPID_cmpMain) == 3 || dds3GetProcessStat(g_mainPID_cmpMain) == 0) {
        g_mainPID_cmpMain = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallAddDevil);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallProcessEnd);

INCLUDE_ASM(const s32, "code_0038AD80", fclCheck);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews01201Spr);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews01101Spr);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews01001Spr);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews00901Spr);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews00801Spr);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews00701Spr);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdLoadSpriteDataEntry);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews00501Spr);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews00401Spr);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews00301Spr);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews00201Spr);

INCLUDE_RODATA(const s32, "code_0038AD80", nwdStrShopNewsNews00101Spr);

INCLUDE_ASM(const s32, "code_0038AD80", Process_nwdNews);

INCLUDE_ASM(const s32, "code_0038AD80", DrawProcess_nwdNews);

INCLUDE_ASM(const s32, "code_0038AD80", nwdLoadSpriteSub);

INCLUDE_ASM(const s32, "code_0038AD80", nwdLoadSprite);

INCLUDE_ASM(const s32, "code_0038AD80", nwdCallNext);

INCLUDE_ASM(const s32, "code_0038AD80", Release_nwdNews);

INCLUDE_ASM(const s32, "code_0038AD80", Shutdown_nwdNews);

INCLUDE_ASM(const s32, "code_0038AD80", nwdProcessStart);

INCLUDE_ASM(const s32, "code_0038AD80", nwdProcessEnd);

INCLUDE_ASM(const s32, "code_0038AD80", nwdProcessCheck);

INCLUDE_ASM(const s32, "code_0038AD80", shpCreateScene);

INCLUDE_ASM(const s32, "code_0038AD80", shpDeleteScene);

INCLUDE_ASM(const s32, "code_0038AD80", shpMakeLightPacket);

INCLUDE_ASM(const s32, "code_0038AD80", SetLight);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetCamera);

INCLUDE_RODATA(const s32, "code_0038AD80", shpMakeLightPacketTable);

INCLUDE_RODATA(const s32, "code_0038AD80", shpMakeLightPacketTable2);

INCLUDE_RODATA(const s32, "code_0038AD80", shpMakeLightPacketTable3);

INCLUDE_RODATA(const s32, "code_0038AD80", SetLightTable);

INCLUDE_RODATA(const s32, "code_0038AD80", SetLightTable2);

INCLUDE_RODATA(const s32, "code_0038AD80", SetLightTable3);

INCLUDE_RODATA(const s32, "code_0038AD80", SetLightTable4);

INCLUDE_RODATA(const s32, "code_0038AD80", SetLightTable5);

INCLUDE_RODATA(const s32, "code_0038AD80", SetLightTable6);

INCLUDE_ASM(const s32, "code_0038AD80", shpShopCamera);

INCLUDE_ASM(const s32, "code_0038AD80", Reset3DGlobal);

INCLUDE_ASM(const s32, "code_0038AD80", stfResetUI);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetMove3D);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetMove3DDef);

INCLUDE_ASM(const s32, "code_0038AD80", ChkMotionEnd);

INCLUDE_ASM(const s32, "code_0038AD80", shpUpdateCamera);

INCLUDE_ASM(const s32, "code_0038AD80", shpDeleteModel);

INCLUDE_ASM(const s32, "code_0038AD80", CreateModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetModel);

INCLUDE_ASM(const s32, "code_0038AD80", addNpcHukidashi);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetMotion);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetMotionSpeed);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSingleMotion);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSmoothLoopMotion);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSmoothSingleMotion);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSwitchMotion);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetMotionPause);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetPosModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetRotModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetRotYModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSclModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSclModelEx);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetDrwModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetDrwModelAll);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetColor);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetParamInit);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetTrans);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetDebugPrint);

INCLUDE_ASM(const s32, "code_0038AD80", shpDrawFieldLine);

INCLUDE_ASM(const s32, "code_0038AD80", shpGetBasic);

INCLUDE_ASM(const s32, "code_0038AD80", shpGetExist);

INCLUDE_ASM(const s32, "code_0038AD80", shpCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_0038AD80", shpCallDeleteScene);

INCLUDE_ASM(const s32, "code_0038AD80", campCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_0038AD80", campCallSetOt);

INCLUDE_ASM(const s32, "code_0038AD80", campRefShpDraw);

INCLUDE_ASM(const s32, "code_0038AD80", scrLeaf4);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefShpDraw);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefShpDraw4);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefShpDraw5);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefShpDraw2);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefShpDraw3);

INCLUDE_ASM(const s32, "code_0038AD80", scrClearProfileFlagsTable);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "code_0038AD80", scrPartsListDatSub);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefDDS3GBWK4);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallGetFigure);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallSprintf2);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallClearProfileFlagsTable);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallSprintf);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallClearProfileFlagsTable2);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallClearProfileFlagsTable3);

INCLUDE_ASM(const s32, "code_0038AD80", shpItemListTestSub);

INCLUDE_ASM(const s32, "code_0038AD80", PartsMoney);

INCLUDE_ASM(const s32, "code_0038AD80", shpPartsListDatSub);

INCLUDE_ASM(const s32, "code_0038AD80", shpCallClearProfileFlagsTable);

INCLUDE_ASM(const s32, "code_0038AD80", Select2);

INCLUDE_ASM(const s32, "code_0038AD80", PartsListDat);

INCLUDE_ASM(const s32, "code_0038AD80", shpCallSetOt);

INCLUDE_ASM(const s32, "code_0038AD80", ScrollBar);

INCLUDE_ASM(const s32, "code_0038AD80", PartsList);

INCLUDE_ASM(const s32, "code_0038AD80", shpItemListTest);

INCLUDE_ASM(const s32, "code_0038AD80", knyDrawKeyHelp);

INCLUDE_ASM(const s32, "code_0038AD80", nwdDrawKeyHelp);

INCLUDE_ASM(const s32, "code_0038AD80", nwdCallChkMessageDraw);

INCLUDE_ASM(const s32, "code_0038AD80", shpRootCursor);

INCLUDE_ASM(const s32, "code_0038AD80", shpRootCursorInit);

INCLUDE_ASM(const s32, "code_0038AD80", shpResetRootLine);

INCLUDE_ASM(const s32, "code_0038AD80", shpCheckRootLineMax);

INCLUDE_ASM(const s32, "code_0038AD80", shpCursorLine);

INCLUDE_ASM(const s32, "code_0038AD80", shpRootLine);

INCLUDE_ASM(const s32, "code_0038AD80", shpRootCursorSelect);

INCLUDE_ASM(const s32, "code_0038AD80", shpBackSpr);

INCLUDE_ASM(const s32, "code_0038AD80", shpCommandHelp);

INCLUDE_ASM(const s32, "code_0038AD80", shpCallGetTitleAlpha);

INCLUDE_ASM(const s32, "code_0038AD80", AbLoadCallback);

INCLUDE_ASM(const s32, "code_0038AD80", fldFacilityMistInit);

INCLUDE_ASM(const s32, "code_0038AD80", fldRefShpDraw);

INCLUDE_ASM(const s32, "code_0038AD80", fldShopMilkhallTmx);

INCLUDE_ASM(const s32, "code_0038AD80", nbRefShpDraw);

INCLUDE_RODATA(const s32, "code_0038AD80", shpSetRotYModelTable);

INCLUDE_RODATA(const s32, "code_0038AD80", shpStrShopKonnouyaTmx);

INCLUDE_RODATA(const s32, "code_0038AD80", shpStrShopBackKonnouSpr);

INCLUDE_RODATA(const s32, "code_0038AD80", shpStrShopMilkhallTmx);

INCLUDE_RODATA(const s32, "code_0038AD80", shpStrShopBackMilkSpr);

