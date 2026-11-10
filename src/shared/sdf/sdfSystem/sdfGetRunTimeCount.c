#include "shared/sdf/sdfSystem/decls.h"

s32 sdfGetRunTimeCount(s32 aBaseTime)
{
    return (HW_READ(T0_COUNT) - aBaseTime) & 0xFFFF;
}
