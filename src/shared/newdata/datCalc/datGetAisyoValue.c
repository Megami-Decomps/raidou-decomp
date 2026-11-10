#include "shared/newdata/datCalc/decls.h"

u32 datGetAisyoValue(datUnitWork_t *work, s32 attr)
{
    return (u16)datGetAisyo(work, attr);
}
