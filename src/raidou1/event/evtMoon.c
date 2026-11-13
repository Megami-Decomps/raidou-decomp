#include "common.h"
/* This version names the save-work pointer by address. */
#define DDS3_GBWK D_0046B4A0
#include "newdata.h"

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

INCLUDE_ASM(const s32, "event/evtMoon", func_001ADA50);

INCLUDE_ASM(const s32, "event/evtMoon", func_001ADA80);

INCLUDE_ASM(const s32, "event/evtMoon", func_001ADAA0);

INCLUDE_ASM(const s32, "event/evtMoon", func_001ADAC0);

INCLUDE_ASM(const s32, "event/evtMoon", func_001ADAE0);

INCLUDE_ASM(const s32, "event/evtMoon", func_001ADB28);

INCLUDE_ASM(const s32, "event/evtMoon", func_001ADB78);

INCLUDE_ASM(const s32, "event/evtMoon", func_001ADBA8);

INCLUDE_ASM(const s32, "event/evtMoon", func_001ADBC8);

INCLUDE_ASM(const s32, "event/evtMoon", func_001ADC50);

INCLUDE_ASM(const s32, "event/evtMoon", func_001AE7E0);

INCLUDE_ASM(const s32, "event/evtMoon", func_001AEA70);

INCLUDE_ASM(const s32, "event/evtMoon", func_001AEAF0);

INCLUDE_ASM(const s32, "event/evtMoon", func_001AEAF8);

INCLUDE_ASM(const s32, "event/evtMoon", func_001AEB80);
INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C5F0);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C5F4);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C5F8);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C5FC);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C600);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C604);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C608);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C60C);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C610);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C614);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C618);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0046C620);

