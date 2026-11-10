#include "shared/sdf/sdfSystem/decls.h"

s32 sdfGetRunTimeCountBase(void)
{
    return HW_READ(T0_COUNT);
}
