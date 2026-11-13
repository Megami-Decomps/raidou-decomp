#include "common.h"
/* This version names the save-work pointer by address. */
#define DDS3_GBWK D_004DE600
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

INCLUDE_ASM(const s32, "event/evtMoon", func_001C3908);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C3930);

void evtEnableMoonCalc(void)
{
    DDS3_GBWK->moonFlags |= 1;
}

void evtDisableMoonCalc(void)
{
    DDS3_GBWK->moonFlags &= ~1;
}

INCLUDE_ASM(const s32, "event/evtMoon", func_001C3968);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C3998);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C39B8);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C39D8);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C39F8);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C3A40);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C3A90);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C3AC0);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C3AE0);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C3B68);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C4830);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C4B38);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C4BB8);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C4BC0);

INCLUDE_ASM(const s32, "event/evtMoon", func_001C4C48);
INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF850);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF854);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF858);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF85C);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF860);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF864);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF868);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF86C);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF870);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF874);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF878);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF87C);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF880);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004DF888);

