#include "common.h"
#include "newdata.h"

extern s32 gMoonFadeFrameMax;
extern s32 D_r1_0046C5FC;
extern s32 D_r1_0046C600;
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

void evtSetAgeOfMoon(u8 age)
{
    DDS3_GBWK->moonPhase = age & 0xF;
    DDS3_GBWK->moonTimer = 0.0f;
}

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
    D_r1_0046C600 = 0;
}

void evtDisableMoonDraw(void)
{
    DDS3_GBWK->moonFlags &= ~2;
    gMoonCnt = 0.0f;
    gMoonFadeFrameMax = 0;
    D_r1_0046C600 = 0;
}

void evtEnableMoonDraw2(void)
{
    DDS3_GBWK->moonFlags |= 2;
    gMoonFadeFrameMax = 1;
    D_r1_0046C600 = 0;
}

void evtDisableMoonDraw2(void)
{
    DDS3_GBWK->moonFlags &= ~2;
    gMoonFadeFrameMax = 0;
    D_r1_0046C600 = 0;
}

void func_r1_001ADAE0(s32 count)
{
    if (count != 0) {
        if (D_r1_0046C600 != 0) {
            D_r1_0046C5FC = (count * D_r1_0046C5FC) / D_r1_0046C600;
        } else {
            if (gMoonFadeFrameMax == 0) {
                count = 0;
            }
            D_r1_0046C5FC = count;
        }
    }
}

void func_r1_001ADB28(s32 count)
{
    DDS3_GBWK->moonFlags |= 2;
    gMoonCnt = 1.0f;
    func_r1_001ADAE0(count);
    D_r1_0046C600 = count;
    gMoonFadeFrameMax = 1;
}

void func_r1_001ADB78(s32 count)
{
    func_r1_001ADAE0(count);
    D_r1_0046C600 = count;
    gMoonFadeFrameMax = 0;
}

s32 func_r1_001ADBA8(void)
{
    if (D_r1_0046C5FC == 0) {
        return 1;
    }
    return D_r1_0046C5FC == D_r1_0046C600;
}

INCLUDE_ASM(const s32, "event/evtMoon", func_r1_001ADBC8);

INCLUDE_ASM(const s32, "event/evtMoon", Process2_evtMoon);

INCLUDE_ASM(const s32, "event/evtMoon", evtMoonAddMove);

INCLUDE_ASM(const s32, "event/evtMoon", func_r1_001AEA70);

INCLUDE_ASM(const s32, "event/evtMoon", func_r1_001AEAF0);

INCLUDE_ASM(const s32, "event/evtMoon", evtMoonInitProcess);

INCLUDE_ASM(const s32, "event/evtMoon", func_r1_001AEB80);
INCLUDE_SDATA(const s32, "event/evtMoon", D_r1_0046C5F0);

INCLUDE_SDATA(const s32, "event/evtMoon", D_r1_0046C5F4);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonFadeFrameMax);

INCLUDE_SDATA(const s32, "event/evtMoon", D_r1_0046C5FC);

INCLUDE_SDATA(const s32, "event/evtMoon", D_r1_0046C600);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonCnt);

INCLUDE_SDATA(const s32, "event/evtMoon", D_r1_0046C608);

INCLUDE_SDATA(const s32, "event/evtMoon", D_r1_0046C60C);

INCLUDE_SDATA(const s32, "event/evtMoon", D_r1_0046C610);

INCLUDE_SDATA(const s32, "event/evtMoon", D_r1_0046C614);

INCLUDE_SDATA(const s32, "event/evtMoon", D_r1_0046C618);

INCLUDE_SDATA(const s32, "event/evtMoon", D_r1_0046C620);

