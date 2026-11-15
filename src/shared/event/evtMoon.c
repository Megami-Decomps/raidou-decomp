#include "common.h"
#include "newdata.h"
#include "evtMoon.h"

extern s32 gMoonFadeFrameMax;
extern s32 Process2_evtMoonData3;
extern s32 Process2_evtMoonData4;
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
    gMoonFadeFrameMax = 1;
    Process2_evtMoonData4 = 0;
}

void evtDisableMoonDraw(void)
{
    DDS3_GBWK->moonFlags &= ~2;
    gMoonCnt = 0.0f;
    gMoonFadeFrameMax = 0;
    Process2_evtMoonData4 = 0;
}

void evtEnableMoonDraw2(void)
{
    DDS3_GBWK->moonFlags |= 2;
    gMoonFadeFrameMax = 1;
    Process2_evtMoonData4 = 0;
}

void evtDisableMoonDraw2(void)
{
    DDS3_GBWK->moonFlags &= ~2;
    gMoonFadeFrameMax = 0;
    Process2_evtMoonData4 = 0;
}

void PreCalcFade(s32 count)
{
    if (count != 0) {
        if (Process2_evtMoonData4 != 0) {
            Process2_evtMoonData3 = (count * Process2_evtMoonData3) / Process2_evtMoonData4;
        } else {
            if (gMoonFadeFrameMax == 0) {
                count = 0;
            }
            Process2_evtMoonData3 = count;
        }
    }
}

void evtMoonFadeIn(s32 count)
{
    DDS3_GBWK->moonFlags |= 2;
    gMoonCnt = 1.0f;
    PreCalcFade(count);
    Process2_evtMoonData4 = count;
    gMoonFadeFrameMax = 1;
}

void evtMoonFadeOut(s32 count)
{
    PreCalcFade(count);
    Process2_evtMoonData4 = count;
    gMoonFadeFrameMax = 0;
}

s32 evtMoonFadeCheck(void)
{
    if (Process2_evtMoonData3 == 0) {
        return 1;
    }
    return Process2_evtMoonData3 == Process2_evtMoonData4;
}

INCLUDE_ASM(const s32, "event/evtMoon", CalcBlendColor);

INCLUDE_ASM(const s32, "event/evtMoon", Process2_evtMoon);

INCLUDE_ASM(const s32, "event/evtMoon", evtMoonAddMove);

INCLUDE_ASM(const s32, "event/evtMoon", Process1_evtMoon);

INCLUDE_ASM(const s32, "event/evtMoon", Shutdown_evtMoon);

INCLUDE_ASM(const s32, "event/evtMoon", evtMoonInitProcess);

INCLUDE_ASM(const s32, "event/evtMoon", fldFreeAddressProcess);
INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData);

INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData2);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonFadeFrameMax);

INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData3);

INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData4);

#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData8);

INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData9);

#endif
INCLUDE_SDATA(const s32, "event/evtMoon", gMoonCnt);

INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData5);

INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData6);

INCLUDE_SDATA(const s32, "event/evtMoon", evtData13);

INCLUDE_SDATA(const s32, "event/evtMoon", addressprocess_id);

INCLUDE_SDATA(const s32, "event/evtMoon", Process2_evtMoonData7);

INCLUDE_SDATA(const s32, "event/evtMoon", evtMoonProcessName);

