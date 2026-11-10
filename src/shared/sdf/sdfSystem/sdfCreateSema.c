#include "shared/sdf/sdfSystem/decls.h"

s32 sdfCreateSema(s32 initCount, s32 maxCount, u32 option)
{
    sdfSemaParam_t param;

    param.initCount = initCount;
    param.maxCount = maxCount;
    param.option = option;
    return CreateSema(&param);
}
