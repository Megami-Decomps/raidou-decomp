#include "shared/newdata/datCalc/decls.h"

u32 datGetAisyoFlag(datUnitWork_t *work, s32 attr)
{
    return datGetAisyo(work, attr) & 0xFFFF0000;
}
