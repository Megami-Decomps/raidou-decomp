#include "common.h"
#include "basic.h"

void dds3PathBasicPauseOn(dds3PathBasicControl_t *pControl)
{
    pControl->flag |= DDS3_PATHBASIC_FLAG_STOP;
}

void dds3PathBasicPauseOff(dds3PathBasicControl_t *pControl)
{
    pControl->flag &= ~DDS3_PATHBASIC_FLAG_STOP;
}

INCLUDE_ASM(const s32, "dds3PathBasic_0011EA58", dds3PathBasicLoopOn);

INCLUDE_ASM(const s32, "dds3PathBasic_0011EA58", dds3PathBasicLoopOff);
