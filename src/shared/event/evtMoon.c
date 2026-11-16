#include "common.h"
#include "newdata.h"
#include "evtMoon.h"
#include "dds3KernelCore.h"
#include "sBtlContinue.h"
#include "facility.h"
#include "code_r1_002C8188.h"
#include "EventBit.h"
#include "fld.h"
#include "dat.h"
#include "code_r1_002CA3F0.h"
#include "code_r1_002CD5A8.h"
#include "code_r1_001CC1D0.h"
#include "dds3KernelDraw.h"
#include "slMain.h"
#include "sdfMath.h"

extern dds3ProcessID_t *addressprocess_id;
extern const char evtMoonProcessName[];
extern s32 Shutdown_evtMoon(dds3ProcessID_t *id);
void sndCallGetRandIntA(void);
void sndCallGetRandIntA2(void);
void sndCallGetRandIntA3(void);

extern s32 gMoonFade;
extern s32 gMoonFadeFrame;
extern s32 gMoonFadeFrameMax;
extern f32 gMoonCnt;

s32 evtGetAgeOfMoon(void)
{
    s32 phase = DDS3_GBWK->moonPhase;
    if (phase >= 9) {
        phase = 8 - (phase & 7);
    }
    return phase;
}

s32 evtGetAgeOfMoon16(void)
{
    return DDS3_GBWK->moonPhase;
}

void evtSetAgeOfMoon(s32 age)
{
    DDS3_GBWK->moonPhase = age & 0xF;
    DDS3_GBWK->moonTimer = 0.0f;
}
#ifdef VERSION_RAIDOU2

INCLUDE_ASM(const s32, "event/evtMoon", evtExitSBtlSub);

INCLUDE_ASM(const s32, "event/evtMoon", evtBtlBarCtrlSub);
#endif

void evtEnableMoonCalc(void)
{
    DDS3_GBWK->moonFlags |= 1;
}

void evtDisableMoonCalc(void)
{
    DDS3_GBWK->moonFlags &= ~1;
}

void evtEnableMoonDraw(void)
{
    DDS3_GBWK->moonFlags |= 2;
    gMoonCnt = 1.0f;
    gMoonFade = 1;
    gMoonFadeFrameMax = 0;
}

void evtDisableMoonDraw(void)
{
    DDS3_GBWK->moonFlags &= ~2;
    gMoonCnt = 0.0f;
    gMoonFade = 0;
    gMoonFadeFrameMax = 0;
}

void evtEnableMoonDraw2(void)
{
    DDS3_GBWK->moonFlags |= 2;
    gMoonFade = 1;
    gMoonFadeFrameMax = 0;
}

void evtDisableMoonDraw2(void)
{
    DDS3_GBWK->moonFlags &= ~2;
    gMoonFade = 0;
    gMoonFadeFrameMax = 0;
}

void PreCalcFade(s32 count)
{
    if (count != 0) {
        if (gMoonFadeFrameMax != 0) {
            gMoonFadeFrame = (count * gMoonFadeFrame) / gMoonFadeFrameMax;
        } else {
            if (gMoonFade == 0) {
                count = 0;
            }
            gMoonFadeFrame = count;
        }
    }
}

void evtMoonFadeIn(s32 count)
{
    DDS3_GBWK->moonFlags |= 2;
    gMoonCnt = 1.0f;
    PreCalcFade(count);
    gMoonFadeFrameMax = count;
    gMoonFade = 1;
}

void evtMoonFadeOut(s32 count)
{
    PreCalcFade(count);
    gMoonFadeFrameMax = count;
    gMoonFade = 0;
}

s32 evtMoonFadeCheck(void)
{
    if (gMoonFadeFrame == 0) {
        return 1;
    }
    return gMoonFadeFrame == gMoonFadeFrameMax;
}

void CalcBlendColor(u32 *pDstCol, u32 *pSrcCol, s32 AlphaRate) {
    s32 i;

    for (i = 0; i < 4; i++) {
        pDstCol[i] = fclBlendColor(pSrcCol[i], pSrcCol[i] & 0xFFFFFF00, AlphaRate);
    }
}

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 115 of 739 words match.  */
#ifdef NON_MATCHING
s32 Process2_evtMoon(dds3ProcessID_t *id)
{
    u32 moonCol[4];
    u32 oldCol[4];
    u32 newCol[4];
    u32 palette[4];
    s32 age;
    s32 age16;
    s32 w;
    s32 h;
    s32 cng;
    s32 tone;
    s32 frame;
    u32 col;
    s32 old = 0;

    if (evtBitCheck(2100) != 0) {
        return 0;
    }
    if (fmProcessChk_slMain() != 0) {
        return 0;
    }
    if (dds3NullDrawProc() != 0) {
        return 0;
    }
    if (gMoonCngCnt > 0) {
        gMoonCngCnt--;
    }
    if (dds3BistaCheck() != 0) {
        return 0;
    }
    if (gMoonFade != 0) {
        gMoonFadeFrame++;
        if (gMoonFadeFrame > gMoonFadeFrameMax) {
            gMoonFadeFrame = gMoonFadeFrameMax;
        }
    } else {
        frame = gMoonFadeFrame - 1;
        if (frame < 0) {
            frame = 0;
        }
        gMoonFadeFrame = frame;
        if (frame == 0) {
            evtDisableMoonDraw();
        }
    }
    if (gMoonFadeFrameMax > 0) {
        gMoonBaseARate = (gMoonFadeFrame << 8) / gMoonFadeFrameMax;
    } else if (gMoonFade != 0) {
        gMoonBaseARate = 256;
    } else {
        gMoonBaseARate = 0;
    }
    if (!(DDS3_GBWK->moonFlags & 2)) {
        return 0;
    }
    age = evtGetAgeOfMoon();
    age16 = evtGetAgeOfMoon16();
    newCol[0] = newCol[1] = newCol[2] = newCol[3] = 0x80808080;
    w = 1228;
    h = 672;
    effDrawSpriteWithCornerFade(0, 0, 0, gMoonBaseARate, 32, moon_spr, 0, 103);
    Process2_evtMoonData7 += 0.02f;
    col = ((s32)(sdfSin(Process2_evtMoonData7) * 30.0f) + 50) | 0x80808000;
    moonCol[0] = moonCol[1] = moonCol[2] = moonCol[3] = col;
    moon_spr->workEntries[3].geometry.bounds[2] = w;
    moon_spr->workEntries[3].geometry.bounds[3] = h;
    CalcBlendColor(palette, moonCol, gMoonBaseARate);
    effDrawSpriteWithCornerColors(-134, -64, 0, palette, 32, moon_spr, 3, 103);
    if (gMoonCngCnt > 0) {
        cng = 85 - gMoonCngCnt;
        if (cng < 10) {
            age16 = gMoonAge16Old;
        }
        if (cng < 5) {
            old = 1;
            oldCol[0] = oldCol[1] = oldCol[2] = oldCol[3] = 0x80808080;
            newCol[0] = newCol[1] = newCol[2] = newCol[3] = 0;
        } else if (cng >= 5 && cng < 16) {
            tone = 128 - (cng * 128 - 640) / 10;
            old = 1;
            oldCol[0] = oldCol[1] = oldCol[2] = oldCol[3] = tone | 0x80808000;
            tone = 128 - tone;
            newCol[0] = newCol[1] = newCol[2] = newCol[3] = tone | 0x80808000;
        } else {
            old = 1;
            oldCol[0] = oldCol[1] = oldCol[2] = oldCol[3] = 0;
            newCol[0] = newCol[1] = newCol[2] = newCol[3] = 0x80808080;
        }
        if (cng < 8) {
            tone = (cng * 65) / 7;
        } else {
            tone = 65 - ((cng - 7) * 65) / 78;
        }
        if (tone > 65) {
            tone = 65;
        }
        if (tone < 0) {
            tone = 0;
        }
        moonCol[0] = moonCol[1] = moonCol[2] = moonCol[3] = tone | 0x80808000;
        w = ((cng * 1024) / 85) * 2;
        h = ((cng * 560) / 85) * 2;
        moon_spr->workEntries[29].geometry.bounds[2] = w + 1024;
        moon_spr->workEntries[29].geometry.bounds[3] = h + 560;
    }
    tone = gMoonBaseARate - 128;
    effDrawSpriteWithCornerFade(112, 72, 0, tone < 0 ? 0 : tone * 2, 32, moon_spr, 2, 103);
    switch (age16) {
    case 0:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 14, 103);
        break;
    case 1:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 15, 103);
        break;
    case 2:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 18, 103);
        break;
    case 3:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 19, 103);
        break;
    case 4:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 22, 103);
        break;
    case 5:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 23, 103);
        break;
    case 6:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 25, 103);
        break;
    case 7:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 28, 103);
        break;
    case 8:
        break;
    case 9:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 27, 103);
        break;
    case 10:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 26, 103);
        break;
    case 11:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 24, 103);
        break;
    case 12:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 21, 103);
        break;
    case 13:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 20, 103);
        break;
    case 14:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 17, 103);
        break;
    case 15:
        effDrawSpriteWithCornerFade(176, 80, 0, gMoonBaseARate, 32, moon_spr, 16, 103);
        break;
    }
    if (gMoonCngCnt > 0) {
        CalcBlendColor(palette, moonCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(-32 - w / 2, -8 - h / 2, 0, palette, 32, moon_spr, 29, 103);
    }
    effDrawSpriteWithCornerFade(0, 272, 0, gMoonBaseARate, 32, moon_spr, 1, 103);
    if (old) {
        switch (gMoonAgeOld) {
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
            CalcBlendColor(palette, oldCol, gMoonBaseARate);
            effDrawSpriteWithCornerColors(560, 216, 0, palette, 32, moon_spr, 13, 103);
            break;
        }
    }
    switch (age) {
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
        CalcBlendColor(palette, newCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(560, 216, 0, palette, 32, moon_spr, 13, 103);
        break;
    }
    if (old) {
        switch (gMoonAgeOld) {
        case 0:
            CalcBlendColor(palette, oldCol, gMoonBaseARate);
            effDrawSpriteWithCornerColors(400, 216, 0, palette, 32, moon_spr, 4, 103);
            break;
        case 1:
            CalcBlendColor(palette, oldCol, gMoonBaseARate);
            effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 7, 103);
            break;
        case 2:
            CalcBlendColor(palette, oldCol, gMoonBaseARate);
            effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 8, 103);
            break;
        case 3:
            CalcBlendColor(palette, oldCol, gMoonBaseARate);
            effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 9, 103);
            break;
        case 4:
            CalcBlendColor(palette, oldCol, gMoonBaseARate);
            effDrawSpriteWithCornerColors(400, 216, 0, palette, 32, moon_spr, 5, 103);
            break;
        case 5:
            CalcBlendColor(palette, oldCol, gMoonBaseARate);
            effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 10, 103);
            break;
        case 6:
            CalcBlendColor(palette, oldCol, gMoonBaseARate);
            effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 11, 103);
            break;
        case 7:
            CalcBlendColor(palette, oldCol, gMoonBaseARate);
            effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 12, 103);
            break;
        case 8:
            CalcBlendColor(palette, oldCol, gMoonBaseARate);
            effDrawSpriteWithCornerColors(400, 216, 0, palette, 32, moon_spr, 6, 103);
            break;
        }
    }
    switch (age) {
    case 0:
        CalcBlendColor(palette, newCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(400, 216, 0, palette, 32, moon_spr, 4, 103);
        break;
    case 1:
        CalcBlendColor(palette, newCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 7, 103);
        break;
    case 2:
        CalcBlendColor(palette, newCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 8, 103);
        break;
    case 3:
        CalcBlendColor(palette, newCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 9, 103);
        break;
    case 4:
        CalcBlendColor(palette, newCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(400, 216, 0, palette, 32, moon_spr, 5, 103);
        break;
    case 5:
        CalcBlendColor(palette, newCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 10, 103);
        break;
    case 6:
        CalcBlendColor(palette, newCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 11, 103);
        break;
    case 7:
        CalcBlendColor(palette, newCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(352, 200, 0, palette, 32, moon_spr, 12, 103);
        break;
    case 8:
        CalcBlendColor(palette, newCol, gMoonBaseARate);
        effDrawSpriteWithCornerColors(400, 216, 0, palette, 32, moon_spr, 6, 103);
        break;
    }
    eglSetOt(103);
    eglLinkAlpha(0);
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtMoon", Process2_evtMoon);
#endif
#else
INCLUDE_ASM(const s32, "event/evtMoon", Process2_evtMoon);
#endif

#ifdef VERSION_RAIDOU1
void evtMoonAddMove(f32 leng)
{
    s32 sub;

    if (evtBitCheck(2100) != 0) {
        return;
    }
    DDS3_GBWK->moonTimer += leng;
    if (!(4000.0f < DDS3_GBWK->moonTimer)) {
        return;
    }
    DDS3_GBWK->moonTimer = 0.0f;
    gMoonAgeOld = evtGetAgeOfMoon();
    gMoonAge16Old = evtGetAgeOfMoon16();
    gMoonCngCnt = 85;
    evtSetAgeOfMoon((DDS3_GBWK->moonPhase + 1) & 0xF);
    evtBitOff(3008);
    evtBitOff(3009);
    evtBitOff(3010);
    evtBitOff(3011);
    evtBitOff(3012);
    evtBitOff(3013);
    evtBitOff(3014);
    evtBitOff(3015);
    evtBitOff(3016);
    switch (evtGetAgeOfMoon()) {
    case 0:
        evtBitOn(3008);
        break;
    case 1:
        evtBitOn(3009);
        break;
    case 2:
        evtBitOn(3010);
        break;
    case 3:
        evtBitOn(3011);
        break;
    case 4:
        evtBitOn(3012);
        break;
    case 5:
        evtBitOn(3013);
        break;
    case 6:
        evtBitOn(3014);
        break;
    case 7:
        evtBitOn(3015);
        break;
    case 8:
        evtBitOn(3016);
        break;
    }
    if (evtGetAgeOfMoon() == 0) {
        mapEsutoMaClearMsg();
        mapRiberaMaClearMsg();
        mapEsutoMa2ClearMsg();
        mapRiberaMa2ClearMsg();
        mapMoudokuClearMsg();
    }
    if (mapCallBitCheck4() != 0) {
        DDS3_GBWK->takaraCnt--;
        if (DDS3_GBWK->takaraCnt < 0) {
            DDS3_GBWK->takaraCnt = 0;
        }
    }
    if (mapCallBitCheck5() != 0) {
        DDS3_GBWK->zeniCnt--;
        if (DDS3_GBWK->zeniCnt < 0) {
            DDS3_GBWK->zeniCnt = 0;
        }
    }
    if (mapGetSkilTakumiMapCheck() != 0) {
        DDS3_GBWK->takumiCnt--;
        if (DDS3_GBWK->takumiCnt < 0) {
            DDS3_GBWK->takumiCnt = 0;
        }
    }
    DDS3_GBWK->teisatuCnt--;
    if (DDS3_GBWK->teisatuCnt < 0) {
        DDS3_GBWK->teisatuCnt = 0;
    }
    sub = datMoonAddMoveSub();
    if (sub < 143) {
        if (sub >= 128) {
            mapDrinkClearMsg();
        }
    }
}
#else
INCLUDE_ASM(const s32, "event/evtMoon", evtMoonAddMove);
#endif

s32 Process1_evtMoon(dds3ProcessID_t *id) {
    moon_spr = effLoadIndexedResource("/map/all/", "geturei.spr", 0);
    evtBitOff(3008);
    evtBitOff(3009);
    evtBitOff(3010);
    evtBitOff(3011);
    evtBitOff(3012);
    evtBitOff(3013);
    evtBitOff(3014);
    evtBitOff(3015);
    evtBitOff(3016);
    return EE_ADDR_S(Process2_evtMoon);
}

INCLUDE_ASM(const s32, "event/evtMoon", Shutdown_evtMoon);

void evtMoonInitProcess(void) {
    if (addressprocess_id == NULL) {
        addressprocess_id = dds3StartProcess(evtMoonProcessName, 0x2B09, 1, 1, Process1_evtMoon, Shutdown_evtMoon, NULL);
        evtEnableMoonCalc();
        evtDisableMoonDraw();
        evtSetAgeOfMoon(0);
        sndCallGetRandIntA();
        sndCallGetRandIntA2();
        sndCallGetRandIntA3();
        sndRefDDS3GBWK();
    }
}

INCLUDE_ASM(const s32, "event/evtMoon", fldFreeAddressProcess);
INCLUDE_SDATA(const s32, "event/evtMoon", moon_spr);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonBaseARate);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonFade);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonFadeFrame);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonFadeFrameMax);

#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData8);

INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData9);

#endif
INCLUDE_SDATA(const s32, "event/evtMoon", gMoonCnt);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonCngCnt);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonAgeOld);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonAge16Old);

INCLUDE_SDATA(const s32, "event/evtMoon", addressprocess_id);

INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData7);

INCLUDE_SDATA(const s32, "event/evtMoon", evtMoonProcessName);

