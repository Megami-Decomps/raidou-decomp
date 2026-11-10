#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_SCR_STAT(void)
{
    if (dds3ExistProcess(EE_ADDR_AS(dds3ProcessID_t *, scrGetIntPara(0))) != 0)
    {
        scrSetIntReturnValue(1);
    }
    else
    {
        scrSetIntReturnValue(0);
    }
    return 1;
}
